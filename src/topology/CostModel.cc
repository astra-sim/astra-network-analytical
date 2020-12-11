/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include <iostream>
#include "CostModel.hh"

using namespace Analytical;

CostModel::CostModel() noexcept = default;

void CostModel::createLink(int count, Latency latency, Bandwidth bandwidth) noexcept {
    auto key = std::make_tuple(latency, bandwidth);
    auto identical_resouce = link_map.find(key);

    if (identical_resouce == link_map.end()) {
        // no identical resource: add
        link_map.emplace(key, count);
    } else {
        // identical resource found: update
        identical_resouce->second += count;
    }
}

void CostModel::createNic(int count, Latency latency, Bandwidth bandwidth) noexcept {
    auto key = std::make_tuple(latency, bandwidth);
    auto identical_resouce = nic_map.find(key);

    if (identical_resouce == nic_map.end()) {
        // no identical resource: add
        nic_map.emplace(key, count);
    } else {
        // identical resource found: update
        identical_resouce->second += count;
    }
}

void CostModel::createNpu(int count, Radix radix) noexcept {
    auto identical_resource = npu_map.find(radix);

    if (identical_resource == npu_map.end()) {
        // no identical resource: add
        npu_map.emplace(radix, count);
    } else {
        // identical resource found: update
        identical_resource->second += count;
    }
}

void CostModel::createSwitch(int count, Radix radix) noexcept {
    auto identical_resource = switch_map.find(radix);

    if (identical_resource == switch_map.end()) {
        // no identical resource: add
        switch_map.emplace(radix, count);
    } else {
        // identical resource found: update
        identical_resource->second += count;
    }
}

void CostModel::computeCost() const noexcept {
    auto links_count = 0;
    auto nics_count = 0;
    auto npus_count = 0;
    auto switches_count = 0;

    std::cout << "[CostModel] Link Usage" << std::endl;
    for (const auto& link : link_map) {
        Latency latency = 0;
        Bandwidth bandwidth = 0;
        std::tie(latency, bandwidth) = link.first;

        auto count = link.second;
        links_count += count;

        std::cout << "\t" << count << " (Latency: " << latency << ", Bandwidth: " << bandwidth << ")" << std::endl;
    }
    std::cout << "[CostModel] Total links usage: " << links_count << std::endl << std::endl;;

    std::cout << "[CostModel] NIC Usage" << std::endl;
    for (const auto& nic : nic_map) {
        Latency latency = 0;
        Bandwidth bandwidth = 0;
        std::tie(latency, bandwidth) = nic.first;
        
        auto count = nic.second;
        nics_count += count;
        
        std::cout << "\t" << count << " (Latency: " << latency << ", Bandwidth: " << bandwidth << ")" << std::endl;
    }
    std::cout << "[CostModel] Total NIC usage: " << nics_count << std::endl << std::endl;;
    
    std::cout << "[CostModel] NPUs Usage" << std::endl;
    for (const auto& npu : npu_map) {
        auto radix = npu.first;
        auto count = npu.second;
        npus_count += count;
        std::cout << "\t" << count << " (Radix: " << radix << ")" << std::endl;
    }
    std::cout << "[CostModel] Total NPU usage: " << npus_count << std::endl << std::endl;;
    
    std::cout << "[CostModel] Switch Usage" << std::endl;
    for (const auto& s : switch_map) {
        auto radix = s.first;
        auto count = s.second;
        switches_count += count;
        std::cout << "\t" << count << " (Radix: " << radix << ")" << std::endl;
    }
    std::cout << "[CostModel] Total switch usage: " << switches_count << std::endl << std::endl;

    // Compute total cost
    auto total_cost = links_count + nics_count + npus_count + switches_count;
    std::cout << "[CostModel] Total resources used: " << total_cost << std::endl;
}
