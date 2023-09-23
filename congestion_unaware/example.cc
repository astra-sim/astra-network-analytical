/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "common/Common.hh"
#include "congestion_unaware/event-queue/EventQueue.hh"
#include "congestion_unaware/topology/HierarchicalTopology.hh"
#include "congestion_unaware/topology/HierarchicalTopologyConfig.hh"

using namespace Analytical;

using TopologyList = HierarchicalTopologyConfig::TopologyList;
using DimensionType = HierarchicalTopologyConfig::DimensionType;

int main(int argc, char* argv[]) {
  // topology configs
  auto dimensions_count = 3;
  auto npus_count_per_dim = 4;
  auto link_bandwidth = 1; //  GB/s
  auto link_latency = 500; // ns
  auto nic_latency = 0;
  auto router_latency = 0;

  // topology
  auto topology_configs = Analytical::Topology::TopologyConfigs();

  for (int i = 0; i < dimensions_count; i++) {
    auto link_bandwidth_b_ns = (double)link_bandwidth * (1 << 30) /
        (1'000'000'000); // link bandwidth in B/ns

    topology_configs.emplace_back(
        npus_count_per_dim, // NPUs count
        link_latency, // link latency (ns)
        link_bandwidth_b_ns, // link bandwidth (B/ns)
        nic_latency, // nic latency (ns)
        router_latency, // router latency (ns)
        0, // memory latency (ns),
        1, // memory bandwidth (GB/s) = (B/ns)
        0 // memory scaling factor
    );
  }

  auto topologies_per_dim = {
      TopologyList::Ring, TopologyList::Ring, TopologyList::Ring};
  auto dimension_types = {
      DimensionType::Node, DimensionType::Node, DimensionType::Node};
  std::vector<int> links_count_per_dim = {2, 2, 2};
  std::vector<double> bw_per_dim = {1, 1, 1};

  auto hierarchy_config = HierarchicalTopologyConfig(
      dimensions_count,
      topologies_per_dim,
      dimension_types,
      links_count_per_dim,
      bw_per_dim);

  auto topology = std::make_shared<Analytical::HierarchicalTopology>(
      topology_configs, hierarchy_config);

  auto [time, _] = topology->send(0, 3, 1048576);
  std::cout << "Time taken: " << time << std::endl;

  return 0;
}
