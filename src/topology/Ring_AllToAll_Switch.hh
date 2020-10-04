#ifndef __RING_ALLTOALL_SWITCH_H__
#define __RING_ALLTOALL_SWITCH_H__

#include <vector>
#include "Topology.hh"
#include "TopologyConfiguration.hh"

namespace AnalyticalBackend {
    class Ring_AllToAll_Switch : public Topology {
    public:
        Ring_AllToAll_Switch(std::vector<TopologyConfiguration> configurations, std::vector<int> nodes_per_dims) noexcept;

        Latency simulateSend(NodeID src, NodeID dest, PayloadSize payload_size) noexcept override;

    private:
        std::vector<TopologyConfiguration> configurations;
        std::vector<int> nodes_per_dims;
    };
}


#endif
