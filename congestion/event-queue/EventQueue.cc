/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "EventQueue.hh"
#include <cassert>

using namespace Congestion;

EventQueue::EventQueue() noexcept : current_time(0) {
  // create empty event queue
  event_queue = std::list<EventList>();
}

EventQueue::~EventQueue() noexcept = default;

Time EventQueue::get_current_time() const noexcept {
  return current_time;
}

bool EventQueue::finished() const noexcept {
  // check whether event queue is empty
  return event_queue.empty();
}

void EventQueue::proceed() noexcept {
  // to proceed, next event should exist
  assert(!finished());

  // proceed to the next timestep
  // update current time
  auto& current_event_list = event_queue.front();
  current_time = current_event_list.get_time();

  // invoke events
  current_event_list.invoke_events();

  // drop processed event
  event_queue.pop_front();
}

void EventQueue::schedule_event(
    Time time,
    Callback callback,
    CallbackArg callback_arg) noexcept {
  // time should be at least larger than current time
  assert(time >= current_time);

  // find the entry to insert event
  auto event_list_it = event_queue.begin();
  while (event_list_it != event_queue.end() &&
         event_list_it->get_time() < time) {
    event_list_it++;
  }

  // check whether new event_list is needed
  if (event_list_it == event_queue.end() || event_list_it->get_time() > time) {
    // insert new event_list
    event_list_it = event_queue.insert(event_list_it, EventList(time));
  }

  // add event to event_list
  event_list_it->schedule_event(callback, callback_arg);
}
