/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include <cassert>
#include "Topology.hh"

using namespace Analytical;

Topology::Topology(TopologyConfigs configs) noexcept :
        configs(configs) {
    // get total npus_count by multiplying npus_count of each dimension
    npus_count = 1;
    for (const auto& config : configs) {
        npus_count *= config.getNpusCount();
    }
}

Topology::~Topology() noexcept = default;

void Topology::checkNpuIdBound(NpuId npu_id) const noexcept {
    assert(npu_id < npus_count && "[Topology, method checkNpuIdBound] NPU ID out of bounds");
}
