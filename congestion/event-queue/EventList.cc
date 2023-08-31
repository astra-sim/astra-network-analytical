/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "EventList.hh"

using namespace Congestion;

EventList::EventList(Time time) noexcept : time(time) {
  // create an empty event list
  events = std::list<Event>();
}

EventList::~EventList() noexcept = default;

Time EventList::get_time() const noexcept {
  return time;
}

void EventList::schedule_event(
    Callback callback,
    CallbackArg callback_arg) noexcept {
  // add the event to the event list
  events.emplace_back(callback, callback_arg);
}

void EventList::invoke_events() noexcept {
  // invoke every events in the event list
  while (!events.empty()) {
    events.front().invoke_event();
    events.pop_front();
  }
}
