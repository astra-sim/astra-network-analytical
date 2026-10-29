// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2026 Advanced Micro Devices, Inc.

#pragma once

#include <astra-network-analytical/common/NetworkParser.h>
#include <astra-network-analytical/congestion_aware/CustomTopology.h>
#include <astra-network-analytical/congestion_aware/FullyConnected.h>
#include <astra-network-analytical/congestion_aware/Ring.h>
#include <astra-network-analytical/congestion_aware/Switch.h>
#include <astra-network-analytical/congestion_aware/Topology.h>
#include <memory>

using namespace NetworkAnalytical;

namespace NetworkAnalyticalCongestionAware {

/**
 * Construct a topology from a NetworkParser.
 *
 * @param network_parser NetworkParser to parse the network input file
 * @return pointer to the constructed topology
 */
[[nodiscard]] std::shared_ptr<Topology> construct_topology(
    const NetworkParser& network_parser) noexcept;

}  // namespace NetworkAnalyticalCongestionAware
