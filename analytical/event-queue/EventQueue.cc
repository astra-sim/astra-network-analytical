/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "EventQueue.hh"

void Analytical::EventQueue::add_event(
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
  assert(EventQueueEntry::compare_time_stamp(current_time, time_stamp) < 0);

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

AstraSim::timespec_t Analytical::EventQueue::get_current_time() const noexcept {
  return current_time;
}

void Analytical::EventQueue::proceed() noexcept {
  auto& event_queue_entry = event_queue.front();

  // proceed current time
  current_time = event_queue_entry.get_time_stamp();

  // run events
  event_queue_entry.run_events();

  // remove queue entry
  event_queue.pop_front();
}

bool Analytical::EventQueue::empty() const noexcept {
  return event_queue.empty();
}

void Analytical::EventQueue::print() const noexcept {
  std::cout << "===== event-queue =====" << std::endl;
  std::cout << "CurrentTime: " << current_time.time_val << std::endl
            << std::endl;
  for (const auto& event_queue_entry : event_queue) {
    event_queue_entry.print();
  }
  std::cout << "======================" << std::endl << std::endl;
}
