/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#pragma once

#include <list>
#include "event-queue/Event.hh"
#include "type/Type.hh"

namespace Congestion {

/**
 * EventList represents a list of events
 * that are scheduled to be happening at the same time.
 */
class EventList {
 public:
  /**
   * Constructor.
   *
   * @param time time of the event list
   */
  explicit EventList(Time time) noexcept;

  /**
   * Destructor.
   */
  ~EventList() noexcept;

  /**
   * Get the time of the event list.
   *
   * @return scheduled event time
   */
  Time get_time() const noexcept;

  /**
   * Add an event to the event list.
   *
   * @param callback callback function pointer
   * @param callback_arg argument for the callback function
   */
  void schedule_event(Callback callback, CallbackArg callback_arg) noexcept;

  /**
   * invoke all events in the event list.
   */
  void invoke_events() noexcept;

 private:
  /// scheduled event time
  Time time;

  /// list of events
  std::list<Event> events;
};

} // namespace Congestion
