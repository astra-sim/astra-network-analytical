/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#pragma once

#include <map>
#include <memory>
#include "type/Type.hh"

namespace Congestion {

class Chunk;
class Link;

/**
 * Node represents a single node in the network.
 */
class Node {
 public:
  /**
   * Constructor.
   *
   * @param id id of the node
   */
  explicit Node(NodeId id) noexcept;

  /**
   * Destructor.
   */
  ~Node() noexcept;

  /**
   * Get id of the node.
   *
   * @return id of the node
   */
  NodeId get_id() const noexcept;

  /**
   * Initiate a chunk transmission.
   *   - This node should be the source of the chunk.
   *
   * @param chunk chunk to send
   */
  void send(std::unique_ptr<Chunk> chunk) noexcept;

  /**
   * Connect this node to another node.
   *
   * @param id id of the node to connect to
   * @param bandwidth bandwidth of the link
   * @param latency latency of the link
   */
  void connect(NodeId id, Bandwidth bandwidth, Latency latency) noexcept;

 private:
  /// node id
  NodeId id;

  /// links to other nodes
  /// map[dest node id] -> link
  std::map<NodeId, std::shared_ptr<Link>> links;

  /**
   * Check if this node is connected to another node.
   *
   * @param dest id of the node to check
   * @return true if connected, false otherwise
   */
  bool connected(NodeId dest) const noexcept;
};

} // namespace Congestion
