/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#pragma once

#include <yaml-cpp/yaml.h>
#include "common/Common.hh"
#include "common/Type.hh"

namespace NetworkAnalytical {

class NetworkParser {
 public:
  explicit NetworkParser(const std::string& path) noexcept;

  [[nodiscard]] int get_dims_count() const noexcept;

  [[nodiscard]] std::vector<int> get_npus_counts_per_dim() const noexcept;

  [[nodiscard]] std::vector<Bandwidth> get_bandwidths_per_dim() const noexcept;

  [[nodiscard]] std::vector<Latency> get_latencies_per_dim() const noexcept;

  [[nodiscard]] std::vector<TopologyBuildingBlock> get_topologies_per_dim()
      const noexcept;

 private:
  int dims_count;
  std::vector<int> npus_counts_per_dim;
  std::vector<Bandwidth> bandwidths_per_dim;
  std::vector<Latency> latencies_per_dim;
  std::vector<TopologyBuildingBlock> topologies_per_dim;

  [[nodiscard]] static TopologyBuildingBlock parse_topology_name(
      const std::string& topology_name) noexcept;

  void parse_network_config_yml(const YAML::Node& network_config) noexcept;

  void check_validity() const noexcept;

  template <typename T>
  std::vector<T> parse_vector(const YAML::Node& node) const noexcept;
};

} // namespace NetworkAnalytical
