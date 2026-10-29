// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2026 Advanced Micro Devices, Inc.

#pragma once

#include <astra-network-analytical/common/TicketModel.h>
#include <astra-network-analytical/common/Type.h>
#include <astra-network-analytical/congestion_aware/Type.h>

#include <algorithm>
#include <list>
#include <map>
#include <memory>
#include <unordered_map>
#include <vector>

using namespace NetworkAnalytical;
using namespace NetworkAnalyticalCongestionAware;

namespace NetworkAnalytical {

/**
 * Abstracts a network BaseTopology.
 */
class BaseTopology {
  public:
    /**
     * Constructor
     */
    BaseTopology() noexcept;
    ~BaseTopology() = default;

    /**
     * Get the number of NPUs in the topology.
     *
     * @return number of NPUs
     */
    [[nodiscard]] int get_npus_count() const noexcept;

    /**
     * Get the number of NPUs in the topology.
     *
     * @return number of NPUs
     */
    int get_devices_count() const noexcept;

    /**
     * Get the number of network dimensions.
     *
     * @return number of network dimensions
     */
    [[nodiscard]] int get_dims_count() const noexcept;

    /**
     * Get the number of NPUs per dimension.
     *
     * @return number of NPUs per dimension
     */
    [[nodiscard]] std::vector<int> get_npus_count_per_dim() const noexcept;

    /**
     * Get the number of devices per dimension.
     *
     * @return number of devicess per dimension
     */
    [[nodiscard]] std::vector<int> get_devs_count_per_dim() const noexcept;

    /**
     * Get the bandwidth (GiB/s) per dimension.
     *
     * @return bandwidth per dimension
     */
    [[nodiscard]] std::vector<Bandwidth> get_bandwidth_per_dim() const noexcept;

    int get_topology_dim(int src, int dst) {
        return 0;
    }

    /**
     * Set level widths. Applies to both ticketed and/or buffer conscious
     *
     * @param level_widths list of number of devices per level. Level 0 assumed
     * to be closest to network and farthest from Sys/GPU
     */
    virtual void set_level_widths(std::vector<int> level_widths) noexcept;

    /**
     * Construct ticket_booth as TicketModel
     *
     * @param tickets_per_level list of number of tickets per level
     */
    virtual void create_ticket_booth(
        std::vector<int> tickets_per_level) noexcept;

    /**
     * If network should consider ticketing
     *
     */
    [[nodiscard]] bool is_ticketed_topology() const noexcept;

    /**
     * If network should consider buffer locations
     *
     * @param buf_lev assumed level that buffers are cached/held
     * @param buf_gran interleaving granularity for memory striping
     */
    virtual void set_buffer_location_and_granularity(
        int buf_lev, Buf_Offset buf_gran) noexcept;

    /**
     * Get number of resources as calculated in ticket_booth constructor
     *
     * @return int number resources
     */
    [[nodiscard]] int get_number_of_resources() noexcept;

    /**
     * Check credits and return if sending allowed
     *
     * @param src given as network device ID (not GPU/MPI sys rank)
     * @param dst given as network device ID (not GPU/MPI sys rank)
     *
     * @return bool if sending allowed for this request
     */
    [[nodiscard]] bool is_send_allowed(DeviceId src, DeviceId dst) noexcept;

    /**
     * Check credits and return if sending allowed
     *
     * @param route ordered vector of Devices that request will travel
     *
     * @return bool if sending allowed for this request
     */
    [[nodiscard]] bool is_send_allowed_on_route(Route route) noexcept;

    /**
     * Check credits and decrement first match
     *
     * @param src given as network device ID (not GPU/MPI sys rank)
     * @param dst given as network device ID (not GPU/MPI sys rank)
     *
     * @return int for resource that was decremented
     */
    std::vector<DeviceId> claimTickets(DeviceId src, DeviceId dst);

    /**
     * Decrement resources along route (Devices)
     *
     * @param route ordered vector of Devices that request will travel
     *
     * @return list of resources that were decremented
     */
    [[nodiscard]] std::vector<DeviceId> claim_tickets_on_route(
        Route route) noexcept;

    /**
     * Increment ticks for given resource
     *
     * @param freed_resources list of resources for which to increment the
     * tickets
     */
    virtual void relinquish_resources(
        std::vector<DeviceId> freed_resources) noexcept;

    /**
     * For later callbacks, figure out most critical resource along route.
     * Policy may differ between implementations Applies to Unaware BE
     *
     * @param src given as network device ID (not GPU/MPI sys rank)
     * @param dst given as network device ID (not GPU/MPI sys rank)
     *
     * @return resource ID considered most "constraining"
     */
    [[nodiscard]] DeviceId query_constraining_resource(DeviceId src,
                                                       DeviceId dst) noexcept;

    /**
     * For later callbacks, figure out most critical resource along route.
     * Policy may differ between implementations Applies to Aware BE
     *
     * @param route ordered vector of Devices that request will travel
     *
     * @return resource ID considered most "constraining"
     */
    [[nodiscard]] DeviceId query_constraining_resource_on_route(
        Route route) noexcept;

    /**
     * Association of resource to system maintained by topology
     *
     * @param src given as network device ID (not GPU/MPI sys rank)
     * @param dst given as network device ID (not GPU/MPI sys rank)
     *
     * @return list of resource ids associated with this request
     */
    [[nodiscard]] std::vector<DeviceId> getAssociatedResources(
        DeviceId src, DeviceId dst) noexcept;

    /**
     * Association of resource to system maintained by topology. Get associated
     * resources along route
     *
     * @param route ordered vector of Devices that request will travel
     *
     * @return list of resource ids associated with this route
     */
    [[nodiscard]] std::vector<DeviceId> get_associated_resources_on_route(
        Route route) noexcept;

    /**
     * Print tickets vector from ticket_booth
     *
     */
    void print_ticket_status() noexcept;

    /**
     * Calculate the network device ID associated with the given GPU and CU
     *
     * @param sys_id GPU/MPI sys rank
     * @param cu_id CU ID relative to system (ie each system CU IDs start at 0)
     *
     * @return device ID
     */
    virtual DeviceId calc_cu_net_id(Sys_ID sys_id, CU_ID cu_id);

    /**
     * Calculate the network device ID associated with the buffer described by
     * the information
     *
     * @param sys_id GPU/MPI sys rank
     * @param buf_off buffer offset (Bytes) for the request
     * @param req_size buffer offset (Bytes) for the request
     *
     * @return device ID
     */
    virtual DeviceId calc_buffer_net_id(Sys_ID sys_id,
                                        Buf_Offset buf_off,
                                        Req_Size req_size);

    /**
     * Calculate the network ID from the system and relative buffer ID within
     * that system. It basically finds the global lower bound and modulos the
     * relative ID by the number of buffers per system. Separate function for
     * code reuse
     *
     * @param sys_id GPU/MPI sys rank
     * @param buf_off relative buffer ID for the request
     *
     * @return device ID
     */
    DeviceId calc_global_buffer_id_from_relative(Sys_ID sys_id,
                                                 DeviceId rel_buf_id);

  public:
    /// number of NPUs in the topology
    int npus_count;

    int num_devices;

    /// number of network dimensions of the topology
    int dims_count;

    /// number of NPUs per each network dimension
    std::vector<int> npus_count_per_dim;

    /// number of devices per each dimension
    std::vector<int> devs_count_per_dim;

    /// network bandwidth (GiB/s) per each network dimension
    std::vector<Bandwidth> bandwidth_per_dim;

    // whether NoC is modeled
    bool _is_buffer_conscious;

    // whether resources are limited in # in flight requests
    bool _is_ticketed_topology;

    // level that buffers are assumed to reside
    int buffer_location;

    // interleaving granularity (in Bytes) of buffer slices amongst devices
    Buf_Offset buffer_granularity;

    // don't expect to share. let stl handle destruction
    std::unique_ptr<TicketModel> ticket_booth;

    // for leveled topologies (ticketed &/ buffer_conscious)
    int n_levels;

    // for leveled topologies (ticketed &/ buffer_conscious)
    std::vector<int> level_widths;
};

}  // namespace NetworkAnalytical
