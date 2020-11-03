/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#ifndef __TOPOLOGY_HH__
#define __TOPOLOGY_HH__

#include <vector>
#include "TopologyConfig.hh"

namespace Analytical {
    class Topology {
    public:
        using TopologyConfigs = std::vector<TopologyConfig>;

        using NpuId = TopologyConfig::NpuId;
        using PayloadSize = TopologyConfig::PayloadSize;
        using Latency = TopologyConfig::Latency;
        using Bandwidth = TopologyConfig::Bandwidth;

        Topology(TopologyConfigs configs) noexcept;

        virtual ~Topology() noexcept = 0;

        virtual double send(NpuId src, NpuId dest, PayloadSize payloadSize) noexcept = 0;

    protected:
        TopologyConfigs configs;  // TopologyConfigs for each dimension
        int npus_count;  // NPUs count of the topology

        void checkNpuIdBound(NpuId npu_id) const noexcept;
    };
}

#endif
