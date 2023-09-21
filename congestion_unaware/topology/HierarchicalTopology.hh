/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#ifndef __HIERARCHICAL_TOPOLOGY_HH__
#define __HIERARCHICAL_TOPOLOGY_HH__

#include "HierarchicalTopologyConfig.hh"
#include "Topology.hh"

namespace Analytical {
class HierarchicalTopology : public Topology {
 public:
  using TopologyList = HierarchicalTopologyConfig::TopologyList;
  using DimensionType = HierarchicalTopologyConfig::DimensionType;

  HierarchicalTopology(
      TopologyConfigs& configs,
      HierarchicalTopologyConfig hierarchy_config) noexcept;

  ~HierarchicalTopology() noexcept override;

  Bandwidth getNpuTotalBandwidthPerDim(int dimension) const noexcept override;

  std::pair<double, int> send(
      NpuId src,
      NpuId dest,
      PayloadSize payload_size) noexcept override;

 private:
  Latency linkLatency(int dimension, int hops_count) const noexcept;
  NpuAddress npuIdToAddress(NpuId npu_id) const noexcept override;
  NpuId npuAddressToId(NpuAddress npu_address) const noexcept override;

  HierarchicalTopologyConfig hierarchy_config;
};
} // namespace Analytical

#endif /* __HIERARCHICAL_TOPOLOGY_HH__ */
