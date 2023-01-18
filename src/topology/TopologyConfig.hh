/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#ifndef __TOPOLOGY_CONFIG_HH__
#define __TOPOLOGY_CONFIG_HH__

#include <cstdint>
#include <vector>

namespace Analytical {
struct TopologyConfig {
 public:
  using NpuId = int; // each NPU's id
  using NpuAddress = std::vector<int>; // NPU's address (integer id for each dimension)
  using PayloadSize = uint64_t; // Byte
  using Latency = double; // ns
  using Bandwidth = double; // GB/s (B/ns)

  TopologyConfig(
      int npus_count,
      int mem_count,
      Latency link_latency,
      Bandwidth link_bandwidth,
      Latency nic_latency,
      Latency router_latency) noexcept;

  TopologyConfig(
      int npus_count,
      Latency link_latency,
      Bandwidth link_bandwidth,
      Latency nic_latency,
      Latency router_latency,
      Latency hbm_latency,
      Bandwidth hbm_bandwidth,
      double hbm_scale) noexcept;

  int getNpusCount() const noexcept;
  Latency getLinkLatency() const noexcept;
  Bandwidth getLinkBandwidth() const noexcept;
  Latency getNicLatency() const noexcept;
  Latency getRouterLatency() const noexcept;
  Latency getHbmLatency() const noexcept;
  Bandwidth getHbmBandwidth() const noexcept;
  double getHbmScale() const noexcept;

  void multiplyLinkBandwidth(double scalar) noexcept;

  int npus_count;
  int mem_count;
  Latency link_latency;
  Bandwidth link_bandwidth;
  Latency nic_latency;
  Latency router_latency;
  Latency hbm_latency;
  Bandwidth hbm_bandwidth;
  double hbm_scale;
};
} // namespace Analytical

#endif /* __TOPOLOGY_CONFIG_HH__ */
