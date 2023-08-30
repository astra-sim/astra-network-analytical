/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#pragma once

#include <list>
#include "event-queue/EventList.hh"
#include "type/Type.hh"

namespace Congestion {

/**
 * EventQueue stores all events and invoke them in order.
 */
class EventQueue {
 public:
  /**
   * Constructor.
   */
  EventQueue() noexcept;

  /**
   * Destructor.
   */
  ~EventQueue() noexcept;

  /**
   * Get current time.
   *
   * @return current time
   */
  Time get_current_time() const noexcept;

  /**
   * Check whether all events scheduled are invoked.
   *
   * @return true if all events are processed, false otherwise
   */
  bool finished() const noexcept;

  /**
   * Proceed to the next time slot and invoke all events scheduled at that time.
   */
  void proceed() noexcept;

  /**
   * Schedule an event at a specific time.
   * @param time time to schedule the event
   * @param callback callback function pointer
   * @param callback_arg callback function argument
   */
  void schedule_event(
      Time time,
      Callback callback,
      CallbackArg callback_arg) noexcept;

 private:
  /// current time of the event queue
  Time current_time;

  /// list of events (EventList) scheduled
  std::list<EventList> event_queue;
};

} // namespace Congestion
