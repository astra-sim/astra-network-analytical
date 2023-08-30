/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#pragma once

#include <list>
#include <memory>
#include "event-queue/EventQueue.hh"
#include "type/Type.hh"

namespace Congestion {

class Chunk;
class Node;

/**
 * Link models physical links between two nodes.
 * Chunks are being queued to this class instances.
 */
class Link {
 public:
  /**
   * Callback to be called when a link becomes free.
   *   - If the link has pending chunks, process the first one.
   *
   * @param link_ptr raw pointer to the link
   */
  static void link_become_free(void* link_ptr) noexcept;

  /**
   * Link common event queue to the Link class.
   *
   * @param event_queue (shared) pointer to the event queue
   */
  static void link_event_queue(
      std::shared_ptr<EventQueue> event_queue) noexcept;

  /**
   * Constructor
   *
   * @param bandwidth bandwidth of the link
   * @param latency latency of the link
   */
  Link(Bandwidth bandwidth, Latency latency) noexcept;

  /**
   * Destructor
   */
  ~Link() noexcept;

  /**
   * Send a chunk through the link.
   * - If the link is free, service the chunk immediately.
   * - If the link is busy, add the chunk to the pending chunks list.
   *
   * @param chunk (unique) pointer to the chunk
   */
  void send(std::unique_ptr<Chunk> chunk) noexcept;

  /**
   * Process the first pending chunk.
   */
  void process_pending_transmission() noexcept;

  /**
   * Check if the link has pending chunks.
   *
   * @return true if the link has pending chunks, false otherwise
   */
  bool pending_chunk_exists() const noexcept;

  /**
   * Set the link as busy.
   */
  void set_busy() noexcept;

  /**
   * Set the link as free.
   */
  void set_free() noexcept;

 private:
  /// Pointer to the shared event queue
  static std::shared_ptr<EventQueue> event_queue;

  /// bandwidth and latency of the link
  Bandwidth bandwidth;
  Latency latency;

  /// queue of pending chunks
  std::list<std::unique_ptr<Chunk>> pending_chunks;

  /// flag to indicate if the link is busy
  bool busy;

  /**
   * Compute the serialization delay of a chunk.
   *
   * @param size size of the target chunk
   * @return serialization delay of the chunk
   */
  Time serialization_delay(ChunkSize size) const noexcept;

  /**
   * Compute the communication delay of a chunk.
   * i.e., communication delay = (link latency) + (serialization delay)
   *
   * @param size size of the target chunk
   * @return communication delay of the chunk
   */
  Time communication_delay(ChunkSize size) const noexcept;

  /**
   * Schedule the transmission of a chunk.
   * - Set the link as busy.
   * - Link becomes free after the serialization delay.
   * - Chunk arrives next node after the communication delay.
   *
   * @param chunk (unique) pointer to the chunk
   */
  void schedule_chunk_transmission(std::unique_ptr<Chunk> chunk) noexcept;
};

} // namespace Congestion
