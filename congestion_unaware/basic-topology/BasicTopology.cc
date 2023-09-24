/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "BasicTopology.hh"

using namespace NetworkAnalytical;
using namespace NetworkAnalyticalCongestionUnaware;

BasicTopology::BasicTopology(
    int nodes_count,
    Bandwidth bandwidth,
    Latency latency) noexcept
    : latency(latency), Topology() {
  assert(nodes_count > 0);
  assert(bandwidth > 0);
  assert(latency >= 0);

  // set bandwidth
  this->bandwidth = bw_GBps_to_Bpns(bandwidth);

  // set nodes count
  set_nodes_count(nodes_count);
}

BasicTopology::~BasicTopology() noexcept = default;

EventTime BasicTopology::send(NodeId src, NodeId dest, ChunkSize size)
    const noexcept {
  assert(0 <= src && src < nodes_count);
  assert(0 <= dest && dest < nodes_count);
  assert(src != dest);
  assert(size > 0);

  // get hops count
  auto hops_count = compute_hops_count(src, dest);

  // return communication delay
  return compute_communication_delay(hops_count, size);
}

Bandwidth BasicTopology::bw_GBps_to_Bpns(Bandwidth bw_GBps) noexcept {
  assert(bw_GBps > 0);

  return bw_GBps * (1 << 30) / (1'000'000'000); // GB/s to B/ns
}

EventTime BasicTopology::compute_communication_delay(
    int hops_count,
    ChunkSize size) const noexcept {
  assert(hops_count > 0);
  assert(size > 0);

  auto link_delay = hops_count * latency;
  auto serialization_delay = size / bandwidth;
  auto comms_delay = link_delay + serialization_delay;

  return static_cast<EventTime>(comms_delay);
}
