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

#ifndef __RING_HH__
#define __RING_HH__

#include <vector>
#include "Topology.hh"
#include "TopologyConfiguration.hh"

namespace Analytical {
    class Ring : public Topology {
    public:
        Ring(const TopologyConfigurations& configurations, int npus_count, bool bidirectional=true) noexcept;

        Latency send(NpuId src_id, NpuId dest_id, PayloadSize payload_size) noexcept override;

    private:
        using Direction = int;  // +1 or -1
        NpuAddress npuIdToAddress(NpuId id) const noexcept override;
        NpuId npuAddressToId(const NpuAddress& address) const noexcept override;

        int npus_count;
        int half_npus_count;
        bool bidirectional;  // whether ring is bidirectional or unidirectional

        Direction computeDirection(NpuId src_id, NpuId dest_id) const noexcept;
        NpuId takeStep(NpuId current_id, Direction direction) const noexcept;
    };
}

#endif
