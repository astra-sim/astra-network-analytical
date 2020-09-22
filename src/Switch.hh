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
                switch_latency(switch_latency),
                total_hops_count(0),
                total_packets_count(0) {
            for (int i = 0; i < nodes_count; i++) {
                src_to_switch_traffic_count.emplace_back(0);
                switch_to_dest_traffic_count.emplace_back(0);
                src_to_dest_traffic_count.emplace_back(std::vector<int>(nodes_count, 0));

                src_to_switch_channel_loads.emplace_back(0);
                switch_to_dest_channel_loads.emplace_back(0);
                src_to_dest_channel_loads.emplace_back(std::vector<int>(nodes_count, 0));
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
         * total number of hops
         */
        int total_hops_count;

        /**
         * total numbers of processed packets
         */
        int total_packets_count;

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

        /**
         * total channel loads for src-to-switch links
         */
        std::vector<int> src_to_switch_channel_loads;

        /**
         * total channel loads for switch-to-dest links
         */
        std::vector<int> switch_to_dest_channel_loads;

        /**
         * total served packets for src-to-dest pairs
         */
        std::vector<std::vector<int>> src_to_dest_channel_loads;

        /**
         * Compute the average number of hops per each packet.
         * @return average number of hops
         */
        double get_average_hop_count() const noexcept;

        /**
         * Compute max channel load of input ports
         * @return <channel load (bytes), src port>
         */
        std::tuple<int, int> get_max_input_channel_load() const noexcept;

        /**
         * Compute max channel load of output ports
         * @return <channel load (bytes), dest port>
         */
        std::tuple<int, int> get_max_output_channel_load() const noexcept;

        /**
         * Print served number of packets per each link (traffic-count)
         */
        void print_served_packets_count() const noexcept;

        /**
         * Print channel loads per each link
         */
        void print_channel_loads() const noexcept;
    };
}


#endif
