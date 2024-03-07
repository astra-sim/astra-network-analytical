/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "common/NetworkFunction.hh"
#include <cassert>
#include <iostream>

using namespace NetworkAnalytical;

Bandwidth NetworkAnalytical::bw_GBps_to_Bpns(const Bandwidth bw_GBps) noexcept {
  assert(bw_GBps > 0);

  // 1 GB is 2^30 B
  // 1 s is 10^9 ns
  return bw_GBps * (1 << 30) / (1'000'000'000); // GB/s to B/ns
}

MultiDimAddress NetworkAnalytical::translate_id_to_address(
    const DeviceId npu_id,
    const std::vector<int>& npus_count_per_dim) noexcept {
  // If units-count if [2, 8, 4], and the given id is 47, then the id should be
  // 47 // 16 = 2, leftover = 47 % 16 = 15
  // 15 // 2 = 7, leftover = 15 % 2 = 1
  // 1 // 1 = 1, leftover = 0
  // therefore the address is [1, 7, 2]

  // number of network dims
  const auto dims_count = static_cast<int>(npus_count_per_dim.size());

  // number of NPUs
  auto npus_count = 1;
  for (const auto npus_count_dim : npus_count_per_dim) {
    npus_count *= npus_count_dim;
  }

  // create empty address
  auto multi_dim_address = MultiDimAddress();
  for (auto i = 0; i < dims_count; i++) {
    multi_dim_address.push_back(-1);
  }

  auto leftover = npu_id;
  auto denominator = npus_count;

  for (auto dim = dims_count - 1; dim >= 0; dim--) {
    // change denominator
    denominator /= npus_count_per_dim[dim];

    // get and update address
    const auto quotient = leftover / denominator;
    leftover %= denominator;

    // update address
    multi_dim_address[dim] = quotient;
  }

  // check address translation
#ifndef NDEBUG
  for (auto i = 0; i < dims_count; i++) {
    assert(0 <= multi_dim_address[i]);
    assert(multi_dim_address[i] < npus_count_per_dim[i]);
  }
#endif

  // return retrieved address
  return multi_dim_address;
}

DeviceId NetworkAnalytical::translate_address_to_id(
    const MultiDimAddress& address,
    const std::vector<int>& npus_count_per_dim) noexcept {
  // If units-count if [2, 8, 4], and the given address is [1, 7, 2],
  // then the id should be
  //       ('1' * 1) --> 1
  //   1 + ('2' * 7) --> 15
  //   15 + ('16' * 2) --> 47
  // ID = 47.

  // number of network dims
  const auto dims_count = static_cast<int>(npus_count_per_dim.size());

  // NPUs up to the current dimension
  auto scalar = 1;

  // device id to compute and return
  auto id = 0;

  // translate address to device id
  for (auto dim = 0; dim < dims_count; dim++) {
    id += (scalar * address[dim]);
    scalar *= npus_count_per_dim[dim];
  }

#ifndef NDEBUG
  // check the validity
  auto npus_count = 1;
  for (const auto npus_count_dim : npus_count_per_dim) {
    npus_count *= npus_count_dim;
  }

  assert(0 <= id && id < npus_count);
#endif

  // return device id
  return id;
}
