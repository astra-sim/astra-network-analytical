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

#include "AllToAll.hh"

using namespace Analytical;

AllToAll::AllToAll(const std::vector<TopologyConfiguration> &configurations, int npus_count) noexcept {
    this->configurations = configurations;

    // connect all npus directly
    for (int n1 = 0; n1 < (npus_count - 1); n1++) {
        for (int n2 = (n1 + 1); n2 < npus_count; n2++) {
            connect(n1, n2, 0);
            connect(n2, n1, 0);
        }
    }
}

Topology::Latency AllToAll::send(NpuId src_id, NpuId dest_id, PayloadSize payload_size) noexcept {
    // 1. Source nic latency
    // 2. route packet from src to dest
    // 3. Dest nic latency
    auto link_latency = nicLatency(0);
    link_latency += route(src_id, dest_id, payload_size);
    link_latency += nicLatency(0);

    auto hbm_latency = hbmLatency(payload_size, 0);

    return criticalLatency(link_latency, hbm_latency);
}

Topology::NpuAddress AllToAll::npuIdToAddress(NpuId id) const noexcept {
    return NpuAddress(1, id);
}

Topology::NpuId AllToAll::npuAddressToId(const NpuAddress& address) const noexcept {
    return address[0];
}
