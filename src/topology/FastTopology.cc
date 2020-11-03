/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "FastTopology.hh"

using namespace Analytical;

FastTopology::FastTopology(TopologyConfigs configs) noexcept :
        Topology(configs) { }

FastTopology::~FastTopology() noexcept = default;

FastTopology::Latency FastTopology::linkLatency(int dimension, int hops_count) const noexcept {
    return hops_count * configs[dimension].getLinkLatency();
}

FastTopology::Latency FastTopology::serializationLatency(int dimension, PayloadSize payload_size) const noexcept {
    return payload_size / configs[dimension].getLinkBandwidth();
}

FastTopology::Latency FastTopology::nicLatency(int dimension) const noexcept {
    return configs[dimension].getNicLatency();
}

FastTopology::Latency FastTopology::routerLatency(int dimension) const noexcept {
    return configs[dimension].getRouterLatency();
}

FastTopology::Latency FastTopology::hbmLatency(int dimension, PayloadSize payload_size) const noexcept {
    auto hbm_latency = configs[dimension].getHbmLatency();  // HBM baseline latency
    hbm_latency += payload_size / configs[dimension].getHbmBandwidth();  // HBM serialization latency
    return hbm_latency * configs[dimension].getHbmScale();  // return scaled HBM latency
}

FastTopology::Latency FastTopology::criticalLatency(Latency communication_latency, Latency hbm_latency) const noexcept {
    // return larger latency
    return (communication_latency > hbm_latency) ? communication_latency : hbm_latency;
}
