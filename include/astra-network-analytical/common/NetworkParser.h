// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2026 Advanced Micro Devices, Inc.

#pragma once

#include <astra-network-analytical/common/Type.h>
#include <iostream>
#include <yaml-cpp/yaml.h>

namespace NetworkAnalytical {

/**
 * NetworkParser parses the network configuration file in YAML format.
 */
class NetworkParser {
  public:
    /**
     * Constructor.
     *
     * @param path path of the yml file
     */
    explicit NetworkParser(const std::string& path) noexcept;

    /**
     * Return the number of network dimensions.
     * Which is calculated by the length of "topology" value
     *
     * @return number of network dimensions
     */
    [[nodiscard]] int get_dims_count() const noexcept;

    /**
     * Read "npus_count" value
     *
     * @return number of NPUs per each demension.
     */
    [[nodiscard]] const std::vector<int>& get_npus_counts_per_dim()
        const noexcept;

    /**
     * Read "network_devices_count" value
     * If not defined, substitute w/ "npus_count"
     *
     * @return number of network elements per each demension.
     */
    [[nodiscard]] const std::vector<int>& get_num_devices_counts_per_dim()
        const noexcept;

    /**
     * Read "network_devices_count" value
     * If not defined, substitute w/ "npus_count"
     *
     * @return number of network elements in first dimension
     */
    [[nodiscard]] int get_num_devices() const noexcept;

    /**
     * Read "bandwidth" value
     *
     * @return bandwidth per each dimension
     */
    [[nodiscard]] const std::vector<Bandwidth>& get_bandwidths_per_dim()
        const noexcept;

    /**
     * Read "latency" value
     *
     * @return link latency per each dimension
     */
    [[nodiscard]] const std::vector<Latency>& get_latencies_per_dim()
        const noexcept;

    /**
     * Read "topology" value and translate it into TopologyBuildingBlock
     * components
     *
     * @return topology building block per each dimension
     */
    [[nodiscard]] const std::vector<TopologyBuildingBlock>&
    get_topologies_per_dim() const noexcept;

    /**
     * Read "latency" matrix for Custom topology
     *
     * @return link latency per each link
     */
    [[nodiscard]] const std::map<int, std::map<int, Latency>>&
    get_latencies_per_link() const noexcept;

    /**
     * Read "bandwidth" matrix for Custom topology
     *
     * @return link bandwidth per each link
     */
    [[nodiscard]] const std::map<int, std::map<int, Bandwidth>>&
    get_bandwidths_per_link() const noexcept;

    [[nodiscard]] const std::map<int, std::map<int, std::vector<DeviceId>>>&
    get_routes_per_pair() const noexcept;

    /**
     * If using ticketing or buffers not at each CU. Set true if read buffer
     * level parameter from network yaml
     *
     * @return bool if it is
     */
    [[nodiscard]] bool is_leveled_topology() const noexcept {
        return _is_leveled_topology;
    }

    /**
     * If using ticketing. Set true if read ticketing parameters from network
     * yaml
     *
     * @return bool if it is
     */
    [[nodiscard]] bool is_ticketed_topology() const noexcept {
        return _is_ticketed_topology;
    }

    /**
     * If using buffers not at each CU. Set true if read buffer level parameter
     * from network yaml
     *
     * @return bool if it is
     */
    [[nodiscard]] bool is_buffer_conscious() const noexcept {
        return _is_buffer_conscious;
    }

    /**
     * Get information on the read # tickets / level
     *
     * @return member variable of tickets_per_level
     */
    [[nodiscard]] const std::vector<int>& getTicketsPerLevel() const;

    /**
     * Get information on the read # nodes / level
     *
     * @return member variable of level_widths
     */
    [[nodiscard]] const std::vector<int>& getLevelWidths() const;

    /**
     * Get information on the read buffer location/level
     *
     * @return member variable of buffer_level
     */
    [[nodiscard]] int get_buffer_level() const;

    /**
     * Get information on the read buffer granularity
     * Defaults to 256B if not specified
     *
     * @return member variable of buffer_granularity
     */
    [[nodiscard]] Buf_Offset getBufferGranularity() const;

    bool routes_per_pair_provided;

  private:
    /// number of network dimensions
    int dims_count;

    /// NPUs count per each dimension
    std::vector<int> npus_count_per_dim;

    /// network devices count per each dimension
    std::vector<int> num_devices_count_per_dim;

    /// network devices count for flat (custom) topologies
    int num_devices;

    /// bandwidth per each dimension
    std::vector<Bandwidth> bandwidth_per_dim;

    /// latency per each dimension
    std::vector<Latency> latency_per_dim;

    /// topology building block per each dimension
    std::vector<TopologyBuildingBlock> topology_per_dim;

    /// tickets per level (device type)
    std::vector<int> tickets_per_level;

    /// number of devices per level (device type)
    std::vector<int> level_widths;

    // if specifying different levels/device types
    bool _is_leveled_topology;

    /// if ticketing user inputs read
    bool _is_ticketed_topology;

    /// if buffer user inputs read
    bool _is_buffer_conscious;

    // "level" that buffers are modeled at
    int buffer_level;

    // granularity for interleaving that buffers are modeled at
    Buf_Offset buffer_granularity;

    /// Custom topology settings:
    std::map<int, std::map<int, Bandwidth>>
        bandwidth_per_link;  // bandwidth of each link in GB/s (1e9 bits/s)

    std::map<int, std::map<int, Latency>>
        latency_per_link;  // latency of each link in ns

    std::map<int, std::map<int, std::vector<DeviceId>>> routes_per_pair;

    /**
     * Parse topology name (in string) into TopologyBuildingBlock enum
     *
     * @param topology_name topology name in string
     *    which can be "Ring", "FullyConnected", "Switch", or "Custom"
     * @return parsed TopologyBuildingBlock enum class value
     */
    [[nodiscard]] static TopologyBuildingBlock parse_topology_name(
        const std::string& topology_name) noexcept;

    /**
     * Parse the given YAML node and retrieve network configuration values
     *
     * @param network_config opened and parsed YAML node
     */
    void parse_network_config_yml(const YAML::Node& network_config) noexcept;

    /**
     * Check the validity and correctness of the parsed network input
     * configurations.
     */
    void check_validity() const noexcept;

    /**
     * Helpful print to verify correct values for latency, bandwidth read from
     * network yaml
     */
    void show_custom_topology(std::map<int, std::map<int, double>> spec,
                              std::string spec_type) noexcept;

    /**
     * Helpful print to verify correct values for tickets_per_level,
     * level_widths read from network yaml
     */
    void show_levels_and_ticketing(std::vector<int> tpl,
                                   std::vector<int> lw) noexcept;

    /**
     * Helpful print to verify correct values for routes read from network yaml
     */
    void show_custom_topology_routes(
        std::map<int, std::map<int, std::vector<DeviceId>>> spec) noexcept;
    /**
     * Given a yaml node whose type is list of type T,
     * Read the value from the node and create a std::vector<T>.
     *
     * @tparam T type of the element to be read
     * @param node YAML node (in list type) to read
     * @return std::vector<T> of read elements
     */
    template <typename T>
    std::vector<T> parse_vector(const YAML::Node& node) const noexcept {
        // create empty vector to store parsed values
        auto parsed_vector = std::vector<T>();

        // try to read each element
        for (const auto& element : node) {
            try {
                // read an element in type T
                const auto element_value = element.as<T>();

                // store the read element
                parsed_vector.push_back(element_value);
            } catch (const YAML::BadConversion& e) {
                // error reading an element from the yaml file as type T
                std::cerr << "[Error] (network/analytical) " << e.what()
                          << std::endl;
                std::exit(-1);
            }
        }

        // return parsed vector
        return parsed_vector;
    }

    template <typename T>
    std::map<int, std::map<int, T>> parse_sparse_matrix(
        const YAML::Node& node) const noexcept {
        // create empty vector to store parsed values
        auto sparse_matrix = std::map<int, std::map<int, T>>();

        // try to read each element
        for (const auto& list : node) {
            int src_node = list.first.as<int>();
            sparse_matrix[src_node] = std::map<int, T>();
            for (const auto& element : list.second) {
                int dst_node = element.first.as<int>();
                try {
                    // read an element in type T
                    const auto element_value = element.second.as<T>();
                    // store the read element
                    sparse_matrix[src_node][dst_node] = element_value;
                } catch (const YAML::BadConversion& e) {
                    // error reading an element from the yaml file as type T
                    std::cerr << "[Error] (network/analytical) " << e.what()
                              << std::endl;
                    std::exit(-1);
                }
            }
        }

        // return parsed vector
        return sparse_matrix;
    }

    template <typename T>
    std::map<int, std::map<int, std::vector<T>>> parse_sparse_matrix_of_vectors(
        const YAML::Node& node) const noexcept {
        // create empty vector to store parsed values
        auto sparse_matrix_of_vectors =
            std::map<int, std::map<int, std::vector<T>>>();

        // try to read each element
        for (const auto& row_of_vectors : node) {
            int src_node = row_of_vectors.first.as<int>();
            sparse_matrix_of_vectors[src_node] =
                std::map<int, std::vector<T>>();
            for (const auto& vector : row_of_vectors.second) {
                int dst_node = vector.first.as<int>();
                sparse_matrix_of_vectors[src_node][dst_node] = std::vector<T>();
                for (const auto& v_elem : vector.second) {
                    try {
                        // read an element in type T
                        const auto elem_val = v_elem.as<T>();
                        // store the read element
                        sparse_matrix_of_vectors[src_node][dst_node].push_back(
                            elem_val);
                    } catch (const YAML::BadConversion& e) {
                        // error reading an element from the yaml file as type T
                        std::cerr << "[Error] (network/analytical) " << e.what()
                                  << std::endl;
                        std::exit(-1);
                    }
                }
            }
        }

        // return parsed vector
        return sparse_matrix_of_vectors;
    }
};

}  // namespace NetworkAnalytical
