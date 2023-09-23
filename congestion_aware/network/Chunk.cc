/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "congestion_aware/network/Chunk.hh"
#include "congestion_aware/network/Link.hh"
#include "congestion_aware/network/Node.hh"

using namespace NetworkAnalyticalCongestionAware;

void Chunk::chunk_arrived_next_node(void* chunk_ptr) noexcept {
  assert(chunk_ptr != nullptr);

  // cast to unique_ptr<Chunk>
  auto chunk = std::unique_ptr<Chunk>(static_cast<Chunk*>(chunk_ptr));

  // mark chunk arrived next node
  chunk->arrived_next_node();

  if (chunk->arrived_dest()) {
    // chunk arrived dest, invoke callback
    // as chunk is unique_ptr, will be destroyed automatically
    chunk->invoke_callback();
  } else {
    // send this chunk to next dest
    auto current_node = chunk->current_node();
    current_node->send(std::move(chunk)); // send chunk to next des
  }
}

Chunk::Chunk(
    ChunkSize size,
    Route route,
    Callback callback,
    CallbackArg callback_arg) noexcept
    : chunk_size(size),
      route(std::move(route)),
      callback(callback),
      callback_arg(callback_arg) {
  assert(size > 0);
  assert(!this->route.empty());
  assert(callback != nullptr);
}

std::shared_ptr<Node> Chunk::current_node() const noexcept {
  // assert the route is not empty
  assert(!route.empty());

  // return the first npu in route
  return route.front();
}

std::shared_ptr<Node> Chunk::next_node() const noexcept {
  // assert the chunk has next dest
  assert(!arrived_dest());

  // return next dest
  auto next_dest = std::next(route.begin(), 1);
  return *next_dest;
}

void Chunk::arrived_next_node() noexcept {
  // if this method is being called,
  // it means the chunk hasn't arrived its final dest yet
  assert(!arrived_dest());

  // pop previous node from the route
  // marking the current node has been changed
  route.pop_front();
}

bool Chunk::arrived_dest() const noexcept {
  // if a chunk arrived dest, route length should be 1
  // i.e., only containing the dest node
  return route.size() == 1;
}

ChunkSize Chunk::get_size() const noexcept {
  // return chunk size
  return chunk_size;
}

void Chunk::invoke_callback() noexcept {
  // invoke callback
  (*callback)(callback_arg);
}
