// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2026 Advanced Micro Devices, Inc.

#include <cassert>
#include <astra-network-analytical/common/NetworkParser.h>
#include <iostream>

using namespace NetworkAnalytical;

NetworkParser::NetworkParser(const std::string& path) noexcept
    : dims_count(-1),
      routes_per_pair_provided(false),
      _is_ticketed_topology(false),
      _is_leveled_topology(false),
      _is_buffer_conscious(false),
      buffer_level(-1),
      buffer_granularity(256) {
    // initialize values
    npus_count_per_dim = {};
    bandwidth_per_dim = {};
    latency_per_dim = {};
    topology_per_dim = {};
    tickets_per_level = {};
    level_widths = {};

    try {
        std::cout << "Will parse " << path << std::endl;
        // load network config file
        const auto network_config = YAML::LoadFile(path);

        // parse network configs
        parse_network_config_yml(network_config);
    } catch (const YAML::BadFile& e) {
        // loading network config file failed
        std::cerr << "[Error] (network/analytical) " << e.what() << std::endl;
        std::exit(-1);
    }
}

int NetworkParser::get_dims_count() const noexcept {
    assert(dims_count > 0);

    return dims_count;
}

const std::vector<int>& NetworkParser::get_npus_counts_per_dim()
    const noexcept {
    assert(dims_count > 0);

    return npus_count_per_dim;
}

const std::vector<int>& NetworkParser::get_num_devices_counts_per_dim()
    const noexcept {
    assert(dims_count > 0);

    if (num_devices_count_per_dim.size() == 0) {
        // if not defined substitute with npu count
        return npus_count_per_dim;
    }
    return num_devices_count_per_dim;
}

int NetworkParser::get_num_devices() const noexcept {
    return num_devices;
}

const std::vector<Bandwidth>& NetworkParser::get_bandwidths_per_dim()
    const noexcept {
    assert(dims_count > 0);

    return bandwidth_per_dim;
}

const std::vector<Latency>& NetworkParser::get_latencies_per_dim()
    const noexcept {
    assert(dims_count > 0);

    return latency_per_dim;
}

const std::vector<TopologyBuildingBlock>& NetworkParser::
    get_topologies_per_dim() const noexcept {
    assert(dims_count > 0);

    return topology_per_dim;
}

const std::map<int, std::map<int, Latency>>& NetworkParser::
    get_latencies_per_link() const noexcept {
    assert(dims_count > 0);
    assert(topology_per_dim[0] == TopologyBuildingBlock::Custom);

    return latency_per_link;
}

const std::map<int, std::map<int, Bandwidth>>& NetworkParser::
    get_bandwidths_per_link() const noexcept {
    assert(dims_count > 0);
    assert(topology_per_dim[0] == TopologyBuildingBlock::Custom);

    return bandwidth_per_link;
}

const std::map<int, std::map<int, std::vector<DeviceId>>>& NetworkParser::
    get_routes_per_pair() const noexcept {
    assert(dims_count > 0);
    assert(topology_per_dim[0] == TopologyBuildingBlock::Custom);

    return routes_per_pair;
}

const std::vector<int>& NetworkParser::getTicketsPerLevel() const {
    assert(!tickets_per_level.empty());

    return tickets_per_level;
}

const std::vector<int>& NetworkParser::getLevelWidths() const {
    assert(!level_widths.empty());

    return level_widths;
}

int NetworkParser::get_buffer_level() const {
    return buffer_level;
}

Buf_Offset NetworkParser::getBufferGranularity() const {
    return buffer_granularity;
}

void NetworkParser::parse_network_config_yml(
    const YAML::Node& network_config) noexcept {
    // parse topology_per_dim
    const auto topology_names =
        parse_vector<std::string>(network_config["topology"]);
    for (const auto& topology_name : topology_names) {
        const auto topology_dim =
            NetworkParser::parse_topology_name(topology_name);
        topology_per_dim.push_back(topology_dim);
    }

    // set dims_count
    dims_count = static_cast<int>(topology_per_dim.size());

    // parse vector values
    npus_count_per_dim = parse_vector<int>(network_config["npus_count"]);

    // semi-optional param
    if (network_config["num_devices"].IsDefined()) {
        num_devices_count_per_dim =
            parse_vector<int>(network_config["num_devices"]);
        num_devices = num_devices_count_per_dim[0];
    }
    // backup
    else {
        num_devices_count_per_dim =
            parse_vector<int>(network_config["npus_count"]);
        num_devices = num_devices_count_per_dim[0];
    }

    if (topology_per_dim[0] == TopologyBuildingBlock::Custom) {
        // sure of change here. Will default to npus if not defined in yaml
        assert(dims_count == 1);
        bandwidth_per_dim.push_back(1.0);
        latency_per_dim.push_back(1.0);
        bandwidth_per_link =
            parse_sparse_matrix<Bandwidth>(network_config["bandwidth"]);
        if (num_devices <= 100) {
            show_custom_topology(bandwidth_per_link, "Bandwidth");
        }
        latency_per_link =
            parse_sparse_matrix<Latency>(network_config["latency"]);
        if (num_devices <= 100) {
            show_custom_topology(latency_per_link, "Latency");
        }
        auto route_conf = network_config["routes"];
        routes_per_pair_provided = route_conf.IsDefined();
        if (routes_per_pair_provided) {
            routes_per_pair = parse_sparse_matrix_of_vectors<int>(route_conf);
            std::cout << " Got route spec" << std::endl;
            if (num_devices <= 100) {
                show_custom_topology_routes(routes_per_pair);
            }
        } else {
            std::cout << " Didn't get route spec" << std::endl;
        }
    } else {
        bandwidth_per_dim =
            parse_vector<Bandwidth>(network_config["bandwidth"]);
        latency_per_dim = parse_vector<Latency>(network_config["latency"]);
    }

    // if ticketed and/or buffered
    if (network_config["level_widths"].IsDefined()) {
        level_widths = parse_vector<int>(network_config["level_widths"]);
        _is_leveled_topology = true;
    }

    // if ticketed
    if (network_config["tickets_per_level"].IsDefined()) {
        tickets_per_level =
            parse_vector<int>(network_config["tickets_per_level"]);
        _is_ticketed_topology = true;
        // if(num_devices <= 100)
        show_levels_and_ticketing(tickets_per_level, level_widths);
    }

    // if buffer conscious
    if (network_config["buffer_level"].IsDefined()) {
        buffer_level = network_config["buffer_level"].as<int>();
        _is_buffer_conscious = true;
        std::cout << "Buffers at level : " << buffer_level << std::endl;
    }
    if (network_config["buffer_granularity"].IsDefined()) {
        buffer_granularity =
            network_config["buffer_granularity"].as<Buf_Offset>();
        std::cout << "Buffers w/ granularity : " << buffer_granularity
                  << std::endl;
    }
    // check the validity of the parsed network config
    check_validity();
}

void NetworkParser::show_custom_topology(
    std::map<int, std::map<int, double>> spec,
    std::string spec_type = "") noexcept {
    std::string units = (spec_type == "Bandwidth") ? " (GB/s)" : " (ns)";
    std::cout << "\n Custom Topology " << spec_type << units
              << " matrix:" << std::endl;

    // Matrix headers
    std::cout << "    ||";
    for (int src_npu = 0; src_npu < num_devices; src_npu++) {
        printf("  %3d  |", src_npu);
    }
    std::cout << std::endl;

    // Headers divisor
    std::cout << "    ||";
    for (int src_npu = 0; src_npu < num_devices; src_npu++) {
        printf("-------|");
    }
    std::cout << std::endl;

    // Print the matrix
    for (int src_npu = 0; src_npu < num_devices; src_npu++) {
        printf("%3d ||", src_npu);
        if (spec.find(src_npu) == spec.end()) {
            for (int dst_npu = 0; dst_npu < num_devices; dst_npu++) {
                std::cout << "   -   |";
            }
        } else {
            auto npu_links = spec.find(src_npu)->second;
            for (int dst_npu = 0; dst_npu < num_devices; dst_npu++) {
                if (npu_links.find(dst_npu) == npu_links.end()) {
                    std::cout << "   -   |";
                } else {
                    if (spec_type == "Bandwidth") {
                        printf("%7.2f|", npu_links.find(dst_npu)->second);
                    } else {
                        printf("%7.0f|", npu_links.find(dst_npu)->second);
                    }
                }
            }
        }
        std::cout << std::endl;
    }
}

void NetworkParser::show_levels_and_ticketing(std::vector<int> tpl,
                                              std::vector<int> lw) noexcept {
    std::cout << "\n Custom Topology ticketing:" << std::endl;

    int n_levels = tpl.size();

    std::cout << "|   level   | # routers | # tickets |" << std::endl;
    std::cout << "-------------------------------------" << std::endl;

    for (int i = n_levels - 1; i >= 0; i--) {
        printf("|     %d     |   %3d     |   %3d     |\n", i, lw[i], tpl[i]);
    }
}

void NetworkParser::show_custom_topology_routes(
    std::map<int, std::map<int, std::vector<DeviceId>>> spec) noexcept {
    std::cout << "\n Custom Topology routes matrix (ignoring end points):"
              << std::endl;

    // Matrix headers
    std::cout << "    ||";
    for (int src_npu = 0; src_npu < num_devices; src_npu++) {
        printf("  %3d    |", src_npu);
    }
    std::cout << std::endl;

    // Headers divisor
    std::cout << "    ||";
    for (int src_npu = 0; src_npu < num_devices; src_npu++) {
        printf("---------|");
    }
    std::cout << std::endl;

    // Print the matrix
    for (int src_npu = 0; src_npu < num_devices; src_npu++) {
        printf("%3d ||", src_npu);
        if (spec.find(src_npu) == spec.end()) {
            for (int dst_npu = 0; dst_npu < num_devices; dst_npu++) {
                std::cout << "    -    |";
            }
        } else {
            auto npu_links = spec.find(src_npu)->second;
            for (int dst_npu = 0; dst_npu < num_devices; dst_npu++) {
                if (npu_links.find(dst_npu) == npu_links.end()) {
                    std::cout << "    -    |";
                } else {
                    bool first_hop = true;
                    int space_left = 9;
                    auto path = npu_links.find(dst_npu)->second;
                    if (path.size() == 2) {
                        printf("%7s  ", "direct");
                    } else {
                        for (int i = 1; i < path.size() - 1; i++) {
                            if (i != 1) {
                                printf(",");
                                space_left -= 1;
                            }
                            first_hop = false;
                            printf("%2d", path[i]);
                            space_left -= 2;
                        }
                        for (int i = space_left; i > 0; i--) {
                            printf(" ");
                        }
                    }
                    printf("|");
                }
            }
        }
        std::cout << std::endl;
    }
}

TopologyBuildingBlock NetworkParser::parse_topology_name(
    const std::string& topology_name) noexcept {
    assert(!topology_name.empty());

    if (topology_name == "Ring") {
        return TopologyBuildingBlock::Ring;
    }

    if (topology_name == "FullyConnected") {
        return TopologyBuildingBlock::FullyConnected;
    }

    if (topology_name == "Switch") {
        return TopologyBuildingBlock::Switch;
    }

    if (topology_name == "Custom") {
        return TopologyBuildingBlock::Custom;
    }

    // shouldn't reach here
    std::cerr << "[Error] (network/analytical) " << "Topology name "
              << topology_name << " not supported" << std::endl;
    std::exit(-1);
}

void NetworkParser::check_validity() const noexcept {
    // dims_count should match
    if (dims_count != npus_count_per_dim.size()) {
        std::cerr << "[Error] (network/analytical) " << "length of npus_count ("
                  << npus_count_per_dim.size()
                  << ") doesn't match with dimensions (" << dims_count << ")"
                  << std::endl;
        std::exit(-1);
    }

    if (topology_per_dim[0] == TopologyBuildingBlock::Custom) {
        // bandwidths should be all positive

        if (num_devices == 0) {
            std::cerr << "[Error] (network/analytical) "
                      << "zero value for num_devices not allowed" << std::endl;
            std::exit(-1);
        }

        for (auto& npu_links : bandwidth_per_link) {
            if (!(0 <= npu_links.first && npu_links.first < num_devices)) {
                std::cerr
                    << "[Error] (network/analytical) Bandwidth specification. "
                    << "Src npu id (" << npu_links.first << ") should be in [0,"
                    << num_devices - 1 << "]" << std::endl;
                std::exit(-1);
            }
            for (auto& link_bw : npu_links.second) {
                if (!(0 <= link_bw.first && link_bw.first < num_devices)) {
                    std::cerr << "[Error] (network/analytical) Bandwidth "
                                 "specification. "
                              << "Dst npu id (" << link_bw.first
                              << ") should be in [0," << num_devices - 1 << "]"
                              << std::endl;
                    std::exit(-1);
                }
                if (link_bw.second <= 0) {
                    std::cerr << "[Error] (network/analytical) Bandwidth "
                                 "specification. "
                              << "bandwidth (" << link_bw.second
                              << ") should be larger than 0" << std::endl;
                    std::exit(-1);
                }
            }
            // std::cout << std::endl;
        }

        for (auto& npu_links : latency_per_link) {
            if (!(0 <= npu_links.first && npu_links.first < num_devices)) {
                std::cerr
                    << "[Error] (network/analytical) Latency specification. "
                    << "Src npu id (" << npu_links.first << ") should be in [0,"
                    << num_devices - 1 << "]" << std::endl;
                std::exit(-1);
            }
            for (auto& link_bw : npu_links.second) {
                if (!(0 <= link_bw.first && link_bw.first < num_devices)) {
                    std::cerr << "[Error] (network/analytical) Latency "
                                 "specification. "
                              << "Dst npu id (" << link_bw.first
                              << ") should be in [0," << num_devices - 1 << "]"
                              << std::endl;
                    std::exit(-1);
                }
                if (link_bw.second < 0) {
                    std::cerr << "[Error] (network/analytical) Latency "
                                 "specification. "
                              << "Latency (" << link_bw.second
                              << ") should be >= 0" << std::endl;
                    std::exit(-1);
                }
            }
            // std::cout << std::endl;
        }

    } else {
        if (dims_count != bandwidth_per_dim.size()) {
            std::cerr << "[Error] (network/analytical) "
                      << "length of bandwidth (" << bandwidth_per_dim.size()
                      << ") doesn't match with dims_count (" << dims_count
                      << ")" << std::endl;
            std::exit(-1);
        }

        if (dims_count != latency_per_dim.size()) {
            std::cerr << "[Error] (network/analytical) "
                      << "length of latency (" << latency_per_dim.size()
                      << ") doesn't match with dims_count (" << dims_count
                      << ")" << std::endl;
            std::exit(-1);
        }

        // bandwidths should be all positive
        for (const auto& bandwidth : bandwidth_per_dim) {
            if (bandwidth <= 0) {
                std::cerr << "[Error] (network/analytical) " << "bandwidth ("
                          << bandwidth << ") should be larger than 0"
                          << std::endl;
                std::exit(-1);
            }
        }

        // latency should be non-negative
        for (const auto& latency : latency_per_dim) {
            if (latency < 0) {
                std::cerr << "[Error] (network/analytical) " << "latency ("
                          << latency << ") should be non-negative" << std::endl;
                std::exit(-1);
            }
        }
    }
    // npus_count should be all positive
    for (const auto& npus_count : npus_count_per_dim) {
        if (npus_count < 1) {
            std::cerr << "[Error] (network/analytical) " << "npus_count ("
                      << npus_count << ") should be 1 or more" << std::endl;
            std::exit(-1);
        }
    }

    // sum of level widths should equal number of net devs
    // assume flat network (ie dimension=1)
    if (_is_leveled_topology) {
        int d_sum = 0;
        for (const auto n_devs : level_widths) {
            if (n_devs < 0) {
                std::cerr << "[Error] (network/analytical) "
                          << "negative # devices in level disallowed "
                          << std::endl;
                std::exit(-1);
            }
            d_sum += n_devs;
        }
        if (d_sum != num_devices) {
            std::cerr << "[Error] (network/analytical) "
                      << "net devices count (" << num_devices
                      << ") != sum of level widths" << std::endl;
            std::exit(-1);
        }
    }

    // buffer level must be valid
    if (_is_buffer_conscious) {
        if (buffer_level < 0 || buffer_level >= level_widths.size()) {
            std::cerr << "[Error] (network/analytical) " << "buffer level ("
                      << buffer_level << ") outside valid range [0,"
                      << level_widths.size() << ")" << std::endl;
            std::exit(-1);
        }
    }

    // number of ticket levels must be valid
    if (_is_ticketed_topology) {
        if (tickets_per_level.size() != level_widths.size()) {
            std::cerr << "[Error] (network/analytical) "
                      << "mismatch in # levels between tickets ("
                      << tickets_per_level.size() << ") and level widths ("
                      << level_widths.size() << ")" << std::endl;
            std::exit(-1);
        }
        // each level must be valid
        for (const auto n_tickets : tickets_per_level) {
            if (n_tickets < 0) {
                std::cerr << "[Error] (network/analytical) "
                          << "negative # tickets disallowed " << std::endl;
                std::exit(-1);
            }
        }
    }
}
