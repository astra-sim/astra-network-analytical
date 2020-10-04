#include <cassert>
#include <algorithm>
#include "Ring.hh"

using namespace AnalyticalBackend;

Ring::Ring(TopologyConfiguration configuration, int nodes_count, bool is_bidirectional) noexcept :
        configuration(configuration),
        nodes_count(nodes_count),
        is_bidirectional(is_bidirectional) {
    half_nodes_count = nodes_count / 2;
}

Topology::Latency Ring::simulateSend(NodeID src, NodeID dest, PayloadSize payload_size) noexcept {
    // compute hops_count
    auto hops_count = -1;

    // compute clockwise distance
    auto clockwise_distance = (dest - src) % nodes_count;
    if (clockwise_distance < 0) {
        // distance negative means src->dest passes node 0: compensate this by adding nodes_count
        clockwise_distance = clockwise_distance + nodes_count;
    }

    if (is_bidirectional) {
        // counterclockwise distance may be shorter
        if (clockwise_distance <= half_nodes_count) {
            // clockwise is shorter
            hops_count = clockwise_distance;
        } else {
            // counterclockwise is shorter
            hops_count = (nodes_count - clockwise_distance);
        }
    } else {
        // not bidirectional: always goes clockwise
        hops_count = clockwise_distance;
    }

    assert(hops_count >= 0 && "[Ring] Routing error: hops count cannot be negative");


    // compute communication latency
    auto comm_latency = hops_count * configuration.getLinkLatency();  // Link delay
    comm_latency += payload_size / configuration.getBandwidth();  // Serialization delay
    comm_latency += configuration.getRouterLatency();  // Switch delay
    if (configuration.getNIC_Enabled()) {
        comm_latency += 2 * configuration.getNIC_Latency();  // NIC delay when nic is enabled
    }

    // compute hbm delay
    auto hbm_latency = configuration.getHBM_Latency();  // HBM delay
    hbm_latency += payload_size / configuration.getHBM_Bandwidth();  // HBM serialization delay
    hbm_latency *= configuration.getHBM_Scale();  // Scale HBM by scaling factor

    auto latency = std::max((int)comm_latency, (int)hbm_latency);
    assert(latency >= 0 && "[Ring] Latency error: latency cannot be negative");

    return latency;
}
