/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#pragma once

#include <memory>
#include "common/Type.hh"
#include "congestion_aware/BasicTopology.hh"
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
  using MultiDimAddress = std::vector<DeviceId>;

  /// store pointer to the basic_topology[of which NPU][of which network dim].
  std::vector<std::vector<std::shared_ptr<BasicTopology>>> basic_topology_map;

  /**
   * Construct the multi-dimensional network topology
   * including the instantiation of devices
   * and constructing each dimension's connectivity
   * using the construct_dimension() method iteratively across all dimensions.
   */
  void construct_topology() noexcept;

  /**
   * Construct dim i of the multi-dimensional network
   * which creates multiple basic building blocks across devices.
   *
   * e.g., let's say we have a 4x3x2 (=24) topology
   * (1) Constructing the 1st dim: we should create 3x2 = 6 basic topologies
   *
   *    [ 0 1 2 3 ]       Group 0
   *    --------------------------
   *    [ 4 5 6 7 ]       Group 1
   *    ---------------------------
   *    [ 8 9 10 11 ]     Group 2
   *    ---------------------------
   *    [ 12 13 14 15 ]   Group 3
   *    ---------------------------
   *    [ 16 17 18 19 ]   Group 4
   *    ---------------------------
   *    [ 20 21 22 23 ]   Group 5
   *
   *    -> We have 24/4 = 6 groups, group stride = 4
   *    -> Within each group: 1 basic topology, basic topology stride = 1
   *
   * (2) Constructing the 2nd dim: we should create 4x2 = 8 basic topologies
   *
   *    [ 0 4 8 ]         Group 0
   *    [ 1 5 9 ]           - Group Base idx: 0
   *    [ 2 6 10 ]          - Within a group, 4 basic topologies
   *    [ 3 7 11 ]              Whose stride is 4.
   *    ---------------------------------------------------------
   *    [ 12 16 20 ]       Group 1
   *    [ 13 17 21 ]        - Group Base idx: 12
   *    [ 14 18 22 ]
   *    [ 15 19 23 ]
   *
   *    -> We have 6/3 = 2 groups, group stride = 4*3 = 12
   *    -> Within each group: 1*4 = 4 basic topology,
   *                          basic topology stride = 1*4 = 4
   *
   * (3) Constructing the 3rd dim: we should create 4x3 = 12 basic topologies
   *     [ 0 12 ]
   *     [ 1 13 ]
   *     [ 2 14 ]
   *     [ 3 15 ]          All in Group 0
   *     [ 4 16 ]
   *     [ 5 17 ]
   *     ...
   *     [ 10 22 ]
   *     [ 11 23 ]
   *     -------------------------------------------
   *
   *    -> We have 2/2 = 1 groups, group stride = 12*2 = 24 (don't care)
   *    -> Within each group: 4*3 = 12 basic topology,
   *                          basic topology stride = 4*3 = 12
   *
   * We therefore set
   *    <initial>
   *        basic_topology_stride = 1
   *        basic_topologies_count = 1
   *        group_stride = 1
   *        groups_count = npus_count
   *    <update before processing>
   *        group_stride *= npus_count[dim]
   *        groups_count /= npus_count[dim]
   *    <update after processing>
   *        basic_topology_stride *= npus_count[dim]
   *        basic_topologies_count *= npus_count[dim]
   *
   * Which are the function parameters below.
   *
   * @param dim dimension to construct
   * @param basic_topology_stride stride info
   * @param basic_topologies_count repetition info
   * @param group_stride stride info
   * @param groups_count repetition info
   */
  void construct_dimension(
      int dim,
      int* basic_topology_stride,
      int* basic_topologies_count,
      int* group_stride,
      int* groups_count) noexcept;

  // TODO: comment
  [[nodiscard]] MultiDimAddress translate_id_to_address(
      DeviceId npu_id) const noexcept;

  // TODO: comment
  [[nodiscard]] DeviceId translate_address_to_id(
      const MultiDimAddress& address) const noexcept;

  // TODO: comment
  void initialize_basic_topology_map() noexcept;
};

} // namespace NetworkAnalyticalCongestionAware
