/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "event-queue/EventQueue.hh"

#include <iostream>

using namespace std;
using namespace Analytical;

void EventQueue::insert(Event event, AstraSim::timespec_t event_time) noexcept {
  for (auto it = event_queue.begin(); it != event_queue.end(); it++) {
    auto timestamp = it->get_time_stamp();
    if (timestamp.time_val == event_time.time_val) {
      // insert here
      it->insert(std::move(event));
      return;
    } else if (timestamp.time_val > event_time.time_val) {
      // should insert here
      auto new_entry = event_queue.emplace(it, event_time);
      new_entry->insert(std::move(event));
      return;
    }
  }

  // reaching here:
  //  (1) no entry at all
  //  (2) all entries smaller than given
  // either way, insert new entry at the end
  auto& new_entry = event_queue.emplace_back(event_time);
  new_entry.insert(std::move(event));
}

void EventQueue::add_event(
    AstraSim::timespec_t time_stamp,
    void (*fun_ptr)(void*),
    void* fun_arg) noexcept {
  // Event Queue is ordered by time_stamp in ascending order.
  // 1. Search Event queue:
  //      (1) if time_stamp is smaller, search next entry
  //      (2) if time_stamp is equal, add event to that entry
  //      (3) if time_stamp is larger, it means no entry matches time_stamp
  //            -> insert new event queue element

  // should assign event that happens later than current_time
  assert(EventQueueEntry::compare_time_stamp(current_time, time_stamp) <= 0);

  for (auto it = event_queue.begin(); it != event_queue.end(); it++) {
    auto time_stamp_compare_result =
        EventQueueEntry::compare_time_stamp(it->get_time_stamp(), time_stamp);
    // if time_stamp is smaller, do nothing
    if (time_stamp_compare_result == 0) {
      // equal time_stamp -> insert event here
      it->add_event(fun_ptr, fun_arg);
      return;
    } else if (time_stamp_compare_result > 0) {
      // entry's time stamp is larger -> no matching queue entry found
      // insert new queue entry
      auto new_queue_entry = event_queue.emplace(it, time_stamp);
      new_queue_entry->add_event(fun_ptr, fun_arg);
      return;
    }
  }

  // flow falls here when
  // (1) event queue was empty
  // (2) given time_stamp is larger than largest entry
  //      -> for both cases, create new entry at the end of the event_queue
  event_queue.emplace_back(time_stamp);
  event_queue.back().add_event(fun_ptr, fun_arg);
}

void EventQueue::proceed() noexcept {
  auto& event_queue_entry = event_queue.front();

  // proceed current time
  current_time = event_queue_entry.get_time_stamp();

  // run events
  event_queue_entry.run_events();

  // remove queue entry
  event_queue.pop_front();
}

AstraSim::timespec_t EventQueue::get_current_time() const noexcept {
  return current_time;
}

bool EventQueue::empty() const noexcept {
  return event_queue.empty();
}

void EventQueue::print() const noexcept {
  cout << "===== event-queue =====" << endl;
  cout << "CurrentTime: " << current_time.time_val << endl << endl;
  for (const auto& event_queue_entry : event_queue) {
    event_queue_entry.print();
  }
  cout << "======================" << endl << endl;
}
