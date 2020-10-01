#ifndef __TORUS2D_HH_
#define __TORUS2D_HH_

#include <tuple>
#include <cmath>
#include "BasicTopology.hh"
#include "TopologyConfiguration.hh"

namespace AnalyticalBackend {
    /**
     * 2D torus : must be square (i.e., same width and height, nodes_count is always n^2)
     */
    class Torus2D : public BasicTopology {
    public:
        Torus2D(int nodes_count, TopologyConfiguration configuration) noexcept :
                nodes_count(nodes_count),
                configuration(configuration) {
            width = (int)std::sqrt(nodes_count);
        }

        double sendAndGetLatency(NodeAddress src, NodeAddress dest, int payload_size) noexcept override;

    private:
        /**
         * A tuple of (yID, xID)
         * e.g., 4x4 torus, NodeID 7 = (1, 3)
         * e.g., 4x4 torus, NodeID 10 = (2, 2)
         */
        using Torus2DNodeID = std::tuple<NodeID, NodeID>;
        using StepDirection1D = int;
        using StepDirection2D = std::tuple<StepDirection1D, StepDirection1D>;

        int nodes_count;
        int width;  // sqrt(nodes_count)
        TopologyConfiguration configuration;

        /**
         * Compare src and dest xID.
         * @param srcID
         * @param destID
         * @return true if xID are equal.
         */
        bool is_xID_Equal(Torus2DNodeID srcID, Torus2DNodeID destID) const noexcept;

        /**
         * Compare src and dest yID.
         * @param srcID
         * @param destID
         * @return true if yID are equal.
         */
        bool is_yID_Equal(Torus2DNodeID srcID, Torus2DNodeID destID) const noexcept;

        /**
         * Encode 1d nodeID to Torus2D nodeID.
         * @param nodeID
         * @return Torus2DNodeID = (yID, xID)
         */
        Torus2DNodeID nodeIDtoTorus2dNodeID(NodeID nodeID) const noexcept;

        /**
         * Computes which direction to proceed, considering only 1-dimensional ring.
         * For 2d torus, you can feed either (src_xID, dest_xID) or (src_yID, dest_yID)
         * @param srcID
         * @param destID
         * @return direction to proceed
         */
        StepDirection1D computeStepDirection1D(NodeID srcID, NodeID destID) const noexcept;

        /**
         * Compute 2d direction to proceed.
         * @param srcID
         * @param destID
         * @return direction tuple to proceed
         */
        StepDirection2D computeStepDirection(Torus2DNodeID srcID, Torus2DNodeID destID) const noexcept;

        /**
         * Take a step to the given direction
         * @param currentID current node
         * @param step direction to take a step
         * @return arrived Torus2dNodeID after taking a step
         */
        Torus2DNodeID takeStep(Torus2DNodeID currentID, StepDirection2D step) const noexcept;
    };
}


#endif
