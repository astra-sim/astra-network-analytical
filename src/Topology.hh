#ifndef __TOPOLOGY_HH__
#define __TOPOLOGY_HH__

namespace AnalyticalBackend {
    /**
     * Abstract class for multiple Topologies
     */
    class Topology {
    public:
        /**
         * Destructor of abstract class -- pure virtual destructor
         */
        virtual ~Topology() = 0;

        /**
         * Send a packet from src to dest node.
         * @param src source node id
         * @param dest destination node id
         * @param packet_size packet size to send (in bytes)
         * @return total time for transmission (transmission latency) in nanoseconds resolution (NS)
         */
        virtual double send(int src, int dest, int packet_size) noexcept = 0;
    };
}

#endif
