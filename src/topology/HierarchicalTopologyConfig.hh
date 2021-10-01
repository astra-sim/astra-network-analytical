/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#ifndef __HIERARCHICALTOPOLOGYCONFIG_HH__
#define __HIERARCHICALTOPOLOGYCONFIG_HH__

#include <string>
#include <vector>
#include "TopologyConfig.hh"

namespace Analytical {
struct HierarchicalTopologyConfig {
 public:
  using Bandwidth = TopologyConfig::Bandwidth;
  enum class TopologyList { Ring, Switch, FullyConnected };
  enum class DimensionType {
    Tile,
    Package,
    Node,
    Pod
  }; // tile-to-tile, within node, within pod, pod-to-pod

  static std::string dimensionTypeToStr(DimensionType dimension_type) noexcept;

  HierarchicalTopologyConfig(
      int dimensions_count,
      std::vector<TopologyList> topologies_per_dim,
      std::vector<DimensionType> dimension_types,
      std::vector<int> links_count_per_dim,
      std::vector<Bandwidth> link_bandwidth_per_dim) noexcept;

  int getDimensionsCount() const noexcept;

  TopologyList getTopologyForDim(int dimension) const noexcept;

  DimensionType getDimensionType(int dimension) const noexcept;

  int getLinksCountForDim(int dimension) const noexcept;

  Bandwidth getLinkBandwidthForDim(int dimension) const noexcept;

 private:
  int dimensions_count;
  std::vector<TopologyList> topologies_per_dim;
  std::vector<DimensionType> dimension_types;
  std::vector<int> links_count_per_dim;
  std::vector<Bandwidth> link_bandwidth_per_dim;
};
} // namespace Analytical

#endif
