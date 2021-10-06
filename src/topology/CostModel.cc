/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "CostModel.hh"
#include <cmath>
#include <iostream>

using namespace Analytical;

CostModel::CostModel() noexcept {
  // Define cost here
  network_total_cost = 0;

  resources_cost_table[DimensionType::Tile][ResourceType::Link] = 1.0;
  resources_cost_table[DimensionType::Tile][ResourceType::Switch] = -1;
  resources_cost_table[DimensionType::Tile][ResourceType::Nic] = -1;

  resources_cost_table[DimensionType::Package][ResourceType::Link] = 4.0;
  resources_cost_table[DimensionType::Package][ResourceType::Switch] = 13.0;
  resources_cost_table[DimensionType::Package][ResourceType::Nic] = 0;

  resources_cost_table[DimensionType::Node][ResourceType::Link] = 4.0;
  resources_cost_table[DimensionType::Node][ResourceType::Switch] = 13.0;
  resources_cost_table[DimensionType::Node][ResourceType::Nic] = 0;

  resources_cost_table[DimensionType::Pod][ResourceType::Link] = 7.4;
  resources_cost_table[DimensionType::Pod][ResourceType::Switch] = 23.7;
  resources_cost_table[DimensionType::Pod][ResourceType::Nic] = 47.8;
}

std::string CostModel::resourceTypeToStr(ResourceType resource_type) noexcept {
  switch (resource_type) {
    case ResourceType::Link:
      return "Link";
    case ResourceType::Switch:
      return "Switch";
    case ResourceType::Nic:
      return "Nic";
  }

  return "";
}

CostModel::~CostModel() = default;

void CostModel::addResource(
    DimensionType dimension_type,
    ResourceType resource,
    int count,
    double bandwidth,
    int switch_radix) noexcept {
  if (resources_cost_table[dimension_type][resource] < 0) {
    std::cout << CostModel::resourceTypeToStr(resource) << " on "
              << HierarchicalTopologyConfig::dimensionTypeToStr(dimension_type)
              << " is not supported!" << std::endl;
    exit(-1);
  }
  auto added_cost = resources_cost_table[dimension_type][resource] * count *
      bandwidth * switch_radix;
  network_total_cost += added_cost;

  std::cout << "[CostModel] Adding (inter-"
            << HierarchicalTopologyConfig::dimensionTypeToStr(dimension_type)
            << ", " << CostModel::resourceTypeToStr(resource)
            << ") bandwidth: " << bandwidth << ", radix: " << switch_radix
            << ", count: " << count << " (added cost: $" << added_cost << ")"
            << std::endl;
}

double CostModel::get_network_cost() const noexcept {
  return network_total_cost;
}
