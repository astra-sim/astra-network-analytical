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

#include "EventQueueEntry.hh"

int Analytical::EventQueueEntry::compare_time_stamp(
    AstraSim::timespec_t time_stamp_a,
    AstraSim::timespec_t time_stamp_b) noexcept {
  // assumption: time_res is always NS
  // FIXME: when using time_res other than NS.

  if (time_stamp_a.time_val < time_stamp_b.time_val) {
    return -1;
  }

  if (time_stamp_a.time_val > time_stamp_b.time_val) {
    return 1;
  }

  return 0;
}

AstraSim::timespec_t Analytical::EventQueueEntry::get_time_stamp()
    const noexcept {
  return time_stamp;
}

void Analytical::EventQueueEntry::add_event(
    void (*fun_ptr)(void*),
    void* fun_arg) noexcept {
  events.emplace_back(fun_ptr, fun_arg);
}

void Analytical::EventQueueEntry::run_events() noexcept {
  while (!events.empty()) {
    auto event = events.front();
    event.run();
    events.pop_front();
  }
}

void Analytical::EventQueueEntry::print() const noexcept {
  std::cout << "EventQueueEntry:" << std::endl;
  std::cout << "\t- TimeStamp: " << (int)time_stamp.time_val << std::endl;
  std::cout << "\t- #Events: " << events.size() << std::endl << std::endl;
}
