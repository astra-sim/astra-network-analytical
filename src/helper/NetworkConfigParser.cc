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

  for (const auto& type : json_configuration["dimension-type"]) {
    if (type == "T") {
      dimension_types.emplace_back(DimensionType::T);
    } else if (type == "N") {
      dimension_types.emplace_back(DimensionType::N);
    } else if (type == "P") {
      dimension_types.emplace_back(DimensionType::P);
    } else if (type == "PP") {
      dimension_types.emplace_back(DimensionType::PP);
    } else {
      std::cout << "[NetworkConfigParser] Given Dimension type " << type
                << " is not supported." << std::endl;
      exit(-1);
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
