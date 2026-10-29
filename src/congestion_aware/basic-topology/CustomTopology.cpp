// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2026 Advanced Micro Devices, Inc.

#include <cassert>
#include <astra-network-analytical/congestion_aware/CustomTopology.h>

using namespace NetworkAnalyticalCongestionAware;

CustomTopology::CustomTopology(
    int npus_count,
    int num_devices,
    std::map<int, std::map<int, Bandwidth>> bandwidths,
    std::map<int, std::map<int, Latency>> latencies,
    RoutesContainer _routes) noexcept
    : Topology() {
    assert(npus_count > 0);
    assert(num_devices > 0);
    assert(num_devices >= npus_count);

    // setup npus and devices count
    this->npus_count = npus_count;
    this->num_devices = num_devices;
    dims_count = 1;                            // we do not to care about this
    npus_count_per_dim.push_back(npus_count);  // we do not to care about this
    bandwidth_per_dim.push_back(10);           // we do not to care about this

    // instantiate devices
    instantiate_devices();

    // Connect devices
    bool bidirectional = false;  // initially leave it like this
    for (auto& node_links : bandwidths) {
        auto src_dev = node_links.first;
        for (auto& link_bw : node_links.second) {
            auto dst_dev = link_bw.first;
            auto bandwidth = link_bw.second * (1'000'000'000) /
                             (1 << 30);  // the link constructor expects GiBps;
            auto latency = latencies[src_dev][dst_dev];
            connect(src_dev, dst_dev, bandwidth, latency, bidirectional);
        }
    }

    // store predefined routes:
    if (_routes.empty()) {
        for (auto& node_links : bandwidths) {
            auto src_dev = node_links.first;
            for (auto& link_bw : node_links.second) {
                auto dst_dev = link_bw.first;
                auto route = Route();
                route.push_back(devices[src_dev]);
                route.push_back(devices[dst_dev]);
                default_routes[src_dev][dst_dev] = route;
            }
        }
    } else {
        for (auto& node_links : _routes) {
            auto src_dev = node_links.first;
            for (auto& path : node_links.second) {
                auto dst_dev = path.first;
                auto route = Route();
                for (auto& hop : path.second) {
                    route.push_back(devices[hop]);
                }
                default_routes[src_dev][dst_dev] = route;
            }
        }
    }
}

Route CustomTopology::route(DeviceId src, DeviceId dest) const noexcept {
    // assert npus are in valid range
    assert(0 <= src && src < num_devices);
    assert(0 <= dest && dest < num_devices);

    return default_routes.at(src).at(dest);
}
