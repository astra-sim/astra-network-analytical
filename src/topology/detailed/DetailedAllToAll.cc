/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "DetailedAllToAll.hh"

using namespace Analytical;

DetailedAllToAll::DetailedAllToAll(TopologyConfigs configs) noexcept :
    DetailedTopology(configs) {
    // connect all (src, dest) pair
    for (int src = 0; src < npus_count; src++) {
        for (int dest = 0; dest < npus_count; dest++) {
            if (src == dest) {
                continue;
            }

            // AllToAll has only one dimension
            connect(src, dest, 0);
        }
    }
}

DetailedAllToAll::~DetailedAllToAll() noexcept = default;

double DetailedAllToAll::send(NpuId src, NpuId dest, PayloadSize payload_size) noexcept {
    // check NPU ID validity
    checkNpuIdBound(src);
    checkNpuIdBound(dest);

    // Check src and dest differs
    if (src == dest) {
        return 0;
    }

    // directly send from src to dest
    auto communication_latency = transmit(src, dest, payload_size);
    communication_latency += serializationLatency(0, payload_size);
    communication_latency += 2 * nicLatency(0);

    auto hbm_latency = hbmLatency(0, payload_size);

    return criticalLatency(communication_latency, hbm_latency);
}
