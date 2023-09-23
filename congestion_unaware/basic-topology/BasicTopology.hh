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
  BasicTopology(int nodes_count, Bandwidth bandwidth, Latency latency) noexcept;

  [[nodiscard]] EventTime send(NodeId src, NodeId dest, ChunkSize size)
      const noexcept override;

 protected:
  [[nodiscard]] virtual int compute_hops_count(NodeId src, NodeId dest)
      const noexcept = 0;

 private:
  [[nodiscard]] static Bandwidth bw_GBps_to_Bpns(Bandwidth bw_GBps) noexcept;

  [[nodiscard]] EventTime compute_communication_delay(
      int hops_count,
      ChunkSize size) const noexcept;

  Bandwidth bandwidth;
  Latency latency;
};

} // namespace NetworkAnalyticalCongestionUnaware
