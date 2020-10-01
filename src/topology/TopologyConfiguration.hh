#ifndef __TOPOLOGYCONFIGURATION_HH__
#define __TOPOLOGYCONFIGURATION_HH__

namespace AnalyticalBackend {
    struct TopologyConfiguration {
    public:
        TopologyConfiguration(double link_bandwidth,
                              double link_latency,
                              double nic_latency,
                              double router_latency,
                              double memory_bandwidth,
                              double memory_latency,
                              double memory_scale) noexcept:
                link_bandwidth(link_bandwidth),
                link_latency(link_latency),
                nic_latency(nic_latency),
                router_latency(router_latency),
                memory_bandwidth(memory_bandwidth),
                memory_latency(memory_latency),
                memory_scale(memory_scale) { }

        double getLinkBandwidth() const noexcept;
        double getLinkLatency() const noexcept;
        double getNICLatency() const noexcept;
        double getRouterLatency() const noexcept;
        double getMemoryBandwidth() const noexcept;
        double getMemoryLatency() const noexcept;
        double getMemoryScale() const noexcept;

    private:
        double link_bandwidth;
        double link_latency;
        double nic_latency;
        double router_latency;
        double memory_bandwidth;
        double memory_latency;
        double memory_scale;
    };
}

#endif