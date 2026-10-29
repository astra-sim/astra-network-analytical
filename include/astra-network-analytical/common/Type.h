// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2026 Advanced Micro Devices, Inc.

#pragma once

#include <cstdint>

namespace NetworkAnalytical {

/// Callback function pointer: "void func(void*)"
using Callback = void (*)(void*);

/// Callback function argument: void*
using CallbackArg = void*;

/// Device ID which starts from 0
using DeviceId = int;

/// Chunk size in Bytes
using ChunkSize = uint64_t;

/// Bandwidth in GiB/s
using Bandwidth = double;

/// Latency in ns
using Latency = double;

/// Event time in ps
using EventTime = int64_t;

/// System/GPU IDs
using Sys_ID = uint32_t;

/// CU
using CU_ID = uint32_t;

/// Request size
using Req_Size = uint64_t;

/// Byte offset into a buffer
using Buf_Offset = uint64_t;

/// Basic multi-dimensional topology building blocks
enum class TopologyBuildingBlock {
    Undefined,
    Custom,
    Ring,
    FullyConnected,
    Switch
};

}  // namespace NetworkAnalytical
