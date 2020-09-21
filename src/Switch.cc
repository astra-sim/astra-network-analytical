#include "Switch.hh"
#include <iostream>

double AnalyticalBackend::Switch::send(int src, int dest, int packet_size) noexcept {
    src_to_switch_traffic_count[src]++;
    switch_to_dest_traffic_count[dest]++;
    src_to_dest_traffic_count[src][dest]++;

    const int hops_count = 2;  // switch network always has 2 hops

    auto delay = hops_count * link_latency;  // communication delay
    delay += hops_count * nic_latency;  // nic delay
    delay += switch_latency;  // switch delay
    delay += (packet_size / bandwidth);  // serialization delay

    return delay;
}

void AnalyticalBackend::Switch::print() const noexcept {
    std::cout << "Packets sent from each source:" << std::endl;
    for (int i = 0; i < nodes_count; i++) {
        std::cout << "\tsrc " << i << ": " << src_to_switch_traffic_count[i] << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Packets received by each destination: " << std::endl;
    for (int i = 0; i < nodes_count; i++) {
        std::cout << "\tdest " << i << ": " << switch_to_dest_traffic_count[i] << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Traffic Table: " << std::endl;
    std::cout << "(row: src, column: dest)" << std::endl;
    std::cout << "\t";
    for (int dest = 0; dest < nodes_count; dest++) {
        std::cout << "(" << dest << ")\t";
    }
    std::cout << std::endl;

    for (int src = 0; src < nodes_count; src++) {
        std::cout << "(" << src << ")\t";
        for (int dest = 0; dest < nodes_count; dest++) {
            std::cout << src_to_dest_traffic_count[src][dest] << "\t";
        }
        std::cout << std::endl;
    }
}