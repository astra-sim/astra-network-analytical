/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "NetworkConfigParser.hh"
#include <fstream>
#include <iostream>

using namespace Analytical;

NetworkConfigParser::NetworkConfigParser(
    const std::string& network_configuration) noexcept {
  auto json_file = std::ifstream(network_configuration, std::ifstream::in);
  if (!json_file) {
    std::cout
        << "[NetworkConfigParser] Failed to open network configuration file at: "
        << network_configuration << std::endl;
    exit(-1);
  }

  json_file >> json_configuration;
  json_file.close();
}

bool NetworkConfigParser::useFastVersion() const noexcept {
  return this->get<bool>("use-fast-version");
}

std::vector<NetworkConfigParser::TopologyList> NetworkConfigParser::
    parseHierarchicalTopologyList() const noexcept {
  auto topologies_per_dim = std::vector<TopologyList>();

  for (const auto& topology : json_configuration["topologies-per-dim"]) {
    if (topology == "Ring") {
      topologies_per_dim.emplace_back(TopologyList::Ring);
    } else if (topology == "FullyConnected") {
      topologies_per_dim.emplace_back(TopologyList::FullyConnected);
    } else if (topology == "Switch") {
      topologies_per_dim.emplace_back(TopologyList::Switch);
    } else {
      std::cout << "[NetworkConfigParser] Given topology " << topology
                << " is not supported." << std::endl;
      exit(-1);
    }
  }

  return topologies_per_dim;
}

std::vector<NetworkConfigParser::DimensionType> NetworkConfigParser::
    parseHierarchicalDimensionType() const noexcept {
  auto dimension_types = std::vector<DimensionType>();

  if (!json_configuration.contains("dimension-type")) {
    for (int i = 0; i < 10; i++) {
      dimension_types.emplace_back(DimensionType::Node);
    }
    return dimension_types;
  }

  for (const auto& type : json_configuration["dimension-type"]) {
    if (type == "Tile") {
      dimension_types.emplace_back(DimensionType::Tile);
    } else if (type == "Package") {
      dimension_types.emplace_back(DimensionType::Package);
    } else if (type == "Node") {
      dimension_types.emplace_back(DimensionType::Node);
    } else if (type == "Pod") {
      dimension_types.emplace_back(DimensionType::Pod);
    } else {
      // default is Node for legacy compatability
      dimension_types.emplace_back(DimensionType::Node);
    }
  }

  return dimension_types;
}

std::vector<int> NetworkConfigParser::parseLinksCountPerDim() const noexcept {
  return this->get<std::vector<int>>("links-count");
}

std::vector<NetworkConfigParser::Bandwidth> NetworkConfigParser::
    parseLinkBandwidthPerDim() const noexcept {
  return this->get<std::vector<Bandwidth>>("link-bandwidth");
}
