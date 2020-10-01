#ifndef __SWITCH_HH__
#define __SWITCH_HH__

#include "BasicTopology.hh"
#include "TopologyConfiguration.hh"

namespace AnalyticalBackend {
    /**
     * Switch: every node is reachable in exactly 2 hops, has switch delay.
     */
    class Switch : public BasicTopology {
    public:
        Switch(int nodes_count, TopologyConfiguration configuration) noexcept :
                nodes_count(nodes_count),
                configuration(configuration) { }

        double sendAndGetLatency(NodeAddress src, NodeAddress dest, int payload_size) noexcept override;

    private:
        int nodes_count;
        TopologyConfiguration configuration;
    };
}


#endif
