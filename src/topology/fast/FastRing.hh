/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#ifndef __FASTRING_HH__
#define __FASTRING_HH__

#include "../TopologyConfig.hh"
#include "FastTopology.hh"

namespace Analytical {
class FastRing : public FastTopology {
 public:
  FastRing(TopologyConfigs& configs) noexcept;

  ~FastRing() noexcept;

  std::pair<double, int> send(
      NpuId src,
      NpuId dest,
      PayloadSize payload_size) noexcept override;
};
} // namespace Analytical

#endif
