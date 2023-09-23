/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#pragma once

#include "common/Common.hh"
#include "common/Type.hh"
#include "common/event-queue/EventList.hh"

namespace NetworkAnalytical {

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
   * Get current time the event_queue is processing.
   *
   * @return current time of the event_queue
   */
  [[nodiscard]] EventTime get_current_time() const noexcept;

  /**
   * Check whether all events scheduled are invoked.
   *
   * @return true if all events are invoked, false otherwise
   */
  [[nodiscard]] bool finished() const noexcept;

  /**
   * Proceed to the next event time slot
   * and invoke all events scheduled at that event time.
   */
  void proceed() noexcept;

  /**
   * Schedule an event at a specific time.
   * @param event_time event time to schedule the event
   * @param callback callback function pointer
   * @param callback_arg callback function argument
   */
  void schedule_event(
      EventTime event_time,
      Callback callback,
      CallbackArg callback_arg) noexcept;

 private:
  /// current time of the event queue
  EventTime current_time;

  /// list of events (EventList) scheduled
  std::list<EventList> event_queue;
};

} // namespace NetworkAnalytical
