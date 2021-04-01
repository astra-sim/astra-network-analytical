/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "FastRing.hh"
#include "cmath"

using namespace Analytical;

FastRing::FastRing(TopologyConfigs& configs) noexcept : FastTopology(configs) {
  auto links_count = npus_count;

  // fixme: assuming 2 links per node
  // cost_model.addResource(CostModel::ResourceType::Npu, npus_count);
  // cost_model.addResource(CostModel::ResourceType::NVLink, links_count);
}

FastRing::~FastRing() noexcept = default;

std::pair<double, int> FastRing::send(
    NpuId src,
    NpuId dest,
    PayloadSize payload_size) noexcept {
  // check NPU ID validity
  checkNpuIdBound(src);
  checkNpuIdBound(dest);

  // Check src and dest differs
  if (src == dest) {
    return std::make_pair(0, -1);
  }

  // ring topology: compute hops count
  // FIXME: assume bidirectional ring (always takes shortest path)
  auto distanceA = std::abs(src - dest);
  auto distanceB = npus_count - distanceA;
  auto hops_count = (distanceA < distanceB)
      ? distanceA
      : distanceB; // shorter one becomes the hop count

  // compute communication latency
  // dimension is 0
  auto communication_latency = linkLatency(0, hops_count);
  communication_latency += serializationLatency(0, payload_size);
  communication_latency += 2 * nicLatency(0); // passes 2 NICs: input and output

  // compute hbm latency
  auto hbm_latency = hbmLatency(0, payload_size);

  // return critical latency
  return std::make_pair(
      criticalLatency(communication_latency, hbm_latency), -1);
}
