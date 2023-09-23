/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "Ring.hh"

using namespace NetworkAnalytical;
using namespace NetworkAnalyticalCongestionUnaware;

Ring::Ring(
    int nodes_count,
    Bandwidth bandwidth,
    Latency latency,
    bool bidirectional) noexcept
    : bidirectional(bidirectional), BasicTopology(nodes_count, bandwidth, latency) {
  assert(nodes_count > 0);
  assert(bandwidth > 0);
  assert(latency >= 0);
}

int Ring::compute_hops_count(NodeId src, NodeId dest) const noexcept {
  assert(0 <= src && src < nodes_count);
  assert(0 <= dest && dest < nodes_count);
  assert(src != dest);

  // for Ring topology
  // 1. compute clockwise and anti-clockwise distance
  // 2. if uni-directional, use clockwise one
  // 3. if bi-directional, use the shorter one

  // compute dist
  auto clockwise_distance = (dest - src);
  if (clockwise_distance < 0) {
    clockwise_distance += nodes_count;
  }
  auto anti_clockwise_distance = nodes_count - clockwise_distance;


  if (!bidirectional) {
    // uni-directional: return clockwise distance
    return clockwise_distance;
  }

  // bi-directional: return shorter distance
  return (clockwise_distance < anti_clockwise_distance) ? clockwise_distance : anti_clockwise_distance;
}
