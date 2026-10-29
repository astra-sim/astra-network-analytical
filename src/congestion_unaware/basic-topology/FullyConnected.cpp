// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2026 Advanced Micro Devices, Inc.

#include <cassert>
#include <astra-network-analytical/congestion_unaware/FullyConnected.h>

using namespace NetworkAnalytical;
using namespace NetworkAnalyticalCongestionUnaware;

FullyConnected::FullyConnected(const int npus_count,
                               const int num_devices,
                               const Bandwidth bandwidth,
                               const Latency latency) noexcept
    : BasicTopology(npus_count, num_devices, bandwidth, latency) {
    assert(npus_count > 0);
    assert(bandwidth > 0);
    assert(latency >= 0);

    // set the building block type
    basic_topology_type = TopologyBuildingBlock::FullyConnected;
}

int FullyConnected::compute_hops_count(const DeviceId src,
                                       const DeviceId dest) const noexcept {
    assert(0 <= src && src < num_devices);
    assert(0 <= dest && dest < num_devices);
    assert(src != dest);

    // for FullyConnected, hops_count is always 1 (src -> dest)
    return 1;
}
