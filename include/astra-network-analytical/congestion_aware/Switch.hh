/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#pragma once

#include <cassert>
#include "common/Type.hh"
#include "congestion_aware/BasicTopology.hh"

using namespace NetworkAnalytical;

namespace NetworkAnalyticalCongestionAware {

/**
 * A switch basic-topology.
 */
class Switch final : public BasicTopology {
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
   * Implementation of route function.
   */
  [[nodiscard]] Route route(DeviceId src, DeviceId dest)
      const noexcept override;

 private:
  /// node_id of the switch node
  DeviceId switch_id;
};

} // namespace NetworkAnalyticalCongestionAware
