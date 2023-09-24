/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "congestion_unaware/topology/Topology.hh"

using namespace NetworkAnalytical;
using namespace NetworkAnalyticalCongestionUnaware;

Topology::Topology() noexcept : nodes_count(-1) {}

void Topology::set_nodes_count(int new_nodes_count) noexcept {
  assert(new_nodes_count > 0);

  nodes_count = new_nodes_count;
}

int Topology::get_nodes_count() const noexcept {
  return nodes_count;
}
