/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#ifndef __FASTTOPOLOGY_HH__
#define __FASTTOPOLOGY_HH__

#include "TopologyConfig.hh"
#include "Topology.hh"

namespace Analytical {
    class FastTopology : public Topology {
    public:
        FastTopology(TopologyConfigs configs) noexcept;

        virtual ~FastTopology() noexcept = 0;

    protected:
        Latency linkLatency(int dimension, int hops_count) const noexcept;
        Latency serializationLatency(int dimension, PayloadSize payload_size) const noexcept;
        Latency nicLatency(int dimension) const noexcept;
        Latency routerLatency(int dimension) const noexcept;
        Latency hbmLatency(int dimension, PayloadSize payload_size) const noexcept;
        Latency criticalLatency(Latency communication_latency, Latency hbm_latency) const noexcept;
    };
}


#endif
