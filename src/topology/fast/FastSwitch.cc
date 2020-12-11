/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "FastSwitch.hh"

using namespace Analytical;

FastSwitch::FastSwitch(TopologyConfigs configs, CostModel& cost_model) noexcept :
        FastTopology(configs, cost_model) {
    auto link_latency = configs[0].getLinkLatency();
    auto link_bandwidth = configs[0].getLinkBandwidth();

    cost_model.createLink(2 * npus_count, link_latency, link_bandwidth);
    cost_model.createNpu(npus_count, 2);
    cost_model.createSwitch(1, 2 * npus_count);
}

FastSwitch::~FastSwitch() noexcept = default;

double FastSwitch::send(NpuId src, NpuId dest, PayloadSize payload_size) noexcept {
    checkNpuIdBound(src);
    checkNpuIdBound(dest);

    // switch topology: hop count is 2
    const auto hops_count = 2;

    // compute communication latency
    // dimension is 0
    auto communication_latency = linkLatency(0, hops_count);
    communication_latency += serializationLatency(0, payload_size);
    communication_latency += 2 * nicLatency(0);  // passes 2 NICs: input and output
    communication_latency += routerLatency(0);

    // compute hbm latency
    auto hbm_latency = hbmLatency(0, payload_size);

    // return critical latency
    return criticalLatency(communication_latency, hbm_latency);
}
