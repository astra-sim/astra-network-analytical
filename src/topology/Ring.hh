#ifndef __RING_HH__
#define __RING_HH__

#include "BasicTopology.hh"
#include "TopologyConfiguration.hh"

namespace AnalyticalBackend {
    /**
     * NodeID increases Clockwise.
     * e.g., 0 -> 1 -> 2 -> 3 -> 4 -> 5 -> 0 (clockwise)
     */
    class Ring : public BasicTopology {
    public:
        /**
         * Bidirectional: always choose the shortest path between clockwise and counterclockwise paths.
         */
        enum class Direction { Clockwise, Counterclockwise, Bidirectional };
        using StepDirection = int;

        Ring(int nodes_count, TopologyConfiguration configuration, Direction direction = Direction::Bidirectional) :
            nodes_count(nodes_count),
            configuration(configuration),
            direction(direction){ };

        double sendAndGetLatency(NodeAddress src, NodeAddress dest, int payload_size) noexcept override;

    private:
        int nodes_count;
        TopologyConfiguration configuration;
        Direction direction;

        /**
         * Computes which direction to proceed.
         * @param srcID
         * @param destID
         * @return direction to proceed
         */
        StepDirection computeStepDirection(NodeID srcID, NodeID destID);

        /**
         * Take a step to the given direction.
         * @param currentID current node
         * @param step direction to take a step
         * @return arrived nodeID after taking a step
         */
        NodeID takeStep(NodeID currentID, StepDirection step);
    };
}

#endif
