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

#include "Switch.hh"

using namespace Analytical;


Switch::Switch(const std::vector<TopologyConfiguration>& configurations, int npus_count) noexcept {
    this->configurations = configurations;

    // set a switch id
    switch_id = npus_count;

    // 1. Connect all NPUs to a switch: input port
    // 2. Connect the switch to all NPUs: output port
    for (auto npu_id = 0; npu_id < npus_count; npu_id++) {
        connect(npu_id, switch_id, 0);  // input port
        connect(switch_id, npu_id, 0);  // output port
    }
}

Topology::Latency Switch::send(NpuId src_id, NpuId dest_id, PayloadSize payload_size) noexcept {
    // Switch routing scheme
    //      1. pass source nic
    //      2. move from src to switch
    //      3. add switch delay
    //      4. move from switch to dest
    //      5. pass destination nic

    auto latency = nicLatency(0);
    latency += route(src_id, switch_id, payload_size);
    latency += routerLatency(0);
    latency += route(switch_id, dest_id, payload_size);
    latency += nicLatency(0);

    return latency;
}

Topology::NpuAddress Switch::npuIdToAddress(NpuId id) const noexcept {
    return NpuAddress(1, id);
}

Topology::NpuId Switch::npuAddressToId(const NpuAddress& address) const noexcept {
    return address[0];
}
