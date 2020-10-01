#ifndef __ALLTOALL_HH_
#define __ALLTOALL_HH_

#include "BasicTopology.hh"
#include "TopologyConfiguration.hh"

namespace AnalyticalBackend {
    /**
     * All-to-All Glueless: reaching every node in one hop, without any switch delay.
     */
    class AllToAll : public BasicTopology {
    public:
        AllToAll(int nodes_count, TopologyConfiguration configuration) noexcept :
                nodes_count(nodes_count),
                configuration(configuration) { }

        double sendAndGetLatency(NodeAddress src, NodeAddress dest, int payload_size) noexcept override;

    private:
        int nodes_count;
        TopologyConfiguration configuration;
    };
}

#endif
