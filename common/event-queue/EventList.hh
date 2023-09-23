/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#pragma once

#include "common/Common.hh"
#include "common/Type.hh"
#include "common/event-queue/Event.hh"

namespace NetworkAnalytical {

/**
 * EventList represents a list of events
 * that are scheduled to be happening at the same time.
 */
class EventList {
 public:
  /**
   * Constructor.
   *
   * @param event_time the time the list of events will take place
   */
  explicit EventList(EventTime event_time) noexcept;

  /**
   * Get the event time of the event list.
   *
   * @return scheduled event time
   */
  [[nodiscard]] EventTime get_event_time() const noexcept;

  /**
   * Add an event to the event list.
   *
   * @param callback callback function pointer
   * @param callback_arg argument for the callback function
   */
  void add_event(Callback callback, CallbackArg callback_arg) noexcept;

  /**
   * invoke all events in the event list.
   */
  void invoke_events() noexcept;

 private:
  /// scheduled event time
  EventTime event_time;

  /// list of events
  std::list<Event> events;
};

} // namespace NetworkAnalytical
