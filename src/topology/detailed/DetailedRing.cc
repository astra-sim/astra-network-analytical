/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "DetailedRing.hh"
#include <cmath>

using namespace Analytical;

DetailedRing::DetailedRing(TopologyConfigs& configs) noexcept
    : DetailedTopology(configs) {
  // FIXME: assume the ring is bidirectional

  // connect 0<->1, 1<->2, ..., (end-2)<->(end-1)
  for (auto src = 0; src < npus_count - 1; src++) {
    auto dest = src + 1;
    connect(src, dest, 0);
    connect(dest, src, 0);
  }

  // connect (end-1) <-> 0
  if (npus_count > 2) {
    auto src = npus_count - 1;
    auto dest = 0;
    connect(src, dest, 0);
    connect(dest, src, 0);
  }
}

DetailedRing::~DetailedRing() noexcept = default;

std::pair<double, int> DetailedRing::send(
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

  // check direction
  auto direction = computeDirection(src, dest);

  // move
  auto communication_latency = 0.0;
  auto current_npu = src;
  while (current_npu != dest) {
    // compute next_npu
    auto next_npu = takeStep(current_npu, direction);

    // transmit to next npu
    communication_latency += transmit(current_npu, next_npu, payload_size);
    communication_latency += 2 * nicLatency(0);
    current_npu = next_npu;
  }

  // serialization delay
  communication_latency += serializationLatency(0, payload_size);

  // hbm
  auto hbm_latency = hbmLatency(0, payload_size);

  return std::make_pair(
      criticalLatency(communication_latency, hbm_latency), -1);
}

DetailedRing::Direction DetailedRing::computeDirection(NpuId src, NpuId dest) {
  // for explanation, assume ring_size = 10
  auto natural_distance = std::abs(
      src - dest); // 1->2->3->4 (src < dest, +1) or 9->8->7 (src > dest, -1)
  auto reverse_distance =
      npus_count - natural_distance; // 0->9->8 (src < dest, -1) or 8->9->0->1
                                     // (src > dest, +1)

  if (natural_distance <= reverse_distance) {
    return (src < dest) ? 1 : -1;
  }

  return (src < dest) ? -1 : 1;
}

DetailedRing::NpuId DetailedRing::takeStep(
    NpuId current_node,
    Direction direction) {
  // take a step
  auto next_node = current_node + direction;

  // ring operation
  if (next_node < 0) {
    next_node += npus_count;
  } else if (next_node >= npus_count) {
    next_node -= npus_count;
  }

  return next_node;
}
