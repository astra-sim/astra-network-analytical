/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "Link.hh"
#include <cassert>

using namespace Analytical;

Link::Link(Latency link_latency) noexcept:
        sent_packets_count(0),
        total_payload_size(0),
        link_latency(link_latency) { }

Link::Latency Link::transmit(PayloadSize payload_size) noexcept {
    // update stats
    sent_packets_count++;
    total_payload_size += payload_size;

    // return link latency
    return link_latency;
}
