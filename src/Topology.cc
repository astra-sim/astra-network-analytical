#include "Topology.hh"

int AnalyticalBackend::Topology::get_hops_count(int src, int dest) const noexcept {
    // 1. Get (srcX, srcY), (destX, destY)
    int srcX = src % width;
    int srcY = src / width;
    int destX = dest % width;
    int destY = dest / width;

    // 2. Compute x hops and y hops
    int xHops = abs(srcX - destX);
    if (xHops > half_width) {
        xHops = width - xHops;
    }

    int yHops = abs(srcY - destY);
    if (yHops > half_width) {
        yHops = width - yHops;
    }

    // 3. return total number of required hops
    return xHops + yHops;
}
