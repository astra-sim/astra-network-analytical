/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#pragma once

#include "common/Common.hh"
#include "common/Type.hh"
#include "congestion_unaware/basic-topology/BasicTopology.hh"
#include "congestion_unaware/topology/Topology.hh"

using namespace NetworkAnalytical;

namespace NetworkAnalyticalCongestionUnaware {

class MultiDimTopology : public Topology {
 public:
  MultiDimTopology() noexcept;

  [[nodiscard]] EventTime send(NodeId src, NodeId dest, ChunkSize size)
      const noexcept override;

  void add_dim(std::unique_ptr<BasicTopology> topology) noexcept;

 private:
  using MultiDimAddress = std::vector<NodeId>;

  int dims_count;
  std::vector<std::unique_ptr<BasicTopology>> topologies_per_dim;
  std::vector<int> nodes_count_per_dim;

  [[nodiscard]] MultiDimAddress translate_address(
      NodeId node_id) const noexcept;

  [[nodiscard]] int get_dim_to_transfer(
      MultiDimAddress src_address,
      MultiDimAddress dest_address) const noexcept;
};

} // namespace NetworkAnalyticalCongestionUnaware
