// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2026 Advanced Micro Devices, Inc.

#pragma once

#include <astra-network-analytical/common/Type.h>
#include <astra-network-analytical/congestion_unaware/BasicTopology.h>

using namespace NetworkAnalytical;

namespace NetworkAnalyticalCongestionUnaware {

/**
 * Implements a ring topology.
 *
 * Ring(8) example:
 * 0 - 1 - 2 - 3
 * |           |
 * 7 - 6 - 5 - 4
 *
 * If ring is uni-directional, then each chunk can flow through:
 * 0 -> 1 -> 2 -> 3 -> 4 -> 5 -> 6 -> 7 -> 0
 *
 * If the ring is bi-directional, then each chunk can flow through:
 * 0 -> 1 -> 2 -> 3 -> 4 -> 5 -> 6 -> 7 -> 0
 * 0 <- 1 <- 2 <- 3 <- 4 <- 5 <- 6 <- 7 <- 0
 */
class Ring final : public BasicTopology {
  public:
    /**
     * Constructor
     *
     * @param npus_count number of NPUs in the Ring
     * @param num_devices number of network nodes in topology
     * @param bandwidth bandwidth of each link
     * @param latency latency of each link
     * @param bidirectional whether the ring is bidirectional, defaults to true
     */
    Ring(int npus_count,
         int num_devices,
         Bandwidth bandwidth,
         Latency latency,
         bool bidirectional = true) noexcept;

  private:
    /**
     * Implements the compute_hops_count method of BasicTopology.
     */
    [[nodiscard]] int compute_hops_count(DeviceId src,
                                         DeviceId dest) const noexcept override;

    /// true if the ring is bidirectional, false otherwise
    bool bidirectional;
};

}  // namespace NetworkAnalyticalCongestionUnaware
