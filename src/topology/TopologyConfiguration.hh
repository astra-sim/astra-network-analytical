#ifndef __TOPOLOGYCONFIGURATION_HH__
#define __TOPOLOGYCONFIGURATION_HH__


namespace AnalyticalBackend {
    struct TopologyConfiguration {
    public:
        using Bandwidth = double;
        using LinkLatency = double;
        using NIC_Latency = double;
        using RouterLatency = double;

        TopologyConfiguration(Bandwidth bandwidth,
                              LinkLatency link_latency,
                              NIC_Latency nic_latency,
                              RouterLatency router_latency) noexcept;

        Bandwidth getBandwidth() const noexcept;
        LinkLatency getLinkLatency() const noexcept;
        NIC_Latency getNIC_Latency() const noexcept;
        RouterLatency getRouterLatency() const noexcept;

    private:
        Bandwidth bandwidth;
        LinkLatency link_latency;
        NIC_Latency nic_latency;
        RouterLatency router_latency;
    };
}

#endif
