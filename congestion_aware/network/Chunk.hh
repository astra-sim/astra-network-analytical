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
 * Chunk class
 */
class Chunk {
 public:
  /**
   * Callback to be invoked when a chunk arrives at the next node.
   *   - if the chunk arrived at its destination, the callback is invoked
   *   - if not, the chunk is sent to the next node
   *
   * @param chunk_ptr: raw pointer to the chunk
   */
  static void chunk_arrived_next_node(void* chunk_ptr) noexcept;

  /**
   * Constructor
   *
   * @param size: size of the chunk
   * @param route: route of the chunk to its destination
   * @param callback: callback to be invoked when the chunk arrives at its
   * destination
   * @param callback_arg: argument to be passed to the callback
   */
  Chunk(
      ChunkSize size,
      Route route,
      Callback callback,
      CallbackArg callback_arg) noexcept;

  /**
   * Get the current node of the chunk
   *
   * @return current node of the chunk
   */
  [[nodiscard]] std::shared_ptr<Node> current_node() const noexcept;

  /**
   * Get the next node of the chunk
   *
   * @return next node of the chunk
   */
  [[nodiscard]] std::shared_ptr<Node> next_node() const noexcept;

  /**
   * Mark the chunk arrived next node
   * i.e., drop the current node from the route
   */
  void arrived_next_node() noexcept;

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
  // Route has the structure of [current Node, next Node, ..., dest Node]
  // e.g., if a chunk starts from node 5, then reaches destination 3,
  // the route would be e.g., [5, 1, 6, 2, 3]
  Route route;

  /// callback to be invoked when the chunk arrives at its destination
  Callback callback;

  /// argument to be passed to the callback
  CallbackArg callback_arg;
};

} // namespace NetworkAnalyticalCongestionAware
