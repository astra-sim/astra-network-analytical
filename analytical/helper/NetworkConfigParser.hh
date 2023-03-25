/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#ifndef __NETWORKCONFIGPARSER_HH__
#define __NETWORKCONFIGPARSER_HH__

#include <vector>
#include "../topology/TopologyConfig.hh"
#include "extern/network_backend/analytical/analytical/topology/HierarchicalTopologyConfig.hh"
#include "json.hh"

namespace Analytical {
class NetworkConfigParser {
 public:
  using TopologyList = HierarchicalTopologyConfig::TopologyList;
  using DimensionType = HierarchicalTopologyConfig::DimensionType;
  using Bandwidth = TopologyConfig::Bandwidth;

  explicit NetworkConfigParser(
      const std::string& network_configuration) noexcept;

  bool useFastVersion() const noexcept;

  template <typename T>
  T get(const char* arg_name) const noexcept {
    return json_configuration[arg_name];
  }

  template <typename T>
  T get (const char* arg_name, T default_value) const noexcept {
    if (json_configuration.contains(arg_name)) {
      return json_configuration[arg_name];
    }
    return default_value;
  }

  std::vector<TopologyList> parseHierarchicalTopologyList() const noexcept;

  std::vector<DimensionType> parseHierarchicalDimensionType() const noexcept;

  std::vector<int> parseLinksCountPerDim() const noexcept;

  std::vector<Bandwidth> parseLinkBandwidthPerDim() const noexcept;

 private:
  nlohmann::json json_configuration;
};
} // namespace Analytical

#endif
