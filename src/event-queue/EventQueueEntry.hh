/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#ifndef __EVENTQUEUEENTRY_HH__
#define __EVENTQUEUEENTRY_HH__

#include <iostream>
#include <list>
#include "Event.hh"
#include "astra-sim/system/AstraNetworkAPI.hh"

namespace Analytical {
struct EventQueueEntry {
 public:
  /**
   * Construct an EventQueueEntry marked with given time_stamp.
   *
   * @param time_stamp time_stamp of this EventQueueEntry.
   */
  explicit EventQueueEntry(AstraSim::timespec_t time_stamp) noexcept
      : time_stamp(time_stamp) {}

  /**
   * Compare time_stamp_a and time_stamp_b.
   *
   * @param another_time_stamp
   * @return (positive int) if time_stamp_a is latter
   *         (negative int) if time_stamp_a is former
   *         (0)            if equal
   *         e.g., if time_stamp_a is (100ns) and time_stamp_b is (200ns),
   *               this function returns (negative int).
   */
  static int compare_time_stamp(
      AstraSim::timespec_t time_stamp_a,
      AstraSim::timespec_t time_stamp_b) noexcept;

  /**
   * time_stamp getter
   * @return time_stamp
   */
  AstraSim::timespec_t get_time_stamp() const noexcept;

  /**
   * Add an event handler.
   *
   * @param fun_ptr pointer to event handler
   * @param fun_arg pointer to event handler argument
   */
  void add_event(void (*fun_ptr)(void*), void* fun_arg) noexcept;

  /**
   * Run all events in `events` list and remove them from the list.
   */
  void run_events() noexcept;

  /**
   * (For debugging purpose)
   * Print the timestamp and number of events in the event-queue.
   */
  void print() const noexcept;

 private:
  /**
   * time stamp of current EventQueueEntry.
   */
  const AstraSim::timespec_t time_stamp;

  /**
   * list of scheduled events.
   */
  std::list<Event> events;
};
} // namespace Analytical

#endif
