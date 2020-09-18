#include <cmath>
#include <iostream>
#include "Torus.hh"

double AnalyticalBackend::Torus::send(int src, int dest, int packet_size) noexcept {
    // 1. Get (srcX, srcY), (destX, destY)
    auto srcX = src % width;
    auto srcY = src / width;
    auto destX = dest % width;
    auto destY = dest / width;

    // 2. Compute x hops and y hops
    auto xHops = std::abs(srcX - destX);
    if (xHops > half_width) {
        xHops = width - xHops;
    }

    int yHops = std::abs(srcY - destY);
    if (yHops > half_width) {
        yHops = width - yHops;
    }

    // total hops = xHops + yHops
    auto hops_count = xHops + yHops;

    // get latency per link
    auto latency_per_link = link_latency + (packet_size / bandwidth);

    // return total latency
    return hops_count * latency_per_link;
}
