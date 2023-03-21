/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "AstraCongestionApi.hh"
#include <iostream>

using namespace Congestion;

std::shared_ptr<EventQueue> AstraCongestionApi::event_queue;

std::shared_ptr<Topology> AstraCongestionApi::topology;

ChunkIdGenerator AstraCongestionApi::chunk_id_generator = {};

EventHandlerTracker AstraCongestionApi::event_handler_tracker = {};

EventHandlerTracker& AstraCongestionApi::get_event_handler_tracker() noexcept {
  return event_handler_tracker;
}

void AstraCongestionApi::process_chunk_arrival(void* args) noexcept {
  auto* data = static_cast<std::tuple<int, int, int, uint64_t, int>*>(args);
  auto [tag, src, dest, count, chunk_id] = *data;

  auto& tracker = AstraCongestionApi::get_event_handler_tracker();
  auto entry = tracker.search_entry(tag, src, dest, count, chunk_id);
  assert(entry.has_value()); // entry must exist

  std::cout << "[Arrival] At time: "
            << AstraCongestionApi::event_queue->get_current_time() << std::endl;
  std::cout << "\tKey: [Tag: " << tag << ", Src: " << src << ", Dest: " << dest
            << ", Count: " << count << ", ID: " << chunk_id << "]" << std::endl;

  if (entry.value()->both_callbacks_registered()) {
    std::cout << "\tBoth callback registered." << std::endl;
    // call both send and recv callback
    entry.value()->invoke_send_handler();
    entry.value()->invoke_recv_handler();

    std::cout << "\tBoth callback invoked." << std::endl;

    // remove entry
    tracker.pop_entry(tag, src, dest, count, chunk_id);
    std::cout << "\tEntry removed." << std::endl;
  } else {
    // run only send callback, as recv is not ready yet.
    std::cout << "\tRecv not registered yet." << std::endl;
    entry.value()->invoke_send_handler();
    std::cout << "\tSend callback invoked." << std::endl;

    // mark the transmission as finished
    entry.value()->set_transmission_finished();
    std::cout << "\tMarked entry as finished." << std::endl;
  }

  std::cout << std::endl;
}

void AstraCongestionApi::link_event_queue(
    std::shared_ptr<EventQueue> event_queue) noexcept {
  AstraCongestionApi::event_queue = event_queue;
}

void AstraCongestionApi::link_topology(
    std::shared_ptr<Topology> topology) noexcept {
  AstraCongestionApi::topology = topology;
}

AstraCongestionApi::AstraCongestionApi(int rank) noexcept
    : AstraSim::AstraNetworkAPI(rank) {}

AstraCongestionApi::~AstraCongestionApi() noexcept = default;

int AstraCongestionApi::sim_comm_size(AstraSim::sim_comm comm, int* size) {
  return 0;
}

int AstraCongestionApi::sim_finish() {
  return 0;
}

double AstraCongestionApi::sim_time_resolution() {
  return 0;
}

int AstraCongestionApi::sim_init(AstraSim::AstraMemoryAPI* MEM) {
  return 0;
}

AstraSim::timespec_t AstraCongestionApi::sim_get_time() {
  auto current_time = event_queue->get_current_time();
  return {AstraSim::NS, current_time};
}

void AstraCongestionApi::sim_schedule(
    AstraSim::timespec_t delta,
    void (*fun_ptr)(void*),
    void* fun_arg) {
  // get current time
  auto event_time = event_queue->get_current_time();

  // calculate event time
  assert(delta.time_res == AstraSim::NS); // fixme: assuming NS
  event_time += delta.time_val;

  // schedule event
  event_queue->schedule_event(event_time, fun_ptr, fun_arg);
}

int AstraCongestionApi::sim_send(
    void* buffer,
    uint64_t count,
    int type,
    int dst,
    int tag,
    AstraSim::sim_request* request,
    void (*msg_handler)(void*),
    void* fun_arg) {
  // query chunk id
  auto src = sim_comm_get_rank();
  auto chunk_id =
      AstraCongestionApi::chunk_id_generator.get_send_id(tag, src, dst, count);

  std::cout << "[sim_send] " << src << " -> " << dst << " (At time: "
            << AstraCongestionApi::event_queue->get_current_time() << ")"
            << std::endl;
  std::cout << "\tKey: [Tag: " << tag << ", Src: " << src << ", Dest: " << dst
            << ", Count: " << count << ", ID: " << chunk_id << "]" << std::endl;

  // search tracker
  auto entry =
      event_handler_tracker.search_entry(tag, src, dst, count, chunk_id);
  if (entry.has_value()) {
    std::cout << "\tEntry found." << std::endl;
    entry.value()->register_send_callback(msg_handler, fun_arg);
    std::cout << "\tSend callback registered." << std::endl;
  } else {
    // create new entry and insert callback
    std::cout << "\tEntry not found." << std::endl;
    auto* new_entry =
        event_handler_tracker.create_new_entry(tag, src, dst, count, chunk_id);
    new_entry->register_send_callback(msg_handler, fun_arg);
    std::cout << "\tSend callback registered with new entry." << std::endl;
  }

  // initiate transmission from src -> dst.
  auto chunk_arrival_arg = std::tuple(tag, src, dst, count, chunk_id);
  auto arg = std::make_unique<decltype(chunk_arrival_arg)>(chunk_arrival_arg);
  auto arg_ptr = static_cast<void*>(arg.release());
  auto route = std::move(topology->route(src, dst));

  auto chunk = std::make_unique<Chunk>(
      count, route, AstraCongestionApi::process_chunk_arrival, arg_ptr);
  topology->send(std::move(chunk));

  std::cout << "\tTransmission initiated" << std::endl;
  std::cout << std::endl;
}

int AstraCongestionApi::sim_recv(
    void* buffer,
    uint64_t count,
    int type,
    int src,
    int tag,
    AstraSim::sim_request* request,
    void (*msg_handler)(void*),
    void* fun_arg) {
  // query chunk id
  auto dst = sim_comm_get_rank();
  auto chunk_id =
      AstraCongestionApi::chunk_id_generator.get_recv_id(tag, src, dst, count);
  std::cout << "[sim_recv] " << src << " -> " << dst << " (At time: "
            << AstraCongestionApi::event_queue->get_current_time() << ")"
            << std::endl;
  std::cout << "\tKey: [Tag: " << tag << ", Src: " << src << ", Dest: " << dst
            << ", Count: " << count << ", ID: " << chunk_id << "]" << std::endl;

  // search tracker
  auto entry =
      event_handler_tracker.search_entry(tag, src, dst, count, chunk_id);
  if (entry.has_value()) {
    std::cout << "\tEntry found." << std::endl;
    // send() already invoked
    // behavior is decided whether the transmission is already finished or not
    if (entry.value()->is_transmission_finished()) {
      std::cout << "\tSend already finished." << std::endl;
      // pop entry
      event_handler_tracker.pop_entry(tag, src, dst, count, chunk_id);

      // run recv callback immediately
      auto delta = AstraSim::timespec_t{AstraSim::NS, 0};
      sim_schedule(delta, msg_handler, fun_arg);
      std::cout << "\tEntry removed and recv callback invoked." << std::endl;
    } else {
      // register recv callback
      std::cout << "\tSend not yet finished." << std::endl;
      entry.value()->register_recv_callback(msg_handler, fun_arg);
      std::cout << "\tRecv callback registered" << std::endl;
    }
  } else {
    // send() not yet called
    // create new entry and insert callback
    std::cout << "\tEntry not found." << std::endl;

    auto* new_entry =
        event_handler_tracker.create_new_entry(tag, src, dst, count, chunk_id);
    new_entry->register_recv_callback(msg_handler, fun_arg);
    std::cout << "\tRecv callback registered with new entry." << std::endl;
  }

  std::cout << std::endl;
}

void AstraCongestionApi::pass_front_end_report(
    AstraSim::AstraSimDataAPI astraSimDataAPI) {
  // todo: implement
}

double AstraCongestionApi::get_BW_at_dimension(int dim) {
  // todo: implement
  // this is for Themis usage
}
