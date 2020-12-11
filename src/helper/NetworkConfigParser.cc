/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "NetworkConfigParser.hh"
#include <iostream>

using namespace Analytical;

NetworkConfigParser::NetworkConfigParser(nlohmann::json& json_configuration) noexcept :
        json_configuration(json_configuration) { }

std::vector<NetworkConfigParser::TopologyList> NetworkConfigParser::parseHierarchicalTopologyList() const noexcept {
    auto topologies_per_dim = std::vector<TopologyList>();

    for (const auto& topology : json_configuration["topologies-per-dim"]) {
        if (topology == "Ring") {
            topologies_per_dim.emplace_back(TopologyList::Ring);
        } else if (topology == "AllToAll") {
            topologies_per_dim.emplace_back(TopologyList::AllToAll);
        } else if (topology == "Switch") {
            topologies_per_dim.emplace_back(TopologyList::Switch);
        } else {
            std::cout << "[NetworkConfigParser] Given topology " << topology << " is not supported." << std::endl;
            exit(-1);
        }
    }

    return topologies_per_dim;
}

bool NetworkConfigParser::useFastVersion() const noexcept {
    return json_configuration["use-fast-version"];
}
