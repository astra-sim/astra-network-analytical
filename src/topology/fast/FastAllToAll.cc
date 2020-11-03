/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "FastAllToAll.hh"

using namespace Analytical;

FastAllToAll::FastAllToAll(TopologyConfigs configs) noexcept :
        FastTopology(configs) { }

FastAllToAll::~FastAllToAll() noexcept = default;

double FastAllToAll::send(NpuId src, NpuId dest, PayloadSize payload_size) noexcept {
    checkNpuIdBound(src);
    checkNpuIdBound(dest);

    // Glueless all-to-all topology: hop count is 1
    const auto hops_count = 1;

    // compute communication latency
    // dimension is 0
    auto communication_latency = linkLatency(0, hops_count);
    communication_latency += serializationLatency(0, payload_size);
    communication_latency += 2 * nicLatency(0);  // passes 2 NICs: input and output

    // compute hbm latency
    auto hbm_latency = hbmLatency(0, payload_size);

    // return critical latency
    return criticalLatency(communication_latency, hbm_latency);
}
