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
        using Latency = double;  // latency in ns
        using Bandwidth = double;  // bandwidth in GB/s (= B/ns)
        using PayloadSize = int;  // payload size in bytes

        TopologyConfiguration(Latency link_latency,
                              Bandwidth link_bandwidth,
                              Latency nic_latency,
                              Latency router_latency,
                              Latency hbm_latency,
                              Bandwidth hbm_bandwidth,
                              double hbm_scalar) noexcept;

        Latency getLinkLatency() const noexcept;
        Bandwidth getLinkBandwidth() const noexcept;
        Latency getNicLatency() const noexcept;
        Latency getRouterLatency() const noexcept;
        Latency getHbmLatency() const noexcept;
        Bandwidth getHbmBandwidth() const noexcept;
        double getHbmScalar() const noexcept;

    private:
        Latency link_latency;
        Bandwidth link_bandwidth;
        Latency nic_latency;
        Latency router_latency;
        Latency hbm_latency;
        Bandwidth hbm_bandwidth;
        double hbm_scalar;
    };
}

#endif
