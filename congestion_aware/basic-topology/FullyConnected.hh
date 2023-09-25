/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#pragma once

#include "congestion_aware/basic-topology/BasicTopology.hh"

namespace NetworkAnalyticalCongestionAware {

/**
 * A FullyConnected basic-topology.
 */
class FullyConnected final : public BasicTopology {
 public:
  /**
   * Constructor.
   *
   * @param npus_count number of npus in the basic-topology
   * @param bandwidth bandwidth of link
   * @param latency latency of link
   */
  FullyConnected(int npus_count, Bandwidth bandwidth, Latency latency) noexcept;

  /**
   * Implementation of route function.
   */
  [[nodiscard]] Route route(DeviceId src, DeviceId dest) const noexcept override;
};

} // namespace NetworkAnalyticalCongestionAware
