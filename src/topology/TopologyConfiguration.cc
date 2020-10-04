#include "TopologyConfiguration.hh"

using namespace AnalyticalBackend;

TopologyConfiguration::TopologyConfiguration(Bandwidth link_bandwidth,
                                             Latency link_latency,
                                             Latency nic_latency,
                                             Latency router_latency,
                                             Bandwidth hbm_bandwidth,
                                             Latency hbm_latency,
                                             Scale hbm_scale) noexcept :
        link_bandwidth(link_bandwidth),
        link_latency(link_latency),
        nic_latency(nic_latency),
        router_latency(router_latency),
        hbm_bandwidth(hbm_bandwidth),
        hbm_latency(hbm_latency),
        hbm_scale(hbm_scale) { }

TopologyConfiguration::Bandwidth TopologyConfiguration::getBandwidth() const noexcept {
    return link_bandwidth;
}

TopologyConfiguration::Latency TopologyConfiguration::getLinkLatency() const noexcept {
    return link_latency;
}

TopologyConfiguration::Latency TopologyConfiguration::getNIC_Latency() const noexcept {
    return nic_latency;
}

TopologyConfiguration::Latency TopologyConfiguration::getRouterLatency() const noexcept {
    return router_latency;
}

TopologyConfiguration::Bandwidth TopologyConfiguration::getHBM_Bandwidth() const noexcept {
    return hbm_bandwidth;
}

TopologyConfiguration::Latency TopologyConfiguration::getHBM_Latency() const noexcept {
    return hbm_latency;
}

TopologyConfiguration::Scale TopologyConfiguration::getHBM_Scale() const noexcept {
    return hbm_scale;
}
