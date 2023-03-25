/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "network/Node.hh"
#include <cassert>
#include "network/Chunk.hh"
#include "network/Link.hh"

using namespace Congestion;

Node::Node(NodeId id) noexcept : id(id) {}

Node::~Node() noexcept = default;

NodeId Node::get_id() const noexcept {
  return id;
}

void Node::send(std::unique_ptr<Chunk> chunk) noexcept {
  // assert this node is the source of the chunk
  assert(chunk->current_node()->get_id() == id);

  // assert the chunk has next dest
  assert(!chunk->arrived_dest());

  // get next dest
  auto next_dest = chunk->next_node();
  auto next_dest_id = next_dest->get_id();

  // assert the next dest is connected
  assert(connected(next_dest_id));

  // send the chunk to the next dest
  links[next_dest_id]->send(std::move(chunk));
}

void Node::connect(NodeId id, Bandwidth bandwidth, Latency latency) noexcept {
  // assert there's no existing connection
  assert(!connected(id));

  // create link
  links[id] = std::make_shared<Link>(bandwidth, latency);
}

bool Node::connected(NodeId dest) const noexcept {
  // check whether the connection exists
  return links.find(dest) != links.end();
}
