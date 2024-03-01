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
  for (const auto npus_count_dim : npus_count_per_dim) {
    npus_count *= npus_count_dim;
  }

  // instantiate devices
  instantiate_devices();

  // construct dimensions
  for (auto dim = 0; dim < dims_count; dim++) {
    construct_dimension(dim);
  }
}

void MultiDimTopology::construct_dimension(const int dim) noexcept {
  // TODO: implement dimension construction
  std::cout << "construct dim " << dim << std::endl;
}
