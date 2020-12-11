/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#ifndef __DETAILEDALLTOALL_HH__
#define __DETAILEDALLTOALL_HH__

#include "DetailedTopology.hh"

namespace Analytical {
class DetailedAllToAll : public DetailedTopology {
 public:
  DetailedAllToAll(TopologyConfigs configs, CostModel& cost_model) noexcept;

  ~DetailedAllToAll() noexcept override;

  double send(NpuId src, NpuId dest, PayloadSize payload_size) noexcept
      override;
};
} // namespace Analytical

#endif
