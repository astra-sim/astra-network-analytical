/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#ifndef __EVENT_QUEUE_HH__
#define __EVENT_QUEUE_HH__

#include <cassert>
#include <list>

#include "event-queue/Event.hh"
#include "event-queue/EventQueueEntry.hh"
#include "astra-sim/system/Common.hh"

namespace Analytical {
class EventQueue {
 public:
  EventQueue() {
    current_time.time_res = AstraSim::NS;
    current_time.time_val = 0;
  }

  ~EventQueue() noexcept = default;

  void insert(Event event, AstraSim::timespec_t event_time) noexcept;

  void add_event(
      AstraSim::timespec_t time_stamp,
      void (*fun_ptr)(void*),
      void* fun_arg) noexcept;

  void proceed() noexcept;

  [[nodiscard]] AstraSim::timespec_t get_current_time() const noexcept;

  [[nodiscard]] bool empty() const noexcept;

  void print() const noexcept;

 private:
  AstraSim::timespec_t current_time = AstraSim::timespec_t();
  std::list<EventQueueEntry> event_queue;
};
} // namespace Analytical

#endif /* __EVENT_QUEUE_HH__ */
