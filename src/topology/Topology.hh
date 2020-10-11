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

#ifndef _TOPOLOGY_HH__
#define _TOPOLOGY_HH__


namespace AnalyticalBackend {
    class Topology {
    public:
        /**
         * NodeID: each node's ID in integer (e.g., 16, 27, 31)
         */
        using NodeID = int;

        /**
         * Payload size is expressed in `Bytes` using `int`
         */
        using PayloadSize = int;

        /**
         * Latency uses the datatype `double`
         */
        using Latency = double;

        virtual ~Topology() noexcept = 0;

        /**
         * Simulate network packet sending from src to dest nodes.
         *
         * @param src source NodeID of packet
         * @param dest destination's NodeID
         * @param payload_size payload size in Bytes
         * @return latency of the communication from src to dest
         */
        virtual Latency simulateSend(NodeID src, NodeID dest, PayloadSize payload_size) noexcept = 0;
    };
}


#endif
