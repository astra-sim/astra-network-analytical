/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "FastTorus2D.hh"
#include "cmath"

using namespace Analytical;

FastTorus2D::FastTorus2D(TopologyConfigs configs, CostModel cost_model) noexcept :
        FastTopology(configs, cost_model) { }

FastTorus2D::~FastTorus2D() noexcept = default;

double FastTorus2D::send(NpuId src, NpuId dest, PayloadSize payload_size) noexcept {
    // check NPU ID validity
    checkNpuIdBound(src);
    checkNpuIdBound(dest);

    // translate into address
    auto src_address = npuIdToAddress(src);
    auto dest_address = npuIdToAddress(dest);

    // get width and height
    auto width = configs[0].getNpusCount();
    auto height = configs[1].getNpusCount();

    // compute communication latency
    // FIXME: assume bidirectional ring (always takes shortest path)
    auto communication_latency = 0.0;

    // width-directional routing
    if (src_address[0] != dest_address[0]) {
        auto distanceA = std::abs(src_address[0] - dest_address[0]);
        auto distanceB = width - distanceA;
        auto hops_count = (distanceA < distanceB) ? distanceA : distanceB;  // shorter one becomes the hop count
        communication_latency += linkLatency(0, hops_count);
    }

    // height-directional routing
    if (src_address[1] != dest_address[1]) {
        auto distanceA = std::abs(src_address[1] - dest_address[1]);
        auto distanceB = height - distanceA;
        auto hops_count = (distanceA < distanceB) ? distanceA : distanceB;  // shorter one becomes the hop count
        communication_latency += linkLatency(1, hops_count);
    }

    // add serialization, nic, router latency
    // FIXME: assuming dim[0] = dim[1] thus using only dim[0]
    communication_latency += serializationLatency(0, payload_size);
    communication_latency += 2 * nicLatency(0);  // passes 2 NICs: input and output

    // compute hbm latency
    // FIXME: assuming hbm[0] = hbm[1] thus using only hbm[0]
    auto hbm_latency = hbmLatency(0, payload_size);

    // return critical latency
    return criticalLatency(communication_latency, hbm_latency);
}

FastTorus2D::NpuAddress FastTorus2D::npuIdToAddress(NpuId npu_id) const noexcept {
    auto width = configs[0].getNpusCount();
    auto height_id = npu_id / width;
    auto width_id = npu_id % width;

    auto npu_address = NpuAddress();
    npu_address.emplace_back(width_id);
    npu_address.emplace_back(height_id);

    return npu_address;
}

FastTorus2D::NpuId FastTorus2D::npuAddressToId(NpuAddress npu_address) const noexcept {
    auto width = configs[0].getNpusCount();
    return npu_address[0] + (width * npu_address[1]);
}
