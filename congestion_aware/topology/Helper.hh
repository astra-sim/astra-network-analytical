/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#pragma once

#include "common/Common.hh"
#include "common/Type.hh"
#include "common/network-parser/NetworkParser.hh"
#include "congestion_aware/topology/Topology.hh"

using namespace NetworkAnalytical;

namespace NetworkAnalyticalCongestionAware {

[[nodiscard]] std::shared_ptr<Topology> construct_topology(
    const NetworkParser& network_parser) noexcept;

} // namespace NetworkAnalyticalCongestionAware
