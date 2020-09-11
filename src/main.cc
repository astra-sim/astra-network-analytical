#include <iostream>
#include "Event.hh"
#include "EventQueueEntry.hh"
#include "EventQueue.hh"
#include "../astra-sim/system/AstraNetworkAPI.hh"


#include "../astra-sim/system/Sys.hh"
#include "../astra-sim/system/SimpleMemory.hh"
#include "AnalyticalNetwork.hh"

int main(int argc, char *argv[]) {
    // configuration variables
    int hosts_count = 16;

    // Network and System layer initialization
    AnalyticalBackend::AnalyticalNetwork *analytical_networks[hosts_count];
    AstraSim::Sys* systems[hosts_count];
    AstraSim::SimpleMemory* memories[hosts_count];

    for (int i = 0; i < hosts_count; i++) {

        analytical_networks[i] = new AnalyticalBackend::AnalyticalNetwork(i);
        memories[i] = new AstraSim::SimpleMemory((AstraSim::AstraNetworkAPI*)analytical_networks[i],500,270,12.5);
        systems[i] = new AstraSim::Sys(analytical_networks[i],  // AstraNetworkAPI
                                       memories[i],  // AstraMemoryAPI
                                       i,  // id
                                       2,  // num_passes
                                       1, 4, 4, 1, 1,  // dimensions
                                       2, 2, 2, 2, 2,  // queues per correspondingdimension
                                       "sample_torus_sys",  // system configuration
                                       "microAllReduce",  // workload configuration
                                       1, 1, 1,  // communication, computation, injection scale
                                       1, 0,  // total_stat_rows and stat_row
                                       "results/",  // stat file path
                                       "sample_run",  // run name
                                       false    // separate_log
        );
    }

    // Setup topology
    auto topology = AnalyticalBackend::AnalyticalNetwork::get_topology();
    topology.initialize(4, 10000000);

    // Initialize event queue
    for (int i = 0; i < hosts_count; i++) {
        systems[i]->workload->fire();
    }

    // Run events
    auto event_queue = AnalyticalBackend::AnalyticalNetwork::get_event_queue();
    while (!event_queue.empty()) {
        event_queue.proceed();
    }

    // Cleanup before termination
    for (int i = 0; i < hosts_count; i++) {
        delete analytical_networks[i];
        delete systems[i];
        delete memories[i];
    }

    return 0;
}
