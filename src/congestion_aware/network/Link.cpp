// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2026 Advanced Micro Devices, Inc.

#include <cassert>
#include <astra-network-analytical/common/NetworkFunction.h>
#include <astra-network-analytical/congestion_aware/Chunk.h>
#include <astra-network-analytical/congestion_aware/Device.h>
#include <astra-network-analytical/congestion_aware/Link.h>

using namespace NetworkAnalytical;
using namespace NetworkAnalyticalCongestionAware;

// declaring static event_queue
std::shared_ptr<EventQueue> Link::event_queue;

void Link::link_become_free(void* const link_ptr) noexcept {
    assert(link_ptr != nullptr);

    // cast to Link*
    auto* const link = static_cast<Link*>(link_ptr);

    // set link free
    link->set_free();

    // process pending chunks if one exist
    if (link->pending_chunk_exists()) {
        link->process_pending_transmission();
    }
}

void Link::set_event_queue(
    std::shared_ptr<EventQueue> event_queue_ptr) noexcept {
    assert(event_queue_ptr != nullptr);

    // set the event queue
    Link::event_queue = std::move(event_queue_ptr);
}

Link::Link(const Bandwidth bandwidth, const Latency latency) noexcept
    : bandwidth(bandwidth),
      latency(latency),
      pending_chunks(),
      busy(false) {
    assert(bandwidth > 0);
    assert(latency >= 0);

    // convert bandwidth from GiB/s to B/ps
    bandwidth_Bpps = bw_GiBps_to_Bpps(bandwidth);

    /// convert latency from ns to ps
    latency_ps = latency_ns_to_ps(latency);
}

void Link::send(std::unique_ptr<Chunk> chunk) noexcept {
    assert(chunk != nullptr);

    if (busy) {
        // link is busy, add to pending chunks
        // TODO: add limit to the queue? report the max queue size? warning
        // about a setting?
        pending_chunks.push_back(std::move(chunk));
    } else {
        // service this chunk immediately
        schedule_chunk_transmission(std::move(chunk));
    }
}

void Link::process_pending_transmission() noexcept {
    // pending chunk should exist
    assert(pending_chunk_exists());

    // get chunk to process
    auto chunk = std::move(pending_chunks.front());
    pending_chunks.pop_front();

    // service this chunk
    schedule_chunk_transmission(std::move(chunk));
}

bool Link::pending_chunk_exists() const noexcept {
    // check pending chunks is not empty
    return !pending_chunks.empty();
}

void Link::set_busy() noexcept {
    // set busy to true
    busy = true;
}

void Link::set_free() noexcept {
    // set busy to false
    busy = false;
}

EventTime Link::serialization_delay(const ChunkSize chunk_size) const noexcept {
    assert(chunk_size > 0);

    // calculate serialization delay (in ps)
    const auto delay = static_cast<Bandwidth>(chunk_size) / bandwidth_Bpps;

    // return serialization delay in EventTime type
    return static_cast<EventTime>(delay);
}

EventTime Link::communication_delay(const ChunkSize chunk_size) const noexcept {
    assert(chunk_size > 0);

    // calculate communication delay (in ps)
    auto delay =
        static_cast<Bandwidth>(chunk_size) / bandwidth_Bpps;  // serialization
    delay += latency_ps;                                      // propagation

    // return communication delay in EventTime type
    return static_cast<EventTime>(delay);
}

void Link::schedule_chunk_transmission(std::unique_ptr<Chunk> chunk) noexcept {
    assert(chunk != nullptr);

    // link should be free
    assert(!busy);

    // set link busy
    set_busy();

    // get metadata
    const auto chunk_size = chunk->get_size();
    const auto current_time = Link::event_queue->get_current_time();

    // schedule link free time
    const auto serialization_time = serialization_delay(chunk_size);
    const auto link_free_time = current_time + serialization_time;

    // schedule chunk arrival event
    const auto communication_time = communication_delay(chunk_size);
    const auto chunk_arrival_time = current_time + communication_time;
    bool call_tx_callback = chunk->tx_is_done();
    auto* const chunk_ptr = static_cast<void*>(chunk.release());
    assert(chunk_arrival_time > link_free_time);
    if (call_tx_callback) {
        Link::event_queue->schedule_event(
            link_free_time, Chunk::last_packet_was_serialized, chunk_ptr);
    }

    Link::event_queue->schedule_event(
        chunk_arrival_time, Chunk::chunk_arrived_next_device, chunk_ptr);

    auto* const link_ptr = static_cast<void*>(this);
    Link::event_queue->schedule_event(link_free_time, link_become_free,
                                      link_ptr);
}
