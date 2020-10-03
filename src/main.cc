#include <iostream>
#include <fstream>
#include <memory>
#include <cmath>
#include "helper/CommandLineParser.hh"
#include "helper/json.hpp"
#include "event-queue/EventQueueEntry.hh"
#include "event-queue/EventQueue.hh"
#include "topology/Topology.hh"
#include "topology/TopologyConfiguration.hh"
#include "topology/Switch.hh"
#include "topology/Torus2D.hh"
#include "api/AnalyticalNetwork.hh"
#include "src/astra-sim/system/Sys.hh"
#include "src/astra-sim/system/SimpleMemory.hh"


namespace po = boost::program_options;

int main(int argc, char* argv[]) {
    // parse configuration.json file
    auto json_file = std::ifstream("../../../Configuration.json", std::ifstream::in);
    nlohmann::json json_configuration;
    json_file >> json_configuration;
    json_file.close();

    // parse command line arguments -- to update configurations if given through command line
    auto cmd_parser = AnalyticalBackend::CommandLineParser();

    cmd_parser.add_command_line_option<std::string>("system-configuration", "System configuration file");
    cmd_parser.add_command_line_option<std::string>("workload-configuration", "Workload configuration file");
    cmd_parser.add_command_line_option<std::string>("topology-name", "Topology name");
    cmd_parser.add_command_line_option<int>("host-count", "Number of hosts");
    cmd_parser.add_command_line_option<double>("bandwidth", "Link bandwidth in GB/s (B/ns)");
    cmd_parser.add_command_line_option<double>("link-latency", "Link latency in ns");
    cmd_parser.add_command_line_option<double>("nic-latency", "NIC latency in ns");
    cmd_parser.add_command_line_option<double>("switch-latency", "Switch latency in ns");
    cmd_parser.add_command_line_option<bool>("print-topology-log", "Print topology log to std::out");
    cmd_parser.add_command_line_option<int>("num-passes", "Number of passes to run");
    cmd_parser.add_command_line_option<int>("num-queues-per-dim", "Number of queues per each dimension");
    cmd_parser.add_command_line_option<float>("comm-scale", "Communication scale");
    cmd_parser.add_command_line_option<float>("compute-scale", "Compute scale");
    cmd_parser.add_command_line_option<float>("injection-scale", "Injection scale");
    cmd_parser.add_command_line_option<std::string>("path", "Path to save result files");
    cmd_parser.add_command_line_option<std::string>("run-name", "Run name");
    cmd_parser.add_command_line_option<int>("total-stat-rows", "Total number of concurrent runs");
    cmd_parser.add_command_line_option<int>("stat-row", "Index of current run (index starts with 0)");
    cmd_parser.add_command_line_option<bool>("rendezvous-protocol", "Whether to enable rendezvous protocol");

    try {
        cmd_parser.parse(argc, argv);
    } catch (const AnalyticalBackend::CommandLineParser::ParsingError& e) {
        std::cout << e.what() << std::endl;
        exit(-1);
    }

    cmd_parser.print_help_message_if_required();


    // configuration variables
    std::string system_configuration = json_configuration["system-configuration"];
    cmd_parser.set_if_defined("system-configuration", &system_configuration);

    std::string workload_configuration = json_configuration["workload-configuration"];
    cmd_parser.set_if_defined("workload-configuration", &workload_configuration);

    std::string topology_name = json_configuration["topology-configuration"]["topology-name"];
    cmd_parser.set_if_defined("topology-name", &topology_name);

    int hosts_count = json_configuration["topology-configuration"]["host-count"];
    cmd_parser.set_if_defined("host-count", &hosts_count);

    double bandwidth = json_configuration["topology-configuration"]["bandwidth"];
    cmd_parser.set_if_defined("bandwidth", &bandwidth);

    double link_latency = json_configuration["topology-configuration"]["link-latency"];
    cmd_parser.set_if_defined("link-latency", &link_latency);

    double nic_latency = json_configuration["topology-configuration"]["nic-latency"];
    cmd_parser.set_if_defined("nic-latency", &nic_latency);

    double switch_latency = json_configuration["topology-configuration"]["switch-latency"];
    cmd_parser.set_if_defined("switch-latency", &switch_latency);

    bool print_topology_log = json_configuration["topology-configuration"]["print-topology-log"];
    cmd_parser.set_if_defined("print-topology-log", &print_topology_log);

    int num_passes = json_configuration["run-configuration"]["num-passes"];
    cmd_parser.set_if_defined("num-passes", &num_passes);

    int num_queues_per_dim = json_configuration["run-configuration"]["num-queues-per-dim"];
    cmd_parser.set_if_defined("num-queues-per-dim", &num_queues_per_dim);

    float comm_scale = json_configuration["run-configuration"]["comm-scale"];
    cmd_parser.set_if_defined("comm-scale", &comm_scale);

    float compute_scale = json_configuration["run-configuration"]["compute-scale"];
    cmd_parser.set_if_defined("compute-scale", &compute_scale);

    float injection_scale = json_configuration["run-configuration"]["injection-scale"];
    cmd_parser.set_if_defined("injection-scale", &injection_scale);

    std::string path = json_configuration["stat-configuration"]["path"];
    cmd_parser.set_if_defined("path", &path);

    std::string run_name = json_configuration["stat-configuration"]["run-name"];
    cmd_parser.set_if_defined("run-name", &run_name);

    int total_stat_rows = json_configuration["stat-configuration"]["total-stat-rows"];
    cmd_parser.set_if_defined("total-stat-rows", &total_stat_rows);

    int stat_row = json_configuration["stat-configuration"]["stat-row"];
    cmd_parser.set_if_defined("stat-row", &stat_row);

    bool rendezvous_protocol = json_configuration["rendezvous-protocol"];
    cmd_parser.set_if_defined("rendezvous-protocol", &rendezvous_protocol);


    // event queue instantiation
    auto event_queue = std::make_shared<AnalyticalBackend::EventQueue>();

    // pointer to topology
    std::shared_ptr<AnalyticalBackend::Topology> topology;

    // Network and System layer initialization
    std::unique_ptr<AnalyticalBackend::AnalyticalNetwork> analytical_networks[hosts_count];
    std::unique_ptr<AstraSim::Sys> systems[hosts_count];
    std::unique_ptr<AstraSim::SimpleMemory> memories[hosts_count];

    // create topology configuration
    auto topology_configuration = AnalyticalBackend::TopologyConfiguration(
            bandwidth,  // link bandwidth (GB/s) = (B/ns)
            link_latency,  // link latency (ns)
            nic_latency,  // nic latency (ns)
            switch_latency,  // router latency (ns): ring doesn't use this
            500,  // memory bandwidth (GB/s) = (B/ns)
            10,  // memory latency (ns),
            1.5  // memory scaling factor
    );

    if (topology_name == "switch") {
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
                    "../../../../../inputs/system/" + system_configuration,  // system configuration
                    "../../../../../inputs/workload/" + workload_configuration,  // workload configuration
                    comm_scale, compute_scale, injection_scale,  // communication, computation, injection scale
                    total_stat_rows, stat_row,  // total_stat_rows and stat_row
                    "../../../result/" + path,  // stat file path
                    run_name,  // run name
                    true,    // separate_log
                    rendezvous_protocol  // randezvous protocol
            );
        }

        topology = std::make_shared<AnalyticalBackend::Switch>(
                hosts_count,  // number of connected nodes
                topology_configuration  // topology configuration
        );
    } else if (topology_name == "torus") {
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
                    "../../../../../inputs/system/" + system_configuration,  // system configuration
                    "../../../../../inputs/workload/" + workload_configuration,  // workload configuration
                    comm_scale, compute_scale, injection_scale,  // communication, computation, injection scale
                    total_stat_rows, stat_row,  // total_stat_rows and stat_row
                    "../../../result/" + path,  // stat file path
                    run_name,  // run name
                    true,    // separate_log
                    rendezvous_protocol  // randezvous protocol
            );
        }

        topology = std::make_shared<AnalyticalBackend::Torus2D>(
                hosts_count,  // number of hosts connected
                topology_configuration  // topology configurarion
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

//    // After run, print topology stats
//    if (print_topology_log) {
//        topology->print();
//    }

    // terminate program
    return 0;
}
