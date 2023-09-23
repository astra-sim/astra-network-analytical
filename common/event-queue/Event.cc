/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "common/event-queue/Event.hh"

using namespace NetworkAnalytical;

Event::Event(Callback callback, CallbackArg callback_arg) noexcept
    : callback(callback), callback_arg(callback_arg) {}

void Event::invoke_event() noexcept {
  // check the validity of the event
  assert(callback != nullptr);

  // invoke the callback function
  (*callback)(callback_arg);
}
