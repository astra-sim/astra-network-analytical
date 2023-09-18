/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#pragma once

#include "topology/Topology.hh"

namespace Congestion {

/**
 * A FullyConnected topology.
 */
class FullyConnected final : public Topology {
 public:
  /**
   * Constructor.
   *
   * @param npus_count number of npus in the topology
   * @param bandwidth bandwidth of link
   * @param latency latency of link
   */
  FullyConnected(int npus_count, Bandwidth bandwidth, Latency latency) noexcept;

  /**
   * Destructor.
   */
  ~FullyConnected() noexcept;

  /**
   * Implementation of route function.
   */
  Route route(NodeId src, NodeId dest) const noexcept override;
};

} // namespace Congestion
