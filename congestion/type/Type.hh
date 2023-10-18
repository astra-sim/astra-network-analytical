/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#pragma once

#include <list>
#include <memory>
#include <string>
#include<iostream>

namespace Congestion {

class Node;

/// Callback function type
using Callback = void (*)(void*);
using CallbackArg = void*;

/// Node
using NodeId = int;

/// Link
using LinkId = unsigned;

/// Chunk and route
using ChunkSize = double;
using Route = std::list<std::shared_ptr<Node>>;

/// Network
using Bandwidth = double;
using Latency = double;

/// Time configuration
using Time = double;


/// Link - information structure
struct link_info{
  LinkId linkId;
  NodeId srcId;
  NodeId destId;
  Bandwidth bandwidth;
  Latency latency;
  bool busy;
};


/// Parser
class HelpMessagePrinted : public std::exception {};

class OptNotSpecifiedError : public std::exception {
  /**
   * @brief Exception thrown when an command line option is not specified
   */
 public:
  explicit OptNotSpecifiedError(const std::string& opt_name) noexcept {
    msg = "Argument " + opt_name + " not specified.";
  }

  const char* what() const noexcept override {
    return msg.c_str();
  }

 private:
  std::string msg;
};

enum class OptType { Single, Multi };

} // namespace Congestion
