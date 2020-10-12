/******************************************************************************
Copyright (c) 2020 Georgia Institute of Technology
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

Author : William Won (william.won@gatech.edu)
*******************************************************************************/

#ifndef __TOPOLOGYCONFIGURATION_HH__
#define __TOPOLOGYCONFIGURATION_HH__


namespace Analytical {
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
