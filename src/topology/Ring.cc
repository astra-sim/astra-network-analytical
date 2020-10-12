/******************************************************************************
Copyright (c) 2020 Georgia Institute of Technology
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

Author : William Won (william.won@gatech.edu)
*******************************************************************************/

#include "Ring.hh"

using namespace Analytical;

Ring::Ring(const TopologyConfigurations& configurations, int npus_count, bool bidirectional) noexcept :
        npus_count(npus_count),
        half_npus_count(npus_count / 2),
        bidirectional(bidirectional) {
    this->configurations = configurations;

    auto n1 = -1;
    auto n2 = -1;

    // 1. Unidirectional ring
    for (n1 = 0; n1 < (npus_count - 1); n1++) {
        n2 = n1 + 1;
        connect(n1, n2, 0);
    }
    n1 = npus_count - 1;
    n2 = 0;
    connect(n1, n2, 0);

    // 2. Bidirectional ring
    if (bidirectional) {
        for (n1 = npus_count - 1; n1 > 0; n1--) {
            n2 = n1 - 1;
            connect(n1, n2, 0);
        }
        n1 = 0;
        n2 = npus_count - 1;
        connect(n1, n2, 0);
    }
}

Topology::Latency Ring::send(NpuId src_id, NpuId dest_id, PayloadSize payload_size) noexcept {
    // compute which direction to move
    auto direction = computeDirection(src_id, dest_id);

    // serialize packet
    auto link_latency = serialize(payload_size, 0);
    link_latency += nicLatency(0);

    // move towards direction until reaching destination
    auto current_id = src_id;
    while (current_id != dest_id) {
        auto next_id = takeStep(current_id, direction);

        link_latency += route(current_id, next_id, payload_size);
        current_id = next_id;
    }

    link_latency += nicLatency(0);

    auto hbm_latency = hbmLatency(payload_size, 0);

    return criticalLatency(link_latency, hbm_latency);
}

Topology::NpuAddress Ring::npuIdToAddress(NpuId id) const noexcept {
    return NpuAddress(1, id);
}

Topology::NpuId Ring::npuAddressToId(const NpuAddress& address) const noexcept {
    return address[0];
}

Ring::Direction Ring::computeDirection(NpuId src_id, NpuId dest_id) const noexcept {
    if (!bidirectional) {
        // unidirectional
        return 1;
    }

    // bidirectional: compute shortest path
    if (src_id < dest_id) {
        auto distance = dest_id - src_id;
        return (distance <= half_npus_count) ? 1 : -1;
    }

    auto distance = src_id - dest_id;
    return (distance <= half_npus_count) ? -1 : 1;
}

Ring::NpuId Ring::takeStep(NpuId current_id, Direction direction) const noexcept {
    // compute next id
    auto next_id = current_id + direction;

    if (next_id >= npus_count) {
        // out of positive bounds
        next_id %= npus_count;
    } else if (next_id < 0) {
        // out of negative bounds
        next_id = npus_count + (next_id % npus_count);
    }

    return next_id;
}
