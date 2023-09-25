/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#pragma once

#include "common/Common.hh"
#include "common/Type.hh"
#include "congestion_aware/network/Type.hh"

using namespace NetworkAnalytical;

namespace NetworkAnalyticalCongestionAware {

/**
 * Chunk class represents a chunk.
 */
class Chunk {
 public:
  /**
   * Callback to be invoked when a chunk arrives at the next device.
   *   - if the chunk arrived at its destination, the callback is invoked
   *   - if not, the chunk is sent to the next device
   *
   * @param chunk_ptr: raw pointer to the chunk
   */
  static void chunk_arrived_next_device(void* chunk_ptr) noexcept;

  /**
   * Constructor
   *
   * @param chunk_size: size of the chunk
   * @param route: route of the chunk to its destination
   * @param callback: callback to be invoked when the chunk arrives at its
   * destination
   * @param callback_arg: argument to be passed to the callback
   */
  Chunk(
      ChunkSize chunk_size,
      Route route,
      Callback callback,
      CallbackArg callback_arg) noexcept;

  /**
   * Get the current device of the chunk
   *
   * @return current device of the chunk
   */
  [[nodiscard]] std::shared_ptr<Device> current_device() const noexcept;

  /**
   * Get the next device of the chunk
   *
   * @return next device of the chunk
   */
  [[nodiscard]] std::shared_ptr<Device> next_device() const noexcept;

  /**
   * Mark the chunk arrived next device
   * i.e., drop the current device from the route
   */
  void mark_arrived_next_device() noexcept;

  /**
   * Check if the chunk arrived at its destination
   * i.e., if the route length is 1 (only destination left)
   *
   * @return true if the chunk arrived at its destination, false otherwise
   */
  [[nodiscard]] bool arrived_dest() const noexcept;

  /**
   * Get the size of the chunk
   *
   * @return size of the chunk
   */
  [[nodiscard]] ChunkSize get_size() const noexcept;

  /**
   * Invoke the callback
   * i.e., this method should be called when the chunk arrives at its
   * destination.
   */
  void invoke_callback() noexcept;

 private:
  /// size of the chunk
  ChunkSize chunk_size;

  /// route of the chunk to its destination
  // Route has the structure of [current device, next device, ..., dest device]
  // e.g., if a chunk starts from device 5, then reaches destination 3,
  // the route would be e.g., [5, 1, 6, 2, 3]
  Route route;

  /// callback to be invoked when the chunk arrives at its destination
  Callback callback;

  /// argument to be passed to the callback
  CallbackArg callback_arg;
};

} // namespace NetworkAnalyticalCongestionAware
