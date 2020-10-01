#ifndef __BASICTOPOLOGY_HH__
#define __BASICTOPOLOGY_HH__

#include "Topology.hh"

namespace AnalyticalBackend {
    /**
     * BasicTopology: Topology that has (logically) only 1 dimension.
     *
     * e.g., Physical 2d (4x4) torus can be expressed as BasicTopology:
     *          by logically regarding it as NodeAddress (0) ~ NodeAddress (15)
     */
    class BasicTopology : public Topology {
    public:
        virtual ~BasicTopology() = 0;

        /**
         * As BasicTopology has only 1 dimension, constructing NodeAddress is straightforward:
         *      NodeAddress = (nodeID).
         */
        NodeAddress nodeIDtoAddress(NodeID nodeID) const noexcept override;
    };
}

#endif
