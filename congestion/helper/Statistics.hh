/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#pragma once

#include <list>
#include <map>
#include <tuple>
#include "type/Type.hh"

namespace Congestion {
/// Declare a global structure to track the link activity

class Statistics {
 public:
  Statistics();
  ~Statistics();

  void recordEntry(LinkId, std::tuple<Time, Time> entry);
  int report();

 private:
  static std::map<LinkId, std::list<std::tuple<Time, Time>>> link_activity;
};
}
