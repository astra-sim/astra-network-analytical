/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "AnalyticalNetwork.hh"

std::shared_ptr<Analytical::EventQueue>
    Analytical::AnalyticalNetwork::event_queue;

std::shared_ptr<Analytical::Topology> Analytical::AnalyticalNetwork::topology;

Analytical::SendRecvTrackingMap
    Analytical::AnalyticalNetwork::send_recv_tracking_map;

void Analytical::AnalyticalNetwork::set_event_queue(
    const std::shared_ptr<EventQueue>& event_queue_ptr) noexcept {
  AnalyticalNetwork::event_queue = event_queue_ptr;
}

void Analytical::AnalyticalNetwork::set_topology(
    const std::shared_ptr<Topology>& topology_ptr) noexcept {
  AnalyticalNetwork::topology = topology_ptr;
}

int Analytical::AnalyticalNetwork::sim_comm_size(
    AstraSim::sim_comm comm,
    int* size) {
  return 0;
}

int Analytical::AnalyticalNetwork::sim_finish() {
  return 0;
}

double Analytical::AnalyticalNetwork::sim_time_resolution() {
  return 0;
}

int Analytical::AnalyticalNetwork::sim_init(AstraSim::AstraMemoryAPI* MEM) {
  return 0;
}

AstraSim::timespec_t Analytical::AnalyticalNetwork::sim_get_time() {
  return event_queue->get_current_time();
}

void Analytical::AnalyticalNetwork::sim_schedule(
    AstraSim::timespec_t delta,
    void (*fun_ptr)(void*),
    void* fun_arg) {
  // 1. compute event_time = current_time + delta
  auto event_time = sim_get_time();

  // FIXME: assuming time_res is always NS
  event_time.time_val += delta.time_val;

  // 2. schedule an event at the event_time
  event_queue->add_event(event_time, fun_ptr, fun_arg);
}

int Analytical::AnalyticalNetwork::sim_send(
    void* buffer,
    int count,
    int type,
    int dst,
    int tag,
    AstraSim::sim_request* request,
    void (*msg_handler)(void*),
    void* fun_arg) {
  // get source id
  auto src = sim_comm_get_rank();

  // compute send latency in ns    // FIXME: if you want to use time_res other
  // than NS
  AstraSim::timespec_t delta;
  delta.time_res = AstraSim::NS;
  delta.time_val =
      (int)topology->send(src, dst, count); // simulate src->dst and get latency

  if (send_recv_tracking_map.has_recv_operation(tag, src, dst, count)) {
    // recv operation already issued.
    // Schedule both send and recv event handler.
    auto recv_event_handler =
        send_recv_tracking_map.pop_recv_event_handler(tag, src, dst, count);
    sim_schedule(delta, msg_handler, fun_arg);
    sim_schedule(
        delta,
        recv_event_handler.get_fun_ptr(),
        recv_event_handler.get_fun_arg());
  } else {
    // recv operation not issued yet.
    // Should assign this send operation to the tracker.

    // schedule send event
    sim_schedule(delta, msg_handler, fun_arg);

    // compute send finish time  // FIXME: if you want to use time_res other
    // than NS
    auto send_finish_time = sim_get_time();
    send_finish_time.time_val += delta.time_val;

    // schedule this into the tracker
    send_recv_tracking_map.insert_send(tag, src, dst, count, send_finish_time);
  }

  return 0;
}

int Analytical::AnalyticalNetwork::sim_recv(
    void* buffer,
    int count,
    int type,
    int src,
    int tag,
    AstraSim::sim_request* request,
    void (*msg_handler)(void*),
    void* fun_arg) {
  // get source id
  auto dst = sim_comm_get_rank();

  if (send_recv_tracking_map.has_send_operation(tag, src, dst, count)) {
    // send operation already issued.
    // should compute delta
    AstraSim::timespec_t delta;
    delta.time_res = AstraSim::NS;

    auto current_time = sim_get_time();
    auto send_finish_time =
        send_recv_tracking_map.pop_send_finish_time(tag, src, dst, count);

    if (EventQueueEntry::compare_time_stamp(current_time, send_finish_time) <
        0) {
      // sent packet still inflight
      // schedule recv handler accordingly.
      delta.time_val = send_finish_time.time_val - current_time.time_val;
    } else {
      // send operation already finished.
      // invoke recv handler immediately
      delta.time_val = 0;
    }

    // schedule recv handler
    sim_schedule(delta, msg_handler, fun_arg);
  } else {
    // send operation not issued.
    // Add recv to the tracker and wait until corresponding sim_send to be
    // invoked.
    send_recv_tracking_map.insert_recv(
        tag, src, dst, count, msg_handler, fun_arg);
  }

  return 0;
}
