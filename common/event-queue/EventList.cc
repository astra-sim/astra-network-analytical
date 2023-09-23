/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "common/event-queue/EventList.hh"

using namespace NetworkAnalytical;

EventList::EventList(EventTime event_time) noexcept : event_time(event_time) {
  // create an empty event list
  events = std::list<Event>();
}

EventTime EventList::get_event_time() const noexcept {
  return event_time;
}

void EventList::add_event(
    Callback callback,
    CallbackArg callback_arg) noexcept {
  // add the event to the event list
  events.emplace_back(callback, callback_arg);
}

void EventList::invoke_events() noexcept {
  // invoke all events in the event list
  while (!events.empty()) {
    events.front().invoke_event();
    events.pop_front();
  }
}
