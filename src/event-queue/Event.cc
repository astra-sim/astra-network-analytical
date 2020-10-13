/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "Event.hh"

void Analytical::Event::run() const noexcept {
  (*fun_ptr)(fun_arg);
}

Analytical::Event::FunPtr Analytical::Event::get_fun_ptr() const noexcept {
  return fun_ptr;
}

void* Analytical::Event::get_fun_arg() const noexcept {
  return fun_arg;
}
