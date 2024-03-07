/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "congestion_unaware/MultiDimTopology.hh"
#include <cassert>
#include <cstdlib>
#include <iostream>
#include "common/NetworkFunction.hh"

using namespace NetworkAnalytical;
using namespace NetworkAnalyticalCongestionUnaware;

MultiDimTopology::MultiDimTopology() noexcept : Topology() {
  // initialize values
  topology_per_dim.clear();
  npus_count_per_dim = {};

  // initialize topology shape
  npus_count = 1;
  dims_count = 0;
}

EventTime MultiDimTopology::send(
    const DeviceId src,
    const DeviceId dest,
    const ChunkSize chunk_size) const noexcept {
  assert(0 <= src && src < npus_count);
  assert(0 <= dest && dest < npus_count);
  assert(chunk_size > 0);

  // translate src and dest to multi-dim address
  const auto src_address = translate_id_to_address(src, npus_count_per_dim);
  const auto dest_address = translate_id_to_address(dest, npus_count_per_dim);

  // get dim to transfer
  const auto dim_to_transfer = get_dim_to_transfer(src_address, dest_address);

  // prepare localized topology and address info
  auto* const topology = topology_per_dim[dim_to_transfer].get();
  const auto src_local_id = src_address[dim_to_transfer];
  const auto dest_local_id = dest_address[dim_to_transfer];

  // run localized communication
  const auto comms_delay =
      topology->send(src_local_id, dest_local_id, chunk_size);

  // return communication delay
  return comms_delay;
}

void MultiDimTopology::append_dimension(
    std::unique_ptr<BasicTopology> topology) noexcept {
  assert(topology.get() != nullptr);

  // increment dims_count
  dims_count++;

  // increase npus_count
  const auto topology_size = topology->get_npus_count();
  npus_count *= topology_size;

  // append bandwidth
  const auto bandwidth = topology->get_bandwidth_per_dim()[0];
  bandwidth_per_dim.push_back(bandwidth);

  // push back topology and npus_count
  topology_per_dim.push_back(std::move(topology));
  npus_count_per_dim.push_back(topology_size);
}

int MultiDimTopology::get_dim_to_transfer(
    const MultiDimAddress& src_address,
    const MultiDimAddress& dest_address) const noexcept {
  assert(!src_address.empty());
  assert(!dest_address.empty());
  assert(src_address.size() == dest_address.size());

  for (auto dim = 0; dim < dims_count; dim++) {
    // check the dim that has different address
    if (src_address[dim] != dest_address[dim]) {
      return dim;
    }
  }

  // shouldn't reach here
  std::cerr << "[Error] (network/analytical/congestion_unaware): "
            << "src and dest have the same address" << std::endl;
  std::exit(-1);
}
