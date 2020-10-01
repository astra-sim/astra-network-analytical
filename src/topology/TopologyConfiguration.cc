#include "TopologyConfiguration.hh"

using namespace AnalyticalBackend;

double TopologyConfiguration::getLinkBandwidth() const noexcept {
    return link_bandwidth;
}

double TopologyConfiguration::getLinkLatency() const noexcept {
    return link_latency;
}

double TopologyConfiguration::getNICLatency() const noexcept {
    return nic_latency;
}

double TopologyConfiguration::getRouterLatency() const noexcept {
    return router_latency;
}

double TopologyConfiguration::getMemoryBandwidth() const noexcept {
    return memory_bandwidth;
}

double TopologyConfiguration::getMemoryLatency() const noexcept {
    return memory_latency;
}

double TopologyConfiguration::getMemoryScale() const noexcept {
    return memory_scale;
}
