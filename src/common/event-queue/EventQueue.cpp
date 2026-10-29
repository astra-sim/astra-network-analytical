// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2026 Advanced Micro Devices, Inc.

#include <cassert>
#include <astra-network-analytical/common/EventQueue.h>

using namespace NetworkAnalytical;

EventQueue::EventQueue() noexcept : current_time(0) {
    // create empty event queue (map)
    event_queue = {};
}

EventTime EventQueue::get_current_time() const noexcept {
    return current_time;
}

bool EventQueue::finished() const noexcept {
    // check whether event queue is empty
    return event_queue.empty();
}

void EventQueue::proceed() noexcept {
    // to proceed, next event should exist
    assert(!finished());

    // get lowest time EventList which is beginning of map
    // EventTime is int64_t so will be ordered as desired without modifying
    // map's comparison operator
    auto current_event_iterator = event_queue.begin();
    auto& current_event_list = current_event_iterator->second;

    // check the validity and update current time. Only when current_time is > 0
    if (__builtin_expect(current_time, 1)) {
        assert(current_event_list.get_event_time() > current_time);
    }
    // assert key and value dont differ
    assert(current_event_iterator->first ==
           current_event_list.get_event_time());
    current_time = current_event_list.get_event_time();

    // invoke events
    current_event_list.invoke_events();

    // drop processed event list
    event_queue.erase(current_event_iterator);
}

void EventQueue::schedule_event(const EventTime event_time,
                                const Callback callback,
                                const CallbackArg callback_arg) noexcept {
    // time should be at least larger than current time
    assert(event_time >= current_time);

    // There can be three scenarios:
    // (1) event list matching with event_time is found
    // (2) there's no event list matching with event_time
    //    need to create new EventList
    // for both 1) and 2) a new event should be added

    // 2)
    if (event_queue.count(event_time) == 0) {
        // if empty initialize
        event_queue.emplace(event_time, EventList(event_time));
    }

    // 1) and/or 2)
    event_queue.at(event_time).add_event(callback, callback_arg);
}
