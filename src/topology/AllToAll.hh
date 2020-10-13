/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#ifndef __ALLTOALL_HH__
#define __ALLTOALL_HH__

#include <vector>
#include "Topology.hh"
#include "TopologyConfiguration.hh"

namespace Analytical {
class AllToAll : public Topology {
 public:
  using TopologyConfigurations = TopologyConfiguration::TopologyConfigurations;

  /**
   * Construct an AllToAll topology.
   * @param configurations configuration per each dimension
   * @param npus_count number of npus connected together
   */
  AllToAll(
      const TopologyConfigurations& configurations,
      int npus_count) noexcept;

  Latency send(NpuId src_id, NpuId dest_id, PayloadSize payload_size) noexcept
      override;

 private:
  NpuAddress npuIdToAddress(NpuId id) const noexcept override;
  NpuId npuAddressToId(const NpuAddress& address) const noexcept override;
};
} // namespace Analytical

#endif
