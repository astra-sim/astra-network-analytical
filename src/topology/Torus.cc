#include <cmath>
#include <iostream>
#include "Torus.hh"

double AnalyticalBackend::Torus::send(int src, int dest, int packet_size) noexcept {
    // compute this
    auto hops_count = 0;

    // 1. move x-dir

    // get coordinates
    auto src_x = src % width;
    auto dest_x = dest % width;
    auto src_y = src / width;
    auto dest_y = dest / width;

    // get current/next coordinates
    auto current_x = src_x;
    auto next_x = src_x;
    auto current_y = src_y;
    auto next_y = src_y;

    // compute which direction to move
    auto x_dir = 0;
    auto x_diff = dest_x - src_x;
    if (x_diff > 0) {
        // src_x is on left of dst_x
        // if within half-width, going right is faster
        // else, going left is faster
        x_dir = (std::abs(x_diff) <= half_width) ? 1 : -1;
    } else if (x_diff < 0) {
        // src_x is on right of dest_x
        // if within half-width, going left is faster
        // else, going right is faster
        x_dir = (std::abs(x_diff) <= half_width) ? -1 : 1;
    }  // else: x_diff == 0 -> no need to move through x-direction

    // move x direction until reaches the destination
    while (current_x != dest_x) {
        // compute next_x
        next_x += x_dir;

        // torus: move circular
        if (next_x < 0) {
            next_x = width + next_x;
        } else if (next_x >= width) {
            next_x = next_x - width;
        }

        // update stats
        hops_count++;
        auto current_node_id = (width * current_y) + current_x;
        auto next_node_id = (width * next_y) + next_x;
        increment_processed_packets(current_node_id, next_node_id);
        increment_channel_load(current_node_id, next_node_id, packet_size);

        // move current_x for next iteration
        current_x = next_x;
    }


    // 2. likely, move through y-axis until destination

    // compute which direction to move
    auto y_dir = 0;
    auto y_diff = dest_y - src_y;
    if (y_diff > 0) {
        // src_y is below dst_y
        // if within half-width, going up is faster
        // else, going down is faster
        y_dir = (std::abs(y_diff) <= half_width) ? 1 : -1;
    } else if (y_diff < 0) {
        // src_y is above dest_y
        // if within half-width, going down is faster
        // else, going up is faster
        y_dir = (std::abs(y_diff) <= half_width) ? -1 : 1;
    }  // else: y_diff == 0 -> no need to move through y-direction

    // move y direction until reaches the destination
    while (current_y != dest_y) {
        // compute next_y
        next_y += y_dir;

        // torus: move circular
        if (next_y < 0) {
            next_y = width + next_y;
        } else if (next_y >= width) {
            next_y = next_y - width;
        }

        // update stats
        hops_count++;
        auto current_node_id = (width * current_y) + current_x;
        auto next_node_id = (width * next_y) + next_x;
        increment_processed_packets(current_node_id, next_node_id);
        increment_channel_load(current_node_id, next_node_id, packet_size);

        // move current_y for next iteration
        current_y = next_y;
    }

    // update stats
    total_hops += hops_count;
    total_processed_packets++;


    // get delay
    auto delay = hops_count * link_latency;  // communication delay
    delay += (hops_count + 1) * nic_latency;  // nic delay
    delay += (packet_size / bandwidth);  // serialization delay

    return (int)delay;
}

void AnalyticalBackend::Torus::print() const noexcept {
    // Processed Packets by link
    print_processed_packets_count();

    // Processed Channel Loads
    print_channel_loads();

    // Maximum channel load
    auto max_channel_load = -1;
    auto max_channel_load_src = -1;
    auto max_channel_load_dest = -1;
    std::tie(max_channel_load, max_channel_load_src, max_channel_load_dest) = get_max_channel_load();
    std::cout << "Maximum Channel load: " << max_channel_load << " Bytes, link ("
              << max_channel_load_src << " -> " << max_channel_load_dest << ")" << std::endl;

    // Average hops
    auto average_hops = get_average_hops();
    std::cout << "Average Hops:" << average_hops << std::endl;
}

void AnalyticalBackend::Torus::increment_processed_packets(int src, int dest) noexcept {
    processed_packets[src][dest]++;
}

void AnalyticalBackend::Torus::increment_channel_load(int src, int dest, int packet_size) noexcept {
    channel_loads[src][dest] += packet_size;
}

std::tuple<int, int, int> AnalyticalBackend::Torus::get_max_channel_load() const noexcept {
    auto max_channel_load = -1;
    auto src = -1;
    auto dest = -1;
    for (int i = 0; i < nodes_count; i++) {
        for (int j = 0; j < nodes_count; j++) {
            if (channel_loads[i][j] > max_channel_load) {
                max_channel_load = channel_loads[i][j];
                src = i;
                dest = j;
            }
        }
    }

    return std::make_tuple(max_channel_load, src, dest);
}

double AnalyticalBackend::Torus::get_average_hops() const noexcept {
    return (double)total_hops / total_processed_packets;
}

void AnalyticalBackend::Torus::print_processed_packets_count() const noexcept {
    std::cout << "Processed Number of Packets by link" << std::endl;
    std::cout << "(row: link src, column: link dest)" << std::endl;
    std::cout << "\t";
    for (int dest = 0; dest < nodes_count; dest++) {
        std::cout << "(" << dest << ")\t";
    }
    std::cout << std::endl;

    for (int src = 0; src < nodes_count; src++) {
        std::cout << "(" << src << ")\t";
        for (int dest = 0; dest < nodes_count; dest++) {
            std::cout << processed_packets[src][dest] << "\t";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void AnalyticalBackend::Torus::print_channel_loads() const noexcept {
    std::cout << "Processed Channel Loads by link" << std::endl;
    std::cout << "(row: link src, column: link dest)" << std::endl;
    std::cout << "\t";
    for (int dest = 0; dest < nodes_count; dest++) {
        std::cout << "(" << dest << ")\t";
    }
    std::cout << std::endl;

    for (int src = 0; src < nodes_count; src++) {
        std::cout << "(" << src << ")\t";
        for (int dest = 0; dest < nodes_count; dest++) {
            std::cout << channel_loads[src][dest] << "\t";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}