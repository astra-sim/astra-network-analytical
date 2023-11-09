/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#pragma once

#include <yaml-cpp/yaml.h>
#include <iostream>
#include "common/Type.hh"

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
  [[nodiscard]] std::vector<int> get_npus_counts_per_dim() const noexcept;

  /**
   * Read "bandwidth" value
   *
   * @return bandwidth per each dimension
   */
  [[nodiscard]] std::vector<Bandwidth> get_bandwidths_per_dim() const noexcept;

  /**
   * Read "latency" value
   *
   * @return link latency per each dimension
   */
  [[nodiscard]] std::vector<Latency> get_latencies_per_dim() const noexcept;

  /**
   * Read "topology" value and translate it into TopologyBuildingBlock
   * components
   *
   * @return topology building block per each dimension
   */
  [[nodiscard]] std::vector<TopologyBuildingBlock> get_topologies_per_dim()
      const noexcept;

 private:
  /// number of network dimensions
  int dims_count;

  /// NPUs count per each dimension
  std::vector<int> npus_count_per_dim;

  /// bandwidth per each dimension
  std::vector<Bandwidth> bandwidth_per_dim;

  /// latency per each dimension
  std::vector<Latency> latency_per_dim;

  /// topology building block per each dimension
  std::vector<TopologyBuildingBlock> topology_per_dim;

  /**
   * Parse topology name (in string) into TopologyBuildingBlock enum
   *
   * @param topology_name topology name in string
   *    which can be "Ring", "FullyConnected", or "Switch"
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
        std::cerr << "[Error] (network/analytical) " << e.what() << std::endl;
        std::exit(-1);
      }
    }

    // return parsed vector
    return parsed_vector;
  }
};

} // namespace NetworkAnalytical
