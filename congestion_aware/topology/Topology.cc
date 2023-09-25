/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "congestion_aware/topology/Topology.hh"

using namespace NetworkAnalyticalCongestionAware;

void Topology::set_event_queue(
    std::shared_ptr<EventQueue> event_queue) noexcept {
  assert(event_queue != nullptr);

  // link this event_queue with Link class.
  Link::set_event_queue(std::move(event_queue));
}

Topology::Topology(int npus_count) noexcept : npus_count(npus_count) {
  assert(npus_count > 0);

  // instantiate npus
  for (auto i = 0; i < npus_count; i++) {
    npus.push_back(std::make_shared<Node>(i));
  }
}

int Topology::get_nodes_count() const noexcept {
  assert(npus_count > 0);

  return npus_count;
}

void Topology::send(std::unique_ptr<Chunk> chunk) noexcept {
  assert(chunk != nullptr);

  // get src npu node_id
  auto src = chunk->current_node()->get_id();

  // assert src is valid
  assert(0 <= src && src < npus_count);

  // initiate transmission from src
  npus[src]->send(std::move(chunk));
}

void Topology::connect(
    DeviceId src,
    DeviceId dest,
    Bandwidth bandwidth,
    Latency latency,
    bool bidirectional) noexcept {
  // assert the src and dest are valid
  assert(0 <= src && src < npus_count);
  assert(0 <= dest && dest < npus_count);

  // assert bandwidth and latency are valid
  assert(bandwidth > 0);
  assert(latency >= 0);

  // connect src -> dest
  npus[src]->connect(dest, bandwidth, latency);

  // if bidirectional, connect dest -> src
  if (bidirectional) {
    npus[dest]->connect(src, bandwidth, latency);
  }
}
