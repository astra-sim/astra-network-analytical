#ifndef __TOPOLOGY_H__
#define __TOPOLOGY_H__

#include <vector>

namespace AnalyticalBackend {
    class Topology {
    public:
        /**
         * NodeID: node in integer number (e.g., node 18, node 27, ...)
         */
        using NodeID = int;

        /**
         * NodeAddress: encoded NodeID in a series of integers, from higher-dim to lower-dim
         *
         * e.g., Dimension 0: 4 * Ring(size: 8)
         *       Dimension 1: Switch(size: 4) = connecting node 0 of each ring
         *       Total #nodes = 32
         *
         * Then, each NodeID can be expressed as
         *      NodeID 0 = NodeAddress (0, 0)
         *      NodeID 1 = NodeAddress (0, 1)
         *      ...
         *      NodeID 7 = NodeAddress (0, 7)
         *      NodeID 8 = NodeAddress (1, 0)
         *      ...
         *      NodeID 31 = NodeAddress(3, 7)
         */
        using NodeAddress = std::vector<NodeID>;

        virtual ~Topology() = 0;

        /**
         * Encode NodeID (int) into NodeAddress.
         * @param nodeID integer node index
         * @return NodeAddress
         */
        virtual NodeAddress nodeIDtoAddress(NodeID nodeID) const noexcept = 0;

        /**
         * Simulate payload communication from src to dest. Return the latency of the communication.
         * @param src source NodeAddress
         * @param dest destination NodeAddress
         * @param payload_size payload size in Bytes
         * @return latency of the communication from src to dest.
         */
        virtual double sendAndGetLatency(NodeAddress src, NodeAddress dest, int payload_size) noexcept = 0;
    };
}

#endif
