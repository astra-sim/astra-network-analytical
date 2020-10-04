#include "TopologyConfiguration.hh"

using namespace AnalyticalBackend;

TopologyConfiguration::TopologyConfiguration(Bandwidth bandwidth,
                                             LinkLatency link_latency,
                                             NIC_Latency nic_latency,
                                             RouterLatency router_latency) noexcept :
        bandwidth(bandwidth),
        link_latency(link_latency),
        nic_latency(nic_latency),
        router_latency(router_latency) { }

TopologyConfiguration::Bandwidth TopologyConfiguration::getBandwidth() const noexcept {
    return bandwidth;
}

TopologyConfiguration::LinkLatency TopologyConfiguration::getLinkLatency() const noexcept {
    return link_latency;
}

TopologyConfiguration::NIC_Latency TopologyConfiguration::getNIC_Latency() const noexcept {
    return nic_latency;
}

TopologyConfiguration::RouterLatency TopologyConfiguration::getRouterLatency() const noexcept {
    return router_latency;
}
