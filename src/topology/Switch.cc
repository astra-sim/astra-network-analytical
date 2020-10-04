#include <cassert>
#include <algorithm>
#include "Switch.hh"

using namespace AnalyticalBackend;

Switch::Switch(TopologyConfiguration configuration, int nodes_count) noexcept :
        configuration(configuration),
        nodes_count(nodes_count) { }

Topology::Latency Switch::simulateSend(NodeID src, NodeID dest, PayloadSize payload_size) noexcept {
    const auto hops_count = 2;  // Always 2

    // compute communication latency
    auto comm_latency = hops_count * configuration.getLinkLatency();  // Link delay
    comm_latency += payload_size / configuration.getBandwidth();  // Serialization delay
    comm_latency += configuration.getRouterLatency();  // Switch delay
    comm_latency += 2 * configuration.getNIC_Latency();  // NIC delay

    // compute hbm delay
    auto hbm_latency = configuration.getHBM_Latency();  // HBM delay
    hbm_latency += payload_size / configuration.getHBM_Bandwidth();  // HBM serialization delay
    hbm_latency *= configuration.getHBM_Scale();  // Scale HBM by scaling factor

    auto latency = std::max((int)comm_latency, (int)hbm_latency);
    assert(latency >= 0 && "[Switch] Latency error: latency cannot be negative");

    return latency;
}
