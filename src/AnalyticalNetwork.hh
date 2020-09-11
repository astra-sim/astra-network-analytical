#ifndef __ANALYTICALNETWORK_HH__
#define __ANALYTICALNETWORK_HH__

#include "../astra-sim/system/AstraNetworkAPI.hh"
#include "EventQueue.hh"
#include "Topology.hh"

namespace AnalyticalBackend {
    class AnalyticalNetwork : public AstraSim::AstraNetworkAPI {
    public:
        static EventQueue& get_event_queue() noexcept;

        static Topology& get_topology() noexcept;
        /**
         * ========================= AstraNetworkAPIs =================================================
         */
        int sim_comm_size(AstraSim::sim_comm comm, int* size) override;
        
        int sim_finish() override;
        
        double sim_time_resolution() override;
        
        int sim_init(AstraSim::AstraMemoryAPI* MEM) override;
        
        AstraSim::timespec_t sim_get_time() override;
        
        void sim_schedule(AstraSim::timespec_t delta, void (*fun_ptr)(void *fun_arg), void *fun_arg) override;
        
        int sim_send(void *buffer, int count, int type, int dst, int tag, AstraSim::sim_request *request, void (*msg_handler)(void *fun_arg), void* fun_arg) override;
        
        int sim_recv(void *buffer, int count, int type, int src, int tag, AstraSim::sim_request *request, void (*msg_handler)(void *fun_arg), void* fun_arg) override;
        /**
         * ===========================================================================================
         */

        AnalyticalNetwork(int rank) : AstraSim::AstraNetworkAPI(rank) { }

    private:
        static EventQueue event_queue;
        static Topology topology;
    };
}

#endif
