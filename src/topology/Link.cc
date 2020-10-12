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

#include "Link.hh"

using namespace Analytical;

Link::Link(Latency link_latency, Bandwidth link_bandwidth) noexcept :
    link_latency(link_latency),
    link_bandwidth(link_bandwidth) { }

Link::Latency Link::send(PayloadSize payload_size) noexcept {
    // compute transmission latency
    auto latency = (payload_size / link_bandwidth) + link_latency;

    // update stats
    served_payloads_count++;
    served_payloads_size += payload_size;
    total_latency += latency;

    return latency;
}
