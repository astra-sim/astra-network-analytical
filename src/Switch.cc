#include "Switch.hh"
#include <iostream>

double AnalyticalBackend::Switch::send(int src, int dest, int packet_size) noexcept {
    // stats
    src_to_switch_traffic_count[src]++;
    switch_to_dest_traffic_count[dest]++;
    src_to_dest_traffic_count[src][dest]++;

    src_to_switch_channel_loads[src] += packet_size;
    switch_to_dest_channel_loads[dest] += packet_size;
    src_to_dest_channel_loads[src][dest] += packet_size;

    const int hops_count = 2;  // switch network always has 2 hops

    total_hops_count += hops_count;
    total_packets_count++;

    // compute delay
    auto delay = hops_count * link_latency;  // communication delay
    delay += hops_count * nic_latency;  // nic delay
    delay += switch_latency;  // switch delay
    delay += (packet_size / bandwidth);  // serialization delay

    return (int)delay;
}

void AnalyticalBackend::Switch::print() const noexcept {
    // Served Packets
    print_served_packets_count();

    // Channel load
    print_channel_loads();

    // Max input channel load
    auto max_input_channel_load = -1;
    auto max_input_channel = -1;
    std::tie(max_input_channel_load, max_input_channel) = get_max_input_channel_load();
    std::cout << "Max input channel load: " << max_input_channel_load
              << " bytes, from input port: " << max_input_channel << std::endl;

    // Max output channel load
    auto max_output_channel_load = -1;
    auto max_output_channel = -1;
    std::tie(max_output_channel_load, max_output_channel) = get_max_output_channel_load();
    std::cout << "Max output channel load: " << max_output_channel_load
              << " bytes, from output port: " << max_output_channel << std::endl;

    // Average hops
    double average_hops = get_average_hop_count();
    std::cout << "Average hops: " << average_hops << std::endl;
}

double AnalyticalBackend::Switch::get_average_hop_count() const noexcept {
    return (double)total_hops_count / total_packets_count;
}

std::tuple<int, int> AnalyticalBackend::Switch::get_max_input_channel_load() const noexcept {
    auto max_channel_load = -1;
    auto input_port = -1;
    for (int src = 0; src < nodes_count; src++) {
        if (src_to_switch_channel_loads[src] > max_channel_load) {
            max_channel_load = src_to_switch_channel_loads[src];
            input_port = src;
        }
    }

    return std::make_tuple(max_channel_load, input_port);
}

std::tuple<int, int> AnalyticalBackend::Switch::get_max_output_channel_load() const noexcept {
    auto max_channel_load = -1;
    auto output_port = -1;
    for (int dest = 0; dest < nodes_count; dest++) {
        if (switch_to_dest_channel_loads[dest] > max_channel_load) {
            max_channel_load = switch_to_dest_channel_loads[dest];
            output_port = dest;
        }
    }

    return std::make_tuple(max_channel_load, output_port);
}

void AnalyticalBackend::Switch::print_channel_loads() const noexcept {
    std::cout << "Channel load for each input port:" << std::endl;
    for (int i = 0; i < nodes_count; i++) {
        std::cout << "\tsrc " << i << ": " << src_to_switch_channel_loads[i] << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Channel load for each output port: " << std::endl;
    for (int i = 0; i < nodes_count; i++) {
        std::cout << "\tdest " << i << ": " << switch_to_dest_channel_loads[i] << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Total served packet size for each src/dest pair: " << std::endl;
    std::cout << "(row: src, column: dest)" << std::endl;
    std::cout << "\t";
    for (int dest = 0; dest < nodes_count; dest++) {
        std::cout << "(" << dest << ")\t";
    }
    std::cout << std::endl;

    for (int src = 0; src < nodes_count; src++) {
        std::cout << "(" << src << ")\t";
        for (int dest = 0; dest < nodes_count; dest++) {
            std::cout << src_to_dest_channel_loads[src][dest] << "\t";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void AnalyticalBackend::Switch::print_served_packets_count() const noexcept {
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

    std::cout << "Number of Packets for each src/dest pair: " << std::endl;
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
    std::cout << std::endl;
}