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

class Ring final : public Topology {
 public:
  explicit Ring(
      int nodes_count,
      Bandwidth bandwidth,
      Latency latency,
      bool bidirectional = true) noexcept;

  [[nodiscard]] EventTime send(NodeId src, NodeId dest, ChunkSize size)
      const noexcept override;

 private:
  Bandwidth bandwidth;
  Latency latency;
  bool bidirectional;
};

} // namespace NetworkAnalyticalCongestionUnaware
