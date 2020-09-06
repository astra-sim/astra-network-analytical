#ifndef __EVENTQUEUEENTRY_HH__
#define __EVENTQUEUEENTRY_HH__


#include <iostream>
#include <list>
#include "Event.hh"

// TODO: remove this before merge ===============================
enum time_type_e {SE, MS, US, NS, FS};

struct timespec_t {
    time_type_e time_res;
    double time_val;
};
// TODO: remove this before merge ===============================


struct EventQueueEntry {
public:
    /**
     * Construct an EventQueueEntry marked with given time_stamp.
     *
     * @param time_stamp time_stamp of this EventQueueEntry.
     */
    explicit EventQueueEntry(timespec_t time_stamp) noexcept : time_stamp(time_stamp) { }

    /**
     * Compare time_stamp_a and time_stamp_b.
     *
     * @param another_time_stamp
     * @return (positive int) if time_stamp_a is latter
     *         (negative int) if time_stamp_a is former
     *         (0)            if equal
     *         e.g., if time_stamp_a is (100ns) and time_stamp_b is (200ns),
     *               this function returns (negative int).
     */
    static int compare_time_stamp(timespec_t time_stamp_a, timespec_t time_stamp_b) noexcept;

    /**
     * time_stamp getter
     * @return time_stamp
     */
    timespec_t get_time_stamp() const noexcept;

    /**
     * Add an event handler.
     *
     * @param fun_ptr pointer to event handler
     * @param fun_arg pointer to event handler argument
     */
    void add_event(void (*fun_ptr)(void *), void *fun_arg) noexcept;

    /**
     * Run all events in `events` list and remove them from the list.
     */
    void run_events() noexcept;

    /**
     * (For debugging purpose)
     * Print the timestamp and number of events in the EventQueue.
     */
    void print() const noexcept;
private:
    /**
     * time stamp of current EventQueueEntry.
     */
    const timespec_t time_stamp;

    /**
     * list of scheduled events.
     */
    std::list<Event> events;
};

#endif
