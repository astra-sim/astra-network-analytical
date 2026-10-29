// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2026 Advanced Micro Devices, Inc.

#pragma once

#include <map>
// #include <utility>
#include <cassert>
#include <astra-network-analytical/common/Type.h>
#include <astra-network-analytical/congestion_aware/Topology.h>

using namespace NetworkAnalytical;

using CommPair = std::pair<DeviceId, DeviceId>;
using RoutesContainer =
    std::map<DeviceId, std::map<DeviceId, std::vector<DeviceId>>>;

namespace NetworkAnalyticalCongestionAware {

class CustomTopology final : public Topology {
  public:
    /**
     * Constructor.
     *
     * @param npus_count number of npus connected to the CustomTopology
     * @param devices_count number of devices in the topology
     * @param bandwidth bandwidth of link
     * @param latency latency of link
     */
    CustomTopology(int npus_count,
                   int devices_count,
                   std::map<DeviceId, std::map<DeviceId, Bandwidth>> bandwidths,
                   std::map<DeviceId, std::map<DeviceId, Latency>> latencies,
                   RoutesContainer _routes = RoutesContainer()) noexcept;

    /**
     * Implementation of route function in Topology.
     */
    [[nodiscard]] Route route(DeviceId src,
                              DeviceId dest) const noexcept override;

  private:
    std::map<DeviceId, std::map<DeviceId, Route>> default_routes;
};

}  // namespace NetworkAnalyticalCongestionAware
