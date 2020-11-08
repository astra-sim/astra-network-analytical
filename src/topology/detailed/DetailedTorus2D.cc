/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include <cmath>
#include <iostream>
#include "DetailedTorus2D.hh"

using namespace Analytical;

DetailedTorus2D::DetailedTorus2D(TopologyConfigs configs) noexcept :
        DetailedTopology(configs) {
    // FIXME: assume the ring is bidirectional
    width = configs[0].getNpusCount();
    height = configs[1].getNpusCount();

    // connect horizontal rings
    for (auto h = 0; h < height; h++) {
        for (auto w = 0; w < width - 1; w++) {
            // connect 0<->1, 1<->2, ..., (end-2)<->(end-1)
            auto src = (width * h) + w;
            auto dest = (width * h) + w + 1;
            connect(src, dest, 0);
            connect(dest, src, 0);
        }

        // connect (end-1) <-> 0
        if (width > 2) {
            auto src = (width * h) + (width - 1);
            auto dest = (width * h);
            connect(src, dest, 0);
            connect(dest, src, 0);
        }
    }

    // connect vertical rings
    for (auto w = 0; w < width; w++) {
        for (auto h = 0; h < height - 1; h++) {
            // connect 0<->1, 1<->2, ..., (end-2)<->(end-1)
            auto src = (width * h) + w;
            auto dest = (width * (h + 1)) + w;
            connect(src, dest, 1);
            connect(dest, src, 1);
        }

        // connect (end-1) <-> 0
        if (height > 2) {
            auto src = (width * (height - 1)) + w;
            auto dest = w;
            connect(src, dest, 1);
            connect(dest, src, 1);
        }
    }
}

DetailedTorus2D::~DetailedTorus2D() noexcept = default;

double DetailedTorus2D::send(NpuId src, NpuId dest, PayloadSize payload_size) noexcept {
    // check NPU ID validity
    checkNpuIdBound(src);
    checkNpuIdBound(dest);

    // Check src and dest differs
    if (src == dest) {
        return 0;
    }

    // address
    auto current_address = npuIdToAddress(src);
    auto dest_address = npuIdToAddress(dest);

    auto communication_latency = 0.0;

    // compute x direction
    auto direction = computeDirection(current_address[0], dest_address[0], width);

    // move x direction
    while (current_address[0] != dest_address[0]) {
        // compute current id
        auto current_id = npuAddressToId(current_address);

        // compute next_address
        current_address[0] = takeStep(current_address[0], direction, width);
        auto next_id = npuAddressToId(current_address);

        // transmit to next npu
        communication_latency += transmit(current_id, next_id, payload_size);
        communication_latency += 2 * nicLatency(0);
    }

    // compute y direction
    direction = computeDirection(current_address[1], dest_address[1], height);

    // move y direction
    while (current_address[1] != dest_address[1]) {
        // compute current id
        auto current_id = npuAddressToId(current_address);

        // compute next_address
        current_address[1] = takeStep(current_address[1], direction, height);
        auto next_id = npuAddressToId(current_address);

        // transmit to next npu
        communication_latency += transmit(current_id, next_id, payload_size);
        communication_latency += 2 * nicLatency(1);
    }

    // serialization delay
    // FIXME: assuming dim0 = dim1, so using dim0 here
    communication_latency += serializationLatency(0, payload_size);
    std::cout << "latency: " << communication_latency << std::endl;

    // hbm
    // FIXME: assuming dim0 = dim1, so using dim0 here
    auto hbm_latency = hbmLatency(0, payload_size);

    return criticalLatency(communication_latency, hbm_latency);
}

DetailedTorus2D::Direction DetailedTorus2D::computeDirection(NpuId src, NpuId dest, int ring_size) {
    // for explanation, assume ring_size = 10
    auto natural_distance = std::abs(src - dest);  // 1->2->3->4 (src < dest, +1) or 9->8->7 (src > dest, -1)
    auto reverse_distance = ring_size - natural_distance;  // 0->9->8 (src < dest, -1) or 8->9->0->1 (src > dest, +1)

    if (natural_distance <= reverse_distance) {
        return (src < dest) ? 1 : -1;
    }

    return (src < dest) ? -1 : 1;
}

DetailedTorus2D::NpuId DetailedTorus2D::takeStep(NpuId current_node, Direction direction, int ring_size) {
    // take a step
    auto next_node = current_node + direction;

    // ring operation
    if (next_node < 0) {
        next_node += ring_size;
    } else if (next_node >= ring_size) {
        next_node -= ring_size;
    }

    return next_node;
}

DetailedTorus2D::NpuAddress DetailedTorus2D::npuIdToAddress(NpuId npu_id) const noexcept {
    auto height_id = npu_id / width;
    auto width_id = npu_id % width;

    auto npu_address = NpuAddress();
    npu_address.emplace_back(width_id);
    npu_address.emplace_back(height_id);

    return npu_address;
}

DetailedTorus2D::NpuId DetailedTorus2D::npuAddressToId(NpuAddress npu_address) const noexcept {
    return npu_address[0] + (width * npu_address[1]);
}
