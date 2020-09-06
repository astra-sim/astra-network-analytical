#include "Event.hh"

void AnalyticalBackend::Event::run() const noexcept {
    (*fun_ptr)(fun_arg);
}
