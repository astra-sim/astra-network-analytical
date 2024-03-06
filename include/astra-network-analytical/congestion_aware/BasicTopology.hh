/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#pragma once

#include <optional>
#include "common/Type.hh"
#include "congestion_aware/Topology.hh"

using namespace NetworkAnalytical;

namespace NetworkAnalyticalCongestionAware {

/**
 * BasicTopology defines 1D topology
 * such as Ring, FullyConnected, and Switch topology,
 * which can be used to construct multi-dimensional topology.
 */
class BasicTopology : public Topology {
 public:
  /**
   * Constructor, when we know the number of NPUs
   * and would like to newly create them.
   *
   * @param npus_count number of NPUs in the topology
   * @param devices_count number of devices in the topology
   * @param bandwidth bandwidth of each link
   * @param latency latency of each link
   */
  BasicTopology(
      int npus_count,
      int devices_count,
      Bandwidth bandwidth,
      Latency latency) noexcept;

  /**
   * Constructor, when we want to add new connectivity
   * among already constructed NPUs.
   *
   * @param npus vector of NPU pointers of the target network
   * @param devices vector of (additional) non-NPU devices (e.g., switch), or
   * nullptr
   * @param bandwidth bandwidth of each link
   * @param latency latency of each link
   */
  BasicTopology(
      const Devices& npus,
      std::optional<Devices> devices,
      Bandwidth bandwidth,
      Latency latency) noexcept;

  /**
   * Destructor.
   */
  virtual ~BasicTopology() noexcept;

  /**
   * Return the type of the basic topology
   * as a TopologyBuildingBlock enum class element.
   *
   * @return type of the basic topology
   */
  [[nodiscard]] TopologyBuildingBlock get_basic_topology_type() const noexcept;

 protected:
  /// bandwidth of each link
  Bandwidth bandwidth;

  /// latency of each link
  Latency latency;

  /// type of the basic topology
  TopologyBuildingBlock basic_topology_type;

  /**
   * Establish NPU connectivity depending on each topology shape.
   * Should be called by the child class constructor.
   */
  virtual void construct_connections() noexcept = 0;

  /**
   * Set the basic topology type.
   *
   * @param topology topology type
   */
  void set_basic_topology_type(TopologyBuildingBlock topology) noexcept;

 private:
  void initialize_values() noexcept;
};

} // namespace NetworkAnalyticalCongestionAware
