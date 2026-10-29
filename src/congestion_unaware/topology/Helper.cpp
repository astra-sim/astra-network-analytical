// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2026 Advanced Micro Devices, Inc.

#include <astra-network-analytical/congestion_unaware/BasicTopology.h>
#include <astra-network-analytical/congestion_unaware/CustomTopology.h>
#include <astra-network-analytical/congestion_unaware/FullyConnected.h>
#include <astra-network-analytical/congestion_unaware/Helper.h>
#include <astra-network-analytical/congestion_unaware/MultiDimTopology.h>
#include <astra-network-analytical/congestion_unaware/Ring.h>
#include <astra-network-analytical/congestion_unaware/Switch.h>
#include <cstdlib>
#include <iostream>

using namespace NetworkAnalytical;
using namespace NetworkAnalyticalCongestionUnaware;

std::shared_ptr<Topology> NetworkAnalyticalCongestionUnaware::
    construct_topology(const NetworkParser& network_parser) noexcept {
    // get network_parser info
    const auto dims_count = network_parser.get_dims_count();
    const auto topologies_per_dim = network_parser.get_topologies_per_dim();
    const auto npus_counts_per_dim = network_parser.get_npus_counts_per_dim();
    const auto num_devices_counts_per_dim =
        network_parser.get_num_devices_counts_per_dim();

    // if dims_count is 1, just create basic topology
    if (dims_count == 1) {
        std::shared_ptr<Topology> basic_topology_ptr;

        // retrieve basic topology info
        const auto topology_type = topologies_per_dim[0];
        const auto npus_count = npus_counts_per_dim[0];
        const auto num_devices_count = num_devices_counts_per_dim[0];
        if (topology_type == TopologyBuildingBlock::Custom) {
            const auto bandwidth_per_link =
                network_parser.get_bandwidths_per_link();
            const auto latencies_per_link =
                network_parser.get_latencies_per_link();
            // return std::make_shared<CustomTopology>(
            //     npus_count, bandwidth_per_link, latencies_per_link);
            basic_topology_ptr = std::make_shared<CustomTopology>(
                npus_count, num_devices_count, bandwidth_per_link,
                latencies_per_link);
        } else {
            const auto bandwidth = network_parser.get_bandwidths_per_dim()[0];
            const auto latency = network_parser.get_latencies_per_dim()[0];

            // create and return basic topology
            switch (topology_type) {
            case TopologyBuildingBlock::Ring:
                return std::make_shared<Ring>(npus_count, num_devices_count,
                                              bandwidth, latency);
                basic_topology_ptr = std::make_shared<Ring>(
                    npus_count, num_devices_count, bandwidth, latency);
                break;
            case TopologyBuildingBlock::Switch:
                return std::make_shared<Switch>(npus_count, num_devices_count,
                                                bandwidth, latency);
                basic_topology_ptr = std::make_shared<Switch>(
                    npus_count, num_devices_count, bandwidth, latency);
                break;
            case TopologyBuildingBlock::FullyConnected:
                // return std::make_shared<FullyConnected>(
                //     npus_count, bandwidth, latency);
                basic_topology_ptr = std::make_shared<FullyConnected>(
                    npus_count, num_devices_count, bandwidth, latency);
                break;
            default:
                // shouldn't reach here
                std::cerr << "[Error] (network/analytical/congestion_unaware)"
                          << "Not supported topology" << std::endl;
                std::exit(-1);
            }
        }

        // make leveled after so that the original constructors can stay
        // unaffected
        if (network_parser.is_leveled_topology()) {
            basic_topology_ptr->set_level_widths(
                network_parser.getLevelWidths());
        }

        // make ticketed after so that the original constructors can stay
        // unaffected
        if (network_parser.is_ticketed_topology()) {
            basic_topology_ptr->create_ticket_booth(
                network_parser.getTicketsPerLevel());
        }

        // make buffer conscious after above constructions so that the
        // original constructors can stay unaffected
        if (network_parser.is_buffer_conscious()) {
            int buf_lev = network_parser.get_buffer_level();
            const auto buf_gran = network_parser.getBufferGranularity();
            basic_topology_ptr->set_buffer_location_and_granularity(buf_lev,
                                                                    buf_gran);
        }

        // return std::shared_ptr
        return basic_topology_ptr;
    }

    const auto bandwidths_per_dim = network_parser.get_bandwidths_per_dim();
    const auto latencies_per_dim = network_parser.get_latencies_per_dim();
    // otherwise, create multi-dim basic-topology
    const auto multi_dim_topology = std::make_shared<MultiDimTopology>();

    // create and append dims
    for (auto dim = 0; dim < dims_count; dim++) {
        // retrieve info
        const auto topology_type = topologies_per_dim[dim];
        const auto npus_count = npus_counts_per_dim[dim];
        const auto num_devices_count = num_devices_counts_per_dim[dim];
        const auto bandwidth = bandwidths_per_dim[dim];
        const auto latency = latencies_per_dim[dim];

        // create a network dim
        std::unique_ptr<BasicTopology> dim_topology;
        switch (topology_type) {
        case TopologyBuildingBlock::Ring:
            dim_topology = std::make_unique<Ring>(npus_count, num_devices_count,
                                                  bandwidth, latency);
            break;
        case TopologyBuildingBlock::Switch:
            dim_topology = std::make_unique<Switch>(
                npus_count, num_devices_count, bandwidth, latency);
            break;
        case TopologyBuildingBlock::FullyConnected:
            dim_topology = std::make_unique<FullyConnected>(
                npus_count, num_devices_count, bandwidth, latency);
            break;
        default:
            // shouldn't reach here
            std::cerr << "[Error] (network/analytical/congestion_unaware)"
                      << "Not supported basic-topology" << std::endl;
            std::exit(-1);
        }

        // make leveled after so that the original constructors can stay
        // unaffected
        if (network_parser.is_leveled_topology()) {
            dim_topology->set_level_widths(network_parser.getLevelWidths());
        }

        // make ticketed after so that the original constructors can stay
        // unaffected
        if (network_parser.is_ticketed_topology()) {
            dim_topology->create_ticket_booth(
                network_parser.getTicketsPerLevel());
        }

        //  make buffer conscious after above constructions so that the
        // original constructors can stay unaffected
        if (network_parser.is_buffer_conscious()) {
            int buf_lev = network_parser.get_buffer_level();
            // defaults to 256B
            const auto buf_gran = network_parser.getBufferGranularity();
            dim_topology->set_buffer_location_and_granularity(buf_lev,
                                                              buf_gran);
        }

        // append network dimension
        multi_dim_topology->append_dimension(std::move(dim_topology));
    }

    // return created multi-dimensional topology
    return multi_dim_topology;
}
