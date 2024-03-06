/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#pragma once

#include <cassert>
#include <memory>
#include "common/Type.hh"
#include "congestion_aware/BasicTopology.hh"

using namespace NetworkAnalytical;

namespace NetworkAnalyticalCongestionAware {

/**
 * Implements a switch topology.
 *
 * Switch(4) example:
 * <-switch->
 * |  |  |  |
 * 0  1  2  3
 *
 * Therefore, the number of NPUs is 4 (excluding the switch),
 * and the number of devices is 5 (including the switch).
 *
 * For example, send(0 -> 2) flows through:
 * 0 -> switch -> 2
 * so takes 2 hops.
 */
class Switch final : public BasicTopology {
 public:
  /**
   * Constructor, when creating new NPUs.
   *
   * @param npus_count number of npus connected to the switch
   * @param bandwidth bandwidth of link
   * @param latency latency of link
   */
  Switch(int npus_count, Bandwidth bandwidth, Latency latency) noexcept;

  Switch(
      const Devices& npus,
      std::shared_ptr<Device> switch_device,
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

 private:
  /// node_id of the switch node
  std::shared_ptr<Device> switch_device;
};

} // namespace NetworkAnalyticalCongestionAware
