/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#ifndef __TOPOLOGY_HH__
#define __TOPOLOGY_HH__

#include <vector>
#include <memory>

#include "topology/CostModel.hh"
#include "topology/TopologyConfig.hh"
#include "event-queue/EventQueue.hh"

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

  virtual ~Topology() noexcept = 0;

  virtual std::pair<double, int> send(
      NpuId src,
      NpuId dest,
      PayloadSize payloadSize) noexcept = 0;

  virtual Bandwidth getNpuTotalBandwidthPerDim(int dimension) const noexcept;

  static void set_event_queue(
      const std::shared_ptr<EventQueue>& event_queue_ptr) noexcept;

  CostModel& getCostModel() noexcept;


 protected:
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

  TopologyConfigs& configs; // TopologyConfigs for each dimension
  int npus_count; // NPUs count of the topology
  CostModel cost_model;

  // Link the shared event Queue
  static std::shared_ptr<EventQueue> event_queue;
};
} // namespace Analytical

#endif /* __TOPOLOGY_HH__ */
