/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#pragma once

#include <cstdint>

namespace NetworkAnalytical {

/// Callback function type
using Callback = void (*)(void*);
using CallbackArg = void*;

/// Node
using DeviceId = int; // starting from 0

/// Chunk and route
using ChunkSize = uint64_t; // in Bytes

/// Network
using Bandwidth = double; // in GB/s
using Latency = double; // in ns

/// Event Time resolution
using EventTime = uint64_t; // in ns

/// Topology Building Blocks
enum class TopologyBuildingBlock { Undefined, Ring, FullyConnected, Switch };

} // namespace NetworkAnalytical
