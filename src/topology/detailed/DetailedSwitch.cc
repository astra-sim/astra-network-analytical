/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "DetailedSwitch.hh"

using namespace Analytical;

DetailedSwitch::DetailedSwitch(TopologyConfigs configs) noexcept :
        DetailedTopology(configs) {
    // set switch id
    switch_id = npus_count;

    // connect all (src, switch) pair
    for (int src = 0; src < npus_count; src++) {
        connect(src, switch_id, 0);
    }

    // connect all (switch, dest) pair
    for (int dest = 0; dest < npus_count; dest++) {
        connect(switch_id, dest, 0);
    }
}

DetailedSwitch::~DetailedSwitch() noexcept = default;

double DetailedSwitch::send(NpuId src, NpuId dest, PayloadSize payload_size) noexcept {
    // check NPU ID validity
    checkNpuIdBound(src);
    checkNpuIdBound(dest);

    // Check src and dest differs
    if (src == dest) {
        return 0;
    }

    // send from src to switch
    // then switch to dest
    auto communication_latency = transmit(src, switch_id, payload_size);
    communication_latency += transmit(switch_id, dest, payload_size);

    // serialization and nic
    communication_latency += serializationLatency(0, payload_size);
    communication_latency += 2 * nicLatency(0);

    // hbm
    auto hbm_latency = hbmLatency(0, payload_size);

    return criticalLatency(communication_latency, hbm_latency);
}
