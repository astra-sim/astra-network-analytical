// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2026 Advanced Micro Devices, Inc.

#pragma once

#include <astra-network-analytical/common/Type.h>
#include <astra-network-analytical/congestion_aware/Topology.h>

using namespace NetworkAnalytical;

namespace NetworkAnalyticalCongestionAware {

/**
 * BasicTopology defines 1D topology
 * such as Ring, FullyConnected, and Switch topology,
 * which can be used to construct multi-dimensional topology.
 */
class BasicTopology : public Topology {
  public:
    /**
     * Constructor.
     *
     * @param npus_count number of NPUs in the topology
     * @param num_devices number of devices in the topology
     * @param bandwidth bandwidth of each link
     * @param latency latency of each link
     */
    BasicTopology(int npus_count,
                  int num_devices,
                  Bandwidth bandwidth,
                  Latency latency) noexcept;

    /**
     * Destructor.
     */
    virtual ~BasicTopology() noexcept;

    /**
     * Return the type of the basic topology
     * as a TopologyBuildingBlock enum class element.
     *
     * @return type of the basic topology
     */
    [[nodiscard]] TopologyBuildingBlock get_basic_topology_type()
        const noexcept;

  protected:
    /// bandwidth of each link
    Bandwidth bandwidth;

    /// latency of each link
    Latency latency;

    /// basic topology type
    TopologyBuildingBlock basic_topology_type;
};

}  // namespace NetworkAnalyticalCongestionAware
