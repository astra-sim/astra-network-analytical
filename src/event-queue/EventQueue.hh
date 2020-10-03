#ifndef __EVENTQUEUE_HH__
#define __EVENTQUEUE_HH__


#include <list>
#include <cassert>
#include "Event.hh"
#include "EventQueueEntry.hh"
#include "src/astra-sim/system/AstraNetworkAPI.hh"

namespace AnalyticalBackend {
    class EventQueue {
    public:
        /**
         * Create new event-queue.
         */
        EventQueue() {
            current_time.time_res = AstraSim::NS;
            current_time.time_val = 0;
        }

        /**
         * Add new event to the event-queue.
         *
         * @param time_stamp time_stamp for the event
         * @param fun_ptr pointer to the event handler
         * @param fun_arg pointer to the event handler argument
         */
        void add_event(AstraSim::timespec_t time_stamp, void (*fun_ptr)(void *), void *fun_arg) noexcept;

        /**
         * Fetch next event_queue entry, proceed current_time, and run scheduled events.
         */
        void proceed() noexcept;

        /**
         * current_time getter
         */
        AstraSim::timespec_t get_current_time() const noexcept;

        /**
         * Check whether event_queue is empty.
         * @return true when event_queue is empty
         *         false otherwise
         */
        bool empty() const noexcept;

        /**
         * (For debugging purpose)
         * Print all EventQueueEntry in the event_queue.
         */
        void print() const noexcept;

    private:
        /**
         * current_time
         */
        AstraSim::timespec_t current_time = AstraSim::timespec_t();

        /**
         * event_queue list that holds events
         */
        std::list<EventQueueEntry> event_queue;
    };
}

#endif
