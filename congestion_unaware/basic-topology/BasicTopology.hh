/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#pragma once

#include "common/Common.hh"
#include "common/Type.hh"
#include "congestion_unaware/topology/Topology.hh"

using namespace NetworkAnalytical;

namespace NetworkAnalyticalCongestionUnaware {

class BasicTopology : public Topology {
 public:
  BasicTopology(int npus_count, Bandwidth bandwidth, Latency latency) noexcept;

  virtual ~BasicTopology() noexcept;

  [[nodiscard]] EventTime send(
      DeviceId src,
      DeviceId dest,
      ChunkSize chunk_size) const noexcept override;

  [[nodiscard]] TopologyBuildingBlock get_basic_topology_type() const noexcept;

 protected:
  [[nodiscard]] virtual int compute_hops_count(DeviceId src, DeviceId dest)
      const noexcept = 0;

  TopologyBuildingBlock basic_topology_type;

 private:
  [[nodiscard]] static Bandwidth bw_GBps_to_Bpns(Bandwidth bw_GBps) noexcept;

  [[nodiscard]] EventTime compute_communication_delay(
      int hops_count,
      ChunkSize size) const noexcept;

  Bandwidth bandwidth;
  Latency latency;
};

} // namespace NetworkAnalyticalCongestionUnaware
