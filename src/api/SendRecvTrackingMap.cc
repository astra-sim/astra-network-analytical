/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "SendRecvTrackingMap.hh"

#include <cassert>

bool Analytical::SendRecvTrackingMap::has_send_operation(
    int tag,
    int src,
    int dest,
    PayloadSize count) const noexcept {
  auto search_result =
      send_recv_tracking_map.find(std::make_tuple(tag, src, dest, count));

  if (search_result == send_recv_tracking_map.end()) {
    // no matching entry found
    return false;
  }

  // check if the found entry is send operation
  return search_result->second.is_send();
}

bool Analytical::SendRecvTrackingMap::has_recv_operation(
    int tag,
    int src,
    int dest,
    PayloadSize count) const noexcept {
  auto search_result =
      send_recv_tracking_map.find(std::make_tuple(tag, src, dest, count));

  if (search_result == send_recv_tracking_map.end()) {
    // no matching entry found
    return false;
  }

  // check if the found entry is recv operation
  return search_result->second.is_recv();
}

AstraSim::timespec_t Analytical::SendRecvTrackingMap::pop_send_finish_time(
    int tag,
    int src,
    int dest,
    PayloadSize count) noexcept {
  assert(
      has_send_operation(tag, src, dest, count) &&
      "<SendRecvTrackingMap::pop_send_finish_time> no matching entry");

  auto send_entry =
      send_recv_tracking_map.find(std::make_tuple(tag, src, dest, count));

  // move sim_finish_time
  auto sim_finish_time = std::move(send_entry->second.get_send_finish_time());

  // pop entry
  send_recv_tracking_map.erase(send_entry);

  // return saved
  return sim_finish_time;
}

Analytical::Event Analytical::SendRecvTrackingMap::pop_recv_event_handler(
    int tag,
    int src,
    int dest,
    PayloadSize count) noexcept {
  assert(
      has_recv_operation(tag, src, dest, count) &&
      "<SendRecvTrackingMap::pop_recv_event_handler> no matching entry");

  auto recv_entry =
      send_recv_tracking_map.find(std::make_tuple(tag, src, dest, count));

  // move event handler
  auto event = std::move(recv_entry->second.get_recv_event());

  // pop entry
  send_recv_tracking_map.erase(recv_entry);

  // return saved
  return event;
}

void Analytical::SendRecvTrackingMap::insert_send(
    int tag,
    int src,
    int dest,
    PayloadSize count,
    AstraSim::timespec_t send_finish_time) noexcept {
  // Check whether entry with the same key exists
  assert(
      (send_recv_tracking_map.find(std::make_tuple(tag, src, dest, count)) ==
       send_recv_tracking_map.end()) &&
      "<SendRecvTrackingMap::insert_send> Same key entry already exist.");

  send_recv_tracking_map.emplace(
      std::make_tuple(tag, src, dest, count),
      SendRecvTrackingMapValue::make_send_value(send_finish_time));
}

void Analytical::SendRecvTrackingMap::insert_recv(
    int tag,
    int src,
    int dest,
    PayloadSize count,
    void (*fun_ptr)(void*),
    void* fun_arg) noexcept {
  assert(
      (send_recv_tracking_map.find(std::make_tuple(tag, src, dest, count)) ==
       send_recv_tracking_map.end()) &&
      "<SendRecvTrackingMap::insert_recv> Same key entry already exist.");

  send_recv_tracking_map.emplace(
      std::make_tuple(tag, src, dest, count),
      SendRecvTrackingMapValue::make_recv_value(fun_ptr, fun_arg));
}

void Analytical::SendRecvTrackingMap::print() const noexcept {
  std::cout << "[SendRecvTrackingMap] Entries not processed: "
            << send_recv_tracking_map.size() << std::endl;
}
