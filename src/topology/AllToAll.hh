#ifndef __ALLTOALL_HH__
#define __ALLTOALL_HH__

#include "Topology.hh"
#include "TopologyConfiguration.hh"

namespace AnalyticalBackend {
    class AllToAll : public Topology {
    public:
        AllToAll(TopologyConfiguration configuration, int nodes_count) noexcept;

        Latency simulateSend(NodeID src, NodeID dest, PayloadSize payload_size) noexcept override;

    private:
        TopologyConfiguration configuration;
        int nodes_count;
    };
}

#endif
