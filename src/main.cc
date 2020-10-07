#include <iostream>
#include <fstream>
#include <memory>
#include <cmath>
#include <cassert>
#include "helper/CommandLineParser.hh"
#include "helper/json.hpp"
#include "event-queue/EventQueueEntry.hh"
#include "event-queue/EventQueue.hh"
#include "topology/Topology.hh"
#include "topology/TopologyConfiguration.hh"
#include "topology/Switch.hh"
#include "topology/Torus2D.hh"
#include "topology/Ring_AllToAll_Switch.hh"
#include "api/AnalyticalNetwork.hh"
#include "astra-sim/system/Sys.hh"
#include "astra-sim/system/memory/SimpleMemory.hh"


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
    cmd_parser.add_command_line_option<int>("dims-count", "Number of dimension");
    cmd_parser.add_command_line_multitoken_option<std::vector<int>>("nodes-per-dim", "Number of nodes per each dimension");
    cmd_parser.add_command_line_multitoken_option<std::vector<double>>("link-bandwidth", "Link bandwidth in GB/s (B/ns)");
    cmd_parser.add_command_line_multitoken_option<std::vector<double>>("link-latency", "Link latency in ns");
    cmd_parser.add_command_line_multitoken_option<std::vector<double>>("nic-latency", "NIC latency in ns");
    cmd_parser.add_command_line_multitoken_option<std::vector<double>>("router-latency", "Switch latency in ns");
    cmd_parser.add_command_line_multitoken_option<std::vector<double>>("hbm-bandwidth", "HBM bandwidth in GB/s (B/ns)");
    cmd_parser.add_command_line_multitoken_option<std::vector<double>>("hbm-latency", "HBM latency in ns");
    cmd_parser.add_command_line_multitoken_option<std::vector<double>>("hbm-scale", "HBM scale");
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

    int dims_count = json_configuration["topology-configuration"]["dims-count"];
    cmd_parser.set_if_defined("dims-count", &dims_count);

    std::vector<int> nodes_per_dim;
    for (int node_per_dim : json_configuration["topology-configuration"]["nodes-per-dim"]) {
        nodes_per_dim.emplace_back(node_per_dim);
    }
    cmd_parser.set_if_defined("nodes-per-dim", &nodes_per_dim);

    std::vector<double> link_bandwidths;
    for (double link_bandwidth : json_configuration["topology-configuration"]["link-bandwidth"]) {
        link_bandwidths.emplace_back(link_bandwidth);
    }
    cmd_parser.set_if_defined("link-bandwidth", &link_bandwidths);

    std::vector<double> link_latencies;
    for (double link_latency: json_configuration["topology-configuration"]["link-latency"]) {
        link_latencies.emplace_back(link_latency);
    }
    cmd_parser.set_if_defined("link-latency", &link_latencies);

    std::vector<double> nic_latencies;
    for (double nic_latency: json_configuration["topology-configuration"]["nic-latency"]) {
        nic_latencies.emplace_back(nic_latency);
    }
    cmd_parser.set_if_defined("nic-latency", &nic_latencies);

    std::vector<double> router_latencies;
    for (double router_latency: json_configuration["topology-configuration"]["router-latency"]) {
        router_latencies.emplace_back(router_latency);
    }
    cmd_parser.set_if_defined("router-latency", &router_latencies);

    std::vector<double> hbm_bandwidths;
    for (double hbm_bandwidth: json_configuration["topology-configuration"]["hbm-bandwidth"]) {
        hbm_bandwidths.emplace_back(hbm_bandwidth);
    }
    cmd_parser.set_if_defined("hbm-bandwidth", &hbm_bandwidths);

    std::vector<double> hbm_latencies;
    for (double hbm_latency: json_configuration["topology-configuration"]["hbm-latency"]) {
        hbm_latencies.emplace_back(hbm_latency);
    }
    cmd_parser.set_if_defined("hbm-latency", &hbm_latencies);

    std::vector<double> hbm_scales;
    for (double hbm_scale: json_configuration["topology-configuration"]["hbm-scale"]) {
        hbm_scales.emplace_back(hbm_scale);
    }
    cmd_parser.set_if_defined("hbm-scale", &hbm_scales);

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
    auto hosts_count = 1;
    for (auto node_per_dim : nodes_per_dim) {
        hosts_count *= node_per_dim;
    }

    std::unique_ptr<AnalyticalBackend::AnalyticalNetwork> analytical_networks[hosts_count];
    AstraSim::Sys *systems[hosts_count];
    std::unique_ptr<AstraSim::SimpleMemory> memories[hosts_count];

    // create topology configuration
    if (topology_name == "Ring_AllToAll_Switch") {
        assert(dims_count == 3 && "[Main] Ring_AllToAll_Switch Dimension doesn't match");

        std::vector<AnalyticalBackend::TopologyConfiguration> topology_configurations;
        for (int d = 0; d < dims_count; d++) {
            topology_configurations.emplace_back(
                    link_bandwidths[d],  // link bandwidth (GB/s) = (B/ns)
                    link_latencies[d],  // link latency (ns)
                    nic_latencies[d],  // nic latency (ns)
                    router_latencies[d],  // router latency (ns): ring doesn't use this
                    hbm_bandwidths[d],  // memory bandwidth (GB/s) = (B/ns)
                    hbm_latencies[d],  // memory latency (ns),
                    hbm_scales[d]  // memory scaling factor
            );
        }

        for (int i = 0; i < hosts_count; i++) {
            analytical_networks[i] = std::make_unique<AnalyticalBackend::AnalyticalNetwork>(i);

            memories[i] = std::make_unique<AstraSim::SimpleMemory>(
                    (AstraSim::AstraNetworkAPI *) (analytical_networks[i].get()),
                    500, 270, 12.5);

            systems[i] = new AstraSim::Sys(
                    analytical_networks[i].get(),  // AstraNetworkAPI
                    memories[i].get(),  // AstraMemoryAPI
                    i,  // id
                    num_passes,  // num_passes
                    nodes_per_dim[0], nodes_per_dim[2], nodes_per_dim[1], 1, 1,  // dimensions
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

        topology = std::make_shared<AnalyticalBackend::Ring_AllToAll_Switch>(
                topology_configurations,  // topology configuration
                nodes_per_dim  // number of connected nodes
        );
    } else if (topology_name == "switch") {
        assert(dims_count == 1 && "[Main] Switch Dimension doesn't match");

        auto topology_configuration = AnalyticalBackend::TopologyConfiguration(
                link_bandwidths[0],  // link bandwidth (GB/s) = (B/ns)
                link_latencies[0],  // link latency (ns)
                nic_latencies[0],  // nic latency (ns)
                router_latencies[0],  // router latency (ns): ring doesn't use this
                hbm_bandwidths[0],  // memory bandwidth (GB/s) = (B/ns)
                hbm_latencies[0],  // memory latency (ns),
                hbm_scales[0]  // memory scaling factor
        );

        for (int i = 0; i < hosts_count; i++) {
            analytical_networks[i] = std::make_unique<AnalyticalBackend::AnalyticalNetwork>(i);

            memories[i] = std::make_unique<AstraSim::SimpleMemory>(
                    (AstraSim::AstraNetworkAPI *) (analytical_networks[i].get()),
                    500, 270, 12.5);

            systems[i] = new AstraSim::Sys(
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
                topology_configuration,  // topology configuration
                nodes_per_dim[0]  // number of connected nodes
        );
    } else if (topology_name == "torus") {
        assert(dims_count == 1 && "[Main] Torus Dimension doesn't match");

        auto topology_configuration = AnalyticalBackend::TopologyConfiguration(
                link_bandwidths[0],  // link bandwidth (GB/s) = (B/ns)
                link_latencies[0],  // link latency (ns)
                nic_latencies[0],  // nic latency (ns)
                router_latencies[0],  // router latency (ns): ring doesn't use this
                hbm_bandwidths[0],  // memory bandwidth (GB/s) = (B/ns)
                hbm_latencies[0],  // memory latency (ns),
                hbm_scales[0]  // memory scaling factor
        );

        auto torus_width = (int)std::sqrt(hosts_count);

        for (int i = 0; i < hosts_count; i++) {
            analytical_networks[i] = std::make_unique<AnalyticalBackend::AnalyticalNetwork>(i);

            memories[i] = std::make_unique<AstraSim::SimpleMemory>(
                    (AstraSim::AstraNetworkAPI *) (analytical_networks[i].get()),
                    500, 270, 12.5);

            systems[i] = new AstraSim::Sys(
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
                topology_configuration,  // topology configurarion
                nodes_per_dim[0]  // number of hosts connected
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

//#include "topology/TopologyConfiguration.hh"
//#include "topology/Ring_AllToAll_Switch.hh"
//#include <vector>
//#include <iostream>
//
//using namespace AnalyticalBackend;
//int main() {
//    auto cs = std::vector<TopologyConfiguration>();
//    cs.emplace_back(1, 1, 0, false, 0, 1, 0, 0);
//    cs.emplace_back(1, 10, 0, false, 0, 1, 0, 0);
//    cs.emplace_back(1, 100, 10, true, 0, 1, 0, 0);
//    auto ds = std::vector<int>();
//    ds.emplace_back(2);
//    ds.emplace_back(8);
//    ds.emplace_back(2);
//    auto t = Ring_AllToAll_Switch(cs, ds);
//
//    for (int i = 1; i < 32; i += 2) {
//        auto j = i - 1;
//        auto l = t.simulateSend(i, j, 0);
//        std::cout << i << "->" << j << ": " << l << std::endl;
//    }
//
//    for (int i = 5; i < 32; i+= 2) {
//        auto j = i - 4;
//        auto l = t.simulateSend(i, j, 0);
//        std::cout << i << "->" << j << ": " << l << std::endl;
//    }
//
//    for (int i = 16; i < 32; i++) {
//        auto j = i - 16;
//        auto l = t.simulateSend(i, j, 0);
//        std::cout << i << "->" << j << ": " << l << std::endl;
//    }
//    return 0;
//}