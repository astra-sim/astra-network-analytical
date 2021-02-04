/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#ifndef __DETAILEDSWITCH_HH__
#define __DETAILEDSWITCH_HH__

#include "DetailedTopology.hh"

namespace Analytical {
class DetailedSwitch : public DetailedTopology {
 public:
  DetailedSwitch(TopologyConfigs& configs) noexcept;

  ~DetailedSwitch() noexcept override;

  std::pair<double, int> send(
      NpuId src,
      NpuId dest,
      PayloadSize payload_size) noexcept override;

 private:
  NpuId switch_id;
};
} // namespace Analytical

#endif
