#include "Event.hh"

void AnalyticalBackend::Event::run() const noexcept {
    (*fun_ptr)(fun_arg);
}

AnalyticalBackend::Event::FunPtr AnalyticalBackend::Event::get_fun_ptr() const noexcept {
    return fun_ptr;
}

void* AnalyticalBackend::Event::get_fun_arg() const noexcept {
    return fun_arg;
}
