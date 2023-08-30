/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "Event.hh"

using namespace Congestion;

Event::Event(Callback callback, CallbackArg callback_arg) noexcept
    : callback(callback), callback_arg(callback_arg) {}

Event::~Event() noexcept = default;

void Event::invoke_event() noexcept {
  // invoke the callback function
  (*callback)(callback_arg);
}
