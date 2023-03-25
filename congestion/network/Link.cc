/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "network/Link.hh"
#include <cassert>
#include "network/Chunk.hh"
#include "network/Node.hh"

using namespace Congestion;

// declaring static event_queue
std::shared_ptr<EventQueue> Link::event_queue;

void Link::link_become_free(void* link_ptr) noexcept {
  // cast to Link*
  auto link = static_cast<Link*>(link_ptr);

  // set link free
  link->set_free();

  // process pending chunks if one exist
  if (link->pending_chunk_exists()) {
    link->process_pending_transmission();
  }
}

void Link::link_event_queue(std::shared_ptr<EventQueue> event_queue) noexcept {
  // link event queue
  Link::event_queue = event_queue;
}

Link::Link(Bandwidth bandwidth, Latency latency) noexcept
    : bandwidth(bandwidth), latency(latency), pending_chunks(), busy(false) {}

Link::~Link() noexcept = default;

void Link::send(std::unique_ptr<Chunk> chunk) noexcept {
  if (busy) {
    // add to pending chunks
    pending_chunks.push_back(std::move(chunk));
  } else {
    // service this chunk immediately
    schedule_chunk_transmission(std::move(chunk));
  }
}

void Link::process_pending_transmission() noexcept {
  // pending chunk should exist
  assert(pending_chunk_exists());

  // get chunk to process
  auto chunk = std::move(pending_chunks.front());
  pending_chunks.pop_front();

  // service this chunk
  schedule_chunk_transmission(std::move(chunk));
}

bool Link::pending_chunk_exists() const noexcept {
  return !pending_chunks.empty();
}

void Link::set_busy() noexcept {
  busy = true;
}

void Link::set_free() noexcept {
  busy = false;
}

Time Link::serialization_delay(ChunkSize size) const noexcept {
  return size / bandwidth;
}

Time Link::communication_delay(ChunkSize size) const noexcept {
  return latency + serialization_delay(size);
}

void Link::schedule_chunk_transmission(std::unique_ptr<Chunk> chunk) noexcept {
  // link should be free
  assert(!busy);

  // set link busy
  set_busy();

  // get metadata
  auto chunk_size = chunk->get_size();
  auto current_time = Link::event_queue->get_current_time();

  // schedule chunk arrival event
  auto communication_time = communication_delay(chunk_size);
  auto chunk_arrival_time = current_time + communication_time;
  auto chunk_ptr = static_cast<void*>(chunk.release());
  Link::event_queue->schedule_event(
      chunk_arrival_time, Chunk::chunk_arrived_next_node, chunk_ptr);

  // schedule link free time
  auto serialization_time = serialization_delay(chunk_size);
  auto link_free_time = current_time + serialization_time;
  auto link_ptr = static_cast<void*>(this);
  Link::event_queue->schedule_event(link_free_time, link_become_free, link_ptr);
}
