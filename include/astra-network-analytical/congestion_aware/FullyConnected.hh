/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#pragma once

#include "common/Type.hh"
#include "congestion_aware/BasicTopology.hh"

namespace NetworkAnalyticalCongestionAware {

/**
 * Implements a FullyConnected topology.
 *
 * FullyConnected(4) example:
 *    0
 *  / | \
 * 3 -|- 1
 *  \ | /
 *   2
 *
 * Therefore, the number of NPUs and devices are both 4.
 *
 * Arbitrary send between two pair of NPUs will take 1 hop.
 */
class FullyConnected final : public BasicTopology {
 public:
  /**
   * Constructor, when creating new NPUs.
   *
   * @param npus_count number of npus in the FullyConnected topology
   * @param bandwidth bandwidth of each link
   * @param latency latency of each link
   */
  FullyConnected(int npus_count, Bandwidth bandwidth, Latency latency) noexcept;

  /**
   * Constructing, when using existing NPUs.
   * @param npus vector to NPU pointers involved in this topology
   * @param bandwidth bandwidth of each link
   * @param latency latency of each link
   */
  FullyConnected(
      const Devices& npus,
      Bandwidth bandwidth,
      Latency latency) noexcept;

  /**
   * Implementation of route function in Topology.
   */
  [[nodiscard]] Route route(DeviceId src, DeviceId dest)
      const noexcept override;

 protected:
  /**
   * Implementation of construct_connections in BasicTopology.
   */
  void construct_connections() noexcept override;
};

} // namespace NetworkAnalyticalCongestionAware
