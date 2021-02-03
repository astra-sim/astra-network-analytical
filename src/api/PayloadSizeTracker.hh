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

        int payloadSizeThroughDim(int dim) const noexcept;

        int totalPayloadSize() const noexcept;

    private:
        int dims_count;
        std::vector<int> payload_size_sent_through_dim;
    };
}

#endif
