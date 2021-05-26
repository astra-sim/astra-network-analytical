/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "CostModel.hh"
#include <cmath>
#include <iostream>

using namespace Analytical;

CostModel::Bandwidth CostModel::nv_link_bandwidth = 50;  // GB/s

CostModel::CostModel() noexcept {
  // Initialize table
  resources_usage_table[ResourceType::NVLink] = std::make_pair(0, 0);
  resources_usage_table[ResourceType::MellanoxSwitch] = std::make_pair(0, 0);
  resources_usage_table[ResourceType::InfiniBandNic] = std::make_pair(0, 0);
  resources_usage_table[ResourceType::Npu] = std::make_pair(0, 0);
  resources_usage_table[ResourceType::TileToTileLink] = std::make_pair(0, 0);

  // Define cost here
  resources_cost_table[ResourceType::NVLink] = 100; // from TeraRack paper
  resources_cost_table[ResourceType::MellanoxSwitch] = 23'698; // QM8790 InfiniBand Switch
  resources_cost_table[ResourceType::InfiniBandNic] = 1'200;
  resources_cost_table[ResourceType::Npu] = 0; // todo: currently disregarded
  resources_cost_table[ResourceType::TileToTileLink] =
      0.5 * resources_cost_table[ResourceType::NVLink]; // fixme: currently assuming 0.5(NVLink) as tile-to-tile link.
}

CostModel::~CostModel() = default;

void CostModel::addResource(ResourceType resource, int count, double additional_info) noexcept {
  // Print log
  if (resource == ResourceType::Npu) {
    std::cout << "[CostModel] Added NPU: " << count << std::endl;
  } else if (resource == ResourceType::TileToTileLink) {
    std::cout << "[CostModel] Added TileToTileLink: " << count << std::endl;
  } else if (resource == ResourceType::NVLink) {
    std::cout << "[CostModel] Added NVLink: " << count << std::endl;
  } else if (resource == ResourceType::MellanoxSwitch) {
    std::cout << "[CostModel] Added NVSwitch: " << count << std::endl;
  } else if (resource == ResourceType::InfiniBandNic) {
    std::cout << "[CostModel] AddInfiniBandNic: " << count << std::endl;
  } else {
    std::cout << "[CostModel] Error, Resource undefined!!! " << count
              << std::endl;
    exit(-1);
  }

  auto cost = 0.0;
  if (resource == ResourceType::NVLink) {
    // scale by bandwidth
    cost = (additional_info / CostModel::nv_link_bandwidth) * resources_cost_table[resource] * count;
    std::cout << "(NVLink) Added cost: " << cost << " (BW: " << additional_info << ", count: " << count << ", unit_cost: " << resources_cost_table[resource] << ")" << std::endl;
  } else if (resource == ResourceType::TileToTileLink) {
    // same metric for NVLink
    cost = (additional_info / CostModel::nv_link_bandwidth) * resources_cost_table[resource] * count;
    std::cout << "(T-T Link) Added cost: " << cost << " (BW: " << additional_info << ", count: " << count << ", unit_cost: " << resources_cost_table[resource] << ")" << std::endl;
  } else {
    cost = resources_cost_table[resource] * count;
    std::cout << "(Resource) Added cost: " << cost << " (count: " << count << ", unit_cost: " << resources_cost_table[resource] << ")" << std::endl;
  }

  std::get<0>(resources_usage_table[resource]) += count;
  std::get<1>(resources_usage_table[resource]) += cost;
}

double CostModel::computeTotalCost() const noexcept {
  auto total_cost = 0.0;

  total_cost += std::get<1>(resources_usage_table.at(ResourceType::Npu));
  total_cost += std::get<1>(resources_usage_table.at(ResourceType::TileToTileLink));
  total_cost += std::get<1>(resources_usage_table.at(ResourceType::NVLink));
  total_cost += std::get<1>(resources_usage_table.at(ResourceType::MellanoxSwitch));
  total_cost += std::get<1>(resources_usage_table.at(ResourceType::InfiniBandNic));

  return total_cost;
}

int CostModel::getRequiredNicsCount(Bandwidth bandwidth) const noexcept {
  const auto nic_bandwidth = 50.0;
  return std::ceil((double)bandwidth / nic_bandwidth);
}

int CostModel::getMellanoxSwitchesCount(int radix) const noexcept {
  const auto switch_radix = 40;
  return std::ceil((double)radix / switch_radix);
}
