/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "TopologyConfig.hh"

using namespace Analytical;

TopologyConfig::TopologyConfig(
    int npus_count,
    Latency link_latency,
    Bandwidth link_bandwidth,
    Latency nic_latency,
    Latency router_latency,
    Latency hbm_latency,
    Bandwidth hbm_bandwidth,
    double hbm_scale) noexcept
    : npus_count(npus_count),
      link_latency(link_latency),
      link_bandwidth(link_bandwidth),
      nic_latency(nic_latency),
      router_latency(router_latency),
      hbm_latency(hbm_latency),
      hbm_bandwidth(hbm_bandwidth),
      hbm_scale(hbm_scale) {}

int TopologyConfig::getNpusCount() const noexcept {
  return npus_count;
}

TopologyConfig::Latency TopologyConfig::getLinkLatency() const noexcept {
  return link_latency;
}

TopologyConfig::Bandwidth TopologyConfig::getLinkBandwidth() const noexcept {
  return link_bandwidth;
}

void TopologyConfig::multiplyLinkBandwidth(double scalar) noexcept {
  link_bandwidth *= scalar;
}

TopologyConfig::Latency TopologyConfig::getNicLatency() const noexcept {
  return nic_latency;
}

TopologyConfig::Latency TopologyConfig::getRouterLatency() const noexcept {
  return router_latency;
}

TopologyConfig::Latency TopologyConfig::getHbmLatency() const noexcept {
  return hbm_latency;
}

TopologyConfig::Bandwidth TopologyConfig::getHbmBandwidth() const noexcept {
  return hbm_bandwidth;
}

double TopologyConfig::getHbmScale() const noexcept {
  return hbm_scale;
}
