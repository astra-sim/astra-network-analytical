/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#ifndef __FASTTORUS2D_HH__
#define __FASTTORUS2D_HH__

#include "../TopologyConfig.hh"
#include "FastTopology.hh"

namespace Analytical {
class FastTorus2D : public FastTopology {
 public:
  FastTorus2D(TopologyConfigs& configs, CostModel& cost_model) noexcept;

  ~FastTorus2D() noexcept;

  double send(NpuId src, NpuId dest, PayloadSize payload_size) noexcept
      override;

 private:
  NpuAddress npuIdToAddress(NpuId npu_id) const noexcept override;
  NpuId npuAddressToId(NpuAddress npu_address) const noexcept override;
};
} // namespace Analytical

#endif
