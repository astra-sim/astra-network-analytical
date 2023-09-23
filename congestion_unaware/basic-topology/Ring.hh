/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#pragma once

#include "common/Common.hh"
#include "common/Type.hh"
#include "congestion_unaware/basic-topology/BasicTopology.hh"

using namespace NetworkAnalytical;

namespace NetworkAnalyticalCongestionUnaware {

class Ring final : public BasicTopology {
 public:
  explicit Ring(
      int nodes_count,
      Bandwidth bandwidth,
      Latency latency,
      bool bidirectional = true) noexcept;

  [[nodiscard]] int compute_hops_count(
      NodeId src,
      NodeId dest) const noexcept override;

 private:
  bool bidirectional;
};

} // namespace NetworkAnalyticalCongestionUnaware
