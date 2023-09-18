/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "event-queue/Event.hh"

#include <cassert>

using namespace Analytical;

Event::Event(CallbackOneArg callback, void* arg1_) noexcept
    : callback_one_arg(callback),
      callback_two_args(nullptr),
      arg1(arg1_),
      arg2(nullptr) {}

Event::Event(CallbackTwoArgs callback, void* arg1_, void* arg2_) noexcept
    : callback_one_arg(nullptr),
      callback_two_args(callback),
      arg1(arg1_),
      arg2(arg2_) {}

Event::CallbackOneArg Event::get_fun_ptr() const noexcept {
  return callback_one_arg;
}

void* Event::get_fun_arg() const noexcept {
  return arg1;
}

void Event::invoke_callback() noexcept {
  if (callback_one_arg != nullptr) {
    (*callback_one_arg)(arg1);
  } else if (callback_two_args != nullptr) {
    (*callback_two_args)(arg1, arg2);
  } else {
    assert(false);
  }
}
