#include <iostream>
#include <fstream>
#include <memory>
#include <cmath>
#include <boost/program_options.hpp>
#include "Event.hh"
#include "EventQueueEntry.hh"
#include "EventQueue.hh"
#include "Topology.hh"
#include "Torus.hh"
#include "Switch.hh"
#include "json.hpp"
#include "../astra-sim/system/AstraNetworkAPI.hh"
#include "../astra-sim/system/Sys.hh"
#include "../astra-sim/system/SimpleMemory.hh"
#include "AnalyticalNetwork.hh"

namespace po = boost::program_options;

int main(int argc, char* argv[]) {
    // parse command line arguments
    auto cmd_description = po::options_description("Command Line Options");
    cmd_description.add_options()
            ("help", "Prints help message")
            ("comm-scale", po::value<int>()->default_value(1), "Communication scale")
            ("compute-scale", po::value<int>()->default_value(1), "Compute scale")
            ("injection-scale", po::value<int>()->default_value(1), "Injection scale");

    po::variables_map vm;
    try {
        po::store(po::parse_command_line(argc, argv, cmd_description), vm);
        po::notify(vm);
    } catch (po::error e) {
        std::cout << e.what() << std::endl;
        std::cout << cmd_description << std::endl;
        exit(-1);
    }

    if (vm.count("help")) {
        // prints help message
        std::cout << cmd_description << std::endl;
    }

    // command line configuration variables
    int comm_scale = vm["comm-scale"].as<int>();
    int compute_scale = vm["compute-scale"].as<int>();
    int injection_scale = vm["injection-scale"].as<int>();

    // parse configuration file
    auto json_file = std::ifstream("Configuration.json", std::ifstream::in);
    nlohmann::json configuration;
    json_file >> configuration;
    json_file.close();

    // configuration variables
    std::string system_configuration = configuration["system_configuration"];
    std::string workload_configuration = configuration["workload_configuration"];
    std::string topology_name = configuration["topology_configuration"]["name"];
    int hosts_count = configuration["topology_configuration"]["hosts_count"];
    int bandwidth = configuration["topology_configuration"]["bandwidth"];
    int link_latency = configuration["topology_configuration"]["link_latency"];
    int nic_latency = configuration["topology_configuration"]["nic_latency"];
    int switch_latency = configuration["topology_configuration"]["switch_latency"];
    bool print_log = configuration["topology_configuration"]["print_log"];
    int num_passes = configuration["run_configuration"]["num_passes"];
    int num_queues_per_dim = configuration["run_configuration"]["num_queues_per_dim"];
    std::string path = configuration["stat_configuration"]["path"];
    std::string run_name = configuration["stat_configuration"]["run_name"];
    int total_stat_rows = configuration["stat_configuration"]["total_stat_rows"];
    int stat_row = configuration["stat_configuration"]["stat_row"];
    bool rendezvous_protocol = configuration["rendezvous_protocol"];

    // event queue instantiation
    auto event_queue = std::make_shared<AnalyticalBackend::EventQueue>();

    // pointer to topology
    std::shared_ptr<AnalyticalBackend::Topology> topology;

    // Network and System layer initialization
    std::unique_ptr<AnalyticalBackend::AnalyticalNetwork> analytical_networks[hosts_count];
    std::unique_ptr<AstraSim::Sys> systems[hosts_count];
    std::unique_ptr<AstraSim::SimpleMemory> memories[hosts_count];

    // Setup Network and System by topology
    if (topology_name.compare("switch") == 0) {
        for (int i = 0; i < hosts_count; i++) {
            analytical_networks[i] = std::make_unique<AnalyticalBackend::AnalyticalNetwork>(i);

            memories[i] = std::make_unique<AstraSim::SimpleMemory>(
                    (AstraSim::AstraNetworkAPI *) (analytical_networks[i].get()),
                    500, 270, 12.5);

            systems[i] = std::make_unique<AstraSim::Sys>(
                    analytical_networks[i].get(),  // AstraNetworkAPI
                    memories[i].get(),  // AstraMemoryAPI
                    i,  // id
                    num_passes,  // num_passes
                    1, 1, hosts_count, 1, 1,  // dimensions
                    num_queues_per_dim, num_queues_per_dim, num_queues_per_dim, num_queues_per_dim, num_queues_per_dim,  // queues per corresponding dimension
                    system_configuration,  // system configuration
                    workload_configuration,  // workload configuration
                    comm_scale, compute_scale, injection_scale,  // communication, computation, injection scale
                    total_stat_rows, stat_row,  // total_stat_rows and stat_row
                    path,  // stat file path
                    run_name,  // run name
                    true,    // separate_log
                    rendezvous_protocol  // randezvous protocol
            );
        }

        topology = std::make_shared<AnalyticalBackend::Switch>(
                hosts_count,  // number of connected nodes
                bandwidth,  // bandwidth (GB/s = B/ns)
                link_latency,  // link latency (ns)
                nic_latency,  // nic latency (ns)
                switch_latency  // switch latency (ns)
        );
    } else if (topology_name.compare("torus") == 0) {
        auto torus_width = (int)std::sqrt(hosts_count);

        for (int i = 0; i < hosts_count; i++) {
            analytical_networks[i] = std::make_unique<AnalyticalBackend::AnalyticalNetwork>(i);

            memories[i] = std::make_unique<AstraSim::SimpleMemory>(
                    (AstraSim::AstraNetworkAPI *) (analytical_networks[i].get()),
                    500, 270, 12.5);

            systems[i] = std::make_unique<AstraSim::Sys>(
                    analytical_networks[i].get(),  // AstraNetworkAPI
                    memories[i].get(),  // AstraMemoryAPI
                    i,  // id
                    num_passes,  // num_passes
                    1, torus_width, torus_width, 1, 1,  // dimensions
                    num_queues_per_dim, num_queues_per_dim, num_queues_per_dim, num_queues_per_dim, num_queues_per_dim,  // queues per corresponding dimension
                    system_configuration,  // system configuration
                    workload_configuration,  // workload configuration
                    comm_scale, compute_scale, injection_scale,  // communication, computation, injection scale
                    total_stat_rows, stat_row,  // total_stat_rows and stat_row
                    path,  // stat file path
                    run_name,  // run name
                    true,    // separate_log
                    rendezvous_protocol  // randezvous protocol
            );
        }

        topology = std::make_shared<AnalyticalBackend::Torus>(
                hosts_count,  // number of hosts connected
                bandwidth,  // bandwidth (GB/s = B/ns)
                link_latency,  // link latency (ns)
                nic_latency  // nic latency (ns)
        );
    } else {
        std::cout << "Topology not implemented!" << std::endl;
        exit(-1);
    }

    // link event queue and topology
    AnalyticalBackend::AnalyticalNetwork::set_event_queue(event_queue);
    AnalyticalBackend::AnalyticalNetwork::set_topology(topology);

    // Initialize event queue
    for (int i = 0; i < hosts_count; i++) {
        systems[i]->workload->fire();
    }

    // Run events
    while (!event_queue->empty()) {
        event_queue->proceed();
    }

    // After run, print topology stats
    if (print_log) {
        topology->print();
    }

    // terminate program
    return 0;
}
