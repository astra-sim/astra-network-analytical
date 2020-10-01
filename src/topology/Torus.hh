#ifndef __TORUS_HH__
#define __TORUS_HH__

#include "Topology.hh"
#include <cmath>
#include <vector>
#include <tuple>

namespace AnalyticalBackend {
    class Torus : public AnalyticalBackend::Topology {
    public:
        Torus(int nodes_count, double bandwidth, double link_latency, double nic_latency) noexcept :
                nodes_count(nodes_count),
                bandwidth(bandwidth),link_latency(link_latency), nic_latency(nic_latency),
                total_hops(0), total_processed_packets(0) {
            width = std::sqrt(nodes_count);
            half_width = width / 2;

            for (int i = 0; i < nodes_count; i++) {
                processed_packets.emplace_back(std::vector<int>(nodes_count, 0));
                channel_loads.emplace_back(std::vector<int>(nodes_count, 0));
            }
        }

        ~Torus() override = default;

        double send(int src, int dest, int packet_size) noexcept override;

        void print() const noexcept override;

    private:
        /**
         * total number of hosts connected
         */
        int nodes_count;

        /**
         * width of a 2d torus
         */
        int width;

        /**
         * half_width of a 2d torus
         */
        int half_width;

        /**
         * link bandwidth (bytes / sec)
         */
        double bandwidth;

        /**
         * link latency (ns)
         */
        double link_latency;

        /**
         * nic latency (ns)
         */
        double nic_latency;

        /**
         * total number of hops of the network packet
         */
        int total_hops;

        /**
         * total processed packets of the network
         */
        int total_processed_packets;

        /**
         * total packets processed per each [src][dest] link.
         * Unlike switch-based, only several links are active
         * e.g., there's no packets_per_link[0][2]
         */
        std::vector<std::vector<int>> processed_packets;

        /**
         * total channel loads in bytes per each [src][dest] link
         * * Unlike switch-based, only several links are active
         * e.g., there's no packets_per_link[0][2]
         */
        std::vector<std::vector<int>> channel_loads;

        /**
         * increment channel load of a link.
         * @param src src of a link
         * @param dest dest node of a link
         * @param packet_size packet_size a link served.
         */
        void increment_channel_load(int src, int dest, int packet_size) noexcept;

        /**
         * increment the number of packets of a given link.
         * @param src src of the link
         * @param dest dest of the link
         */
        void increment_processed_packets(int src, int dest) noexcept;

        /**
         * Get the maximum channel load among all links.
         * @return <max channel load, link src, link dst>
         */
        std::tuple<int, int, int> get_max_channel_load() const noexcept;

        /**
         * Get average hops.
         * @return average hops
         */
        double get_average_hops() const noexcept;

        /**
         * Print the number of processed packets of each link.
         */
        void print_processed_packets_count() const noexcept;

        /**
         * Print channel load of each link.
         */
        void print_channel_loads() const noexcept;
    };
}

#endif