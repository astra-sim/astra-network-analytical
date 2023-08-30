/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#pragma once

#include "topology/Topology.hh"

namespace Congestion {

/**
 * A switch topology.
 */
class Switch final : public Topology {
 public:
  /**
   * Constructor.
   *
   * @param npus_count number of npus in a switch
   * @param bandwidth bandwidth of link
   * @param latency latency of link
   */
  Switch(int npus_count, Bandwidth bandwidth, Latency latency) noexcept;

  /**
   * Destructor.
   */
  ~Switch() noexcept;

  /**
   * Implementation of route function.
   */
  Route route(NodeId src, NodeId dest) const noexcept override;

 private:
  /// id of the switch node
  NodeId switch_id;
};

} // namespace Congestion
