// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2026 Advanced Micro Devices, Inc.

#include <astra-network-analytical/congestion_aware/Helper.h>
#include <cstdlib>
#include <iostream>

using namespace NetworkAnalytical;
using namespace NetworkAnalyticalCongestionAware;

std::shared_ptr<Topology> NetworkAnalyticalCongestionAware::construct_topology(
    const NetworkParser& network_parser) noexcept {
    // get network_parser info
    const auto dims_count = network_parser.get_dims_count();
    const auto topologies_per_dim = network_parser.get_topologies_per_dim();
    const auto npus_counts_per_dim = network_parser.get_npus_counts_per_dim();
    const auto num_devices_counts_per_dim =
        network_parser.get_num_devices_counts_per_dim();
    const auto bandwidths_per_dim = network_parser.get_bandwidths_per_dim();
    const auto latencies_per_dim = network_parser.get_latencies_per_dim();

    // retrieve basic basic-topology info
    const auto topology_type = topologies_per_dim[0];
    const auto npus_count = npus_counts_per_dim[0];
    const auto num_devices = num_devices_counts_per_dim[0];
    const auto bandwidth = bandwidths_per_dim[0];
    const auto latency = latencies_per_dim[0];

    std::shared_ptr<Topology> topo_ptr;

    switch (topology_type) {
    case TopologyBuildingBlock::Custom:
        if (network_parser.routes_per_pair_provided) {
            topo_ptr = std::make_shared<CustomTopology>(
                npus_count, num_devices,
                network_parser.get_bandwidths_per_link(),
                network_parser.get_latencies_per_link(),
                network_parser.get_routes_per_pair());
        } else {
            topo_ptr = std::make_shared<CustomTopology>(
                npus_count, num_devices,
                network_parser.get_bandwidths_per_link(),
                network_parser.get_latencies_per_link());
        }

        break;
    case TopologyBuildingBlock::Ring:
        topo_ptr =
            std::make_shared<Ring>(npus_count, num_devices, bandwidth, latency);
        break;
    case TopologyBuildingBlock::Switch:
        topo_ptr = std::make_shared<Switch>(npus_count, num_devices, bandwidth,
                                            latency);
        break;
    case TopologyBuildingBlock::FullyConnected:
        topo_ptr = std::make_shared<FullyConnected>(npus_count, num_devices,
                                                    bandwidth, latency);
        break;
    default:
        // shouldn't reach here
        std::cerr << "[Error] (network/analytical/congestion_aware) "
                  << "not supported basic-topology" << std::endl;
        std::exit(-1);
    }

    // make leveled after so that the original constructors can stay
    // unaffected
    if (network_parser.is_leveled_topology()) {
        topo_ptr->set_level_widths(network_parser.getLevelWidths());
    }

    // make ticketed after so that the original constructors can stay
    // unaffected
    if (network_parser.is_ticketed_topology()) {
        topo_ptr->create_ticket_booth(network_parser.getTicketsPerLevel());
    }

    // make buffer conscious after above constructions so that the
    // original constructors can stay unaffected
    if (network_parser.is_buffer_conscious()) {
        int buf_lev = network_parser.get_buffer_level();
        // defaults to 256B
        const auto buf_gran = network_parser.getBufferGranularity();
        topo_ptr->set_buffer_location_and_granularity(buf_lev, buf_gran);
    }

    return topo_ptr;
}
