/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#ifndef __COSTMODEL_HH__
#define __COSTMODEL_HH__

#include <map>
#include <string>
#include <tuple>
#include "HierarchicalTopologyConfig.hh"
#include "TopologyConfig.hh"

namespace Analytical {
class CostModel {
 public:
  // Resources definition
  enum class ResourceType { Link, Nic, Switch };

  static std::string resourceTypeToStr(ResourceType resource_type) noexcept;

  using ResourceInfo = std::pair<int, double>; // (count, total_cost)
  using Bandwidth = TopologyConfig::Bandwidth;
  using DimensionType = HierarchicalTopologyConfig::DimensionType;

  CostModel() noexcept;

  ~CostModel();

  void addResource(
      DimensionType dimension_type,
      ResourceType resource,
      int count,
      double bandwidth,
      int switch_radix = 1) noexcept;

  double get_network_cost() const noexcept;

 private:
  double network_total_cost;
  std::map<DimensionType, std::map<ResourceType, double>> resources_cost_table;
};
} // namespace Analytical

#endif
