/******************************************************************************
Copyright (c) 2020 Georgia Institute of Technology
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

Author : William Won (william.won@gatech.edu)
*******************************************************************************/

#ifndef __EVENTQUEUE_HH__
#define __EVENTQUEUE_HH__

#include <cassert>
#include <list>
#include "Event.hh"
#include "EventQueueEntry.hh"
#include "astra-sim/system/AstraNetworkAPI.hh"

namespace Analytical {
class EventQueue {
 public:
  /**
   * Create new event-queue.
   */
  EventQueue() {
    current_time.time_res = AstraSim::NS;
    current_time.time_val = 0;
  }

  /**
   * Add new event to the event-queue.
   *
   * @param time_stamp time_stamp for the event
   * @param fun_ptr pointer to the event handler
   * @param fun_arg pointer to the event handler argument
   */
  void add_event(
      AstraSim::timespec_t time_stamp,
      void (*fun_ptr)(void*),
      void* fun_arg) noexcept;

  /**
   * Fetch next event_queue entry, proceed current_time, and run scheduled
   * events.
   */
  void proceed() noexcept;

  /**
   * current_time getter
   */
  AstraSim::timespec_t get_current_time() const noexcept;

  /**
   * Check whether event_queue is empty.
   * @return true when event_queue is empty
   *         false otherwise
   */
  bool empty() const noexcept;

  /**
   * (For debugging purpose)
   * Print all EventQueueEntry in the event_queue.
   */
  void print() const noexcept;

 private:
  /**
   * current_time
   */
  AstraSim::timespec_t current_time = AstraSim::timespec_t();

  /**
   * event_queue list that holds events
   */
  std::list<EventQueueEntry> event_queue;
};
} // namespace Analytical

#endif
