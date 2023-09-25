/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "congestion_unaware/basic-topology/Ring.hh"

using namespace NetworkAnalytical;
using namespace NetworkAnalyticalCongestionUnaware;

Ring::Ring(
    const int npus_count,
    const Bandwidth bandwidth,
    const Latency latency,
    const bool bidirectional) noexcept
    : bidirectional(bidirectional),
      BasicTopology(npus_count, bandwidth, latency) {
  assert(npus_count > 0);
  assert(bandwidth > 0);
  assert(latency >= 0);

  // set the building block type
  basic_topology_type = TopologyBuildingBlock::Ring;
}

int Ring::compute_hops_count(const DeviceId src, const DeviceId dest)
    const noexcept {
  assert(0 <= src && src < npus_count);
  assert(0 <= dest && dest < npus_count);
  assert(src != dest);

  // for Ring basic-topology
  // 1. compute clockwise and anti-clockwise distance
  // 2. if uni-directional, use clockwise one
  // 3. if bi-directional, use the shorter one

  // compute dist
  auto clockwise_distance = (dest - src);
  if (clockwise_distance < 0) {
    clockwise_distance += npus_count;
  }
  const auto anti_clockwise_distance = npus_count - clockwise_distance;

  if (!bidirectional) {
    // uni-directional: return clockwise distance
    return clockwise_distance;
  }

  // bi-directional: return shorter distance
  return (clockwise_distance < anti_clockwise_distance)
      ? clockwise_distance
      : anti_clockwise_distance;
}
