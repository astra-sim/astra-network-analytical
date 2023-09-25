/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#pragma once

#include "common/Common.hh"
#include "common/Type.hh"

using namespace NetworkAnalytical;

namespace NetworkAnalyticalCongestionUnaware {

class Topology {
 public:
  Topology() noexcept;

  [[nodiscard]] virtual EventTime send(
      DeviceId src,
      DeviceId dest,
      ChunkSize chunk_size) const noexcept = 0;

  [[nodiscard]] int get_npus_count() const noexcept;

 protected:
  int npus_count;
};

} // namespace NetworkAnalyticalCongestionUnaware
