/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "congestion_aware/MultiDimTopology.hh"
#include <cassert>
#include <iostream>
#include "common/NetworkFunction.hh"
#include "congestion_aware/FullyConnected.hh"
#include "congestion_aware/Ring.hh"
#include "congestion_aware/Switch.hh"

using namespace NetworkAnalytical;
using namespace NetworkAnalyticalCongestionAware;

MultiDimTopology::MultiDimTopology(
    const int dims_count,
    std::vector<TopologyBuildingBlock>&& topology_per_dim,
    std::vector<int>&& npus_count_per_dim,
    std::vector<Bandwidth>&& bandwidth_per_dim,
    std::vector<Latency>&& latency_per_dim) noexcept
    : basic_topology_map(), Topology() {
  // assert value validity
  assert(dims_count > 0);
  assert(topology_per_dim.size() == dims_count);
  assert(npus_count_per_dim.size() == dims_count);
  assert(bandwidth_per_dim.size() == dims_count);
  assert(latency_per_dim.size() == dims_count);

  for (auto i = 0; i < dims_count; i++) {
    assert(topology_per_dim[i] != TopologyBuildingBlock::Undefined);
    assert(npus_count_per_dim[i] > 0);
    assert(bandwidth_per_dim[i] > 0);
    assert(latency_per_dim[i] >= 0);
  }

  // move values
  this->dims_count = dims_count;
  this->topology_per_dim = std::move(topology_per_dim);
  this->npus_count_per_dim = std::move(npus_count_per_dim);
  this->bandwidth_per_dim = std::move(bandwidth_per_dim);
  this->latency_per_dim = std::move(latency_per_dim);

  // construct multi-dimension topology
  construct_topology();
}

Route MultiDimTopology::route(const DeviceId src, const DeviceId dest)
    const noexcept {
  // assert validity
  assert(0 <= src && src < npus_count);
  assert(0 <= dest && dest < npus_count);

  // assert src and dest differs
  assert(src != dest);

  // full route to return
  auto route = Route();

  // insert src to the route
  route.push_back(devices[src]);

  // translate src and dest to multi-dim address
  auto current_address = translate_id_to_address(src, npus_count_per_dim);
  const auto dest_address = translate_id_to_address(dest, npus_count_per_dim);

  // check which dims have different address
  for (auto dim = 0; dim < dims_count; dim++) {
    const auto current_addr_dim = current_address[dim];
    const auto dest_addr_dim = dest_address[dim];

    if (current_addr_dim == dest_addr_dim) {
      continue;
    }

    // should route over this basic topology
    const auto current_id =
        translate_address_to_id(current_address, npus_count_per_dim);
    const auto basic_topology = basic_topology_map[current_id][dim];
    auto sub_route = basic_topology->route(current_addr_dim, dest_addr_dim);

    // append this sub_route to the full route
    auto sub_route_it = std::next(sub_route.begin());
    while (sub_route_it != sub_route.end()) {
      route.push_back(*sub_route_it);
      sub_route_it = std::next(sub_route_it);
    }

    // update current addr
    current_address[dim] = dest_address[dim];
  }

  // return full route
  return route;
}

void MultiDimTopology::construct_topology() noexcept {
  // compute the number of NPUs
  npus_count = 1;
  for (const auto basic_topology_npus_count : npus_count_per_dim) {
    npus_count *= basic_topology_npus_count;
  }
  devices_count = npus_count; // switches will be added as required

  // instantiate devices
  for (auto id = 0; id < npus_count; id++) {
    std::ignore = create_device(id);
  }

  // initialize basic topology map
  initialize_basic_topology_map();

  // initialize map storing intra/inter-group strides and repetitions count
  auto basic_topology_stride = 1;
  auto basic_topologies_count = 1;
  auto group_stride = 1;
  auto groups_count = npus_count;

  // construct dimensions
  for (auto dim = 0; dim < dims_count; dim++) {
    construct_dimension(
        dim,
        &basic_topology_stride,
        &basic_topologies_count,
        &group_stride,
        &groups_count);
  }
}

void MultiDimTopology::construct_dimension(
    const int dim,
    int* const basic_topology_stride,
    int* const basic_topologies_count,
    int* const group_stride,
    int* const groups_count) noexcept {
  // get the number of NPUs for each basic topology we're constructing
  const auto basic_topology_npus_count = npus_count_per_dim[dim];

  // start from base index 0
  auto group_base_idx = 0;
  auto basic_topology_base_idx = 0;

  // update group stride/repetition before processing
  *group_stride *= basic_topology_npus_count;
  *groups_count /= basic_topology_npus_count;

  // there are multiple "topology groups" within a multi-dim topology
  for (auto group_iter = 0; group_iter < *groups_count; group_iter++) {
    // here, a "topology group" is being constructed

    // first, initialize basic_topology_base_idx
    // which will be used for basic topologies within a group
    basic_topology_base_idx = group_base_idx;

    // each "topology group" has multiple "basic topologies"
    for (auto basic_topology_iter = 0;
         basic_topology_iter < *basic_topologies_count;
         basic_topology_iter++) {
      // here, a "basic topology" is being constructed

      // we will create a vector with target NPUs in this basic topology
      auto basic_topology_npus = Devices();

      // which has <basic_topology_npus_count> number of NPUs
      // starting from basic_topology_base_idx
      auto topology_npu_idx = basic_topology_base_idx;

      // append all NPUs within this basic topology
      for (auto basic_topo_npus_iter = 0;
           basic_topo_npus_iter < basic_topology_npus_count;
           basic_topo_npus_iter++) {
        basic_topology_npus.push_back(devices[topology_npu_idx]);
        topology_npu_idx += *basic_topology_stride;
      }

      // instantiate basic topology
      const auto topology_type = topology_per_dim[dim];
      const auto bandwidth = bandwidth_per_dim[dim];
      const auto latency = latency_per_dim[dim];
      std::shared_ptr<Device> switch_device = nullptr;
      std::shared_ptr<BasicTopology> basic_topology = nullptr;

      switch (topology_type) {
        case TopologyBuildingBlock::Ring:
          // create ring topology
          basic_topology =
              std::make_shared<Ring>(basic_topology_npus, bandwidth, latency);
          break;

        case TopologyBuildingBlock::Switch:
          // create a new switch device
          switch_device = create_device(devices_count);
          devices_count++;

          // create switch topology
          basic_topology = std::make_shared<Switch>(
              basic_topology_npus, switch_device, bandwidth, latency);
          break;

        case TopologyBuildingBlock::FullyConnected:
          // create FC topology
          basic_topology = std::make_shared<FullyConnected>(
              basic_topology_npus, bandwidth, latency);
          break;

        default:
          // shouldn't reach here
          std::cerr << "[Error] (network/analytical/congestion_aware) "
                    << "not supported basic-topology" << std::endl;
          std::exit(-1);
      }

      // assign this created basic topology to the map
      for (const auto& npu : basic_topology_npus) {
        const auto npu_id = npu->get_id();
        basic_topology_map[npu_id][dim] = basic_topology;
      }

      // process next basic topology
      basic_topology_base_idx++;
    }

    // process next topology group
    group_base_idx += *group_stride;
  }

  // update basic topology stride/repetition after processing
  *basic_topology_stride *= basic_topology_npus_count;
  *basic_topologies_count *= basic_topology_npus_count;
}

void MultiDimTopology::initialize_basic_topology_map() noexcept {
  // create basic_topology_map[#NPUs][#dims] = (basic topology pointer)
  for (auto i = 0; i < npus_count; i++) {
    basic_topology_map.emplace_back(dims_count, nullptr);
  }
}
