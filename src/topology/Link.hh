/******************************************************************************
Copyright (c) 2020 Georgia Institute of Technology
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

Author : William Won (william.won@gatech.edu)
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
