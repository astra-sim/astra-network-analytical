// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2026 Advanced Micro Devices, Inc.

#pragma once

#include <algorithm>
#include <astra-network-analytical/common/BaseTopology.h>
#include <astra-network-analytical/common/EventQueue.h>
#include <astra-network-analytical/congestion_aware/Chunk.h>
#include <astra-network-analytical/congestion_aware/Device.h>
#include <iostream>
#include <memory>
#include <vector>

using namespace NetworkAnalytical;

namespace NetworkAnalyticalCongestionAware {

/**
 * Topology abstracts a network topology.
 */
class Topology : public BaseTopology {
  public:
    /**
     * Set the event queue to be used by the topology.
     *
     * @param event_queue pointer to the event queue
     */
    static void set_event_queue(
        std::shared_ptr<EventQueue> event_queue) noexcept;

    /**
     * Constructor.
     */
    Topology() noexcept;

    // stats
    virtual std::vector<int> calc_router_occupancy(int sys_id);

    /**
     * Construct the route from src to dest.
     * Route is a list of devices (pointers) that the chunk should traverse,
     * including the src and dest devices themselves.
     *
     * e.g., route(0, 3) = [0, 5, 7, 2, 3]
     *
     * @param src src NPU id
     * @param dest dest NPU id
     *
     * @return route from src NPU to dest NPU
     */
    [[nodiscard]] virtual Route route(DeviceId src,
                                      DeviceId dest) const noexcept = 0;

    [[nodiscard]] virtual Route route(DeviceId src,
                                      DeviceId dest,
                                      uint64_t chunk_address) const noexcept;

    /**
     * Initiate a transmission of a chunk.
     *
     * @param chunk chunk to be transmitted
     */
    void send(std::unique_ptr<Chunk> chunk) noexcept;

    /**
     * Check credits and return if sending allowed
     *
     * @param route ordered vector of Devices that request will travel
     *
     * @return bool if sending allowed for this request
     */
    virtual bool is_send_allowed_on_route(Route route) const noexcept;

    /**
     * Decrement resources along route (Devices)
     *
     * @param route ordered vector of Devices that request will travel
     *
     * @return list of resources that were decremented
     */
    virtual std::vector<DeviceId> claim_tickets_on_route(Route route) noexcept;

    /**
     * Association of resource to system maintained by topology. Get associated
     * resources along route
     *
     * @param route ordered vector of Devices that request will travel
     *
     * @return list of resource ids associated with this route
     */
    virtual std::vector<DeviceId> get_associated_resources_on_route(
        Route route) const noexcept;

    /**
     * For later callbacks, figure out most critical resource along route.
     * Policy may differ between implementations
     *
     * @param route ordered vector of Devices that request will travel
     *
     * @return resource ID considered most "constraining"
     */
    virtual DeviceId query_constraining_resource_on_route(
        Route route) const noexcept;

  protected:
    /// holds the entire device instances in the topology
    std::vector<std::shared_ptr<Device>> devices;

    /**
     * Instantiate Device objects in the topology.
     */
    void instantiate_devices() noexcept;

    /**
     * Connect src -> dest with the given bandwidth and latency.
     * (i.e., a `Link` gets constructed between the two npus)
     *
     * if bidirectional=true, dest -> src connection is also established.
     *
     * @param src src device id
     * @param dest dest device id
     * @param bandwidth bandwidth of link
     * @param latency latency of link
     * @param bidirectional true if connection is bidirectional, false otherwise
     */
    void connect(DeviceId src,
                 DeviceId dest,
                 Bandwidth bandwidth,
                 Latency latency,
                 bool bidirectional = true) noexcept;
};

}  // namespace NetworkAnalyticalCongestionAware
