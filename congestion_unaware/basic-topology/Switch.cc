/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "Switch.hh"

using namespace NetworkAnalytical;
using namespace NetworkAnalyticalCongestionUnaware;

Switch::Switch(int nodes_count, Bandwidth bandwidth, Latency latency) noexcept
    : BasicTopology(nodes_count, bandwidth, latency) {
  assert(nodes_count > 0);
  assert(bandwidth > 0);
  assert(latency >= 0);
}

int Switch::compute_hops_count(NodeId src, NodeId dest) const noexcept {
  assert(0 <= src && src < nodes_count);
  assert(0 <= dest && dest < nodes_count);
  assert(src != dest);

  // for switch, hops_count is always 2 (src - switch - dest)
  return 2;
}