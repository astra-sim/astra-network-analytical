#ifndef __SWITCH_HH__
#define __SWITCH_HH__

#include "Topology.hh"
#include "TopologyConfiguration.hh"

namespace AnalyticalBackend {
    class Switch : public Topology {
    public:
        Switch(TopologyConfiguration configuration, int nodes_count) noexcept;

        Latency simulateSend(NodeID src, NodeID dest, PayloadSize payload_size) noexcept override;

    private:
        TopologyConfiguration configuration;
        int nodes_count;
    };
}

#endif
