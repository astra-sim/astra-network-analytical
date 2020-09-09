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
        Topology() : width(-1), half_width(-1) { }

        /**
         * Set the width of the torus
         * @param width width of 2d torus
         * @param bandwidth bandwidth, in bytes/sec.
         */
        void initialize(int width, int bandwidth) noexcept;

        /**
         * Get the number of hops from src to dest.
         * @param src source node id
         * @param dest destination node id
         * @return number of total hops required
         */
        int get_hops_count(int src, int dest) const noexcept;

        /**
         * get bandwidth of a link.
         * @return bandwidth, in bytes/sec.
         */
        int get_bandwidth() const noexcept;

    private:
        /**
         * Torus width
         */
        int width;

        /**
         * Torus width in half
         */
         int half_width;

         /**
          * Bandwidth of a link, in bytes/sec.
          */
         int bandwidth;
    };
}

#endif