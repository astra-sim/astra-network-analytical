/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#ifndef __LINK_HH__
#define __LINK_HH__

#include "TopologyConfig.hh"

namespace Analytical {
    class Link {
    public:
        using Latency = TopologyConfig::Latency;
        using PayloadSize = TopologyConfig::PayloadSize;

        Link(Latency link_latency) noexcept;

        Latency transmit(PayloadSize payload_size) noexcept;

    private:
        int sent_packets_count;
        int total_payload_size;
        Latency link_latency;
    };
}

#endif
