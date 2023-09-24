/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "congestion_unaware/topology/Helper.hh"
#include "congestion_unaware/basic-topology/FullyConnected.hh"
#include "congestion_unaware/basic-topology/Ring.hh"
#include "congestion_unaware/basic-topology/Switch.hh"
#include "congestion_unaware/multi-dim-topology/MultiDimTopology.hh"

using namespace NetworkAnalytical;
using namespace NetworkAnalyticalCongestionUnaware;

std::shared_ptr<Topology> NetworkAnalyticalCongestionUnaware::
    construct_topology(const NetworkParser& network_parser) noexcept {
  // get network_parser info
  const auto dims_count = network_parser.get_dims_count();
  const auto topologies_per_dim = network_parser.get_topologies_per_dim();
  const auto npus_counts_per_dim = network_parser.get_npus_counts_per_dim();
  const auto bandwidths_per_dim = network_parser.get_bandwidths_per_dim();
  const auto latencies_per_dim = network_parser.get_latencies_per_dim();

  // if dims_count is 1, just create basic topology
  if (dims_count == 1) {
    // retrieve basic topology info
    const auto topology_type = topologies_per_dim[0];
    const auto npus_count = npus_counts_per_dim[0];
    const auto bandwidth = bandwidths_per_dim[0];
    const auto latency = latencies_per_dim[0];

    switch (topology_type) {
      case TopologyBuildingBlock::Ring:
        return std::make_shared<Ring>(npus_count, bandwidth, latency);
      case TopologyBuildingBlock::Switch:
        return std::make_shared<Switch>(npus_count, bandwidth, latency);
      case TopologyBuildingBlock::FullyConnected:
        return std::make_shared<FullyConnected>(npus_count, bandwidth, latency);
      default:
        // shouldn't reaach here
        std::cerr << "[analytical/congestion_unaware] not supported topology"
                  << std::endl;
        std::exit(-1);
    }
  }

  // otherwise, create multi-dim topology
  auto multi_dim_topology = std::make_shared<MultiDimTopology>();

  // create and append dims
  for (auto dim = 0; dim < dims_count; dim++) {
    // retrieve info
    const auto topology_type = topologies_per_dim[dim];
    const auto npus_count = npus_counts_per_dim[dim];
    const auto bandwidth = bandwidths_per_dim[dim];
    const auto latency = latencies_per_dim[dim];

    // create dim
    std::unique_ptr<BasicTopology> dim_topology;
    switch (topology_type) {
      case TopologyBuildingBlock::Ring:
        dim_topology = std::make_unique<Ring>(npus_count, bandwidth, latency);
        break;
      case TopologyBuildingBlock::Switch:
        dim_topology = std::make_unique<Switch>(npus_count, bandwidth, latency);
        break;
      case TopologyBuildingBlock::FullyConnected:
        dim_topology =
            std::make_unique<FullyConnected>(npus_count, bandwidth, latency);
        break;
      default:
        // shouldn't reaach here
        std::cerr << "[analytical/congestion_unaware] not supported topology"
                  << std::endl;
        std::exit(-1);
    }
    multi_dim_topology->add_dim(std::move(dim_topology));
  }

  // return created topology
  return multi_dim_topology;
}
