/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#pragma once

#include "common/Common.hh"

namespace NetworkAnalyticalCongestionAware {

/// Declaring Network Components
class Chunk;
class Link;
class Node;

/// Node
using NodeId = int;

/// Route
using Route = std::list<std::shared_ptr<Node>>;

} // namespace NetworkAnalyticalCongestionAware
