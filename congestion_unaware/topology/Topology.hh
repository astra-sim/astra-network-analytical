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

  [[nodiscard]] virtual EventTime send(NodeId src, NodeId dest, ChunkSize size)
      const noexcept = 0;

  [[nodiscard]] int get_nodes_count() const noexcept;

 protected:
  int nodes_count;

  void set_nodes_count(int new_nodes_count) noexcept;
};

} // namespace NetworkAnalyticalCongestionUnaware
