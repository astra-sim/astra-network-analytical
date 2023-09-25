/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "MultiDimTopology.hh"

using namespace NetworkAnalytical;
using namespace NetworkAnalyticalCongestionUnaware;

MultiDimTopology::MultiDimTopology() noexcept : dims_count(0), Topology() {
  topologies_per_dim.clear();
  nodes_count_per_dim = {};

  // initialize nodes count
  set_nodes_count(1);
}

EventTime MultiDimTopology::send(NodeId src, NodeId dest, ChunkSize size)
    const noexcept {
  // translate src and dest to multi-dim address
  auto src_address = translate_address(src);
  auto dest_address = translate_address(dest);

  // get dim to transfer
  auto dim_to_transfer = get_dim_to_transfer(src_address, dest_address);

  // communication information
  auto* topology = topologies_per_dim[dim_to_transfer].get();
  auto src_local_id = src_address[dim_to_transfer];
  auto dest_local_id = dest_address[dim_to_transfer];

  // run communication
  auto comms_delay = topology->send(src_local_id, dest_local_id, size);

  // return communication delay
  return comms_delay;
}

void MultiDimTopology::add_dim(
    std::unique_ptr<BasicTopology> topology) noexcept {
  // increment dims_count
  dims_count++;

  // increase nodes_count
  auto topology_size = topology->get_nodes_count();
  nodes_count *= topology_size;

  // push back basic-topology and nodes_count
  topologies_per_dim.push_back(std::move(topology));
  nodes_count_per_dim.push_back(topology_size);
}

MultiDimTopology::MultiDimAddress MultiDimTopology::translate_address(
    NodeId node_id) const noexcept {
  // If units-count if [2, 8, 4], and the given id is 47, then the id should be
  // 47 // 16 = 2, leftover = 47 % 16 = 15
  // 15 // 2 = 7, leftover = 15 % 2 = 1
  // 1 // 1 = 1, leftover = 0
  // therefore the address is [1, 7, 2]

  // create empty address
  auto multi_dim_address = MultiDimAddress();
  for (auto i = 0; i < dims_count; i++) {
    multi_dim_address.push_back(-1);
  }

  auto leftover = node_id;
  auto denominator = nodes_count;

  for (auto dim = dims_count - 1; dim >= 0; dim--) {
    // change denominator
    denominator /= nodes_count_per_dim[dim];

    // get and update address
    auto quotient = leftover / denominator;
    leftover %= denominator;

    // update address
    multi_dim_address[dim] = quotient;
  }

  // check address translation
  for (auto i = 0; i < dims_count; i++) {
    assert(0 <= multi_dim_address[i]);
    assert(multi_dim_address[i] < nodes_count_per_dim[i]);
  }

  // return retrieved address
  return multi_dim_address;
}

int MultiDimTopology::get_dim_to_transfer(
    MultiDimAddress src_address,
    MultiDimAddress dest_address) const noexcept {
  auto dim_to_transfer = -1;
  auto found = false;

  for (auto dim = 0; dim < dims_count; dim++) {
    // check the dim that has different address
    if (src_address[dim] != dest_address[dim]) {
      // if found is true, this means
      // at least two dims have different address
      // which analytical model doesn't support
      if (found) {
        std::cerr
            << "[Error] (analytical/congestion_unaware) "
            << "Analytical model only supports transfers over a single network dimension"
            << std::endl;
        std::exit(-1);
      }

      dim_to_transfer = dim;
      found = true;
    }
  }

  // return found dim
  assert(found);
  assert(0 <= dim_to_transfer && dim_to_transfer < dims_count);

  return dim_to_transfer;
}
