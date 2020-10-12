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

#include <cmath>
#include <cassert>
#include "Torus2D.hh"

using namespace Analytical;

Torus2D::Torus2D(const TopologyConfigurations& configurations, int npus_count) noexcept {
    this->configurations = configurations;

    width = (int)std::sqrt(npus_count);
    half_width = width / 2;
    assert(width * width == npus_count && "[Torus2D, constructor] Currently Torus2D only supports square");

    // connect each row
    for (int row = 0; row < width; row++) {
        // iterate over NPUs, except for last column
        for (int col = 0; col < (width - 1); col++) {
            auto n1 = (row * width) + col;
            auto n2 = n1 + 1;  // node on right
            connect(n1, n2, 0);
            connect(n2, n1, 0);
        }

        // last column NPU
        auto n1 = (row * width) + (width - 1);
        auto n2 = row * width;
        connect(n1, n2, 0);
        connect(n2, n1, 0);
    }

    // connect each column
    for (int col = 0; col < width; col++) {
        // iterate over NPUs, except for top row
        for (int row = 0; row < (width - 1); row++) {
            auto n1 = (row * width) + col;
            auto n2 = n1 + width;  // node on top
            connect(n1, n2, 0);
            connect(n2, n1, 0);
        }

        // top column NPU
        auto n1 = (width - 1) * width + col;
        auto n2 = col;
        connect(n1, n2, 0);
        connect(n2, n1, 0);
    }
}

Topology::Latency Torus2D::send(NpuId src_id, NpuId dest_id, PayloadSize payload_size) noexcept {
    if (src_id == dest_id) {
        // guard statement
        return 0;
    }

    auto src_row = src_id / width;
    auto src_col = src_id % width;
    auto dest_row = dest_id / width;
    auto dest_col = dest_id % width;

    // xy routing
    // TODO: finish xy routing
    return 0;
//    auto direction = 0;
//
//    auto hbm_latency = hbmLatency(0);
//
//    return criticalLatency(link_latency, hbm_latency);
}

Topology::NpuAddress Torus2D::npuIdToAddress(NpuId id) const noexcept {
    return NpuAddress(1, id);
}

Topology::NpuId Torus2D::npuAddressToId(const NpuAddress& address) const noexcept {
    return address[0];
}
