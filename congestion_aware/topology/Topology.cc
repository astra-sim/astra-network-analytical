/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "Topology.hh"
#include "network/Chunk.hh"
#include "network/Link.hh"
#include "network/Node.hh"

using namespace Congestion;

void Topology::link_event_queue(
    std::shared_ptr<EventQueue> event_queue) noexcept {
  // link this event_queue with Link class.
  Link::link_event_queue(event_queue);
}

Topology::Topology(int npus_count) noexcept : npus_count(npus_count) {
  assert(npus_count > 0);

  // instantiate npus
  for (auto i = 0; i < npus_count; i++) {
    npus.push_back(std::make_shared<Node>(i));
  }
}

Topology::~Topology() noexcept = default;

void Topology::send(std::unique_ptr<Chunk> chunk) noexcept {
  // get src npu id
  auto src = chunk->current_node()->get_id();

  // initiate transmission from src
  npus[src]->send(std::move(chunk));
}

void Topology::connect(
    NodeId src,
    NodeId dest,
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
