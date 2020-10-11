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
