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

#ifndef __RING_ALLTOALL_SWITCH_H__
#define __RING_ALLTOALL_SWITCH_H__

#include <vector>
#include "Topology.hh"
#include "TopologyConfiguration.hh"

namespace Analytical {
    class Ring_AllToAll_Switch : public Topology {
    public:
        Ring_AllToAll_Switch(std::vector<TopologyConfiguration> configurations, std::vector<int> nodes_per_dims) noexcept;

        Latency simulateSend(NodeID src, NodeID dest, PayloadSize payload_size) noexcept override;

    private:
        std::vector<TopologyConfiguration> configurations;
        std::vector<int> nodes_per_dims;

        int tiles_per_gpu;
        int half_tiles_per_gpu;
        int tiles_per_node;
    };
}


#endif
