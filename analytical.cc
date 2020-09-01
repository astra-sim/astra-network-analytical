#include "analytical.hh"
#include "astra-sim/system/Sys.hh"
static EventQueue Analytical::all_events;
int Analytical::sim_comm_size(sim_comm comm, int *size) {
    return 0;
}
int Analytical::sim_finish() {
    return 0;
}
double Analytical::sim_time_resolution() {
    return 0;
}
int Analytical::sim_init(AstraMemoryAPI *MEM) {
    return 0;
}

timespec_t Analytical::sim_get_time() {

}
void Analytical::sim_schedule(timespec_t delta, void (*fun_ptr)(void *fun_arg), void *fun_arg) {

}
int Analytical::sim_send(void *buffer, int count, int type, int dst, int tag, sim_request *request, void (*msg_handler)(void *fun_arg), void *fun_arg) {

}
int Analytical::sim_recv(void *buffer, int count, int type, int src, int tag, sim_request *request, void (*msg_handler)(void *fun_arg), void *fun_arg) {

}

int main(){
    Analytical *backends[64];
    Sys *systems[64];
    for(int i=0;i<64;i++){
        backends[i]=new Analytical();
        systems[i]=new Sys (backends[i], NULL, i, 2, 4, 4, 4, 1, 1,
                            2, 2, 2, 2, 2, "sample_torus_sys", "DLRM", 1, 1, 1,
                            1, 0, "results/", "test");
    }
    for(int i=0;i<64;i++){
        systems[i]->workload->fire();
    }
    while(Analytical::all_events.size()>0){
        Event event=Analytical::all_events.front();
        (*(event.fun_ptr))(event.fun_arg);
        Analytical::all_events.pop_front();
    }
    return;
}