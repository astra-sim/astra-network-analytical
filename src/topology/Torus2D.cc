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

#include <cassert>
#include <algorithm>
#include <cmath>
#include "Torus2D.hh"

using namespace AnalyticalBackend;

Torus2D::Torus2D(TopologyConfiguration configuration, int nodes_count) noexcept :
        configuration(configuration),
        nodes_count(nodes_count) {
    width = std::sqrt(nodes_count);
    half_width = width / 2;
}

Topology::Latency Torus2D::simulateSend(NodeID src, NodeID dest, PayloadSize payload_size) noexcept {
    // compute hops_count
    auto hops_count = -1;

    // compute coordinates
    auto src_x = src % width;
    auto src_y = src / width;
    auto dest_x = dest % width;
    auto dest_y = dest / width;

    // compute x distance
    auto distance_x = std::abs(dest_x - src_x) % width;
    if (distance_x > half_width) {
        distance_x = width - distance_x;
    }

    // compute y distance
    auto distance_y = std::abs(dest_y - src_y) % width;
    if (distance_y > half_width) {
        distance_y = width - distance_y;
    }

    // hops_count = x_dist + y_dist
    hops_count = distance_x + distance_y;
    assert(hops_count >= 0 && "[Torus2D] Routing error: hops count cannot be negative");


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
    assert(latency >= 0 && "[Torus2D] Latency error: latency cannot be negative");

    return latency;
}
