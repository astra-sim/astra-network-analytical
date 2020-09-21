#include <cmath>
#include <iostream>
#include "Torus.hh"

double AnalyticalBackend::Torus::send(int src, int dest, int packet_size) noexcept {
    // compute this
    auto hops_count = 0;

    // 1. move x-dir

    // get src and dest points' x coordinate
    auto src_x = src % width;
    auto dest_x = dest % width;

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
    auto current_x = src_x;
    auto next_x = src_x;
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

        // move current_x for next iteration
        current_x = next_x;
    }


    // 2. likely, move through y-axis until destination

    // get src and dest points' x coordinate
    auto src_y = src / width;
    auto dest_y = dest / width;

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
    auto current_y = src_y;
    auto next_y = src_y;
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

        // move current_y for next iteration
        current_y = next_y;
    }


    // get delay
    auto delay = hops_count * link_latency;  // communication delay
    delay += (hops_count + 1) * nic_latency;  // nic delay
    delay += (packet_size / bandwidth);  // serialization delay

    return delay;
}

void AnalyticalBackend::Torus::print() const noexcept {
    // TODO: implement torus route tracking and printing
    std::cout << "Torus print not yet implemented!" << std::endl;
}