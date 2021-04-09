/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#ifndef __PAYLOADSIZETRACKER_HH__
#define __PAYLOADSIZETRACKER_HH__

#include <vector>

namespace Analytical {
class PayloadSizeTracker {
 public:
  PayloadSizeTracker(int dims_count) noexcept;

  void addPayloadSize(int payload_size, int dim) noexcept;

  double payloadSizeThroughDim(int dim) const noexcept;

  double totalPayloadSize() const noexcept;

 private:
  int dims_count;
  std::vector<double> payload_size_sent_through_dim;
};
} // namespace Analytical

#endif
