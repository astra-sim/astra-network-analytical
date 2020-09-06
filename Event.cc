#include "Event.hh"

void Event::run() const noexcept {
    (*fun_ptr)(fun_arg);
}
