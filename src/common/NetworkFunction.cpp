// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2026 Advanced Micro Devices, Inc.

#include <cassert>
#include <astra-network-analytical/common/NetworkFunction.h>
#include <cstdint>

using namespace NetworkAnalytical;

Bandwidth NetworkAnalytical::bw_GiBps_to_Bpps(
    const Bandwidth bw_GBps) noexcept {
    assert(bw_GBps > 0);

    // 1 GiB is 2^30 B
    // 1 s is 10^12 ps
    return bw_GBps * (1 << 30) / 1e12;  // GiB/s to B/ps
}

int64_t NetworkAnalytical::latency_ns_to_ps(const Latency latency) noexcept {
    assert(latency > 0);

    // translate ns to ps
    const auto latency_ps = latency * 1000;
    return static_cast<int64_t>(latency_ps);
}
