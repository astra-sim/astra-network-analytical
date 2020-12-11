/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#ifndef __COSTMODEL_HH__
#define __COSTMODEL_HH__

#include <map>
#include <tuple>
#include "TopologyConfig.hh"

namespace Analytical {
class CostModel {
 public:
  using Latency = TopologyConfig::Latency;
  using Bandwidth = TopologyConfig::Bandwidth;
  using Radix = int;

  CostModel() noexcept;

  void createLink(int count, Latency latency, Bandwidth bandwidth) noexcept;
  void createNic(int count, Latency latency, Bandwidth bandwidth) noexcept;
  void createNpu(int count, Radix radix) noexcept;
  void createSwitch(int count, Radix radix) noexcept;

  void computeCost() const noexcept;

 private:
  std::map<std::tuple<Latency, Bandwidth>, int> link_map;
  std::map<std::tuple<Latency, Bandwidth>, int> nic_map;
  std::map<Radix, int> npu_map;
  std::map<Radix, int> switch_map;
};
} // namespace Analytical

#endif
