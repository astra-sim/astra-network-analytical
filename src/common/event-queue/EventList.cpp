// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2026 Advanced Micro Devices, Inc.

#include <cassert>
#include <astra-network-analytical/common/EventList.h>

using namespace NetworkAnalytical;

EventList::EventList(const EventTime event_time) noexcept
    : event_time(event_time) {
    assert(event_time >= 0);

    // create an empty event list
    events = std::list<Event>();
}

EventTime EventList::get_event_time() const noexcept {
    return event_time;
}

void EventList::add_event(const Callback callback,
                          const CallbackArg callback_arg) noexcept {
    assert(callback != nullptr);

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
