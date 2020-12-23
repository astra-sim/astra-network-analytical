/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "FastRing_AllToAll_Switch.hh"
#include <iostream>
#include "cmath"

using namespace Analytical;

FastRing_AllToAll_Switch::FastRing_AllToAll_Switch(TopologyConfigs& configs) noexcept
    : FastTopology(configs) {
  // Ring
  auto ring_size = configs[0].getNpusCount();
  auto ring_links_count = ring_size;
  auto rings_count = npus_count / ring_size;
  auto total_ring_links_count = rings_count * ring_links_count;

  // AllToAll
  auto a2a_size = configs[1].getNpusCount();
  auto a2a_count = configs[2].getNpusCount() * ring_size;
  auto a2a_links_count = (a2a_size - 1) * a2a_size / 2;
  auto total_a2a_links_count = a2a_count * a2a_links_count;

  // Switch
  auto switch_links_count = npus_count;
  auto switches_count = cost_model.getNVSwitchesCount(switch_links_count);

  // Cost
  cost_model.addResource(CostModel::Resource::Npu, npus_count);

  cost_model.addResource(CostModel::Resource::NVLink, total_ring_links_count);
  cost_model.addResource(CostModel::Resource::NVLink, total_a2a_links_count);
  cost_model.addResource(CostModel::Resource::NVLink, switch_links_count);

  cost_model.addResource(CostModel::Resource::NVSwitch, switches_count);

  cost_model.addResource(
      CostModel::Resource::InfiniBandNic, switch_links_count);
}

FastRing_AllToAll_Switch::~FastRing_AllToAll_Switch() noexcept = default;

double FastRing_AllToAll_Switch::send(
    NpuId src,
    NpuId dest,
    PayloadSize payload_size) noexcept {
  // check NPU ID validity
  checkNpuIdBound(src);
  checkNpuIdBound(dest);

  // translate into address
  auto src_address = npuIdToAddress(src);
  auto dest_address = npuIdToAddress(dest);

  // compute communication latency
  auto communication_latency = 0.0;

  // check scale-out dimension
  if (src_address[2] != dest_address[2]) {
    // use scale-out switch
    const auto hops_count = 2;
    communication_latency += linkLatency(2, hops_count);
    communication_latency += serializationLatency(2, payload_size);
    communication_latency += 2 * nicLatency(2);
    communication_latency += routerLatency(2);

    auto hbm_latency = hbmLatency(2, payload_size);

    return criticalLatency(communication_latency, hbm_latency);
  }

  // scale-up dimension transmission
  auto ring_size = configs[0].getNpusCount();

  // check ring-directional movement
  // FIXME: assuming ring is bidirectional
  if (src_address[0] != dest_address[0]) {
    auto distanceA = std::abs(src_address[0] - dest_address[0]);
    auto distanceB = ring_size - distanceA;
    auto hops_count = (distanceA < distanceB)
        ? distanceA
        : distanceB; // shorter one becomes the hop count
    communication_latency += linkLatency(0, hops_count);
    communication_latency += serializationLatency(0, payload_size);
  }

  // check all-to-all movement
  if (src_address[1] != dest_address[1]) {
    // use scale-out switch
    const auto hops_count = 1;
    communication_latency += linkLatency(1, hops_count);
    communication_latency += serializationLatency(1, payload_size);
  }

  // compute hbm latency
  // FIXME: assuming hbm[0] = hbm[1] thus using only hbm[0]
  auto hbm_latency = hbmLatency(0, payload_size);

  // return critical latency
  return criticalLatency(communication_latency, hbm_latency);
}

FastRing_AllToAll_Switch::NpuAddress FastRing_AllToAll_Switch::npuIdToAddress(
    NpuId npu_id) const noexcept {
  auto ring_size = configs[0].getNpusCount();
  auto ring_id = npu_id % ring_size;

  auto all_to_all_size = configs[1].getNpusCount();
  auto node_size = ring_size * all_to_all_size;

  auto all_to_all_id = (npu_id % node_size) / ring_size;
  auto node_id = npu_id / node_size;

  auto npu_address = NpuAddress();
  npu_address.emplace_back(ring_id);
  npu_address.emplace_back(all_to_all_id);
  npu_address.emplace_back(node_id);

  //    std::cout << npu_id << ": " << ring_id << " " << all_to_all_id << " " <<
  //    node_id << std::endl;
  return npu_address;
}

FastRing_AllToAll_Switch::NpuId FastRing_AllToAll_Switch::npuAddressToId(
    NpuAddress npu_address) const noexcept {
  auto ring_size = configs[0].getNpusCount();
  auto all_to_all_size = configs[1].getNpusCount();
  auto node_size = ring_size * all_to_all_size;

  return npu_address[0] + (ring_size * npu_address[1]) +
      (node_size * npu_address[2]);
}
