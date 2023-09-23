/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "congestion_unaware/topology/FullyConnected.hh"

using namespace NetworkAnalytical;
using namespace NetworkAnalyticalCongestionUnaware;

FullyConnected::FullyConnected(
    int nodes_count,
    Bandwidth bandwidth,
    Latency latency) noexcept
    : bandwidth(bandwidth), latency(latency) {
  assert(nodes_count > 0);
  assert(bandwidth > 0);
  assert(latency >= 0);

  // set bandwidth
  this->bandwidth = Topology::bw_GBps_to_Bpns(bandwidth);

  // set nodes_count
  set_nodes_count(nodes_count);
}

EventTime FullyConnected::send(NodeId src, NodeId dest, ChunkSize size)
    const noexcept {
  assert(0 <= src && src < nodes_count);
  assert(0 <= dest && dest < nodes_count);

  // for FullyConnected topology, hops_count is always 1
  auto hops_count = 1;

  // get communication delay
  auto comms_delay =
      Topology::communication_delay(bandwidth, latency, hops_count, size);
  return comms_delay;
}
