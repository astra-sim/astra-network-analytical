#ifndef __TORUS_HH__
#define __TORUS_HH__

#include "Topology.hh"

namespace AnalyticalBackend {
    class Torus : public AnalyticalBackend::Topology {
    public:
        explicit Torus(int width, int bandwidth) noexcept : width(width), bandwidth(bandwidth) {
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
    };
}

#endif