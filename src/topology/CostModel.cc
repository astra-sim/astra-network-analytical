/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "CostModel.hh"
#include <cmath>
#include <iostream>

using namespace Analytical;

CostModel::CostModel() noexcept {
  // Initialize table
  resources_usage_table[Resource::NVLink] = 0;
  resources_usage_table[Resource::NVSwitch] = 0;
  resources_usage_table[Resource::InfiniBandNic] = 0;
  resources_usage_table[Resource::Npu] = 0;
  resources_usage_table[Resource::TileToTileLink] = 0;

  // Define cost here
  resources_cost_table[Resource::NVLink] = 100; // from TeraRack paper
  resources_cost_table[Resource::NVSwitch] = 5'200; // from TeraRack paper
  resources_cost_table[Resource::InfiniBandNic] = 1'200;
  resources_cost_table[Resource::Npu] = 0; // todo: currently disregarded
  resources_cost_table[Resource::TileToTileLink] =
      0; // todo: currently disregarded
}

CostModel::~CostModel() = default;

void CostModel::addResource(Resource resource, int count) noexcept {
  // Print log
  if (resource == Resource::Npu) {
    std::cout << "[CostModel] Added NPU: " << count << std::endl;
  } else if (resource == Resource::TileToTileLink) {
    std::cout << "[CostModel] Added TileToTileLink: " << count << std::endl;
  } else if (resource == Resource::NVLink) {
    std::cout << "[CostModel] Added NVLink: " << count << std::endl;
  } else if (resource == Resource::NVSwitch) {
    std::cout << "[CostModel] Added NVSwitch: " << count << std::endl;
  } else if (resource == Resource::InfiniBandNic) {
    std::cout << "[CostModel] AddInfiniBandNic: " << count << std::endl;
  } else {
    std::cout << "[CostModel] Error, Resource undefined!!! " << count
              << std::endl;
    exit(-1);
  }

  resources_usage_table[resource] += count;
}

double CostModel::computeTotalCost() const noexcept {
  auto total_cost = 0.0;

  total_cost += resources_usage_table.find(Resource::Npu)->second *
      resources_cost_table.find(Resource::Npu)->second;

  total_cost += resources_usage_table.find(Resource::TileToTileLink)->second *
      resources_cost_table.find(Resource::TileToTileLink)->second;

  total_cost += resources_usage_table.find(Resource::NVLink)->second *
      resources_cost_table.find(Resource::NVLink)->second;

  total_cost += resources_usage_table.find(Resource::NVSwitch)->second *
      resources_cost_table.find(Resource::NVSwitch)->second;

  total_cost += resources_usage_table.find(Resource::InfiniBandNic)->second *
      resources_cost_table.find(Resource::InfiniBandNic)->second;

  return total_cost;
}

int CostModel::getNVSwitchesCount(int radix) const noexcept {
  const auto switch_radix = 128;
  return std::ceil((double)radix / switch_radix);
}
