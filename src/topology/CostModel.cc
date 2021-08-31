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

  resources_cost_table[ResourceType::Link] = 2;
  resources_cost_table[ResourceType::Nic] = 48;
  resources_cost_table[ResourceType::Switch] = 24;
  resources_cost_table[ResourceType::Npu] = 0;
}

CostModel::~CostModel() = default;

void CostModel::addResource(
    ResourceType resource,
    int count,
    double bandwidth,
    int radix) noexcept {
  if (resource == ResourceType::Npu) {
    auto added_cost = resources_cost_table[resource] * count;
    network_total_cost += added_cost;

    std::cout << "[CostModel] Added NPU: " << count << ", cost: " << added_cost
              << std::endl;
  } else if (resource == ResourceType::Link) {
    auto added_cost = resources_cost_table[resource] * count * bandwidth;
    network_total_cost += added_cost;

    std::cout
        << "[CostModel] Added Link: " << count << ", cost: " << added_cost
        << std::endl;
  } else if (resource == ResourceType::Nic) {
    auto added_cost = resources_cost_table[resource] * count * bandwidth;
    network_total_cost += added_cost;

    std::cout
        << "[CostModel] Added Nid: " << count << ", cost: " << added_cost
        << std::endl;
  } else if (resource == ResourceType::Switch) {
    auto added_cost =
        resources_cost_table[resource] * count * bandwidth * radix;
    network_total_cost += added_cost;

    std::cout
        << "[CostModel] Switch: " << count << ", cost: " << added_cost
        << std::endl;
  } else {
    std::cout << "[CostModel] Error, Resource undefined!!! " << count
              << std::endl;
    exit(-1);
  }
}

double CostModel::get_network_cost() const noexcept {
  return network_total_cost;
}
