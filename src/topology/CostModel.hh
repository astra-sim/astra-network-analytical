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
  /**
   * How to add new Resource element
   *   1. Add new resource element to Resource enum class
   *   2. Update CostModel() constructor
   *          set resource_usage_table to 0
   *          set resource_cost_table to cost
   *   3. Update computeTotalCost() method
   *          add cost to total_cost
   */
  // Resources definition
  enum class ResourceType {
    // fixme: Update this accordingly when required
    Npu, // todo: must be refined or removed
    TileToTileLink, // todo: must be refined or removed
    NVLink,
    MellanoxSwitch,
    InfiniBandNic, // todo: change name
  };

  using ResourceInfo = std::pair<int, double>;  // (count, total_cost)
  using Bandwidth = TopologyConfig::Bandwidth;

  static Bandwidth nv_link_bandwidth;

  CostModel() noexcept;

  ~CostModel();

  void addResource(ResourceType resource, int count, double additional_info) noexcept;

  double computeTotalCost() const noexcept;

  int getRequiredNicsCount(Bandwidth bandwidth) const noexcept;

  int getMellanoxSwitchesCount(int radix) const noexcept;

 private:
  std::map<ResourceType, ResourceInfo> resources_usage_table;
  std::map<ResourceType, int> resources_cost_table;

  int nvSwitchRadix;
};
} // namespace Analytical

#endif
