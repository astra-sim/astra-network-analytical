// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2026 Advanced Micro Devices, Inc.

#include <cassert>
#include <astra-network-analytical/common/NetworkFunction.h>
#include <astra-network-analytical/congestion_unaware/CustomTopology.h>

using namespace NetworkAnalytical;
using namespace NetworkAnalyticalCongestionUnaware;

CustomTopology::CustomTopology(
    const int npus_count,
    const int num_devices,
    const std::map<int, std::map<int, Bandwidth>> bandwidths,
    const std::map<int, std::map<int, Latency>> latencies) noexcept
    : Topology() {
    assert(npus_count > 0);

    // set topology shape
    this->npus_count = npus_count;
    npus_count_per_dim.push_back(npus_count);
    dims_count = 1;
    this->num_devices = num_devices;

    // Create default link matrices
    for (int i = 0; i < num_devices; i++) {
        bandwidth_per_link.push_back(std::vector<Bandwidth>(num_devices, 0));
        latency_per_link.push_back(std::vector<Latency>(num_devices, 0));
    }

    // Fill bandwidth matrix
    for (auto& node_links : bandwidths) {
        for (auto& link_bw : node_links.second) {
            // adjust for from GiB/s to B/ps
            Bandwidth bandwidth_Bpps = bw_GiBps_to_Bpps(link_bw.second);
            bandwidth_per_link[node_links.first][link_bw.first] =
                bandwidth_Bpps;
            // bandwidth_per_link[node_links.first][link_bw.first] =
            // link_bw.second;
        }
    }

    // Fill latency matrix
    for (auto& node_links : latencies) {
        for (auto& link_bw : node_links.second) {
            // from ns to ps
            Latency latency_ps = latency_ns_to_ps(link_bw.second);
            latency_per_link[node_links.first][link_bw.first] = latency_ps;
            // latency_per_link[node_links.first][link_bw.first] =
            // link_bw.second;
        }
    }

    bandwidth_per_dim = std::vector<Bandwidth>(dims_count, 1);
}

// default destructor
CustomTopology::~CustomTopology() noexcept = default;

EventTime CustomTopology::send(const DeviceId src,
                               const DeviceId dest,
                               const ChunkSize chunk_size) const noexcept {
    // assert(0 <= src && src < npus_count);
    // assert(0 <= dest && dest < npus_count);
    assert(0 <= src && src < num_devices);
    assert(0 <= dest && dest < num_devices);
    assert(src != dest);
    assert(chunk_size > 0);

    // return communication delay
    return compute_communication_delay(src, dest, 0, chunk_size);
}

EventTime CustomTopology::receive(const DeviceId src,
                                  const DeviceId dest,
                                  const ChunkSize chunk_size) const noexcept {
    // assert(0 <= src && src < npus_count);
    // assert(0 <= dest && dest < npus_count);
    assert(0 <= src && src < num_devices);
    assert(0 <= dest && dest < num_devices);
    assert(src != dest);
    assert(chunk_size > 0);

    // return communication delay
    return compute_communication_delay(src, dest, 1, chunk_size);
}

EventTime CustomTopology::compute_communication_delay(
    const DeviceId src,
    const DeviceId dest,
    int hops,
    const ChunkSize chunk_size) const noexcept {
    assert(chunk_size > 0);
    assert(hops >= 0);

    // compute link delay and serialization delay
    auto link_delay = latency_per_link[src][dest];
    auto link_bw = bandwidth_per_link[src][dest];
    if (__builtin_expect(link_bw <= 0, 0)) {
        std::cerr
            << "[Error] (network/analytical) Transmission over a link with  "
            << " Bandwidth equal to " << link_bw << std::endl;
        std::exit(-1);
    }

    auto serialization_delay = static_cast<double>(chunk_size) / link_bw;

    // comms_delay is the summation of the two
    auto comms_delay = link_delay * hops + serialization_delay;

    // return EventTime type of comms_delay
    auto ret_comm_daley = static_cast<EventTime>(comms_delay);
    return ret_comm_daley;
    // return static_cast<EventTime>(comms_delay);
}

TopologyBuildingBlock CustomTopology::get_basic_topology_type() const noexcept {
    return TopologyBuildingBlock::Custom;
}
