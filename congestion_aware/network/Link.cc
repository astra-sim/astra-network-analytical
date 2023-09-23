/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "congestion_aware/network/Link.hh"
#include "congestion_aware/network/Chunk.hh"
#include "congestion_aware/network/Node.hh"

using namespace NetworkAnalyticalCongestionAware;

// declaring static event_queue
std::shared_ptr<EventQueue> Link::event_queue;

void Link::link_become_free(void* link_ptr) noexcept {
  assert(link_ptr != nullptr);

  // cast to Link*
  auto* link = static_cast<Link*>(link_ptr);

  // set link free
  link->set_free();

  // process pending chunks if one exist
  if (link->pending_chunk_exists()) {
    link->process_pending_transmission();
  }
}

void Link::set_event_queue(
    std::shared_ptr<EventQueue> event_queue_ptr) noexcept {
  assert(event_queue_ptr != nullptr);

  // set the event queue
  Link::event_queue = std::move(event_queue_ptr);
}

Link::Link(Bandwidth bandwidth, Latency latency) noexcept
    : bandwidth(bandwidth), latency(latency), pending_chunks(), busy(false) {
  assert(bandwidth > 0);
  assert(latency >= 0);
}

void Link::send(std::unique_ptr<Chunk> chunk) noexcept {
  assert(chunk != nullptr);

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

EventTime Link::serialization_delay(ChunkSize size) const noexcept {
  assert(size > 0);

  // calculate serialization delay
  auto delay = size / bandwidth;

  return static_cast<EventTime>(delay);
}

EventTime Link::communication_delay(ChunkSize size) const noexcept {
  assert(size > 0);

  // calculate communication delay
  auto delay = latency + (size / bandwidth);

  return static_cast<EventTime>(delay);
}

void Link::schedule_chunk_transmission(std::unique_ptr<Chunk> chunk) noexcept {
  assert(chunk != nullptr);

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
