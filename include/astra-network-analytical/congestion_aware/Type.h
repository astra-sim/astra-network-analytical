// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2026 Advanced Micro Devices, Inc.

#pragma once

#include <list>
#include <memory>

namespace NetworkAnalyticalCongestionAware {

/// Forward declarations of network components
class Chunk;
class Link;
class Device;

/// Route is a list of devices
using Route = std::list<std::shared_ptr<Device>>;

}  // namespace NetworkAnalyticalCongestionAware
