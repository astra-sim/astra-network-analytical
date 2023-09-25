/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "congestion_aware/network/Node.hh"
#include "congestion_aware/network/Chunk.hh"
#include "congestion_aware/network/Link.hh"

using namespace NetworkAnalyticalCongestionAware;

Node::Node(DeviceId id) noexcept : node_id(id) {
  assert(id >= 0);
}

DeviceId Node::get_id() const noexcept {
  return node_id;
}

void Node::send(std::unique_ptr<Chunk> chunk) noexcept {
  assert(chunk != nullptr);

  // assert this node is the source of the chunk
  assert(chunk->current_node()->get_id() == node_id);

  // assert the chunk hasn't arrived its final destination yet
  assert(!chunk->arrived_dest());

  // get next dest
  auto next_dest = chunk->next_node();
  auto next_dest_id = next_dest->get_id();

  // assert the next dest is connected to this node
  assert(connected(next_dest_id));

  // send the chunk to the next dest
  // delegate this task to the link
  links[next_dest_id]->send(std::move(chunk));
}

void Node::connect(DeviceId id, Bandwidth bandwidth, Latency latency) noexcept {
  assert(id >= 0);
  assert(bandwidth > 0);
  assert(latency >= 0);

  // assert there's no existing connection
  assert(!connected(id));

  // create link
  links[id] = std::make_shared<Link>(bandwidth, latency);
}

bool Node::connected(DeviceId dest) const noexcept {
  assert(dest >= 0);

  // check whether the connection exists
  return links.find(dest) != links.end();
}
