/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "Ring.hh"

using namespace Congestion;

Ring::Ring(
    int npus_count,
    Bandwidth bandwidth,
    Latency latency,
    bool bidirectional) noexcept
    : bidirectional(bidirectional), Topology(npus_count) {
  // assert npus_count is valid
  assert(npus_count > 0);

  // assert bandwidth and latency are valid
  assert(bandwidth > 0);
  assert(latency >= 0);

  // connect npus in a ring
  for (auto i = 0; i < npus_count - 1; i++) {
    connect(i, i + 1, bandwidth, latency, bidirectional);
  }
  connect(npus_count - 1, 0, bandwidth, latency, bidirectional);
}

Ring::~Ring() noexcept = default;

Route Ring::route(NodeId src, NodeId dest) const noexcept {
  // assert npus are in valid range
  assert(0 <= src && src < npus_count);
  assert(0 <= dest && dest < npus_count);

  // construct route, starting with src
  auto route = Route();

  auto step = 1; // default: clockwise
  if (bidirectional) {
    // check whether going anticlockwise is shorter
    auto clockwise_dist = dest - src;
    if (clockwise_dist < 0) {
      clockwise_dist += npus_count;
    }
    auto anticlockwise_dist = npus_count - clockwise_dist;

    if (anticlockwise_dist < clockwise_dist) {
      // traverse the ring anticlockwise
      step = -1;
    }
  }

  // construct the route
  auto current = src;
  while (current != dest) {
    // traverse the ring until reaches dest
    route.push_back(npus[current]);
    current = (current + step);

    if (current < 0) {
      current += npus_count;
    } else if (current >= npus_count) {
      current -= npus_count;
    }
  }

  // arrives at dest
  route.push_back(npus[dest]);

  return route;
}
