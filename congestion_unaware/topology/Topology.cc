/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "congestion_unaware/topology/Topology.hh"

using namespace NetworkAnalytical;
using namespace NetworkAnalyticalCongestionUnaware;

Topology::Topology() noexcept : npus_count(-1) {}

int Topology::get_npus_count() const noexcept {
  assert(npus_count > 0);

  // return npus_count
  return npus_count;
}
