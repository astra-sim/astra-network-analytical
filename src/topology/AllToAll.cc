/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "AllToAll.hh"

using namespace Analytical;

AllToAll::AllToAll(
    const TopologyConfigurations& configurations,
    int npus_count) noexcept {
  this->configurations = configurations;

  // connect all npus directly
  for (int n1 = 0; n1 < (npus_count - 1); n1++) {
    for (int n2 = (n1 + 1); n2 < npus_count; n2++) {
      connect(n1, n2, 0);
      connect(n2, n1, 0);
    }
  }
}

Topology::Latency AllToAll::send(
    NpuId src_id,
    NpuId dest_id,
    PayloadSize payload_size) noexcept {
  if (src_id == dest_id) {
    // guard statement
    return 0;
  }

  // 1. Source nic latency
  // 2. route packet from src to dest
  // 3. Dest nic latency
  auto link_latency = serialize(payload_size, 0);
  link_latency += nicLatency(0);
  link_latency += route(src_id, dest_id, payload_size);
  link_latency += nicLatency(0);

  auto hbm_latency = hbmLatency(payload_size, 0);

  return criticalLatency(link_latency, hbm_latency);
}

Topology::NpuAddress AllToAll::npuIdToAddress(NpuId id) const noexcept {
  return NpuAddress(1, id);
}

Topology::NpuId AllToAll::npuAddressToId(
    const NpuAddress& address) const noexcept {
  return address[0];
}
