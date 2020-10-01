#ifndef __COMPLEXTOPOLOGY_H__
#define __COMPLEXTOPOLOGY_H__

#include "Topology.hh"

namespace AnalyticalBackend {
    /**
     * ComplexTopology: A topology that has at least 2 (logical) dimensions.
     *
     * e.g., dimension 0 Rings connected via Dimension 1 Switch.
     */
    class ComplexTopology : public Topology {
    public:
        /**
         * Break finishes routing after matching that dimension,
         * FallThrough keeps routing lower dimension after matching that dimension.
         *
         * e.g., Dim 0: 8 * (nodes tiles All-to-All)
         *       Dim 1: Switch that connects all 32 nodes
         *       Then, as switch at dim 1 connects all lower-level nodes, no need to keep routing after using the switch.
         *       Thus, set RoutingOption as [Break, LastLevel]
         *
         * e.g., Dim 0: 8 * (nodes tiles All-to-All)
         *       Dim 1: Switch that connects 8 nodes (node 0 of each All-to-All fabric)
         *       Then, as switch at dim 1 doesn't connect lower-level nodes,
         *       lower level All-to-All should keep routing.
         *       Thus, set RoutingOption as [FallThrough, LastLevel]
         */
        enum class RoutingOption { Break, FallThrough, LastLevel };

        /**
         * EscapeNode tries to locally destine specific node to escape that dimension.
         * LocalMatch tries to match src and dest local nodeID to escape that dimension.
         *
         * e.g., Dim 0: 8 * (Ring of size 4)
         *       Dim 1: Ring of Size 8 (connecting node 3 of each Dim 0 ring)
         *       Then, for a packet to reach Dim 1, Dim 0 should route it to node 3.
         *       Thus, set EscapeScheme to [TopLevel, Escape].
         *
         * e.g., Dim 0: 8 * (Ring of Size 4)
         *       Dim 1: 4 All-to-All of size 8 (connecting each node of Dim 0)
         *       Then, for a packet to reach Dim 1, src and dest should have the same Dim 0 NodeID.
         *       Thus, set EscapeScheme to [TopLevel, LocalMatch].
         */
        enum class EscapeScheme { TopLevel, EscapeNode, LocalMatch };

        virtual ~ComplexTopology() = 0;
    };
}

#endif
