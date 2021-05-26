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
  auto package_size = 1;

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
    if (dimension_type == DimensionType::T) {
      // package size is defined by Tile-to-tile dimension
      package_size *= topology_size;
    }

    auto nic_latency = configs[dim].getNicLatency();
    auto topologies_count = npus_count / topology_size;

    if (topology == TopologyList::Ring) {
      auto links_count_per_node = links_count / 2;
      auto ring_links_count = topology_size * links_count_per_node;
      auto total_ring_links_count = topologies_count * ring_links_count;

      if (dimension_type == DimensionType::T) {
        cost_model.addResource(
            CostModel::ResourceType::TileToTileLink, total_ring_links_count, link_bandwidth);
      } else if (dimension_type == DimensionType::N) {
        cost_model.addResource(
            CostModel::ResourceType::NVLink, total_ring_links_count, link_bandwidth);
      } else if (dimension_type == DimensionType::P) {
        cost_model.addResource(
            CostModel::ResourceType::NVLink, total_ring_links_count, link_bandwidth);
      } else if (dimension_type == DimensionType::PP) {
        std::cout << "Ring topology used for Inter-Pod dimension. Currently only Switch topology is supported." << std::endl;
        exit(-1);
//        if (nic_latency > 0) {
//          // 1 Link and 2 NICs
//          auto package_links_count = total_ring_links_count / package_size;
//          auto package_nics_count = package_links_count * 2;
//
//          // fixme: pod-to-pod dimension may use other than NVLink
//          cost_model.addResource(
//              CostModel::ResourceType::NVLink, package_links_count, link_bandwidth);
//          cost_model.addResource(
//              CostModel::ResourceType::InfiniBandNic, package_nics_count, -1);
//        } else {
//          // fixme: pod-to-pod dimension may use other than NVLink
//          cost_model.addResource(
//              CostModel::ResourceType::NVLink, total_ring_links_count, link_bandwidth);
//        }
      } else {
        std::cout
            << "[HierarchicalTopology, constructor] Given dimension type not implemented"
            << std::endl;
        exit(-1);
      }
    } else if (topology == TopologyList::FullyConnected) {
      auto links_count_per_node = links_count / adjacent_packages_count;
      auto a2a_links_count =
          (topology_size * (topology_size - 1) / 2) * links_count_per_node;
      auto total_a2a_links_count = topologies_count * a2a_links_count;

      if (dimension_type == DimensionType::T) {
        cost_model.addResource(
            CostModel::ResourceType::TileToTileLink, total_a2a_links_count, -1);
      } else if (dimension_type == DimensionType::N) {
        cost_model.addResource(
            CostModel::ResourceType::NVLink, total_a2a_links_count, link_bandwidth);
      } else if (dimension_type == DimensionType::P) {
        cost_model.addResource(
            CostModel::ResourceType::NVLink, total_a2a_links_count, link_bandwidth);
      } else if (dimension_type == DimensionType::PP) {
        std::cout << "FullyConnected topology used for Inter-Pod dimension. Currently only Switch topology is supported." << std::endl;
        exit(-1);
//        if (nic_latency > 0) {
//          // 1 Link and 2 NICs
//          auto package_links_count = total_a2a_links_count / package_size;
//          auto package_nics_count = package_links_count * 2;
//
//          // fixme: pod-to-pod dimension may use other than NVLink
//          cost_model.addResource(
//              CostModel::ResourceType::NVLink, package_links_count, link_bandwidth);
//          cost_model.addResource(
//              CostModel::ResourceType::InfiniBandNic, package_nics_count, -1);
//        } else {
//          // fixme: pod-to-pod dimension may use other than NVLink
//          cost_model.addResource(
//              CostModel::ResourceType::NVLink, total_a2a_links_count, link_bandwidth);
//        }
      } else {
        std::cout
            << "[HierarchicalTopology, constructor] Given dimension type not implemented"
            << std::endl;
        exit(-1);
      }
    } else if (topology == TopologyList::Switch) {
      auto links_count_per_node = links_count;
      auto switch_links_count = topology_size_up_to * links_count_per_node;
      auto switches_count = cost_model.getMellanoxSwitchesCount(switch_links_count);

      if (dimension_type == DimensionType::T) {
        // fixme: Switch in tile-to-tile level (if possible) may not use
        // NVSwitch
        cost_model.addResource(
            CostModel::ResourceType::TileToTileLink, switch_links_count, -1);
        cost_model.addResource(CostModel::ResourceType::MellanoxSwitch, switches_count, -1);
      } else if (dimension_type == DimensionType::N) {
        cost_model.addResource(CostModel::ResourceType::NVLink, switch_links_count, link_bandwidth);
        cost_model.addResource(CostModel::ResourceType::MellanoxSwitch, switches_count, -1);
      } else if (dimension_type == DimensionType::P) {
        cost_model.addResource(CostModel::ResourceType::NVLink, switch_links_count, link_bandwidth);
        cost_model.addResource(CostModel::ResourceType::MellanoxSwitch, switches_count, -1);
      } else if (dimension_type == DimensionType::PP) {
        // fixme: assuming NIC is attached at package level, not each tile.
        // get the package size
        auto package_size = configs[0].getNpusCount();
        auto package_total_bandwidth = package_size * link_bandwidth * links_count;
        auto nics_count_per_package = cost_model.getRequiredNicsCount(package_total_bandwidth);
        auto packages_count = topology_size_up_to / package_size;
        auto nics_count = nics_count_per_package * packages_count;
        auto switches_count = cost_model.getMellanoxSwitchesCount(nics_count);

          // fixme: pod-to-pod dimension may use other than NVLink
          cost_model.addResource(
              CostModel::ResourceType::NVLink, nics_count, CostModel::nv_link_bandwidth);
        cost_model.addResource(
            CostModel::ResourceType::InfiniBandNic, nics_count, -1);
          cost_model.addResource(CostModel::ResourceType::MellanoxSwitch, switches_count, -1);

//        if (nic_latency > 0) {
//          // 1 Link and 1 NICs
//          auto package_links_count = switch_links_count / package_size;
//          auto package_nics_count = package_links_count;
//          auto package_switches_count =
//              cost_model.getMellanoxSwitchesCount(package_links_count);
//
//          // fixme: pod-to-pod dimension may use other than NVLink
//          cost_model.addResource(
//              CostModel::ResourceType::NVLink, package_links_count, link_bandwidth);
//          cost_model.addResource(
//              CostModel::ResourceType::InfiniBandNic, package_nics_count, -1);
//          cost_model.addResource(
//              CostModel::ResourceType::MellanoxSwitch, package_switches_count, -1);
//        } else {
//          // fixme: pod-to-pod dimension may use other than NVLink
//          cost_model.addResource(
//              CostModel::ResourceType::NVLink, switch_links_count, link_bandwidth);
//          cost_model.addResource(CostModel::ResourceType::MellanoxSwitch, switches_count, -1);
//        }
      } else {
        std::cout
            << "[HierarchicalTopology, constructor] Given dimension type not implemented"
            << std::endl;
        exit(-1);
      }
    } else {
      std::cout << "[HierarchicalTopology, constructor] Topology at dimension "
                << dim << " not defined" << std::endl;
      exit(-1);
    }
  }

  // add NPUs
  cost_model.addResource(CostModel::ResourceType::Npu, npus_count, -1);
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
