/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "FastSwitch.hh"

using namespace Analytical;

FastSwitch::FastSwitch(TopologyConfigs& configs) noexcept
    : FastTopology(configs) {
  auto links_count = npus_count;
  auto switches_count = cost_model.getNVSwitchesCount(npus_count);

  cost_model.addResource(CostModel::Resource::Npu, npus_count);
  cost_model.addResource(CostModel::Resource::NVLink, links_count);
  cost_model.addResource(CostModel::Resource::NVSwitch, switches_count);
}

FastSwitch::~FastSwitch() noexcept = default;

std::pair<double, int> FastSwitch::send(
    NpuId src,
    NpuId dest,
    PayloadSize payload_size) noexcept {
  checkNpuIdBound(src);
  checkNpuIdBound(dest);

  // Check src and dest differs
  if (src == dest) {
    return std::make_pair(0, -1);
  }

  // switch topology: hop count is 2
  const auto hops_count = 2;

  // compute communication latency
  // dimension is 0
  auto communication_latency = linkLatency(0, hops_count);
  communication_latency += serializationLatency(0, payload_size);
  communication_latency += 2 * nicLatency(0); // passes 2 NICs: input and output
  communication_latency += routerLatency(0);

  // compute hbm latency
  auto hbm_latency = hbmLatency(0, payload_size);

  // return critical latency
  return std::make_pair(criticalLatency(communication_latency, hbm_latency), -1);
}
