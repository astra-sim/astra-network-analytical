// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2026 Advanced Micro Devices, Inc.

#pragma once

#include <astra-network-analytical/common/Type.h>
#include <astra-network-analytical/congestion_aware/BasicTopology.h>

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
     * Constructor.
     *
     * @param npus_count number of npus in the FullyConnected topology
     * @param devices_count number of devices in the topology
     * @param bandwidth bandwidth of each link
     * @param latency latency of each link
     */
    FullyConnected(int npus_count,
                   int devices_count,
                   Bandwidth bandwidth,
                   Latency latency) noexcept;

    /**
     * Implementation of route function in Topology.
     */
    [[nodiscard]] Route route(DeviceId src,
                              DeviceId dest) const noexcept override;
};

}  // namespace NetworkAnalyticalCongestionAware
