#ifndef __ANALYTICAL_HH__
#define __ANALYTICAL_HH__
#include "astra-sim/system/AstraNetworkAPI.hh"
class Event{
    public:
        void (*fun_ptr)(void *fun_arg);
        void *fun_arg;
        timespec_t time_stamp;
};
//class QueueEntry{
//    public:
//        std::list<Event> events;
//        timespec_t time_stamp;
//};
class EventQueue{
    public:
        std::list<QueueEntry> discrete_events;
};
class Analytical: public AstraNetworkAPI{
    public:
        static EventQueue all_events;
        int sim_comm_size(sim_comm comm, int* size);
        int sim_comm_get_rank(){return rank;};
        int sim_comm_set_rank(int rank){this->rank=rank;return this->rank;};
        int sim_finish();
        double sim_time_resolution();
        int sim_init(AstraMemoryAPI* MEM);
        timespec_t sim_get_time()=0;
        void sim_schedule(timespec_t delta, void (*fun_ptr)(void *fun_arg), void *fun_arg);
        int sim_send(void *buffer, int count, int type, int dst, int tag, sim_request *request, void (*msg_handler)(void *fun_arg), void* fun_arg);
        int sim_recv(void *buffer, int count, int type, int src, int tag, sim_request *request, void (*msg_handler)(void *fun_arg), void* fun_arg);
};


#endif