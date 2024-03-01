/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "congestion_aware/Helper.hh"
#include <cstdlib>
#include <iostream>
#include <memory>
#include "congestion_aware/FullyConnected.hh"
#include "congestion_aware/MultiDimTopology.hh"
#include "congestion_aware/Ring.hh"
#include "congestion_aware/Switch.hh"

using namespace NetworkAnalytical;
using namespace NetworkAnalyticalCongestionAware;

std::shared_ptr<Topology> NetworkAnalyticalCongestionAware::construct_topology(
    const NetworkParser& network_parser) noexcept {
  // get network_parser info
  const auto dims_count = network_parser.get_dims_count();
  auto topology_per_dim = network_parser.get_topology_per_dim();
  auto npus_count_per_dim = network_parser.get_npus_count_per_dim();
  auto bandwidth_per_dim = network_parser.get_bandwidth_per_dim();
  auto latency_per_dim = network_parser.get_latency_per_dim();

  if (dims_count == 1) {
    // retrieve basic basic-topology info
    const auto topology_type = topology_per_dim[0];
    const auto npus_count = npus_count_per_dim[0];
    const auto bandwidth = bandwidth_per_dim[0];
    const auto latency = latency_per_dim[0];

    switch (topology_type) {
      case TopologyBuildingBlock::Ring:
        return std::make_shared<Ring>(npus_count, bandwidth, latency);
      case TopologyBuildingBlock::Switch:
        return std::make_shared<Switch>(npus_count, bandwidth, latency);
      case TopologyBuildingBlock::FullyConnected:
        return std::make_shared<FullyConnected>(npus_count, bandwidth, latency);
      default:
        // shouldn't reach here
        std::cerr << "[Error] (network/analytical/congestion_aware) "
                  << "not supported basic-topology" << std::endl;
        std::exit(-1);
    }
  }

  // Multi-dimensional topology
  auto topology = std::make_shared<MultiDimTopology>(
      dims_count,
      std::move(topology_per_dim),
      std::move(npus_count_per_dim),
      std::move(bandwidth_per_dim),
      std::move(latency_per_dim));

  std::cerr << "[Error] MultiDim" << std::endl;
  std::exit(-1);
}
