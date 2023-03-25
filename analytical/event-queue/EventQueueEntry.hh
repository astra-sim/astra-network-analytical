/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#ifndef __EVENT_QUEUE_ENTRY_HH__
#define __EVENT_QUEUE_ENTRY_HH__

#include <list>

#include "event-queue/Event.hh"
#include "astra-sim/system/Common.hh"

namespace Analytical {
struct EventQueueEntry {
 public:
  explicit EventQueueEntry(AstraSim::timespec_t time_stamp) noexcept
      : time_stamp(time_stamp) {}

  ~EventQueueEntry() noexcept = default;

  static int compare_time_stamp(
      AstraSim::timespec_t time_stamp_a,
      AstraSim::timespec_t time_stamp_b) noexcept;

  AstraSim::timespec_t get_time_stamp() const noexcept;

  void add_event(void (*fun_ptr)(void*), void* fun_arg) noexcept;

  void insert(Event event) noexcept;

  void run_events() noexcept;

  void print() const noexcept;

 private:
  const AstraSim::timespec_t time_stamp;
  std::list<Event> events;
};
} // namespace Analytical

#endif /* __EVENT_QUEUE_ENTRY_HH__ */
