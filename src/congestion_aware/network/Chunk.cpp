// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2026 Advanced Micro Devices, Inc.

#include <cassert>
#include <astra-network-analytical/congestion_aware/Chunk.h>
#include <astra-network-analytical/congestion_aware/Device.h>
#include <astra-network-analytical/congestion_aware/Link.h>
#include <iostream>

using namespace NetworkAnalyticalCongestionAware;

void Chunk::last_packet_was_serialized(void* const chunk_ptr) noexcept {
    assert(chunk_ptr != nullptr);
    auto chunk = std::unique_ptr<Chunk>(static_cast<Chunk*>(chunk_ptr));
    assert(chunk->first_hop);
    assert(chunk->last_packet);
    // cast to unique_ptr<Chunk>
    chunk->invoke_tx_callback();
}

void Chunk::chunk_arrived_next_device(void* const chunk_ptr) noexcept {
    assert(chunk_ptr != nullptr);

    // cast to unique_ptr<Chunk>
    auto chunk = std::unique_ptr<Chunk>(static_cast<Chunk*>(chunk_ptr));

    // mark chunk arrived next node
    chunk->mark_arrived_next_device();

    if (chunk->arrived_dest()) {
        // chunk arrived dest, invoke callback if it's the last packet
        // as chunk is unique_ptr, will be destroyed automatically
        if (chunk->last_packet) {
            chunk->invoke_callback();
        }
    } else {
        // send this chunk to next dest
        chunk->mark_left_first_device();
        const auto current_node = chunk->current_device();
        current_node->send(std::move(chunk));  // send chunk to next destination
    }
}

Chunk::Chunk(const ChunkSize chunk_size,
             Route route,
             const Callback tx_callback,
             const CallbackArg tx_callback_arg,
             const Callback rx_callback,
             const CallbackArg rx_callback_arg,
             const bool last_packet) noexcept
    : chunk_size(chunk_size),
      route(std::move(route)),
      callback(rx_callback),
      callback_arg(rx_callback_arg),
      tx_callback(tx_callback),
      tx_callback_arg(tx_callback_arg),
      last_packet(last_packet),
      inmediate_tx_release(true),
      first_hop(true) {
    // rx_callback = Callback();
    assert(chunk_size > 0);
    assert(!this->route.empty());
    assert(callback != nullptr);
}

Chunk::Chunk(const ChunkSize chunk_size,
             Route route,
             const Callback callback,
             const CallbackArg callback_arg,
             const bool last_packet) noexcept
    : chunk_size(chunk_size),
      route(std::move(route)),
      callback(callback),
      callback_arg(callback_arg),
      last_packet(last_packet),
      inmediate_tx_release(false),
      first_hop(true) {
    assert(chunk_size > 0);
    assert(!this->route.empty());
    assert(callback != nullptr);
}

std::shared_ptr<Device> Chunk::current_device() const noexcept {
    // assert the route is not empty
    assert(!route.empty());

    // return the first npu in route
    return route.front();
}

std::shared_ptr<Device> Chunk::next_device() const noexcept {
    // assert the chunk has next dest
    assert(!arrived_dest());

    // return next dest
    const auto next_dest = std::next(route.begin(), 1);
    return *next_dest;
}

void Chunk::mark_arrived_next_device() noexcept {
    // if this method is being called,
    // it means the chunk hasn't arrived its final dest yet
    assert(!arrived_dest());

    // pop previous node from the route
    // marking the current node has been changed
    route.pop_front();
}

bool Chunk::arrived_dest() const noexcept {
    // if a chunk arrived dest, route length should be 1
    // i.e., only containing the dest node
    return route.size() == 1;
}

ChunkSize Chunk::get_size() const noexcept {
    assert(chunk_size > 0);

    // return chunk size
    return chunk_size;
}

void Chunk::invoke_callback() noexcept {
    // invoke callback
    (*callback)(callback_arg);
}

void Chunk::invoke_tx_callback() noexcept {
    (*tx_callback)(tx_callback_arg);
}
