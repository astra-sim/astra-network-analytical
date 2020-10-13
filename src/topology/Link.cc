/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "Link.hh"
#include <cassert>

using namespace Analytical;

Link::Link(Latency link_latency) noexcept : link_latency(link_latency) {}

Link::Link() noexcept : Link(-1) {}

Link::Latency Link::send(PayloadSize payload_size) noexcept {
  assert(
      link_latency > 0 &&
      "[Link, method send] link latency is less than zero. Default constructor may be accidentally triggered somewhere.");

  // update stats
  served_payloads_count++;
  served_payloads_size += payload_size;
  total_latency += link_latency;

  return link_latency;
}
