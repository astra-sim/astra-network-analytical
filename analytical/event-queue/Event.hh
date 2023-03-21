/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#ifndef __EVENT_HH__
#define __EVENT_HH__

namespace Analytical {
struct Event {
 public:
  typedef void (*FunPtr)(void*);

  /**
   * Construct new event.
   * @param fun_ptr pointer to event handler
   * @param fun_arg pointer to event handler argument
   */
  Event(void (*fun_ptr)(void*), void* fun_arg) noexcept
      : fun_ptr(fun_ptr), fun_arg(fun_arg){};

  /**
   * Run the event handler.
   */
  void run() const noexcept;

  /**
   * fun_ptr getter
   * @return fun_ptr
   */
  FunPtr get_fun_ptr() const noexcept;

  /**
   * fun_arg getter
   * @return fun_arg
   */
  void* get_fun_arg() const noexcept;

 private:
  /**
   * function pointer directing the event handler
   */
  FunPtr fun_ptr;

  /**
   * pointer to the event handler argument.
   */
  void* fun_arg;
};
} // namespace Analytical

#endif
