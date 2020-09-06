#include <iostream>
#include "Event.hh"
#include "EventQueueEntry.hh"
#include "EventQueue.hh"
#include "../astra-sim/system/AstraNetworkAPI.hh"


void print_name(void *name) {
    std::cout << "print_name: " << *(static_cast<std::string*>(name)) << std::endl;
}

void print_int(void *number) {
    std::cout << "print_int: " << *(static_cast<int*>(number)) << std::endl;

}

int main(int argc, char *argv[]) {
    auto ts1 = AstraSim::timespec_t();
    ts1.time_res = AstraSim::NS;
    ts1.time_val = 100;

    auto ts2 = AstraSim::timespec_t();
    ts2.time_res = AstraSim::NS;
    ts2.time_val = 200;

    auto ts3 = AstraSim::timespec_t();
    ts3.time_res = AstraSim::NS;
    ts3.time_val = 300;

    int num1 = 3;
    int num2 = 7;
    std::string name = "hello world";

    auto event_queue = AnalyticalBackend::EventQueue();

    event_queue.add_event(ts1, print_int, static_cast<void *>(&num1));
    event_queue.add_event(ts3, print_int, static_cast<void *>(&num1));
    event_queue.add_event(ts1, print_int, static_cast<void *>(&num2));
    event_queue.add_event(ts2, print_name, static_cast<void *>(&name));
    event_queue.add_event(ts2, print_int, static_cast<void *>(&num1));
    event_queue.add_event(ts3, print_name, static_cast<void *>(&name));

    while (!event_queue.empty()) {
        event_queue.print();
        event_queue.proceed();
    }

    event_queue.print();

    return 0;
}
