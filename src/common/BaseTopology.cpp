// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2026 Advanced Micro Devices, Inc.

#include <cassert>
#include <astra-network-analytical/common/BaseTopology.h>
#include <iostream>

using namespace NetworkAnalytical;

BaseTopology::BaseTopology() noexcept
    : npus_count(-1),
      num_devices(-1),
      dims_count(-1),
      buffer_location(-1),
      buffer_granularity(256),
      _is_buffer_conscious(false),
      _is_ticketed_topology(false),
      ticket_booth(nullptr) {}

int BaseTopology::get_npus_count() const noexcept {
    assert(npus_count > 0);

    return npus_count;
}

int BaseTopology::get_devices_count() const noexcept {
    assert(num_devices > 0);
    assert(npus_count > 0);
    assert(num_devices >= npus_count);

    return num_devices;
}

int BaseTopology::get_dims_count() const noexcept {
    assert(dims_count > 0);

    return dims_count;
}

std::vector<int> BaseTopology::get_npus_count_per_dim() const noexcept {
    return npus_count_per_dim;
}

std::vector<int> BaseTopology::get_devs_count_per_dim() const noexcept {
    return devs_count_per_dim;
}

std::vector<Bandwidth> BaseTopology::get_bandwidth_per_dim() const noexcept {
    return bandwidth_per_dim;
}

void BaseTopology::set_level_widths(std::vector<int> level_widths) noexcept {
    assert(level_widths.size() > 0);
    this->level_widths = level_widths;
    n_levels = level_widths.size();
}

void BaseTopology::create_ticket_booth(
    std::vector<int> tickets_per_level) noexcept {
    assert(tickets_per_level.size() == level_widths.size());
    ticket_booth =
        std::make_unique<TicketModel>(level_widths, tickets_per_level);

    // only set if specifying required values
    _is_ticketed_topology = true;
}

bool BaseTopology::is_ticketed_topology() const noexcept {
    return _is_ticketed_topology;
}

void BaseTopology::set_buffer_location_and_granularity(
    int buf_lev, Buf_Offset buf_gran) noexcept {
    buffer_location = buf_lev;
    buffer_granularity = buf_gran;
    _is_buffer_conscious = true;
}

int BaseTopology::get_number_of_resources() noexcept {
    return ticket_booth->get_number_of_resources();
}

bool BaseTopology::is_send_allowed(DeviceId src, DeviceId dst) noexcept {
    assert(_is_ticketed_topology);
    std::vector<DeviceId> associated_resources =
        getAssociatedResources(src, dst);

    bool has_tickets = false;
    for (const auto rid : associated_resources) {
        if (ticket_booth->has_tickets(rid)) {
            has_tickets = true;
        }
    }

    return has_tickets;
}

std::vector<DeviceId> BaseTopology::claimTickets(DeviceId src, DeviceId dst) {
    assert(_is_ticketed_topology);

    std::vector<DeviceId> associated_resources =
        getAssociatedResources(src, dst);

    for (const auto rid : associated_resources) {
        assert(ticket_booth->has_tickets(rid));
        ticket_booth->decrement_ticket(rid);
    }

    return associated_resources;
}

void BaseTopology::relinquish_resources(
    std::vector<DeviceId> freed_resources) noexcept {
    assert(_is_ticketed_topology);

    for (const auto rid : freed_resources) {
        // resource id validation handled by ticket booth
        ticket_booth->increment_ticket(rid);
    }
}

DeviceId BaseTopology::query_constraining_resource(DeviceId src,
                                                   DeviceId dst) noexcept {
    assert(_is_ticketed_topology);

    // assume any of connected set is constraining resource

    std::vector<DeviceId> associated_resources =
        getAssociatedResources(src, dst);

    DeviceId assoc_resource = -1;
    for (const auto rid : associated_resources) {
        if (!ticket_booth->has_tickets(rid)) {
            assoc_resource = rid;
            break;
        }
    }

    return assoc_resource;
}

void BaseTopology::print_ticket_status() noexcept {
    assert(_is_ticketed_topology);
    ticket_booth->print_ticket_status();
}

// private
std::vector<DeviceId> BaseTopology::getAssociatedResources(
    DeviceId src, DeviceId dst) noexcept {
    assert(_is_ticketed_topology);

    // assume highest level (0) is connected to sys
    // the parameter "src" is a net device
    // single resource for now
    // assume highest level network devices are numbered starting at 0

    std::vector<DeviceId> associated_resources = {};
    associated_resources.push_back(src);

    return associated_resources;
}

DeviceId BaseTopology::calc_cu_net_id(Sys_ID sys_id, CU_ID cu_id) {
    /*
     * lots of comments in function as the mapping is hardcoded and not obvious
     * sys_id global
     * cu_id local to sys (ie each sys has CUs 0, 1, ...)
     * assume highest level blocked between systems
     * but interleaved within system
     *    eg top level net devices 0,1,2,3
     *       sys 0,1
     *       cus (per sys) 0,1,2,3
     *       => (sys=0,cu=[0,2]) -> net_intf=0
     *          (sys=0,cu=[1,3]) -> net_intf=1
     *          (sys=1,cu=[0,2]) -> net_intf=2
     *          (sys=1,cu=[1,3]) -> net_intf=3
     */

    DeviceId cu_net_id = -1;

    // n_cu_ports is number of network interfaces connected to Sys(s)
    int n_net_intfs;
    if (!is_ticketed_topology()) {
        // if not ticketed then assume all net devs at "highest level" aka
        // CU/Sys ports
        n_net_intfs = num_devices;
    } else {
        // if ticketed then ask for number at "highest level" aka CU/Sys ports
        n_net_intfs = ticket_booth->get_width_highest_level();
    }

    // 1) sys to net_intf mapping
    //    3 options
    // a) # npus < # net_intfs
    //    => multiple net_intfs per sys
    //       blocked mapping
    //    eg # net_intfs = 4
    //       # sys = 2
    //       => sys=0 -> net_intf=[0,1]
    //          sys=1 -> net_inft=[2,3]
    // b) # npus = # net_intfs
    //    => 1 net_intf per sys
    //       1:1 mapping (simple)
    // c) # npus > # net_intfs
    //    => 1 net_intf for multiple sys
    //       blocked mapping
    //    eg # net_intfs = 2
    //       # sys = 4
    //       => sys=[0,1] -> net_intf=0
    //          sys=[2,3] -> net_intf=1

    // long story short, calculate 2 values
    //    net_intf low bound
    //    # net_intf per sys ( >= 1)

    // 2) cu_id to net_intf mapping given sys to net_intf
    //    # cus per sys unknown to topology
    //    therefore just modulus
    //    1 option
    // a) # cus >= # sys
    //    => given sys_net_intfs list=[a,b,c,...]
    //       modulo map cu_id into that list
    //    eg sys=2 and net_intf=[9,10,11,12]
    //       cu_id = 14
    //       => cu_net_id = 11 = net_intf[(cu_id % 4)]

    // long story short, given 2 values
    //    net_intf low bound
    //    # net_intf per sys ( >= 1)
    // calculate cu_net_id = net_intf_lb + (cu_id % n_net_intfs_per_sys)

    // 1)
    DeviceId net_intf_lb;
    DeviceId n_net_intfs_per_sys;
    if (n_net_intfs >= npus_count) {
        // a), b)
        n_net_intfs_per_sys = n_net_intfs / npus_count;
        net_intf_lb = sys_id * n_net_intfs_per_sys;
    } else {
        // c)
        n_net_intfs_per_sys = 1;
        DeviceId n_sys_per_net_intf = npus_count / n_net_intfs;
        net_intf_lb = sys_id / n_sys_per_net_intf;
    }

    // 2)
    DeviceId cu_id_dev_id = (DeviceId)cu_id;
    cu_net_id = net_intf_lb + (cu_id_dev_id % n_net_intfs_per_sys);

    // clang-format off

  #ifdef VERBOSE_BACKEND
    std::cout << "Calculated net ID " << cu_net_id << " from sys " << sys_id
              << " and cu " << cu_id << std::endl;

  #endif

    // clang-format on

    return cu_net_id;
}

DeviceId BaseTopology::calc_buffer_net_id(Sys_ID sys_id,
                                          Buf_Offset buf_off,
                                          Req_Size req_size) {
    // TODO decide if size should be param or if that should be separate
    // function

    if (buffer_location == -1) {
        // buffer level/location not given by user so assume bufs at CUs
        // ie skip this discernment and assume data available at CU
        // return sys_id so the caller treats the buffer as co-located with the
        // NPU
        return sys_id;
    }

    auto rel_buf_id = buf_off / buffer_granularity;

    return calc_global_buffer_id_from_relative(sys_id, rel_buf_id);
}

DeviceId BaseTopology::calc_global_buffer_id_from_relative(
    Sys_ID sys_id, DeviceId rel_buf_id) {
    int buf_id = -1;

    int n_bufs_total;
    DeviceId buf_base_id = 0;
    // if (!is_leveled_topology()) {
    if (!is_ticketed_topology()) {
        // if not ticketed then assume all net devs at "highest level" aka
        // CU/Sys ports
        n_bufs_total = num_devices;
    } else {
        n_bufs_total = ticket_booth->get_width_of_level(buffer_location);

        // count # devices before this level
        for (int l = 0; l < buffer_location; l++) {
            buf_base_id += ticket_booth->get_width_of_level(l);
        }
    }

    /*
     *  Here, n_bufs_total and buf_base_id (id starting of all buffs) known
     *  The target will be in range [buf_base_id, buf_base_id+n_bufs_total)
     *  This range will be shared across systems
     *
     *  2 options
     *  1)   if npus_count < n_bufs_total then mult buffs per sys
     *      <------------npus_count----------->
     *      ----------------- -----------------
     *      | sys 0         | | sys 1         |
     *      ----------------- -----------------
     *      | buf 0 | buf 1 | | buf 2 | buf 3 |
     *      ----------------- -----------------
     *     ^ <---------n_bufs_total----------->
     *  buf_base_id
     *
     *  2)   if npus_count >= n_bufs_total then one buff per sys
     *      <------------npus_count----------->
     *      ----------------- -----------------
     *      | sys 0 | sys 1 | | sys 2 | sys 3 |
     *      ----------------- -----------------
     *      | buf 0         | | buf 1         |
     *      ----------------- -----------------
     *     ^ <---------n_bufs_total----------->
     *  buf_base_id
     *
     *
     *  The goal : determine buf_lb to reduce valid range to
     *     [buf_base_id+buf_lb, buf_base_id+buf_lb+n_bufs_per_sys)
     */

    int n_bufs_per_sys = 1;
    DeviceId buf_lb;
    if (npus_count >= n_bufs_total) {
        // 2)
        int n_sys_per_buf = npus_count / n_bufs_total;  // integer division
        int rel_buf_lb = sys_id / n_sys_per_buf;        // integer division
        buf_lb = buf_base_id + rel_buf_lb;
    } else {
        // 1)
        n_bufs_per_sys = n_bufs_total / npus_count;
        int rel_buf_lb = sys_id * n_bufs_per_sys;
        buf_lb = buf_base_id + rel_buf_lb;
    }

    /*
     * The target will be in range
     *    [buf_lb, buf_lb+n_bufs_per_sys)
     *
     * For now, assume all topologies use buffer_granularity to RR across memory
     * devices
     */
    buf_id = buf_lb + (rel_buf_id % n_bufs_per_sys);

    // clang-format off

  #ifdef VERBOSE_BACKEND
    std::cout << "Calculated buffer ID " << dev_id << " from sys " << sys_id
              << " and buff off " << buf_off << std::endl;

  #endif
  
  // clang-format off

  return buf_id;
}
