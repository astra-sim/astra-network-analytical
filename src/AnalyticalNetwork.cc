#include "AnalyticalNetwork.hh"

std::shared_ptr<AnalyticalBackend::EventQueue> AnalyticalBackend::AnalyticalNetwork::event_queue;

std::unique_ptr<AnalyticalBackend::Topology> AnalyticalBackend::AnalyticalNetwork::topology;

void AnalyticalBackend::AnalyticalNetwork::set_event_queue(const std::shared_ptr<EventQueue> &event_queue_ptr) noexcept {
    AnalyticalNetwork::event_queue = event_queue_ptr;
}

void AnalyticalBackend::AnalyticalNetwork::set_topology(Topology *new_topology) noexcept {
    AnalyticalNetwork::topology.reset(new_topology);
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
    return event_queue->get_current_time();
}

void AnalyticalBackend::AnalyticalNetwork::sim_schedule(AstraSim::timespec_t delta, void (*fun_ptr)(void *),
                                                        void *fun_arg) {
    // 1. compute event_time = current_time + delta
    auto event_time = sim_get_time();

    // FIXME: assuming time_res is always NS
    event_time.time_val += delta.time_val;

    // 2. schedule an event at the event_time
    event_queue->add_event(event_time, fun_ptr, fun_arg);
}

int AnalyticalBackend::AnalyticalNetwork::sim_send(void *buffer, int count, int type, int dst, int tag,
                                                   AstraSim::sim_request *request, void (*msg_handler)(void *),
                                                   void *fun_arg) {
    // 1. get source id
    auto src = sim_comm_get_rank();

    // 2. compute latency in ns
    AstraSim::timespec_t delta;
    delta.time_res = AstraSim::NS;
    delta.time_val = topology->send(src, dst, count);  // simulate src->dst and get latency

    // 3. schedule an event
    sim_schedule(delta, msg_handler, fun_arg);

    return 0;
}

int AnalyticalBackend::AnalyticalNetwork::sim_recv(void *buffer, int count, int type, int src, int tag,
                                                   AstraSim::sim_request *request, void (*msg_handler)(void *),
                                                   void *fun_arg) {
    // 1. get source id
    auto dst = sim_comm_get_rank();

    // 2. compute latency in ns
    AstraSim::timespec_t delta;
    delta.time_res = AstraSim::NS;
    delta.time_val = topology->send(src, dst, count);  // simulate src->dst and get latency

    // 3. schedule an event
    sim_schedule(delta, msg_handler, fun_arg);

    return 0;
}
