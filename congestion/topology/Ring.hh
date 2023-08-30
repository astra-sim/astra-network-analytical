/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#pragma once

#include "topology/Topology.hh"

namespace Congestion {

/**
 * A ring topology.
 */
class Ring final : public Topology {
 public:
  /**
   * Constructor.
   *
   * @param npus_count number of npus in a ring
   * @param bandwidth bandwidth of link
   * @param latency latency of link
   * @param bidirectional true if ring is bidirectional, false otherwise
   */
  Ring(
      int npus_count,
      Bandwidth bandwidth,
      Latency latency,
      bool bidirectional = false) noexcept;

  /**
   * Destructor.
   */
  ~Ring() noexcept;

  /**
   * Implementation of route function.
   */
  Route route(NodeId src, NodeId dest) const noexcept override;

 private:
  /// mark whether the ring is bidirectional.
  bool bidirectional;
};

} // namespace Congestion
