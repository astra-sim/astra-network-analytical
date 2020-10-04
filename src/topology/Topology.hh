#ifndef _TOPOLOGY_HH__
#define _TOPOLOGY_HH__


namespace AnalyticalBackend {
    class Topology {
    public:
        /**
         * NodeID: each node's ID in integer (e.g., 16, 27, 31)
         */
        using NodeID = int;

        /**
         * Payload size is expressed in `Bytes` using `int`
         */
        using PayloadSize = int;

        /**
         * Latency uses the datatype `double`
         */
        using Latency = double;

        virtual ~Topology() noexcept = 0;

        /**
         * Simulate network packet sending from src to dest nodes.
         *
         * @param src source NodeID of packet
         * @param dest destination's NodeID
         * @param payload_size payload size in Bytes
         * @return latency of the communication from src to dest
         */
        virtual Latency simulateSend(NodeID src, NodeID dest, PayloadSize payload_size) noexcept = 0;
    };
}


#endif
