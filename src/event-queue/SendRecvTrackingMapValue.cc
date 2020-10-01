#include "SendRecvTrackingMapValue.hh"

AnalyticalBackend::SendRecvTrackingMapValue AnalyticalBackend::SendRecvTrackingMapValue::make_send_value(
        AstraSim::timespec_t send_finish_time) noexcept {
    return {OperationType::send, send_finish_time, nullptr, nullptr};
}

AnalyticalBackend::SendRecvTrackingMapValue AnalyticalBackend::SendRecvTrackingMapValue::make_recv_value(
        void (*fun_ptr)(void *), void *fun_arg) noexcept {
    return {OperationType::recv, AstraSim::timespec_t(), fun_ptr, fun_arg};
}

bool AnalyticalBackend::SendRecvTrackingMapValue::is_send() const noexcept {
    return operation_type == OperationType::send;
}

bool AnalyticalBackend::SendRecvTrackingMapValue::is_recv() const noexcept {
    return operation_type == OperationType::recv;
}

AstraSim::timespec_t AnalyticalBackend::SendRecvTrackingMapValue::get_send_finish_time() const noexcept {
    return send_finish_time;
}

AnalyticalBackend::Event AnalyticalBackend::SendRecvTrackingMapValue::get_recv_event() const noexcept {
    return recv_event;
}
