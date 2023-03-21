/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#ifndef __TOPOLOGY_HH__
#define __TOPOLOGY_HH__

#include <vector>
#include "CostModel.hh"
#include "TopologyConfig.hh"

namespace Analytical {
class Topology {
 public:
  using TopologyConfigs = std::vector<TopologyConfig>;

  using NpuId = TopologyConfig::NpuId;
  using NpuAddress = TopologyConfig::NpuAddress;
  using PayloadSize = TopologyConfig::PayloadSize;
  using Latency = TopologyConfig::Latency;
  using Bandwidth = TopologyConfig::Bandwidth;

  Topology(TopologyConfigs& configs) noexcept;

  CostModel& getCostModel() noexcept;

  virtual ~Topology() noexcept = 0;

  virtual std::pair<double, int> send(
      NpuId src,
      NpuId dest,
      PayloadSize payloadSize) noexcept = 0;

  virtual Bandwidth getNpuTotalBandwidthPerDim(int dimension) const noexcept;

 protected:
  TopologyConfigs& configs; // TopologyConfigs for each dimension
  int npus_count; // NPUs count of the topology

  CostModel cost_model;

  void checkNpuIdBound(NpuId npu_id) const noexcept;

  Latency serializationLatency(int dimension, PayloadSize payload_size)
      const noexcept;
  Latency nicLatency(int dimension) const noexcept;
  Latency routerLatency(int dimension) const noexcept;
  Latency hbmLatency(int dimension, PayloadSize payload_size) const noexcept;
  Latency criticalLatency(Latency communication_latency, Latency hbm_latency)
      const noexcept;

  virtual NpuAddress npuIdToAddress(NpuId npu_id) const noexcept;
  virtual NpuId npuAddressToId(NpuAddress npu_address) const noexcept;
};
} // namespace Analytical

#endif
