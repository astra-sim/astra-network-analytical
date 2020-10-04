#ifndef __TOPOLOGYCONFIGURATION_HH__
#define __TOPOLOGYCONFIGURATION_HH__


namespace AnalyticalBackend {
    struct TopologyConfiguration {
    public:
        /**
         * Bandwidth in GB/s = B/ns
         */
        using Bandwidth = double;

        /**
         * Latency in ns
         */
        using Latency = double;

        /**
         * Scaling factor
         */
        using Scale = double;

        TopologyConfiguration(Bandwidth link_bandwidth,
                              Latency link_latency,
                              Latency nic_latency,
                              Latency router_latency,
                              Bandwidth hbm_bandwidth,
                              Latency hbm_latency,
                              Scale hbm_scale) noexcept;

        Bandwidth getBandwidth() const noexcept;
        Latency getLinkLatency() const noexcept;
        Latency getNIC_Latency() const noexcept;
        Latency getRouterLatency() const noexcept;
        Bandwidth getHBM_Bandwidth() const noexcept;
        Latency getHBM_Latency() const noexcept;
        Scale getHBM_Scale() const noexcept;

    private:
        Bandwidth link_bandwidth;
        Latency link_latency;
        Latency nic_latency;
        Latency router_latency;
        Bandwidth hbm_bandwidth;
        Latency hbm_latency;
        Scale hbm_scale;
    };
}

#endif
