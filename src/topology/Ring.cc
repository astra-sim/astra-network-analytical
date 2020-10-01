#include "Ring.hh"

using namespace AnalyticalBackend;

double Ring::sendAndGetLatency(NodeAddress src, NodeAddress dest, int payload_size) noexcept {
    auto currentID = src[0];
    auto destID = dest[0];

    // compute the number of hops required
    auto hops_count = 0;

    // take steps while reaching the destination
    auto step = computeStepDirection(currentID, destID);
    while (currentID != destID) {
        currentID = takeStep(currentID, step);
        hops_count++;
    }

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

Ring::StepDirection Ring::computeStepDirection(NodeID srcID, NodeID destID) {
    if (direction == Direction::Clockwise) {
        return 1;
    }

    if (direction == Direction::Counterclockwise) {
        return -1;
    }

    // Bidirectional
    int half_node_index = nodes_count / 2;
    if (srcID <= half_node_index) {
        // check can be reached clockwise
        return (srcID + 1) <= destID && destID <= (srcID + half_node_index) ? 1 : -1;
    }

    // check can be reached counterclockwise
    return (srcID - half_node_index) <= destID && destID <= (srcID - 1) ? -1 : 1;
}

Ring::NodeID Ring::takeStep(NodeID currentID, StepDirection step) {
    auto nextID = currentID + step;

    // Ring circulates
    if (nextID >= nodes_count) {
        nextID %= nodes_count;
    } else if (nextID < 0) {
        nextID = nodes_count + (nextID % nodes_count);
    }

    return nextID;
}
