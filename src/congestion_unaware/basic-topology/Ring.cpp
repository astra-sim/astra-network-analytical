// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2026 Advanced Micro Devices, Inc.

#include <cassert>
#include <astra-network-analytical/congestion_unaware/Ring.h>

using namespace NetworkAnalytical;
using namespace NetworkAnalyticalCongestionUnaware;

Ring::Ring(const int npus_count,
           const int num_devices,
           const Bandwidth bandwidth,
           const Latency latency,
           const bool bidirectional) noexcept
    : bidirectional(bidirectional),
      BasicTopology(npus_count, num_devices, bandwidth, latency) {
    assert(npus_count > 0);
    assert(bandwidth > 0);
    assert(latency >= 0);

    // set the building block type
    basic_topology_type = TopologyBuildingBlock::Ring;
}

int Ring::compute_hops_count(const DeviceId src,
                             const DeviceId dest) const noexcept {
    assert(0 <= src && src < num_devices);
    assert(0 <= dest && dest < num_devices);
    assert(src != dest);

    // for Ring topology
    // 1. compute clockwise and anti-clockwise distance
    // 2. if unidirectional, use clockwise one
    // 3. if bidirectional, use the shorter one

    // compute clockwise distance
    auto clockwise_distance = (dest - src);
    if (clockwise_distance < 0) {
        clockwise_distance += npus_count;
    }

    // compute anticlockwise distance
    const auto anticlockwise_distance = npus_count - clockwise_distance;

    // unidirectional: return clockwise distance
    if (!bidirectional) {
        return clockwise_distance;
    }

    // bidirectional: return shorter distance
    return (clockwise_distance < anticlockwise_distance)
               ? clockwise_distance
               : anticlockwise_distance;
}
