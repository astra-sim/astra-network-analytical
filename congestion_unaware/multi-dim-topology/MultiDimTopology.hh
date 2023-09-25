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

  [[nodiscard]] EventTime send(
      DeviceId src,
      DeviceId dest,
      ChunkSize chunk_size) const noexcept override;

  void add_dim(std::unique_ptr<BasicTopology> topology) noexcept;

 private:
  using MultiDimAddress = std::vector<DeviceId>;

  int dims_count;
  std::vector<std::unique_ptr<BasicTopology>> topologies_per_dim;
  std::vector<int> npus_count_per_dim;

  [[nodiscard]] MultiDimAddress translate_address(
      DeviceId npu_id) const noexcept;

  [[nodiscard]] int get_dim_to_transfer(
      const MultiDimAddress& src_address,
      const MultiDimAddress& dest_address) const noexcept;
};

} // namespace NetworkAnalyticalCongestionUnaware
