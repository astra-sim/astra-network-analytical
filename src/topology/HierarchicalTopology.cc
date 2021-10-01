/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "HierarchicalTopology.hh"
#include <iostream>

using namespace Analytical;

HierarchicalTopology::HierarchicalTopology(
    TopologyConfigs& configs,
    HierarchicalTopologyConfig hierarchy_config) noexcept
    : Topology(configs), hierarchy_config(hierarchy_config) {
  // Update bandwidth as required
  auto topology_size_up_to = 1;

  for (int dim = 0; dim < hierarchy_config.getDimensionsCount(); dim++) {
    auto topology = hierarchy_config.getTopologyForDim(dim);
    auto links_count = hierarchy_config.getLinksCountForDim(dim);
    auto link_bandwidth = hierarchy_config.getLinkBandwidthForDim(dim);
    auto topology_size = configs[dim].getNpusCount();
    auto adjacent_packages_count = topology_size - 1;

    auto bandwidth_scalar = 1.0;

    // compute bandwidth_scalar
    if (topology == TopologyList::Ring) {
      if (links_count % 2 != 0) {
        std::cout
            << "[HierarchicalTopology, constructor] [Warning] Links-count at dimension "
            << dim << " (Ring) has " << links_count << " links (not even)."
            << std::endl;
        bandwidth_scalar = links_count - 1;
      } else {
        bandwidth_scalar = links_count;
      }
    } else if (topology == TopologyList::FullyConnected) {
      if (links_count % adjacent_packages_count != 0) {
        std::cout
            << "[HierarchicalTopology, constructor] [Warning] Links-count at dimension "
            << dim << " (FullyConnected) has " << links_count
            << " links (not a multiple of " << adjacent_packages_count << ")."
            << std::endl;
      }
      bandwidth_scalar = links_count / adjacent_packages_count;
    } else if (topology == TopologyList::Switch) {
      bandwidth_scalar = links_count;
    } else {
      std::cout << "[HierarchicalTopology, constructor] Topology at dimension "
                << dim << " not defined" << std::endl;
      exit(-1);
    }

    // update bandwidth
    if (bandwidth_scalar == 0) {
      std::cout
          << "[Hierarchical, constructor] [Error] Bandwidth scalar of dimension "
          << dim << " is 0." << std::endl;
      exit(-1);
    }
    configs[dim].multiplyLinkBandwidth(bandwidth_scalar);

    // compute cost model
    auto dimension_type = hierarchy_config.getDimensionType(dim);

    topology_size_up_to *= topology_size;
    auto topologies_count = npus_count / topology_size;

    if (topology == TopologyList::Ring) {
      auto links_count_per_node = links_count / 2;
      auto ring_links_count = topology_size * links_count_per_node;
      auto total_ring_links_count = topologies_count * ring_links_count;

      cost_model.addResource(
          dimension_type,
          CostModel::ResourceType::Link,
          total_ring_links_count,
          link_bandwidth);
    } else if (topology == TopologyList::FullyConnected) {
      auto links_count_per_node = links_count / adjacent_packages_count;
      auto a2a_links_count =
          (topology_size * (topology_size - 1) / 2) * links_count_per_node;
      auto total_a2a_links_count = topologies_count * a2a_links_count;

      cost_model.addResource(
          dimension_type,
          CostModel::ResourceType::Link,
          total_a2a_links_count,
          link_bandwidth);
    } else if (topology == TopologyList::Switch) {
      auto switches_count = npus_count / topology_size_up_to;
      auto switch_radix = topology_size_up_to * links_count;
      auto total_links_count = switch_radix * switches_count;
      cost_model.addResource(
          dimension_type,
          CostModel::ResourceType::Link,
          total_links_count,
          link_bandwidth);
      cost_model.addResource(
          dimension_type,
          CostModel::ResourceType::Switch,
          switches_count,
          link_bandwidth,
          switch_radix);
      cost_model.addResource(
          dimension_type,
          CostModel::ResourceType::Nic,
          total_links_count,
          link_bandwidth);
    } else {
      std::cout << "[HierarchicalTopology, constructor] Topology at dimension "
                << dim << " not defined" << std::endl;
      exit(-1);
    }
  }
}

HierarchicalTopology::~HierarchicalTopology() noexcept = default;

HierarchicalTopology::Latency HierarchicalTopology::linkLatency(
    int dimension,
    int hops_count) const noexcept {
  return hops_count * configs[dimension].getLinkLatency();
}

std::pair<double, int> HierarchicalTopology::send(
    NpuId src,
    NpuId dest,
    PayloadSize payload_size) noexcept {
  checkNpuIdBound(src);
  checkNpuIdBound(dest);

  if (src == dest) {
    return std::make_pair(0, -1);
  }

  // FIXME: assuming only one dimension, from higher, is used at a time
  auto src_address = npuIdToAddress(src);
  auto dest_address = npuIdToAddress(dest);

  // find mismatching dim
  auto dim = hierarchy_config.getDimensionsCount() - 1;
  while (dim >= 0) {
    if (src_address[dim] != dest_address[dim]) {
      break;
    }
    dim--;
  }

  // use that dimension
  auto communication_latency = 0;
  auto topology = hierarchy_config.getTopologyForDim(dim);

  if (topology == TopologyList::Ring) {
    auto distanceA = std::abs(src_address[dim] - dest_address[dim]);
    auto distanceB = configs[dim].getNpusCount() - distanceA;
    auto hops_count = (distanceA < distanceB)
        ? distanceA
        : distanceB; // shorter one becomes the hop count

    // compute communication latency
    // dimension is 0
    communication_latency = linkLatency(dim, hops_count);
    communication_latency += serializationLatency(dim, payload_size);
    communication_latency += 2 * nicLatency(dim);
  } else if (topology == TopologyList::FullyConnected) {
    const auto hops_count = 1;
    communication_latency = linkLatency(dim, hops_count);
    communication_latency += serializationLatency(dim, payload_size);
    communication_latency += 2 * nicLatency(dim);
  } else if (topology == TopologyList::Switch) {
    const auto hops_count = 2;
    communication_latency = linkLatency(dim, hops_count);
    communication_latency += serializationLatency(dim, payload_size);
    communication_latency += 2 * nicLatency(dim);
    communication_latency += routerLatency(dim);
  }

  auto hbm_latency = hbmLatency(dim, payload_size);

  return std::make_pair(
      criticalLatency(communication_latency, hbm_latency), dim);
}

HierarchicalTopology::NpuAddress HierarchicalTopology::npuIdToAddress(
    NpuId npu_id) const noexcept {
  // If units-count if [2, 8, 4], and the given id is 47, then the id should be
  // (id % (topology size of current dim)) / (topology size of last dim)
  //       1 = 47 % (2) / (1)
  //       7 = 47 % (2 * 8) / (1 * 2)
  //       2 = 47 % (2 * 8 * 4) / (1 * 2 * 8)
  // [1, 7, 2] is the result
  auto address = NpuAddress();

  auto current_topology_size = 1;
  auto last_topology_size = 1;

  for (auto dim = 0; dim < hierarchy_config.getDimensionsCount(); dim++) {
    current_topology_size *= configs[dim].getNpusCount();

    auto index = (npu_id % current_topology_size) / last_topology_size;
    address.emplace_back(index);

    last_topology_size *= configs[dim].getNpusCount();
  }

  return address;
}

HierarchicalTopology::NpuId HierarchicalTopology::npuAddressToId(
    NpuAddress npu_address) const noexcept {
  // If units-count if [2, 8, 4], and the address is [1, 7, 3], then the id
  // should be address * (topology size up to the dimension)
  //       1 * (1)
  //     + 7 * (1 * 2)
  //     + 3 * (1 * 2 * 8)
  auto id = 0;

  auto topology_size = 1;
  for (auto dim = 0; dim < hierarchy_config.getDimensionsCount(); dim++) {
    id += npu_address[dim] * topology_size;
    topology_size *= configs[dim].getNpusCount();
  }

  return id;
}

HierarchicalTopology::Bandwidth HierarchicalTopology::
    getNpuTotalBandwidthPerDim(int dimension) const noexcept {
  // links_count[dim] * link_BW[dim]
  auto topology = hierarchy_config.getTopologyForDim(dimension);

  auto topology_size = configs[dimension].getNpusCount();
  auto adjacent_packages_count = topology_size - 1;

  auto links_count = hierarchy_config.getLinksCountForDim(dimension);
  auto link_bandwidth = hierarchy_config.getLinkBandwidthForDim(dimension);

  if (topology == TopologyList::Ring) {
    links_count -= (links_count % 2); // make even number
  } else if (topology == TopologyList::FullyConnected) {
    links_count -=
        (links_count %
         adjacent_packages_count); // make multiplier of adjacent_packages_count
  } else if (topology == TopologyList::Switch) {
    // pass
  } else {
    std::cout
        << "[HierarchicalTopology, method getNpuTotalBandwidthPerDim] Given topology for dimension "
        << dimension << " not defined." << std::endl;
    exit(-1);
  }

  return links_count * link_bandwidth; // GB/s
}
