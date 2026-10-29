// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2026 Advanced Micro Devices, Inc.

#pragma once
#include <astra-network-analytical/common/Type.h>
#include <astra-network-analytical/congestion_unaware/Topology.h>
#include <iostream>
#include <map>

using namespace NetworkAnalytical;

namespace NetworkAnalyticalCongestionUnaware {

/**
 * CustomTopology defines 1D FullyConnected topology with per-link bandwidth and
 * latency configurations.
 */
class CustomTopology : public Topology {
  public:
    /**
     * Constructor.
     *
     * @param npus_count number of NPUs in the topology
     * @param bandwidth bandwidth of each link in the topology
     * @param latency latency of each link in the topology
     */
    CustomTopology(const int npus_count,
                   const int num_devices,
                   std::map<int, std::map<int, Bandwidth>> bandwidths,
                   std::map<int, std::map<int, Latency>> latencies) noexcept;

    /**
     * Destructor.
     */
    virtual ~CustomTopology() noexcept;

    /**
     * Implement the send method of Topology.
     */
    [[nodiscard]] EventTime send(DeviceId src,
                                 DeviceId dest,
                                 ChunkSize chunk_size) const noexcept override;

    [[nodiscard]] EventTime receive(
        DeviceId src,
        DeviceId dest,
        ChunkSize chunk_size) const noexcept override;

    /**
     * Return the type of the basic topology
     * as a TopologyBuildingBlock enum class element.
     *
     * @return type of the basic topology
     */
    [[nodiscard]] TopologyBuildingBlock get_basic_topology_type()
        const noexcept;

  protected:
    /// type of the basic topology
    TopologyBuildingBlock basic_topology_type;

  private:
    /**
     * Analytically compute the communication delay.
     *
     * @param hops_count number of hops between src and dest
     * @param chunk_size size of the chunk
     * @return communication delay to send a chunk between src and dest
     */
    EventTime compute_communication_delay(const DeviceId src,
                                          const DeviceId dest,
                                          int hops,
                                          ChunkSize chunk_size) const noexcept;

    std::vector<std::vector<Bandwidth>> bandwidth_per_link;

    /// latency of each link in ns
    std::vector<std::vector<Latency>> latency_per_link;
};

}  // namespace NetworkAnalyticalCongestionUnaware
