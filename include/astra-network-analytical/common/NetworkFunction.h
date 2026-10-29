// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2026 Advanced Micro Devices, Inc.

#pragma once

#include <astra-network-analytical/common/Type.h>
#include <cstdint>

namespace NetworkAnalytical {

/**
 * Convert bandwidth from GiB/s to B/ps.
 * Here, 1 GB = 2^30 B
 * 1 s = 10^12 ps
 *
 * @param bw_GBps bandwidth in GiB/s
 * @return translated bandwidth in B/ps
 */
[[nodiscard]] Bandwidth bw_GiBps_to_Bpps(Bandwidth bw_GBps) noexcept;

/**
 * @brief Convert latency from ns to ps
 * 1 ns = 1,000 ps.
 *
 * @param latency latency in ns
 * @return int64_t translated latency in ps
 */
[[nodiscard]] int64_t latency_ns_to_ps(Latency latency) noexcept;

}  // namespace NetworkAnalytical
