/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#ifndef __DETAILEDRING_HH__
#define __DETAILEDRING_HH__

#include "DetailedTopology.hh"

namespace Analytical {
class DetailedRing : public DetailedTopology {
 public:
  DetailedRing(TopologyConfigs& configs) noexcept;

  ~DetailedRing() noexcept override;

  double send(NpuId src, NpuId dest, PayloadSize payload_size) noexcept
      override;

 private:
  using Direction = int;

  Direction computeDirection(NpuId src, NpuId dest);
  NpuId takeStep(NpuId current_node, Direction direction);
};
} // namespace Analytical

#endif
