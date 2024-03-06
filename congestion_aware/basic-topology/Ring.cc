/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "congestion_aware/Ring.hh"
#include <cassert>
#include <optional>

using namespace NetworkAnalyticalCongestionAware;

Ring::Ring(
    const int npus_count,
    const Bandwidth bandwidth,
    const Latency latency,
    const bool bidirectional) noexcept
    : bidirectional(bidirectional),
      BasicTopology(npus_count, npus_count, bandwidth, latency) {
  assert(npus_count > 0);
  assert(bandwidth > 0);
  assert(latency >= 0);

  // set topology type
  set_basic_topology_type(TopologyBuildingBlock::Ring);

  // construct connectivity
  construct_connections();
}

Ring::Ring(
    const Devices& npus,
    const Bandwidth bandwidth,
    const Latency latency,
    const bool bidirectional) noexcept
    : bidirectional(bidirectional),
      BasicTopology(npus, std::nullopt, bandwidth, latency) {
  assert(!npus.empty());
  assert(bandwidth > 0);
  assert(latency >= 0);

  // set topology type
  set_basic_topology_type(TopologyBuildingBlock::Ring);

  // construct connectivity
  construct_connections();
}

Route Ring::route(const DeviceId src, const DeviceId dest) const noexcept {
  // assert npus are in valid range
  assert(0 <= src && src < npus_count);
  assert(0 <= dest && dest < npus_count);

  // construct empty route
  auto route = Route();

  auto step = 1; // default direction: clockwise
  if (bidirectional) {
    // check whether going anticlockwise is shorter
    auto clockwise_dist = dest - src;
    if (clockwise_dist < 0) {
      clockwise_dist += npus_count;
    }
    const auto anticlockwise_dist = npus_count - clockwise_dist;

    if (anticlockwise_dist < clockwise_dist) {
      // traverse the ring anticlockwise
      step = -1;
    }
  }

  // construct the route
  auto current = src;
  while (current != dest) {
    // traverse the ring until reaches dest
    route.push_back(devices[current]);
    current = (current + step);

    // wrap around
    if (current < 0) {
      current += npus_count;
    } else if (current >= npus_count) {
      current -= npus_count;
    }
  }

  // arrives at dest
  route.push_back(devices[dest]);

  // return the constructed route
  return route;
}

void Ring::construct_connections() noexcept {
  // connect npus in a ring
  for (auto i = 0; i < npus_count - 1; i++) {
    const auto src = devices[i];
    const auto dest = devices[i + 1];
    connect(src, dest, bandwidth, latency, bidirectional);
  }

  // append the last connection
  // but this should be skipped when
  //    (1) the ring size is 2 and
  //    (2) connectivity is bidirectional
  // cause for this scenario, the returning connectivity has been
  // already established by the above statement.

  if (npus_count == 2 && bidirectional) {
    // skip for such scenario
    return;
  }

  // connect returning connection
  const auto src = devices[npus_count - 1];
  const auto dest = devices[0];
  connect(src, dest, bandwidth, latency, bidirectional);
}
