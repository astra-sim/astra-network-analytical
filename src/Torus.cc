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