// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2026 Advanced Micro Devices, Inc.

#pragma once

#include <astra-network-analytical/common/Type.h>
#include <tuple>

namespace NetworkAnalytical {

/**
 * Event is a wrapper for a callback function and its argument.
 */
class Event {
  public:
    /**
     * Constructor.
     *
     * @param callback function pointer
     * @param callback_arg argument of the callback function
     */
    Event(Callback callback, CallbackArg callback_arg) noexcept;

    /**
     * Invoke the callback function.
     */
    void invoke_event() noexcept;

    /**
     * Get the callback function and the argument.
     *
     * @return callback function and its argument
     */
    [[nodiscard]] std::pair<Callback, CallbackArg> get_handler_arg()
        const noexcept;

  private:
    /// pointer to the callback function
    Callback callback;

    /// argument of the callback function
    CallbackArg callback_arg;
};

}  // namespace NetworkAnalytical
