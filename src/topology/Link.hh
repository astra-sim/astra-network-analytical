/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#ifndef __LINK_HH__
#define __LINK_HH__

#include "TopologyConfiguration.hh"

namespace Analytical {
class Link {
 public:
  using Latency = TopologyConfiguration::Latency;
  using Bandwidth = TopologyConfiguration::Bandwidth;
  using PayloadSize = TopologyConfiguration::PayloadSize;

  /**
   * Construct new link.
   *
   * @param configuration Link configuration
   */
  explicit Link(Latency link_latency) noexcept;

  Link() noexcept; // default constructor -- should not be called explicitly

  /**
   * Send a payload of given size and update link stats.
   * Return the latency of transmission.
   *
   * @return latency for this transmission
   */
  Latency send(PayloadSize payload_size) noexcept;

 private:
  Latency link_latency;

  int served_payloads_count; // the number of served payloads
  PayloadSize served_payloads_size; // summation of payloads' size which passed
                                    // this link
  Latency total_latency; // summation of total latency of each send
};
} // namespace Analytical

#endif
