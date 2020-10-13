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
