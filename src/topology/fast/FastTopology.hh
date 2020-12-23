/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#ifndef __FASTTOPOLOGY_HH__
#define __FASTTOPOLOGY_HH__

#include "../Topology.hh"
#include "../TopologyConfig.hh"

namespace Analytical {
class FastTopology : public Topology {
 public:
  FastTopology(TopologyConfigs& configs) noexcept;

  ~FastTopology() noexcept override = 0;

 protected:
  Latency linkLatency(int dimension, int hops_count) const noexcept;
};
} // namespace Analytical

#endif
