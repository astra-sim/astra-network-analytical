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

#include "SendRecvTrackingMap.hh"

#include <cassert>

bool AnalyticalBackend::SendRecvTrackingMap::has_send_operation(int tag, int src, int dest, int count) const noexcept {
    auto search_result = send_recv_tracking_map.find(std::make_tuple(tag, src, dest, count));

    if (search_result == send_recv_tracking_map.end()) {
        // no matching entry found
        return false;
    }

    // check if the found entry is send operation
    return search_result->second.is_send();
}

bool AnalyticalBackend::SendRecvTrackingMap::has_recv_operation(int tag, int src, int dest, int count) const noexcept {
    auto search_result = send_recv_tracking_map.find(std::make_tuple(tag, src, dest, count));

    if (search_result == send_recv_tracking_map.end()) {
        // no matching entry found
        return false;
    }

    // check if the found entry is recv operation
    return search_result->second.is_recv();
}

AstraSim::timespec_t
AnalyticalBackend::SendRecvTrackingMap::pop_send_finish_time(int tag, int src, int dest, int count) noexcept {
    assert(has_send_operation(tag, src, dest, count)
           && "<SendRecvTrackingMap::pop_send_finish_time> no matching entry");

    auto send_entry = send_recv_tracking_map.find(std::make_tuple(tag, src, dest, count));

    // move sim_finish_time
    auto sim_finish_time = std::move(send_entry->second.get_send_finish_time());

    // pop entry
    send_recv_tracking_map.erase(send_entry);

    // return saved
    return sim_finish_time;
}

AnalyticalBackend::Event
AnalyticalBackend::SendRecvTrackingMap::pop_recv_event_handler(int tag, int src, int dest, int count) noexcept {
    assert(has_recv_operation(tag, src, dest, count)
           && "<SendRecvTrackingMap::pop_recv_event_handler> no matching entry");

    auto recv_entry = send_recv_tracking_map.find(std::make_tuple(tag, src, dest, count));

    // move event handler
    auto event = std::move(recv_entry->second.get_recv_event());

    // pop entry
    send_recv_tracking_map.erase(recv_entry);

    // return saved
    return event;
}

void AnalyticalBackend::SendRecvTrackingMap::insert_send(int tag, int src, int dest, int count,
                                                         AstraSim::timespec_t send_finish_time) noexcept {
    // Check whether entry with the same key exists
    assert((send_recv_tracking_map.find(std::make_tuple(tag, src, dest, count)) == send_recv_tracking_map.end())
           && "<SendRecvTrackingMap::insert_send> Same key entry already exist.");

    send_recv_tracking_map.emplace(std::make_tuple(tag, src, dest, count),
                                   SendRecvTrackingMapValue::make_send_value(send_finish_time));
}

void AnalyticalBackend::SendRecvTrackingMap::insert_recv(int tag, int src, int dest, int count,
                                                         void (*fun_ptr)(void *), void *fun_arg) noexcept {
    assert((send_recv_tracking_map.find(std::make_tuple(tag, src, dest, count)) == send_recv_tracking_map.end())
           && "<SendRecvTrackingMap::insert_recv> Same key entry already exist.");

    send_recv_tracking_map.emplace(std::make_tuple(tag, src, dest, count),
                                   SendRecvTrackingMapValue::make_recv_value(fun_ptr, fun_arg));
}

void AnalyticalBackend::SendRecvTrackingMap::print() const noexcept {
    std::cout << "[SendRecvTrackingMap] Entries not processed: " << send_recv_tracking_map.size() << std::endl;
}
