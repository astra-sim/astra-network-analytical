#include "AnalyticalNetwork.hh"

AnalyticalBackend::EventQueue AnalyticalBackend::AnalyticalNetwork::event_queue;

AnalyticalBackend::Topology AnalyticalBackend::AnalyticalNetwork::topology;

AnalyticalBackend::EventQueue& AnalyticalBackend::AnalyticalNetwork::get_event_queue() noexcept {
    return AnalyticalNetwork::event_queue;
}

AnalyticalBackend::Topology& AnalyticalBackend::AnalyticalNetwork::get_topology() noexcept {
    return AnalyticalNetwork::topology;
}

int AnalyticalBackend::AnalyticalNetwork::sim_comm_size(AstraSim::sim_comm comm, int *size) {
    return 0;
}

int AnalyticalBackend::AnalyticalNetwork::sim_finish() {
    return 0;
}

double AnalyticalBackend::AnalyticalNetwork::sim_time_resolution() {
    return 0;
}

int AnalyticalBackend::AnalyticalNetwork::sim_init(AstraSim::AstraMemoryAPI *MEM) {
    return 0;
}

AstraSim::timespec_t AnalyticalBackend::AnalyticalNetwork::sim_get_time() {
    return event_queue.get_current_time();
}

void AnalyticalBackend::AnalyticalNetwork::sim_schedule(AstraSim::timespec_t delta, void (*fun_ptr)(void *),
                                                        void *fun_arg) {
    // 1. compute event_time = current_time + delta
    auto event_time = sim_get_time();

    // FIXME: assuming time_res is always NS
    event_time.time_val += delta.time_val;

    // 2. schedule an event at the event_time
    event_queue.add_event(event_time, fun_ptr, fun_arg);
}

int AnalyticalBackend::AnalyticalNetwork::sim_send(void *buffer, int count, int type, int dst, int tag,
                                                   AstraSim::sim_request *request, void (*msg_handler)(void *),
                                                   void *fun_arg) {
    // 1. get source id
    auto src = sim_comm_get_rank();

    // 2. compute number of required hops
    auto hopsCount = topology.get_hops_count(src, dst);

    // 3. compute latency in ns
    AstraSim::timespec_t delta;
    delta.time_res = AstraSim::NS;

    double link_latency_in_ns = (double)count / topology.get_bandwidth() * 1000000000;
    delta.time_val = (int)(link_latency_in_ns * hopsCount);

    // 4. schedule an event
    sim_schedule(delta, msg_handler, fun_arg);

    return 0;
}

int AnalyticalBackend::AnalyticalNetwork::sim_recv(void *buffer, int count, int type, int src, int tag,
                                                   AstraSim::sim_request *request, void (*msg_handler)(void *),
                                                   void *fun_arg) {
    // 1. get source id
    auto dst = sim_comm_get_rank();

    // 2. compute number of required hops
    auto hopsCount = topology.get_hops_count(src, dst);

    // 3. compute latency in ns
    AstraSim::timespec_t delta;
    delta.time_res = AstraSim::NS;

    double link_latency_in_ns = (double)count / topology.get_bandwidth() * 1000000000;
    delta.time_val = (int)(link_latency_in_ns * hopsCount);

    // 4. schedule an event
    sim_schedule(delta, msg_handler, fun_arg);

    return 0;
}
