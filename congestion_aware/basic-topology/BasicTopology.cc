/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "congestion_aware/BasicTopology.hh"
#include <cassert>
#include <tuple>

using namespace NetworkAnalytical;
using namespace NetworkAnalyticalCongestionAware;

BasicTopology::BasicTopology(
    const int npus_count,
    const int devices_count,
    const Bandwidth bandwidth,
    const Latency latency) noexcept
    : bandwidth(bandwidth),
      latency(latency),
      basic_topology_type(TopologyBuildingBlock::Undefined),
      Topology() {
  assert(npus_count > 0);
  assert(devices_count > 0);
  assert(devices_count >= npus_count);
  assert(bandwidth > 0);
  assert(latency >= 0);

  // setup npus and devices count
  this->npus_count = npus_count;
  this->devices_count = devices_count;

  // initialize values
  initialize_values();

  // create new devices
  for (auto id = 0; id < devices_count; id++) {
    std::ignore = create_device(id);
  }
}

BasicTopology::BasicTopology(
    const Devices& npus,
    const std::optional<Devices> devices,
    const Bandwidth bandwidth,
    const Latency latency) noexcept
    : bandwidth(bandwidth),
      latency(latency),
      basic_topology_type(TopologyBuildingBlock::Undefined),
      Topology() {
  assert(!npus.empty());
  assert(bandwidth > 0);
  assert(latency >= 0);

  // setup npus and devices count
  npus_count = static_cast<int>(npus.size());
  devices_count = npus_count;

  // initialize values
  initialize_values();

  // push back NPUs
  for (const auto& npu : npus) {
    this->devices.push_back(npu);
  }

  // push back additional devices (if exist)
  if (devices.has_value()) {
    // update devices count
    devices_count += static_cast<int>(devices.value().size());

    for (const auto& device : devices.value()) {
      this->devices.push_back(device);
    }
  }
}

// default destructor
BasicTopology::~BasicTopology() noexcept = default;

TopologyBuildingBlock BasicTopology::get_basic_topology_type() const noexcept {
  assert(basic_topology_type != TopologyBuildingBlock::Undefined);

  // get basic topology type
  return basic_topology_type;
}

void BasicTopology::set_basic_topology_type(
    const TopologyBuildingBlock topology) noexcept {
  basic_topology_type = topology;
  topology_per_dim.push_back(topology);
}

void BasicTopology::initialize_values() noexcept {
  dims_count = 1;
  npus_count_per_dim.push_back(npus_count);
  bandwidth_per_dim.push_back(bandwidth);
  latency_per_dim.push_back(latency);
}
