// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2026 Advanced Micro Devices, Inc.

#pragma once

#include <astra-network-analytical/common/BaseTopology.h>
#include <astra-network-analytical/common/Type.h>

#include <algorithm>
#include <list>
#include <map>
#include <memory>
#include <vector>

using namespace NetworkAnalytical;

namespace NetworkAnalytical {

/**
 * Abstracts a network topology.
 */
class Topology : public BaseTopology {
  public:
    /**
     * Constructor
     */
    Topology() noexcept;
    /**
     * Estimate the time to be taken to transmit a chunk of size chunk_size
     * from src NPU to dest NPU.
     *
     * @param src src NPU ID
     * @param dest dest NPU ID
     * @param chunk_size size of the chunk to send
     * @return time to send the chunk from src to dest
     */
    [[nodiscard]] virtual EventTime send(
        DeviceId src, DeviceId dest, ChunkSize chunk_size) const noexcept = 0;

    [[nodiscard]] virtual EventTime receive(
        DeviceId src, DeviceId dest, ChunkSize chunk_size) const noexcept = 0;
};

}  // namespace NetworkAnalytical
