// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2026 Advanced Micro Devices, Inc.

#include <cassert>
#include <astra-network-analytical/congestion_aware/BasicTopology.h>

using namespace NetworkAnalytical;
using namespace NetworkAnalyticalCongestionAware;

BasicTopology::BasicTopology(const int npus_count,
                             const int num_devices,
                             const Bandwidth bandwidth,
                             const Latency latency) noexcept
    : bandwidth(bandwidth),
      latency(latency),
      basic_topology_type(TopologyBuildingBlock::Undefined),
      Topology() {
    assert(npus_count > 0);
    assert(num_devices > 0);
    assert(num_devices >= npus_count);
    assert(bandwidth > 0);
    assert(latency >= 0);

    // setup npus and devices count
    this->npus_count = npus_count;
    this->num_devices = num_devices;
    dims_count = 1;
    npus_count_per_dim.push_back(npus_count);
    bandwidth_per_dim.push_back(bandwidth);

    // instantiate devices
    instantiate_devices();
}

// default destructor
BasicTopology::~BasicTopology() noexcept = default;

TopologyBuildingBlock BasicTopology::get_basic_topology_type() const noexcept {
    assert(basic_topology_type != TopologyBuildingBlock::Undefined);

    return basic_topology_type;
}
