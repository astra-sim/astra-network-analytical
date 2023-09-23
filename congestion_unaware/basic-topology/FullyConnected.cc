/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "FullyConnected.hh"

using namespace NetworkAnalytical;
using namespace NetworkAnalyticalCongestionUnaware;

FullyConnected::FullyConnected(
    int nodes_count,
    Bandwidth bandwidth,
    Latency latency) noexcept
    : BasicTopology(nodes_count, bandwidth, latency) {
  assert(nodes_count > 0);
  assert(bandwidth > 0);
  assert(latency >= 0);
}

int FullyConnected::compute_hops_count(NodeId src, NodeId dest) const noexcept {
  assert(0 <= src && src < nodes_count);
  assert(0 <= dest && dest < nodes_count);
  assert(src != dest);

  // for FullyConnected, hops_count is always 1 (src - dest)
  return 1;
}