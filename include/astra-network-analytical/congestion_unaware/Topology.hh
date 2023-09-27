/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#pragma once

#include <vector>
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

  [[nodiscard]] int get_dims_count() const noexcept;

  [[nodiscard]] std::vector<int> get_npus_count_per_dim() const noexcept;

 protected:
  /// number of NPUs
  int npus_count;

  /// number of network dimensions
  int dims_count;

  /// number of NPUs per dimension
  std::vector<int> npus_count_per_dim;
};

} // namespace NetworkAnalyticalCongestionUnaware
