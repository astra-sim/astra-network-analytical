/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#pragma once

#include "type/Type.hh"

namespace Congestion {

/**
 * Event is a wrapper for a callback function and its argument.
 */
class Event {
 public:
  /**
   * Constructor.
   *
   * @param callback callback function pointer
   * @param callback_arg argument for the callback function
   */
  Event(Callback callback, CallbackArg callback_arg) noexcept;

  /**
   * Destructor.
   */
  ~Event() noexcept;

  /**
   * Invoke the callback function with the argument.
   */
  void invoke_event() noexcept;

 private:
  /// pointer to the callback function
  Callback callback;

  /// argument for the callback function
  CallbackArg callback_arg;
};

} // namespace Congestion
