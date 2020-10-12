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

#ifndef __SWITCH_HH__
#define __SWITCH_HH__

#include <vector>
#include "Topology.hh"
#include "TopologyConfiguration.hh"

namespace Analytical {
    class Switch : public Topology {
    public:
        /**
         * Constrct a switch.
         * @param configurations configuration for each dimensino
         *              - Simple switch has only 1 dim
         * @param npus_count number of npus connected to the switch
         */
        Switch(const std::vector<TopologyConfiguration>& configurations, int npus_count) noexcept;

        Latency send(NpuId src_id, NpuId dest_id, PayloadSize payload_size) noexcept override;

    private:
        NpuAddress npuIdToAddress(NpuId id) const noexcept override;
        NpuId npuAddressToId(const NpuAddress& address) const noexcept override;

        int switch_id;  // id of the switch node
    };
}

#endif
