// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2026 Advanced Micro Devices, Inc.

#include <cassert>
#include <astra-network-analytical/congestion_aware/Switch.h>

using namespace NetworkAnalyticalCongestionAware;

Switch::Switch(const int npus_count,
               const int num_devices,
               const Bandwidth bandwidth,
               const Latency latency) noexcept
    : BasicTopology(npus_count, num_devices + 1, bandwidth, latency) {
    // e.g., if npus_count=8, then
    // there are total 9 devices, where ordinary npus are 0-7, and switch is 8
    assert(npus_count > 0);
    assert(bandwidth > 0);
    assert(latency >= 0);

    // set switch id
    switch_id = npus_count;

    // connect npus and switches, the link should be bidirectional
    for (auto i = 0; i < num_devices; i++) {
        connect(i, switch_id, bandwidth, latency, true);
    }
}

Route Switch::route(DeviceId src, DeviceId dest) const noexcept {
    // assert npus are in valid range
    assert(0 <= src && src < num_devices);
    assert(0 <= dest && dest < num_devices);

    // construct route
    // start at source, and go to switch, then go to destination
    auto route = Route();
    route.push_back(devices[src]);
    route.push_back(devices[switch_id]);
    route.push_back(devices[dest]);

    return route;
}
