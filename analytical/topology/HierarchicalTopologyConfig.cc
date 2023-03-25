/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "HierarchicalTopologyConfig.hh"

using namespace Analytical;

HierarchicalTopologyConfig::HierarchicalTopologyConfig(
    int dimensions_count,
    std::vector<TopologyList> topologies_per_dim,
    std::vector<DimensionType> dimension_types,
    std::vector<int> links_count_per_dim,
    std::vector<Bandwidth> link_bandwidth_per_dim) noexcept
    : dimensions_count(dimensions_count),
      topologies_per_dim(topologies_per_dim),
      dimension_types(dimension_types),
      links_count_per_dim(links_count_per_dim),
      link_bandwidth_per_dim(link_bandwidth_per_dim) {}

std::string HierarchicalTopologyConfig::dimensionTypeToStr(
    DimensionType dimension_type) noexcept {
  switch (dimension_type) {
    case DimensionType::Tile:
      return "Tile";
    case DimensionType::Package:
      return "Package";
    case DimensionType::Node:
      return "Node";
    case DimensionType::Pod:
      return "Pod";
  }

  return "";
}

int HierarchicalTopologyConfig::getDimensionsCount() const noexcept {
  return dimensions_count;
}

HierarchicalTopologyConfig::DimensionType HierarchicalTopologyConfig::
    getDimensionType(int dimension) const noexcept {
  return dimension_types[dimension];
}

HierarchicalTopologyConfig::TopologyList HierarchicalTopologyConfig::
    getTopologyForDim(int dimension) const noexcept {
  return topologies_per_dim[dimension];
}

int HierarchicalTopologyConfig::getLinksCountForDim(
    int dimension) const noexcept {
  return links_count_per_dim[dimension];
}

HierarchicalTopologyConfig::Bandwidth HierarchicalTopologyConfig::
    getLinkBandwidthForDim(int dimension) const noexcept {
  return link_bandwidth_per_dim[dimension];
}
