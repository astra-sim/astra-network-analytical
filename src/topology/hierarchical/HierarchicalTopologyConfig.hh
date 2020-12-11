/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#ifndef __HIERARCHICALTOPOLOGYCONFIG_HH__
#define __HIERARCHICALTOPOLOGYCONFIG_HH__

#include <vector>

namespace Analytical {
struct HierarchicalTopologyConfig {
 public:
  enum class TopologyList { Ring, Switch, AllToAll };

  HierarchicalTopologyConfig(
      int dimensions_count,
      std::vector<TopologyList> topologies_per_dim) noexcept;

  int getDimensionsCount() const noexcept;
  TopologyList getTopologyForDim(int dimension) const noexcept;

 private:
  int dimensions_count;
  std::vector<TopologyList> topologies_per_dim;
};
} // namespace Analytical

#endif
