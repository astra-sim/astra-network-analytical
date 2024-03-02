/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "congestion_aware/MultiDimTopology.hh"
#include <cassert>
#include <iostream>

using namespace NetworkAnalytical;
using namespace NetworkAnalyticalCongestionAware;

MultiDimTopology::MultiDimTopology(
    const int dims_count,
    std::vector<TopologyBuildingBlock>&& topology_per_dim,
    std::vector<int>&& npus_count_per_dim,
    std::vector<Bandwidth>&& bandwidth_per_dim,
    std::vector<Latency>&& latency_per_dim) noexcept
    : Topology() {
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
  // TODO: implement
  auto route = Route();
  return route;
}

void MultiDimTopology::construct_topology() noexcept {
  // compute the number of NPUs
  npus_count = 1;
  for (const auto basic_topology_npus_count : npus_count_per_dim) {
    npus_count *= basic_topology_npus_count;
  }

  // instantiate devices
  instantiate_devices();

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

      // first compute the list of NPUs within this basic topology
      auto basic_topology_npus = std::vector<int>();

      // which has <basic_topology_npus_count> number of NPUs
      // starting from basic_topology_base_idx
      auto topology_npu_idx = basic_topology_base_idx;

      // append all NPUs within this basic topology
      for (auto basic_topo_npus_iter = 0;
           basic_topo_npus_iter < basic_topology_npus_count;
           basic_topo_npus_iter++) {
        basic_topology_npus.push_back(topology_npu_idx);
        topology_npu_idx += *basic_topology_stride;
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
