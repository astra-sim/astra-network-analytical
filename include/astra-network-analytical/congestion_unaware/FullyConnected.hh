/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#pragma once

#include "common/Type.hh"
#include "congestion_unaware/BasicTopology.hh"

using namespace NetworkAnalytical;

namespace NetworkAnalyticalCongestionUnaware {

class FullyConnected final : public BasicTopology {
 public:
  FullyConnected(int npus_count, Bandwidth bandwidth, Latency latency) noexcept;

  [[nodiscard]] int compute_hops_count(DeviceId src, DeviceId dest)
      const noexcept override;
};

} // namespace NetworkAnalyticalCongestionUnaware
