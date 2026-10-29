// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2026 Advanced Micro Devices, Inc.

#pragma once

#include <astra-network-analytical/common/Type.h>
#include <astra-network-analytical/congestion_unaware/BasicTopology.h>

using namespace NetworkAnalytical;

namespace NetworkAnalyticalCongestionUnaware {

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
 * Therefore, arbitrary send between two pair of NPUs will take 1 hop.
 */
class FullyConnected final : public BasicTopology {
  public:
    /**
     * Constructor.
     *
     * @param npus_count number of NPUs in the FullyConnected topology
     * @param num_devices number of network nodes in topology
     * @param bandwidth bandwidth of each link
     * @param latency latency of each link
     */
    FullyConnected(int npus_count,
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
