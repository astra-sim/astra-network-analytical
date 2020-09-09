#ifndef __TOPOLOGY_HH__
#define __TOPOLOGY_HH__

#include <iostream>
#include <list>
#include "Event.hh"
#include "../astra-sim/system/AstraNetworkAPI.hh"

namespace AnalyticalBackend {
    // TODO: currently assuming torus topology. Please make this class topology-agnostic in the future.
    class Topology {
    public:
        /**
         * Constructor of torus topology.
         * @param width
         */
        Topology(int width) : width(width), half_width(width / 2) { }

        /**
         * Get the number of hops from src to dest.
         * @param src source node id
         * @param dest destination node id
         * @return number of total hops required
         */
        int get_hops_count(int src, int dest) const noexcept;

    private:
        /**
         * Torus width
         */
        int width;

        /**
         * Torus width in half
         */
         int half_width;
    };
}

#endif