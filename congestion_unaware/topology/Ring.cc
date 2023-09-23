/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "congestion_unaware/topology/Ring.hh"

using namespace NetworkAnalytical;
using namespace NetworkAnalyticalCongestionUnaware;

Ring::Ring(
    int nodes_count,
    Bandwidth bandwidth,
    Latency latency,
    bool bidirectional) noexcept
    : latency(latency), bidirectional(bidirectional) {
  assert(nodes_count > 0);
  assert(bandwidth > 0);
  assert(latency >= 0);

  // set bandwidth
  this->bandwidth = Topology::bw_GBps_to_Bpns(bandwidth);

  // set nodes_count
  set_nodes_count(nodes_count);
}

EventTime Ring::send(NodeId src, NodeId dest, ChunkSize size) const noexcept {
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

  // compute hops count
  auto hops_count = clockwise_distance;
  if (bidirectional && (anti_clockwise_distance < clockwise_distance)) {
    hops_count = anti_clockwise_distance;
  }

  // get communication delay
  auto comms_delay =
      Topology::communication_delay(bandwidth, latency, hops_count, size);
  return comms_delay;
}
