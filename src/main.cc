#include <iostream>
#include <memory>
#include <cmath>
#include "Event.hh"
#include "EventQueueEntry.hh"
#include "EventQueue.hh"
#include "Topology.hh"
#include "Torus.hh"
#include "Switch.hh"
#include "../astra-sim/system/AstraNetworkAPI.hh"
#include "../astra-sim/system/Sys.hh"
#include "../astra-sim/system/SimpleMemory.hh"
#include "AnalyticalNetwork.hh"

int main(int argc, char *argv[]) {
    // configuration variables
    auto hosts_count = 16;

    // derived configurations
    int torus_width = std::sqrt(hosts_count);

    // Network and System layer initialization
    std::unique_ptr<AnalyticalBackend::AnalyticalNetwork> analytical_networks[hosts_count];
    std::unique_ptr<AstraSim::Sys> systems[hosts_count];
    std::unique_ptr<AstraSim::SimpleMemory> memories[hosts_count];

    for (int i = 0; i < hosts_count; i++) {
        analytical_networks[i] = std::make_unique<AnalyticalBackend::AnalyticalNetwork>(i);

        memories[i] = std::make_unique<AstraSim::SimpleMemory>(
                (AstraSim::AstraNetworkAPI*)(analytical_networks[i].get()),
                500, 270, 12.5);

        systems[i] = std::make_unique<AstraSim::Sys>(
                analytical_networks[i].get(),  // AstraNetworkAPI
                memories[i].get(),  // AstraMemoryAPI
                i,  // id
                2,  // num_passes
                hosts_count, 1, 1, 1, 1,  // dimensions
                2, 2, 2, 2, 2,  // queues per corresponding dimension
                "sys_inputs/sample_a2a_sys",  // system configuration
                "workload_inputs/DLRM_HybridParallel",  // workload configuration
                1, 1, 1,  // communication, computation, injection scale
                1, 0,  // total_stat_rows and stat_row
                "../results/",  // stat file path
                "sample_run",  // run name
                true,    // separate_log
                false  // randezvous protocol
        );
    }

    // Setup event queue
    auto event_queue = std::make_shared<AnalyticalBackend::EventQueue>();
    AnalyticalBackend::AnalyticalNetwork::set_event_queue(event_queue);

    // Setup topology
    std::shared_ptr<AnalyticalBackend::Topology> topology = std::make_shared<AnalyticalBackend::Switch>(
            hosts_count,  // number of connected nodes
            25,  // bandwidth (GB/s = B/ns)
            10,  // link latency (ns)
            10,  // nic latency (ns)
            10  // switch latency (ns)
    );
    AnalyticalBackend::AnalyticalNetwork::set_topology(topology);

    // Initialize event queue
    for (int i = 0; i < hosts_count; i++) {
        systems[i]->workload->fire();
    }

    // Run events
    while (!event_queue->empty()) {
        event_queue->proceed();
    }

    topology->print();

    return 0;
}
