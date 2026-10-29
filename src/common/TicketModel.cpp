// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2026 Advanced Micro Devices, Inc.

#include <cassert>
#include <astra-network-analytical/common/TicketModel.h>
// for printing ticket status
#include <cstdio>

using namespace NetworkAnalytical;

TicketModel::TicketModel(std::vector<int> arg_level_widths,
                         std::vector<int> arg_tickets_per_level) noexcept {
    assert(!arg_level_widths.empty());
    assert(!arg_tickets_per_level.empty());
    assert(arg_level_widths.size() == arg_tickets_per_level.size());

    // member vars
    n_levels = arg_level_widths.size();
    level_widths.assign(arg_level_widths.begin(), arg_level_widths.end());
    tickets_per_level.assign(arg_tickets_per_level.begin(),
                             arg_tickets_per_level.end());
    n_resources =
        std::accumulate(arg_level_widths.begin(), arg_level_widths.end(),
                        0);  // sum ints w/ initial val 0

    int n_assigned = 0;
    for (int i = 0; i < n_levels; i++) {
        int l_width = level_widths[i];
        for (int j = 0; j < l_width; j++) {
            tickets.push_back(tickets_per_level[i]);
            n_assigned++;
        }
    }
}

int TicketModel::calc_resource_level(
    const DeviceId resource_id) const noexcept {
    assert(is_valid_resource_id(resource_id));

    int level = -1;
    int runsum = 0;
    for (int i = 0; i < n_levels; i++) {
        runsum += level_widths[i];
        if (resource_id < runsum) {
            level = i;
            break;
        }
    }

    return level;
}

int TicketModel::get_number_of_resources() const noexcept {
    return n_resources;
}

int TicketModel::get_width_of_level(const int level) const noexcept {
    assert(level < n_levels);
    return level_widths[level];
}

int TicketModel::get_width_highest_level() const noexcept {
    return get_width_of_level(0);
}

int TicketModel::get_number_of_levels() const noexcept {
    return n_levels;
}

bool TicketModel::is_valid_resource_id(
    const DeviceId resource_id) const noexcept {
    // function will be used for other asserts. so return bool

    // zero indexed. resource_id \in [0,n_resources)
    if (resource_id >= 0 && resource_id < n_resources) {
        return true;
    }
    return false;
}

bool TicketModel::has_tickets(const DeviceId resource_id) const noexcept {
    assert(is_valid_resource_id(resource_id));

    if (tickets[resource_id] > 0) {
        return true;
    }
    return false;
}

void TicketModel::decrement_ticket(const DeviceId resource_id) noexcept {
    assert(is_valid_resource_id(resource_id));
    assert(tickets[resource_id] > 0);

    tickets[resource_id]--;
}

void TicketModel::increment_ticket(const DeviceId resource_id) noexcept {
    assert(is_valid_resource_id(resource_id));
    // assert adding one won't surpass given limits
    assert(tickets[resource_id] + 1 <=
           tickets_per_level[calc_resource_level(resource_id)]);

    tickets[resource_id]++;
}

void TicketModel::print_ticket_status() const noexcept {
    assert(tickets.size() == n_resources);

    printf("Ticket status: \n");
    for (int i = 0; i < n_resources; i++) {
        printf("\trid %d : %d\n", i, tickets[i]);
    }
}
