/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#ifndef __NETWORKCONFIGPARSER_HH__
#define __NETWORKCONFIGPARSER_HH__

#include <vector>
#include "json.hh"
#include "../topology/hierarchical/HierarchicalTopologyConfig.hh"

namespace Analytical {
    class NetworkConfigParser {
    public:
        using TopologyList = HierarchicalTopologyConfig::TopologyList;

        NetworkConfigParser(nlohmann::json& json_configuration) noexcept;

        std::vector<TopologyList> parseHierarchicalTopologyList() const noexcept;

        bool useFastVersion() const noexcept;

    private:
        nlohmann::json json_configuration;
    };
}

#endif
