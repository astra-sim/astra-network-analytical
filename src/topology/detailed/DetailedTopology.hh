/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#ifndef __DETAILEDTOPOLOGY_HH__
#define __DETAILEDTOPOLOGY_HH__

#include <map>
#include "../Topology.hh"
#include "../TopologyConfig.hh"
#include "Link.hh"

namespace Analytical {
class DetailedTopology : public Topology {
 public:
  DetailedTopology(TopologyConfigs& configs, CostModel& cost_model) noexcept;

  ~DetailedTopology() noexcept override = 0;

 protected:
  void connect(NpuId src, NpuId dest, int dimension) noexcept;

  Latency transmit(NpuId src, NpuId dest, PayloadSize payload_size) noexcept;

 private:
  std::map<NpuId, std::map<NpuId, Link>> links;
};
} // namespace Analytical

#endif
