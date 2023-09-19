/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "network/Chunk.hh"
#include <cassert>
#include "network/Link.hh"
#include "network/Node.hh"

using namespace Congestion;

void Chunk::chunk_arrived_next_node(void* chunk_ptr) noexcept {
  // cast to unique_ptr<Chunk>
  auto chunk = std::unique_ptr<Chunk>(static_cast<Chunk*>(chunk_ptr));

  // mark chunk arrived next node
  chunk->arrived_next_node();

  if (chunk->arrived_dest()) {
    // chunk arrived dest, invoke callback
    chunk->invoke_callback();
  } else {
    // send this chunk to next dest
    auto current_node = chunk->current_node();
    current_node->send(std::move(chunk));
  }
}

Chunk::Chunk(
    ChunkSize size,
    Route route,
    Callback callback,
    CallbackArg callback_arg) noexcept
    : size(size),
      route(std::move(route)),
      callback(callback),
      callback_arg(callback_arg) {}

Chunk::~Chunk() noexcept = default;

std::shared_ptr<Node> Chunk::current_node() const noexcept {
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
  // this method to be called, the chunk hasn't arrived its final dest yet
  assert(!arrived_dest());

  // pop previous npu from route
  route.pop_front();
}

bool Chunk::arrived_dest() const noexcept {
  // if a chunk arrived dest, route length should be 1
  return route.size() == 1;
}

ChunkSize Chunk::get_size() const noexcept {
  return size;
}

void Chunk::invoke_callback() noexcept {
  // invoke callback
  (*callback)(callback_arg);
}
