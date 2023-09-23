/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "congestion_unaware/topology/Topology.hh"

using namespace NetworkAnalytical;
using namespace NetworkAnalyticalCongestionUnaware;

Topology::Topology() noexcept : nodes_count(-1) {}

EventTime Topology::communication_delay(
    Bandwidth bandwidth,
    Latency latency,
    int hops_count,
    ChunkSize size) noexcept {
  assert(bandwidth > 0);
  assert(latency >= 0);
  assert(size > 0);

  auto link_delay = hops_count * latency;
  auto serialization_delay = size / bandwidth;
  auto comms_delay = link_delay + serialization_delay;

  return static_cast<EventTime>(comms_delay);
}

Bandwidth Topology::bw_GBps_to_Bpns(Bandwidth bw_GBps) noexcept {
  return bw_GBps * (1 << 30) / (1'000'000'000); // GB/s to B/ns
}

void Topology::set_nodes_count(int new_nodes_count) noexcept {
  assert(new_nodes_count > 0);

  nodes_count = new_nodes_count;
}
