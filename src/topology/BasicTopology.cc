#include "BasicTopology.hh"

using namespace AnalyticalBackend;

BasicTopology::~BasicTopology() = default;

BasicTopology::NodeAddress BasicTopology::nodeIDtoAddress(NodeID nodeID) const noexcept {
    return std::vector<NodeID>(1, nodeID);
}
