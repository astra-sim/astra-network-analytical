#include "EventQueueEntry.hh"

int EventQueueEntry::compare_time_stamp(timespec_t time_stamp_a, timespec_t time_stamp_b) noexcept {
    // assumption: time_res is always NS
    // FIXME: when using time_res other than NS.

    if (time_stamp_a.time_val < time_stamp_b.time_val) {
        return -1;
    }

    if (time_stamp_a.time_val > time_stamp_b.time_val) {
        return 1;
    }

    return 0;
}

timespec_t EventQueueEntry::get_time_stamp() const noexcept {
    return time_stamp;
}

void EventQueueEntry::add_event(void (*fun_ptr)(void *), void *fun_arg) noexcept {
    events.emplace_back(fun_ptr, fun_arg);
}

void EventQueueEntry::run_events() noexcept {
    while (!events.empty()) {
        auto event = events.front();
        event.run();
        events.pop_front();
    }
}

void EventQueueEntry::print() const noexcept {
    std::cout << "EventQueueEntry:" << std::endl;
    std::cout << "\t- TimeStamp: " << (int)time_stamp.time_val << std::endl;
    std::cout << "\t- #Events: " << events.size() << std::endl << std::endl;
}
