/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#ifndef __DETAILEDTORUS2D_HH__
#define __DETAILEDTORUS2D_HH__

#include "DetailedTopology.hh"

namespace Analytical {
class DetailedTorus2D : public DetailedTopology {
 public:
  DetailedTorus2D(TopologyConfigs& configs) noexcept;

  ~DetailedTorus2D() noexcept override;

  std::pair<double, int> send(NpuId src, NpuId dest, PayloadSize payload_size) noexcept
      override;

 private:
  using Direction = int;

  int width;
  int height;

  NpuAddress npuIdToAddress(NpuId npu_id) const noexcept override;
  NpuId npuAddressToId(NpuAddress npu_address) const noexcept override;

  Direction computeDirection(NpuId src, NpuId dest, int ring_size);
  NpuId takeStep(NpuId current_node, Direction direction, int ring_size);
};
} // namespace Analytical

#endif
