// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2026 Advanced Micro Devices, Inc.

#pragma once

#include <astra-network-analytical/common/Type.h>

#include <cstdint>
#include <list>
#include <numeric>
#include <vector>

namespace NetworkAnalytical {

class TicketModel {
    // allow parameterization of credit levels
    // topology (backend) will define this for Astra/Frontend APIs
    // for example

    //  modeling at GPU level
    //  --------------------   --------------------
    // |                    | |                    |
    // | GPU0: 128 credits  | | GPU1: 128 credits  |
    // |                    | |                    |
    //  --------------------   --------------------
    //            ^                      ^
    //            |                      |
    //             ----------------------
    //
    //  => credits_per_level = { 128 }
    //  => level_widths = { 2 }

    //  modeling at CU level. Inrementing (closer to network / farther from Sys)
    //  levels have tigher constraints
    //  -------------------------------   -------------------------------
    // |CU0:16 |CU1:16 |CU2:16 |CU3:16 | |CU0:16 |CU1:16 |CU2:16 |CU3:16 |
    // |   XCD0:16     |   XCD1:16     | |   XCD0:16     |   XCD1:16     |
    // |        GPU0: 32 credits       | |        GPU0: 32 credits       |
    // |                               | |                               |
    //  -------------------------------   -------------------------------
    //                 ^                                 ^
    //                 |                                 |
    //                 -----------------------------------
    //
    //  => credits_per_level = { 16, 16, 32 }
    //  => level_widths = { 8, 4, 2 }

  public:
    /**
     * Constructor.
     *
     * @param level_widths list of number of devices per level. Level 0 assumed
     * to be closest to network and farthest from Sys/GPU
     * @param tickets_per_level list of number of tickets per level
     */
    TicketModel(std::vector<int> arg_level_widths,
                std::vector<int> arg_tickets_per_level) noexcept;

    TicketModel(std::vector<int> arg_tickets) noexcept;

    /**
     * Calculate the corresponding level (device type) for a given resource
     *
     * @param resource_id resource ID to check
     *
     * @return level of resource in ticketing hierarchy
     */
    int calc_resource_level(DeviceId resource_id) const noexcept;

    /**
     * Query the total number of ticketed resources. Equal to sum of all level
     * widths
     *
     * @return total number of resources
     */
    int get_number_of_resources() const noexcept;

    /**
     * Query the number of devices at the given level
     *
     * @param level to check width
     *
     * @return level width (number of devices)
     */
    int get_width_of_level(int level) const noexcept;

    /**
     * Query the number of devices at highest level (closest to Sys) which
     * corresponds to WF/CU ports
     *
     * @return level width (number of devices) at n_levels - 1
     */
    int get_width_highest_level() const noexcept;

    /**
     * Get number of levels of ticketing model
     *
     * @return number of levels corresponding to device types
     */
    int get_number_of_levels() const noexcept;

    /**
     * Check if resource is valid number/int for given topology
     *
     * @param resource_id resource ID to check
     *
     * @return bool true (valid) if within [0,n_resources)
     */
    bool is_valid_resource_id(DeviceId resource_id) const noexcept;

    /**
     * Check tickets available for the resource
     *
     * @param resource_id resource ID to read
     *
     * @return bool if tickets at index resource ID strictly greater than 0
     */
    bool has_tickets(DeviceId resource_id) const noexcept;

    /**
     * Directly decrement tickets vector using resource ID as index
     *
     * @param resource_id resource ID to be incremented
     */
    void decrement_ticket(DeviceId resource_id) noexcept;

    /**
     * Directly increment tickets vector using resource ID as index
     *
     * @param resource_id resource ID to be incremented
     */
    void increment_ticket(DeviceId resource_id) noexcept;

    /**
     * Print tickets vector values
     *
     */
    void print_ticket_status() const noexcept;

  private:
    // user param. const after init
    int n_levels;

    // user param. const after init
    int n_resources;

    // user param. const after init
    std::vector<int> level_widths;

    // user param. const after init
    std::vector<int> tickets_per_level;

    // values change through simulation
    std::vector<int> tickets;
};

}  // namespace NetworkAnalytical
