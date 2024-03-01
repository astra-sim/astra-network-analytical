/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#pragma once

#include "common/Type.hh"
#include "congestion_aware/Topology.hh"

using namespace NetworkAnalytical;

namespace NetworkAnalyticalCongestionAware {

class MultiDimTopology : public Topology {
 public:
  MultiDimTopology(
      int dims_count,
      std::vector<TopologyBuildingBlock>&& topology_per_dim,
      std::vector<int>&& npus_count_per_dim,
      std::vector<Bandwidth>&& bandwidth_per_dim,
      std::vector<Latency>&& latency_per_dim) noexcept;

  /**
   * Implementation of route function in Topology.
   */
  [[nodiscard]] Route route(DeviceId src, DeviceId dest)
      const noexcept override;

 private:
  /**
   * Construct the multi-dimensional network topology
   * including the instantiation of devices
   * and constructing each dimension's connectivity.
   */
  void construct_topology() noexcept;

  /**
   * Construct dim i of the multi-dimensional network
   * which creates multiple basic building blocks across devices.
   *
   * @param dim network dimension to construct
   */
  void construct_dimension(int dim) noexcept;
};

} // namespace NetworkAnalyticalCongestionAware
