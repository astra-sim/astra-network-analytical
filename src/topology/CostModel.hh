/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#ifndef __COSTMODEL_HH__
#define __COSTMODEL_HH__

#include <map>
#include <tuple>
#include "TopologyConfig.hh"

namespace Analytical {
class CostModel {
 public:
  // Resources definition
  enum class ResourceType {
    // fixme: Update this accordingly when required
    Npu, // todo: must be refined or removed
    Link,
    Nic,
    Switch
  };

  using ResourceInfo = std::pair<int, double>; // (count, total_cost)
  using Bandwidth = TopologyConfig::Bandwidth;

  CostModel() noexcept;

  ~CostModel();

  void addResource(
      ResourceType resource,
      int count,
      double bandwidth,
      int radix) noexcept;

  double get_network_cost() const noexcept;

 private:
  double network_total_cost;
  std::map<ResourceType, int> resources_cost_table;
};
} // namespace Analytical

#endif
