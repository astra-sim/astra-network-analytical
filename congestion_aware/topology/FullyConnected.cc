/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "FullyConnected.hh"

using namespace Congestion;

FullyConnected::FullyConnected(
    int npus_count,
    Bandwidth bandwidth,
    Latency latency) noexcept
    : Topology(npus_count) {
  // assert npus_count is valid
  assert(npus_count > 0);

  // assert bandwidth and latency are valid
  assert(bandwidth > 0);
  assert(latency >= 0);

  // fully-connect every src-dest pairs
  for (auto src = 0; src < npus_count; src++) {
    for (auto dest = 0; dest < npus_count; dest++) {
      if (src != dest) {
        connect(src, dest, bandwidth, latency, false);
      }
    }
  }
}

FullyConnected::~FullyConnected() noexcept = default;

Route FullyConnected::route(NodeId src, NodeId dest) const noexcept {
  // assert npus are in valid range
  assert(0 <= src && src < npus_count);
  assert(0 <= dest && dest < npus_count);

  // construct route
  // directly connected
  auto route = Route();
  route.push_back(npus[src]);
  route.push_back(npus[dest]);

  return route;
}
