/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "HierarchicalTopologyConfig.hh"

using namespace Analytical;

HierarchicalTopologyConfig::HierarchicalTopologyConfig(
    int dimensions_count,
    std::vector<TopologyList> topologies_per_dim,
    std::vector<int> links_count_per_dim) noexcept
    : dimensions_count(dimensions_count),
      topologies_per_dim(topologies_per_dim),
      links_count_per_dim(links_count_per_dim) {}

int HierarchicalTopologyConfig::getDimensionsCount() const noexcept {
  return dimensions_count;
}

HierarchicalTopologyConfig::TopologyList HierarchicalTopologyConfig::
    getTopologyForDim(int dimension) const noexcept {
  return topologies_per_dim[dimension];
}

int HierarchicalTopologyConfig::getLinksCountForDim(
    int dimension) const noexcept {
  return links_count_per_dim[dimension];
}
