/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#ifndef __FASTRING_ALLTOALL_SWITCH_HH__
#define __FASTRING_ALLTOALL_SWITCH_HH__

#include "../TopologyConfig.hh"
#include "FastTopology.hh"

namespace Analytical {
class FastRing_AllToAll_Switch : public FastTopology {
 public:
  FastRing_AllToAll_Switch(
    TopologyConfigs& configs,
    CostModel& cost_model) noexcept;

  ~FastRing_AllToAll_Switch() noexcept;

  double send(NpuId src, NpuId dest, PayloadSize payload_size) noexcept
      override;

 private:
  NpuAddress npuIdToAddress(NpuId npu_id) const noexcept override;
  NpuId npuAddressToId(NpuAddress npu_address) const noexcept override;
};
} // namespace Analytical

#endif
