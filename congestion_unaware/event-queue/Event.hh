/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#ifndef __EVENT_HH__
#define __EVENT_HH__

namespace Analytical {
class Event {
 public:
  typedef void (*CallbackOneArg)(void*);
  typedef void (*CallbackTwoArgs)(void*, void*);

  Event(CallbackOneArg callback, void* arg1) noexcept;
  Event(CallbackTwoArgs callback, void* arg1, void* arg2) noexcept;
  ~Event() noexcept = default;

  CallbackOneArg get_fun_ptr() const noexcept;
  void* get_fun_arg() const noexcept;

  void invoke_callback() noexcept;

 private:
  CallbackOneArg callback_one_arg;
  CallbackTwoArgs callback_two_args;
  void* arg1;
  void* arg2;
};
} // namespace Analytical

#endif /* __EVENT_HH__ */
