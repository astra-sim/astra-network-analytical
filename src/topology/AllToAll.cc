#include "AllToAll.hh"

using namespace AnalyticalBackend;

double AllToAll::sendAndGetLatency(NodeAddress src, NodeAddress dest, int payload_size) noexcept {
    auto srcID = src[0];
    auto destID = dest[0];  // FIXME: IDs not used, use it when logging is required

    const auto hops_count = 1;  // Glueless All-to-All

    // Compute High Bandwidth Memory latency
    double memory_latency = configuration.getMemoryLatency();  // Memory Latency
    memory_latency += payload_size / configuration.getMemoryBandwidth();  // Memory bandwidth
    memory_latency *= configuration.getMemoryScale();  // Scale memory latency

    // Compute network communication latency
    double network_latency = hops_count * configuration.getLinkLatency();  // Link latency
    network_latency += payload_size / configuration.getLinkBandwidth();  // Link Bandwidth
    network_latency += 2 * configuration.getNICLatency();  // NIC latency

    // Return maximum latency, as HBM and communication is overlapped
    return (int)std::max(memory_latency, network_latency);
}
