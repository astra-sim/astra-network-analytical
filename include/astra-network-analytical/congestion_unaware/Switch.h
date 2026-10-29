// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2026 Advanced Micro Devices, Inc.

#pragma once

#include <astra-network-analytical/common/Type.h>
#include <astra-network-analytical/congestion_unaware/BasicTopology.h>

using namespace NetworkAnalytical;

namespace NetworkAnalyticalCongestionUnaware {

/**
 * Implements a switch topology.
 *
 * Switch(4) example:
 * <-switch->
 * |  |  |  |
 * 0  1  2  3
 *
 * For example, send(0 -> 2) flows through:
 * 0 -> switch -> 2
 * so takes 2 hops.
 */
class Switch final : public BasicTopology {
  public:
    /**
     * Constructor.
     *
     * @param npus_count number of NPUs in the Switch
     * @param num_devices number of network nodes in topology
     * @param bandwidth bandwidth of each link
     * @param latency latency of each link
     */
    Switch(int npus_count,
           int num_devices,
           Bandwidth bandwidth,
           Latency latency) noexcept;

  private:
    /**
     * Implements the compute_hops_count method of BasicTopology.
     */
    [[nodiscard]] int compute_hops_count(DeviceId src,
                                         DeviceId dest) const noexcept override;
};

}  // namespace NetworkAnalyticalCongestionUnaware
