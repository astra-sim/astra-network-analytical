#ifndef __TORUS_HH__
#define __TORUS_HH__

#include "Topology.hh"

namespace AnalyticalBackend {
    class Torus : public AnalyticalBackend::Topology {
    public:
        Torus() noexcept = default;
        ~Torus() override = default;
        double send(int src, int dest, int packet_size) noexcept override;
    };
}

#endif