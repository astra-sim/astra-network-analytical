/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "Topology.hh"
#include <cassert>

using namespace Analytical;

Topology::Topology(TopologyConfigs& configs) noexcept : configs(configs) {
  // create cost model
  cost_model = CostModel();

  // get total npus_count by multiplying npus_count of each dimension
  npus_count = 1;
  for (const auto& config : configs) {
    npus_count *= config.getNpusCount();
  }
}

Topology::~Topology() noexcept = default;

CostModel& Topology::getCostModel() noexcept {
  return cost_model;
}

void Topology::checkNpuIdBound(NpuId npu_id) const noexcept {
  assert(
      npu_id < npus_count &&
      "[Topology, method checkNpuIdBound] NPU ID out of bounds");
}

Topology::NpuAddress Topology::npuIdToAddress(NpuId npu_id) const noexcept {
  // Baseline implementation: assume 1d topology
  return NpuAddress(1, npu_id);
}

Topology::NpuId Topology::npuAddressToId(
    NpuAddress npu_address) const noexcept {
  // Baseline implementation: assume 1d topology
  return npu_address[0];
}

Topology::Latency Topology::serializationLatency(
    int dimension,
    PayloadSize payload_size) const noexcept {
  return payload_size / configs[dimension].getLinkBandwidth();
}

Topology::Latency Topology::nicLatency(int dimension) const noexcept {
  return configs[dimension].getNicLatency();
}

Topology::Latency Topology::routerLatency(int dimension) const noexcept {
  return configs[dimension].getRouterLatency();
}

Topology::Bandwidth Topology::getNpuTotalBandwidthPerDim(
    int dimension) const noexcept {
  return configs[dimension].getLinkBandwidth();
}

Topology::Latency Topology::hbmLatency(int dimension, PayloadSize payload_size)
    const noexcept {
  auto hbm_latency = configs[dimension].getHbmLatency(); // HBM baseline latency
  hbm_latency += payload_size /
      configs[dimension].getHbmBandwidth(); // HBM serialization latency
  return hbm_latency *
      configs[dimension].getHbmScale(); // return scaled HBM latency
}

Topology::Latency Topology::criticalLatency(
    Latency communication_latency,
    Latency hbm_latency) const noexcept {
  // return larger latency
  return (communication_latency > hbm_latency) ? communication_latency
                                               : hbm_latency;
}
