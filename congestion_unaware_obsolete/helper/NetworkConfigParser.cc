/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include <congestion_unaware/helper/NetworkConfigParser.hh>

#include <fstream>
#include <iostream>

using namespace std;
using namespace Analytical;

NetworkConfigParser::NetworkConfigParser(
    const string& network_configuration) noexcept {
  auto json_file = ifstream(network_configuration, ifstream::in);
  if (!json_file) {
    cout
        << "[NetworkConfigParser] Failed to open network configuration file at: "
        << network_configuration << endl;
    exit(-1);
  }

  json_file >> json_configuration;
  json_file.close();
}

vector<NetworkConfigParser::TopologyList> NetworkConfigParser::
    parseHierarchicalTopologyList() const noexcept {
  auto topologies_per_dim = vector<TopologyList>();

  for (const auto& topology : json_configuration["topologies-per-dim"]) {
    if (topology == "Ring") {
      topologies_per_dim.emplace_back(TopologyList::Ring);
    } else if (topology == "FullyConnected") {
      topologies_per_dim.emplace_back(TopologyList::FullyConnected);
    } else if (topology == "Switch") {
      topologies_per_dim.emplace_back(TopologyList::Switch);
    } else {
      cout << "[NetworkConfigParser] Given basic-topology " << topology
           << " is not supported." << endl;
      exit(-1);
    }
  }

  return topologies_per_dim;
}

vector<NetworkConfigParser::DimensionType> NetworkConfigParser::
    parseHierarchicalDimensionType() const noexcept {
  auto dimension_types = vector<DimensionType>();

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

vector<int> NetworkConfigParser::parseLinksCountPerDim() const noexcept {
  return this->get<vector<int>>("links-count");
}

vector<NetworkConfigParser::Bandwidth> NetworkConfigParser::
    parseLinkBandwidthPerDim() const noexcept {
  return this->get<vector<Bandwidth>>("link-bandwidth");
}
