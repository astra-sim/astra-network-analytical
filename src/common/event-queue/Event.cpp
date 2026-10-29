// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2026 Advanced Micro Devices, Inc.

#include <cassert>
#include <astra-network-analytical/common/Event.h>

using namespace NetworkAnalytical;

Event::Event(const Callback callback, const CallbackArg callback_arg) noexcept
    : callback(callback),
      callback_arg(callback_arg) {
    assert(callback != nullptr);
}

void Event::invoke_event() noexcept {
    // check the validity of the event
    assert(callback != nullptr);

    // invoke the callback function
    (*callback)(callback_arg);
}

std::pair<Callback, CallbackArg> Event::get_handler_arg() const noexcept {
    // check the validity of the event
    assert(callback != nullptr);

    return {callback, callback_arg};
}
