/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#pragma once

#include <memory>
#include "common/EventQueue.hh"
#include "common/Type.hh"
#include "congestion_aware/Type.hh"

using namespace NetworkAnalytical;

namespace NetworkAnalyticalCongestionAware {

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
   * Link event queue to the Link class.
   *
   * @param event_queue (shared) pointer to the event queue
   */
  static void set_event_queue(
      std::shared_ptr<EventQueue> event_queue_ptr) noexcept;

  /**
   * Constructor
   *
   * @param bandwidth bandwidth of the link
   * @param latency latency of the link
   */
  Link(Bandwidth bandwidth, Latency latency) noexcept;

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
  [[nodiscard]] bool pending_chunk_exists() const noexcept;

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
  Bandwidth bandwidth_Bpns;
  Latency latency;

  /// queue of pending chunks
  std::list<std::unique_ptr<Chunk>> pending_chunks;

  /// flag to indicate if the link is busy
  bool busy;

  /**
   * Compute the serialization delay of a chunk.
   *
   * @param chunk_size size of the target chunk
   * @return serialization delay of the chunk
   */
  [[nodiscard]] EventTime serialization_delay(
      ChunkSize chunk_size) const noexcept;

  /**
   * Compute the communication delay of a chunk.
   * i.e., communication delay = (link latency) + (serialization delay)
   *
   * @param chunk_size size of the target chunk
   * @return communication delay of the chunk
   */
  [[nodiscard]] EventTime communication_delay(
      ChunkSize chunk_size) const noexcept;

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

} // namespace NetworkAnalyticalCongestionAware