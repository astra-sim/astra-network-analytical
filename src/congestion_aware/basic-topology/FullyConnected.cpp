// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2026 Advanced Micro Devices, Inc.

#include <cassert>
#include <astra-network-analytical/congestion_aware/FullyConnected.h>

using namespace NetworkAnalyticalCongestionAware;

FullyConnected::FullyConnected(const int npus_count,
                               const int num_devices,
                               const Bandwidth bandwidth,
                               const Latency latency) noexcept
    : BasicTopology(npus_count, num_devices, bandwidth, latency) {
    assert(npus_count > 0);
    assert(bandwidth > 0);
    assert(latency >= 0);

    // set topology type
    basic_topology_type = TopologyBuildingBlock::FullyConnected;

    // fully-connect every src-dest pairs
    for (auto src = 0; src < num_devices; src++) {
        for (auto dest = 0; dest < num_devices; dest++) {
            if (src != dest) {
                connect(src, dest, bandwidth, latency, false);
            }
        }
    }
}

Route FullyConnected::route(const DeviceId src,
                            const DeviceId dest) const noexcept {
    // assert npus are in valid range
    assert(0 <= src && src < num_devices);
    assert(0 <= dest && dest < num_devices);

    // construct route
    // directly connected
    auto route = Route();
    route.push_back(devices[src]);
    route.push_back(devices[dest]);

    return route;
}
