#ifndef __RING_HH__
#define __RING_HH__

#include "Topology.hh"
#include "TopologyConfiguration.hh"

namespace AnalyticalBackend {
    class Ring : public Topology {
    public:
        Ring(TopologyConfiguration configuration, int nodes_count, bool is_bidirectional = true) noexcept;

        Latency simulateSend(NodeID src, NodeID dest, PayloadSize payload_size) noexcept override;

    private:
        TopologyConfiguration configuration;
        int nodes_count;
        bool is_bidirectional;

        int half_nodes_count;
    };
}


#endif
