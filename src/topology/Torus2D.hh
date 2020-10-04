#ifndef __TORUS2D_HH__
#define __TORUS2D_HH__

#include "Topology.hh"
#include "TopologyConfiguration.hh"

namespace AnalyticalBackend {
    class Torus2D : public Topology {
    public:
        Torus2D(TopologyConfiguration configuration, int nodes_count) noexcept;

        Latency simulateSend(NodeID src, NodeID dest, PayloadSize payload_size) noexcept override;

    private:
        TopologyConfiguration configuration;
        int nodes_count;
        int width;
        int half_width;
    };
}


#endif
