/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "DetailedTopology.hh"
#include <cassert>
#include <iostream>

using namespace Analytical;

DetailedTopology::DetailedTopology(TopologyConfigs configs) noexcept:
        Topology(configs) { }

DetailedTopology::~DetailedTopology() noexcept = default;

void DetailedTopology::connect(NpuId src, NpuId dest, int dimension) noexcept {
    auto src_entry = links.find(src);
    if (src_entry == links.end()) {
        // create new entry
        auto insert_result = links.emplace(src, std::map<NpuId, Link>());
        src_entry = std::get<0>(insert_result);
    }

    auto link_latency = configs[dimension].getLinkLatency();
    auto create_result = src_entry->second.emplace(dest, Link(link_latency));

    assert(std::get<1>(create_result) && "[DetailedTopology, method connect] Link already exists");
}

DetailedTopology::Latency DetailedTopology::transmit(NpuId src, NpuId dest, PayloadSize payload_size) noexcept {
    std::cout << "send: " << src << " -> " << dest << std::endl;
    auto src_entry = links.find(src);
    assert(src_entry != links.end() && "[DetailedTopology, method transmit] Link doesn't exist.");

    auto dest_entry = src_entry->second.find(dest);
    assert(dest_entry != src_entry->second.end() && "[DetailedTopology, method transmit] Link doesn't exist.");

    auto link = dest_entry->second;
    return link.transmit(payload_size);
}
