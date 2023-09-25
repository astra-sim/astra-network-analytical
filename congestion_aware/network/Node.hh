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
 * Node represents a single node in the network.
 */
class Node {
 public:
  /**
   * Constructor.
   *
   * @param id node_id of the node
   */
  explicit Node(DeviceId id) noexcept;

  /**
   * Get node_id of the node.
   *
   * @return node_id of the node
   */
  [[nodiscard]] DeviceId get_id() const noexcept;

  /**
   * Initiate a chunk transmission.
   * You must invoke this method using the source node of the chunk.
   *
   * @param chunk chunk to send
   */
  void send(std::unique_ptr<Chunk> chunk) noexcept;

  /**
   * Connect this node to another node.
   *
   * @param id node_id of the node to connect to
   * @param bandwidth bandwidth of the link
   * @param latency latency of the link
   */
  void connect(DeviceId id, Bandwidth bandwidth, Latency latency) noexcept;

 private:
  /// node node_id
  DeviceId node_id;

  /// links to other nodes
  /// map[dest node node_id] -> link
  std::map<DeviceId, std::shared_ptr<Link>> links;

  /**
   * Check if this node is connected to another node.
   *
   * @param dest node_id of the node to check
   * @return true if connected, false otherwise
   */
  [[nodiscard]] bool connected(DeviceId dest) const noexcept;
};

} // namespace NetworkAnalyticalCongestionAware
