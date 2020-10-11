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
