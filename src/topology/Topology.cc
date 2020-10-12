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

#include "Topology.hh"
#include <cassert>

using namespace Analytical;

void Topology::connect(NpuId src_id, NpuId dest_id, int dimension) noexcept {
    assert((dimension < configurations.size()) && "[Topology, method connect] dimension out of bound");
    auto configuration = configurations[dimension];

    auto link_latency = configuration.getLinkLatency();
    auto link_bandwidth = configuration.getLinkBandwidth();

    links[src_id][dest_id] = Link(link_latency, link_bandwidth);
}

Topology::Latency Topology::route(NpuId src_id, NpuId dest_id, PayloadSize payload_size) noexcept {
    assert((links.find(src_id) != links.end()) && "[Topology, method route] src doesn't have any outgoing link");
    assert((links[src_id].find(dest_id) != links[src_id].end())
           && "[Topology, method route] link src->dest doesn't exist");
    return links[src_id][dest_id].send(payload_size);
}

Topology::Latency Topology::routerLatency(int dimension) const noexcept {
    assert((dimension < configurations.size()) && "[Topology, method routerLatency] dimension out of bound");
    return configurations[dimension].getRouterLatency();
}

Topology::Latency Topology::nicLatency(int dimension) const noexcept {
    assert((dimension < configurations.size()) && "[Topology, method nicLatency] dimension out of bound");
    return configurations[dimension].getNicLatency();
}

Topology::Latency Topology::hbmLatency(PayloadSize payload_size, int dimension) const noexcept {
    assert((dimension < configurations.size()) && "[Topology, method hbmLatency] dimension out of bound");
    auto configuration = configurations[dimension];

    auto hbm_latency = configuration.getHbmLatency();
    auto hbm_bandwidth = configuration.getHbmBandwidth();
    auto hbm_scalar = configuration.getHbmScalar();

    auto latency = hbm_latency + (payload_size / hbm_bandwidth);
    return hbm_scalar * latency;
}

Topology::Latency Topology::criticalLatency(Latency link_latency, Latency hbm_latency) noexcept {
    if (link_latency >= hbm_latency) {
        // communication bound
        communication_bounds_count++;
        return link_latency;
    }

    // hbm bound
    hbm_bounds_count++;
    return hbm_latency;
}
