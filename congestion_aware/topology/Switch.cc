/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "Switch.hh"

using namespace Congestion;

Switch::Switch(int npus_count, Bandwidth bandwidth, Latency latency) noexcept
    : switch_id(npus_count), Topology(npus_count + 1) {
  // e.g., if npus_count=8, then
  // there are total 9 nodes, where ordinary npus are 0-7, and switch is 8

  // assert npus_count is valid
  assert(npus_count > 0);

  // assert bandwidth and latency are valid
  assert(bandwidth > 0);
  assert(latency >= 0);

  // connect npus and switches, the link should be bidirectional
  for (auto i = 0; i < npus_count; i++) {
    connect(i, switch_id, bandwidth, latency, true);
  }
}

Switch::~Switch() noexcept = default;

Route Switch::route(NodeId src, NodeId dest) const noexcept {
  // assert npus are in valid range
  assert(0 <= src && src < npus_count);
  assert(0 <= dest && dest < npus_count);

  // construct route
  // start at source, and go to switch, then go to destination
  auto route = Route();
  route.push_back(npus[src]);
  route.push_back(npus[switch_id]);
  route.push_back(npus[dest]);

  return route;
}
