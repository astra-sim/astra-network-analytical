/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#pragma once

#include "common/Type.hh"
#include "congestion_aware/Topology.hh"

using namespace NetworkAnalytical;

namespace NetworkAnalyticalCongestionAware {

class BasicTopology : public Topology {
 public:
  BasicTopology(
      int npus_count,
      int devices_count,
      Bandwidth bandwidth,
      Latency latency) noexcept;

  virtual ~BasicTopology() noexcept;

  [[nodiscard]] TopologyBuildingBlock get_basic_topology_type() const noexcept;

 protected:
  Bandwidth bandwidth;
  Latency latency;

  TopologyBuildingBlock basic_topology_type;
};

} // namespace NetworkAnalyticalCongestionAware
