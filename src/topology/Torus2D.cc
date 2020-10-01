#include "Torus2D.hh"
#include "assert.h"

#include <iostream>
using namespace AnalyticalBackend;

double Torus2D::sendAndGetLatency(NodeAddress src, NodeAddress dest, int payload_size) noexcept {
    auto srcNodeID = src[0];
    auto destNodeID = dest[0];

    auto currentID = nodeIDtoTorus2dNodeID(srcNodeID);
    auto destID = nodeIDtoTorus2dNodeID(destNodeID);

    // compute hop count
    auto hops_count = 0;

    // take step to x direction
    auto step = computeStepDirection(currentID, destID);

    while (!is_xID_Equal(currentID, destID)) {
        currentID = takeStep(currentID, step);
        hops_count++;
    }

    // take step to y direction
    step = computeStepDirection(currentID, destID);
    while (!is_yID_Equal(currentID, destID)) {
        currentID = takeStep(currentID, step);
        hops_count++;
    }

    assert(currentID == destID && "After xy routing, should reach destination");

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

bool Torus2D::is_xID_Equal(Torus2DNodeID srcID, Torus2DNodeID destID) const noexcept {
    return std::get<1>(srcID) == std::get<1>(destID);
}

bool Torus2D::is_yID_Equal(Torus2DNodeID srcID, Torus2DNodeID destID) const noexcept {
    return std::get<0>(srcID) == std::get<0>(destID);
}

Torus2D::Torus2DNodeID Torus2D::nodeIDtoTorus2dNodeID(NodeID nodeID) const noexcept {
    auto yID = nodeID / width;
    auto xID = nodeID % width;

    return std::make_tuple(yID, xID);
}

Torus2D::StepDirection1D Torus2D::computeStepDirection1D(NodeID srcID, NodeID destID) const noexcept {
    int half_width = width / 2;
    if (srcID <= half_width) {
        // check can be reached clockwise
        return (srcID + 1) <= destID && destID <= (srcID + half_width) ? 1 : -1;
    }

    // check can be reached counterclockwise
    return (srcID - half_width) <= destID && destID <= (srcID - 1) ? -1 : 1;
}

Torus2D::StepDirection2D Torus2D::computeStepDirection(Torus2DNodeID srcID, Torus2DNodeID destID) const noexcept {
    auto src_yID = -1;
    auto src_xID = -1;
    std::tie(src_yID, src_xID) = srcID;

    auto dest_yID = -1;
    auto dest_xID = -1;
    std::tie(dest_yID, dest_xID) = destID;

    // xy routing
    if (src_xID != dest_xID) {
        // move x direction first
        auto x_direction = computeStepDirection1D(src_xID, dest_xID);
        return std::make_tuple(0, x_direction);
    }

    // move y direction
    auto y_direction = computeStepDirection1D(src_yID, dest_yID);
    return std::make_tuple(y_direction, 0);
}

Torus2D::Torus2DNodeID Torus2D::takeStep(Torus2DNodeID currentID, StepDirection2D step) const noexcept {
    auto next_yID = -1;
    auto next_xID = -1;
    std::tie(next_yID, next_xID) = currentID;

    auto step_y = 0;
    auto step_x = 0;
    std::tie(step_y, step_x) = step;

    next_xID += step_x;
    next_yID += step_y;

    // Torus circulates
    if (next_xID >= width) {
        next_xID %= width;
    } else if (next_xID < 0) {
        next_xID = width + (next_xID % width);
    }

    if (next_yID >= width) {
        next_yID %= width;
    } else if (next_yID < 0) {
        next_yID = width + (next_yID % width);
    }

    return std::make_tuple(next_yID, next_xID);
}
