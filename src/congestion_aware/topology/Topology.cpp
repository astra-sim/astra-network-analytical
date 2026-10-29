// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2026 Advanced Micro Devices, Inc.

#include <cassert>
#include <astra-network-analytical/congestion_aware/Link.h>
#include <astra-network-analytical/congestion_aware/Topology.h>

using namespace NetworkAnalyticalCongestionAware;

Topology::Topology() noexcept : BaseTopology() {
    npus_count_per_dim = {};
}

Route Topology::route(DeviceId src,
                      DeviceId dest,
                      uint64_t chunk_address) const noexcept {
// default to without address
#ifdef VERBOSE_BACKEND
    std::cout << "Defaulting routing to no address version" << std::endl;
#endif
    return route(src, dest);
}

void Topology::set_event_queue(
    std::shared_ptr<EventQueue> event_queue) noexcept {
    assert(event_queue != nullptr);

    // pass the given event_queue to Link
    Link::set_event_queue(std::move(event_queue));
}

bool Topology::is_send_allowed_on_route(Route route) const noexcept {
    assert(_is_ticketed_topology);

    std::vector<DeviceId> associated_resources =
        get_associated_resources_on_route(route);

    bool has_tickets = true;
    for (const auto rid : associated_resources) {
        if (!ticket_booth->has_tickets(rid)) {
            has_tickets = false;
            // exit early
            break;
        }
    }

    return has_tickets;
}

std::vector<DeviceId> Topology::claim_tickets_on_route(Route route) noexcept {
    assert(_is_ticketed_topology);

    // assume this list is exact
    // not "possibilities"
    // ie multiple choices per hop are not allowed
    // allows assertion in loop
    std::vector<DeviceId> associated_resources =
        get_associated_resources_on_route(route);

    for (const auto rid : associated_resources) {
        assert(ticket_booth->has_tickets(rid));
        ticket_booth->decrement_ticket(rid);
    }

    return associated_resources;
}

std::vector<DeviceId> Topology::get_associated_resources_on_route(
    Route route) const noexcept {
    assert(_is_ticketed_topology);

    // assume all resource IDs directly match device IDs

    // IMPORTANT: only count/view resources decrementing levels
    //    e.g. for route on nodes 0, 8, 9, 10, 11, 7 and
    //        levels 0, 1, 1, 1, 1, 0 only count tickets
    //        for nodes 0, 8

    std::vector<DeviceId> associated_resources = {};

    auto first_dev_id = route.front()->get_id();
    auto last_dev_id = route.back()->get_id();
    int first_level = ticket_booth->calc_resource_level(first_dev_id);
    int last_level = ticket_booth->calc_resource_level(last_dev_id);

    // if a response
    if (last_level < first_level) {
        return associated_resources;
    }

    // strictly greater than all possible. ie last = n_levels > [0,n_levels-1]
    int previous_level = -1;  // ticket_booth->get_number_of_levels();
    for (auto device : route) {
        DeviceId dev_id = device->get_id();
        int dev_level = ticket_booth->calc_resource_level(dev_id);
        if (dev_level > previous_level) {
            associated_resources.push_back(dev_id);
        }
        previous_level = dev_level;
    }

    return associated_resources;
}

DeviceId Topology::query_constraining_resource_on_route(
    Route route) const noexcept {
    assert(_is_ticketed_topology);

    // assume most constraining is the highest (closer to sys) level
    //    ie first found if searching via decrementing/route
    //    get_associated_resources_on_route returns ordered list

    std::vector<DeviceId> associated_resources =
        get_associated_resources_on_route(route);

    DeviceId constraining_resource = -1;
    for (const auto rid : associated_resources) {
        if (!ticket_booth->has_tickets(rid)) {
            constraining_resource = rid;
            break;
        }
    }

    return constraining_resource;
}

void Topology::send(std::unique_ptr<Chunk> chunk) noexcept {
    assert(chunk != nullptr);

    // get src npu node_id
    const auto src = chunk->current_device()->get_id();

    // assert src is valid
    assert(0 <= src && src < num_devices);

    // initiate transmission from src
    devices[src]->send(std::move(chunk));
}

void Topology::connect(const DeviceId src,
                       const DeviceId dest,
                       const Bandwidth bandwidth,
                       const Latency latency,
                       const bool bidirectional) noexcept {
    // assert the src and dest are valid
    assert(0 <= src && src < num_devices);
    assert(0 <= dest && dest < num_devices);

    // assert bandwidth and latency are valid
    assert(bandwidth > 0);
    assert(latency >= 0);

    // connect src -> dest
    devices[src]->connect(dest, bandwidth, latency);

    // if bidirectional, connect dest -> src
    if (bidirectional) {
        devices[dest]->connect(src, bandwidth, latency);
    }
}

void Topology::instantiate_devices() noexcept {
    // instantiate all devices
    for (auto i = 0; i < num_devices; i++) {
        devices.push_back(std::make_shared<Device>(i));
    }
}

// stats
std::vector<int> Topology::calc_router_occupancy(int sys_id) {
    // dummy to make Topology not pure virtual
    std::vector<int> occupancy;
    return occupancy;
}
