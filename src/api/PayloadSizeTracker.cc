/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "PayloadSizeTracker.hh"
#include <iostream>

using namespace Analytical;

PayloadSizeTracker::PayloadSizeTracker(int dims_count) noexcept
    : dims_count(dims_count) {
  payload_size_sent_through_dim = std::vector<int>(dims_count, 0);
}

void PayloadSizeTracker::addPayloadSize(int payload_size, int dim) noexcept {
  payload_size_sent_through_dim[dim] += payload_size;
}

int PayloadSizeTracker::payloadSizeThroughDim(int dim) const noexcept {
  return payload_size_sent_through_dim[dim];
}

int PayloadSizeTracker::totalPayloadSize() const noexcept {
  auto total_payload = 0;
  for (const auto payload : payload_size_sent_through_dim) {
    total_payload += payload;
  }
  return total_payload;
}
