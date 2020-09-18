#ifndef __TORUS_HH__
#define __TORUS_HH__

#include "Topology.hh"

namespace AnalyticalBackend {
    class Torus : public AnalyticalBackend::Topology {
    public:
        Torus(int width, int bandwidth, int link_latency) noexcept :
                width(width), bandwidth(bandwidth), link_latency(link_latency) {
            half_width = width / 2;
        }

        ~Torus() override = default;

        double send(int src, int dest, int packet_size) noexcept override;

    private:
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
        int bandwidth;

        /**
         * link latency (ns)
         */
        int link_latency;
    };
}

#endif