/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "congestion_aware/Topology.hh"
#include <cassert>
#include "congestion_aware/Link.hh"

using namespace NetworkAnalyticalCongestionAware;

void Topology::set_event_queue(
    std::shared_ptr<EventQueue> event_queue) noexcept {
  assert(event_queue != nullptr);

  // pass the given event_queue to Link
  Link::set_event_queue(std::move(event_queue));
}

Topology::Topology() noexcept
    : npus_count(-1), devices_count(-1), dims_count(-1) {
  npus_count_per_dim = {};
  devices = {};
  topology_per_dim = {};
  bandwidth_per_dim = {};
  latency_per_dim = {};
}

int Topology::get_devices_count() const noexcept {
  assert(devices_count > 0);
  assert(npus_count > 0);
  assert(devices_count >= npus_count);

  return devices_count;
}

int Topology::get_npus_count() const noexcept {
  assert(devices_count > 0);
  assert(npus_count > 0);
  assert(devices_count >= npus_count);

  return npus_count;
}

int Topology::get_dims_count() const noexcept {
  assert(dims_count > 0);

  return dims_count;
}

TopologyBuildingBlock Topology::get_topology_of_dim(
    const int dim) const noexcept {
  assert(0 <= dim && dim < dims_count);

  return topology_per_dim[dim];
}

int Topology::get_npus_count_of_dim(const int dim) const noexcept {
  assert(0 <= dim && dim < dims_count);

  return npus_count_per_dim[dim];
}

Bandwidth Topology::get_bandwidth_of_dim(const int dim) const noexcept {
  assert(0 <= dim && dim < dims_count);

  return bandwidth_per_dim[dim];
}

Latency Topology::get_latency_of_dim(const int dim) const noexcept {
  assert(0 <= dim && dim < dims_count);

  return latency_per_dim[dim];
}

void Topology::send(std::unique_ptr<Chunk> chunk) const noexcept {
  assert(chunk != nullptr);

  // get src npu node_id
  const auto src = chunk->current_device();
  const auto src_id = chunk->current_device()->get_id();

  // assert src is valid
  assert(0 <= src_id && src_id < devices_count);

  // initiate transmission from src
  src->send(std::move(chunk));
}

void Topology::connect(
    const std::shared_ptr<Device>& src,
    const std::shared_ptr<Device>& dest,
    const Bandwidth bandwidth,
    const Latency latency,
    const bool bidirectional) noexcept {
  // assert bandwidth and latency are valid
  assert(bandwidth > 0);
  assert(latency >= 0);

  const auto src_id = src->get_id();
  const auto dest_id = dest->get_id();

  // connect src -> dest
  src->connect(dest_id, bandwidth, latency);

  // if bidirectional, connect dest -> src
  if (bidirectional) {
    dest->connect(src_id, bandwidth, latency);
  }
}

std::shared_ptr<Device> Topology::create_device(const DeviceId id) noexcept {
  // check whether a device with id already exists
#ifndef DEBUG
  for (const auto& device : devices) {
    assert(device->get_id() != id);
  }
#endif

  // create new device
  const auto new_device = std::make_shared<Device>(id);
  devices.push_back(new_device);

  // return the pointer
  return new_device;
}
