#ifndef __SWITCH_HH__
#define __SWITCH_HH__

#include <vector>
#include "Topology.hh"

namespace AnalyticalBackend {
    class Switch : public Topology {
    public:
        Switch(int nodes_count, int bandwidth, int link_latency, int nic_latency, int switch_latency) noexcept :
                nodes_count(nodes_count),
                bandwidth(bandwidth),
                link_latency(link_latency),
                nic_latency(nic_latency),
                switch_latency(switch_latency) {
            for (int i = 0; i < nodes_count; i++) {
                src_to_switch_traffic_count.emplace_back(0);
                switch_to_dest_traffic_count.emplace_back(0);
                src_to_dest_traffic_count.emplace_back(std::vector<int>(nodes_count, 0));
            }
        }

        double send(int src, int dest, int packet_size) noexcept override;

        /**
         * for debug purpose
         * print stuff required for debugging
         */
        void print() const noexcept override;

    private:
        /**
         * number of nodes connected
         */
        int nodes_count;

        /**
         * link bandwidth (bytes / sec)
         */
        int bandwidth;

        /**
         * link latency (ns)
         */
        int link_latency;

        /**
         * nic delay (ns)
         */
        int nic_latency;

        /**
         * latency of switch itself (like router delay)
         * (ns)
         */
        int switch_latency;

        /**
         * count the total number of packets each source sent.
         */
        std::vector<int> src_to_switch_traffic_count;

        /**
         * count the total number of packets each dest received.
         */
        std::vector<int> switch_to_dest_traffic_count;

        /**
         * count the total number of packets for each src-dest pair.
         */
        std::vector<std::vector<int>> src_to_dest_traffic_count;
    };
}


#endif
