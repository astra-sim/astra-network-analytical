/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include <array>
#include <cassert>
#include <cmath>
#include <iostream>
#include <memory>
#include "api/AnalyticalNetwork.hh"
#include "astra-sim/system/Sys.hh"
#include "astra-sim/system/memory/SimpleMemory.hh"
#include "astra-sim/workload/CSVWriter.hh"
#include "event-queue/EventQueue.hh"
#include "event-queue/EventQueueEntry.hh"
#include "extern/network_backend/analytical/analytical/topology/HierarchicalTopology.hh"
#include "extern/network_backend/analytical/analytical/topology/HierarchicalTopologyConfig.hh"
#include "helper/CommandLineParser.hh"
#include "helper/NetworkConfigParser.hh"
#include "helper/json.hh"
#include "topology/CostModel.hh"
#include "topology/Topology.hh"
#include "topology/TopologyConfig.hh"

namespace po = boost::program_options;

int main(int argc, char* argv[]) {
  /**
   * Configuration parsing
   */
  auto cmd_parser = Analytical::CommandLineParser();

  // Define command line arguments here
  cmd_parser.add_command_line_option<std::string>(
      "network-configuration", "Network configuration file");
  cmd_parser.add_command_line_option<std::string>(
      "system-configuration", "System configuration file");
  cmd_parser.add_command_line_option<std::string>(
      "workload-configuration", "Workload configuration file");
  cmd_parser.add_command_line_option<int>(
      "num-passes", "Number of passes to run");
  cmd_parser.add_command_line_option<int>(
      "num-queues-per-dim", "Number of queues per each dimension");
  cmd_parser.add_command_line_option<float>(
      "comm-scale", "Communication scale");
  cmd_parser.add_command_line_option<float>("compute-scale", "Compute scale");
  cmd_parser.add_command_line_option<float>(
      "injection-scale", "Injection scale");
  cmd_parser.add_command_line_option<std::string>(
      "path", "Path to save result files");
  cmd_parser.add_command_line_option<std::string>("run-name", "Run name");
  cmd_parser.add_command_line_option<int>(
      "total-stat-rows", "Total number of concurrent runs");
  cmd_parser.add_command_line_option<int>(
      "stat-row", "Index of current run (index starts with 0)");
  cmd_parser.add_command_line_option<bool>(
      "rendezvous-protocol", "Whether to enable rendezvous protocol");

  // Define network-related command line arguments here
  cmd_parser.add_command_line_multitoken_option<std::vector<int>>(
      "units-count", "Units count per each dimension");
  cmd_parser.add_command_line_multitoken_option<std::vector<int>>(
      "links-count", "Links count per each dimension");
  cmd_parser.add_command_line_multitoken_option<std::vector<double>>(
      "link-bandwidth", "Link bandwidth per each dimension");
  cmd_parser.add_command_line_multitoken_option<std::vector<double>>(
      "link-latency", "Link latency per each dimension");

  // Parse command line arguments
  try {
    cmd_parser.parse(argc, argv);
  } catch (const Analytical::CommandLineParser::ParsingError& e) {
    std::cout
        << "[Analytical, main] Command line argument parsing error caputured. Error message: "
        << std::endl;
    std::cout << e.what() << std::endl;
    exit(-1);
  }

  cmd_parser.print_help_message_if_required();

  // 1. Retrieve network-agnostic configs
  std::string system_configuration = "system path not defined";
  cmd_parser.set_if_defined("system-configuration", &system_configuration);

  std::string workload_configuration = "workload path not defined";
  cmd_parser.set_if_defined("workload-configuration", &workload_configuration);

  int num_passes = 1;
  cmd_parser.set_if_defined("num-passes", &num_passes);

  int num_queues_per_dim = 1;
  cmd_parser.set_if_defined("num-queues-per-dim", &num_queues_per_dim);

  float comm_scale = 1;
  cmd_parser.set_if_defined("comm-scale", &comm_scale);

  float compute_scale = 1;
  cmd_parser.set_if_defined("compute-scale", &compute_scale);

  float injection_scale = 1;
  cmd_parser.set_if_defined("injection-scale", &injection_scale);

  std::string path = "path not defined";
  cmd_parser.set_if_defined("path", &path);

  std::string run_name = "unnamed run";
  cmd_parser.set_if_defined("run-name", &run_name);

  int total_stat_rows = 1;
  cmd_parser.set_if_defined("total-stat-rows", &total_stat_rows);

  int stat_row = 0;
  cmd_parser.set_if_defined("stat-row", &stat_row);

  bool rendezvous_protocol = false;
  cmd_parser.set_if_defined("rendezvous-protocol", &rendezvous_protocol);

  // 2. Retrieve network configs
  std::string network_configuration = "";
  cmd_parser.set_if_defined("network-configuration", &network_configuration);
  if (network_configuration.empty()) {
    std::cout << "[Analytical, main] Network configuration file path not given!"
              << std::endl;
    exit(-1);
  }

  // parse configuration.json file
  auto network_parser = Analytical::NetworkConfigParser(network_configuration);
  auto dimensions_count = network_parser.get<int>("dimensions-count", 1);
  std::vector<double> zeros = std::vector<double>(dimensions_count, 0);
  std::vector<double> ones = std::vector<double>(dimensions_count, 1);

  auto topology_name = network_parser.get<std::string>("topology-name", "Hierarchical");
  
  auto units_counts = network_parser.get<std::vector<int>>("units-count");
  cmd_parser.set_if_defined("units-count", &units_counts);
  auto link_latencies = network_parser.get<std::vector<double>>("link-latency");
  cmd_parser.set_if_defined("link-latency", &link_latencies);
  auto link_bandwidths =
      network_parser.get<std::vector<double>>("link-bandwidth");
  cmd_parser.set_if_defined("link-bandwidth", &link_bandwidths);
  auto nic_latencies = network_parser.get<std::vector<double>>("nic-latency", zeros);
  auto router_latencies =
      network_parser.get<std::vector<double>>("router-latency", zeros);
  auto hbm_latencies = network_parser.get<std::vector<double>>("hbm-latency", zeros);
  auto hbm_bandwidths =
      network_parser.get<std::vector<double>>("hbm-bandwidth", ones);
  auto hbm_scales = network_parser.get<std::vector<double>>("hbm-scale", zeros);

  /**
   * Instantitiation: Event Queue, System, Memory, Topology, etc.
   */
  // event queue instantiation
  auto event_queue = std::make_shared<Analytical::EventQueue>();

  // compute total number of npus by multiplying counts of each dimension
  auto npus_count = 1;
  for (auto units_count : units_counts) {
    npus_count *= units_count;
  }

  // number of nodes for each system layer dimension
  auto physical_dims = std::vector<int>();

  // Network and System layer initialization
  std::unique_ptr<Analytical::AnalyticalNetwork>
      analytical_networks[npus_count];
  AstraSim::Sys* systems[npus_count];
  std::unique_ptr<AstraSim::SimpleMemory> memories[npus_count];

  // pointer to topology
  std::shared_ptr<Analytical::Topology> topology;

  // topology configuration for each dimension
  auto topology_configs = Analytical::Topology::TopologyConfigs();
  for (int i = 0; i < dimensions_count; i++) {
    auto link_bandwidth_b_ns = (double)link_bandwidths[i] * (1 << 30) /
        (1'000'000'000); // link bandwidth in B/ns

    topology_configs.emplace_back(
        units_counts[i], // NPUs count
        link_latencies[i], // link latency (ns)
        link_bandwidth_b_ns, // link bandwidth (B/ns)
        nic_latencies[i], // nic latency (ns)
        router_latencies[i], // router latency (ns)
        hbm_latencies[i], // memory latency (ns),
        hbm_bandwidths[i], // memory bandwidth (GB/s) = (B/ns)
        hbm_scales[i] // memory scaling factor
    );
  }

  // Instantiate topology
  if (topology_name == "Hierarchical") {
    // Parse networks per each dimension
    auto topologies_per_dim = network_parser.parseHierarchicalTopologyList();
    auto dimension_types = network_parser.parseHierarchicalDimensionType();
    auto links_count_per_dim = network_parser.parseLinksCountPerDim();
    cmd_parser.set_if_defined("links-count", &links_count_per_dim);

    auto hierarchy_config = Analytical::HierarchicalTopologyConfig(
        dimensions_count,
        topologies_per_dim,
        dimension_types,
        links_count_per_dim,
        link_bandwidths);

    topology = std::make_shared<Analytical::HierarchicalTopology>(
        topology_configs, hierarchy_config);
    for (int dim = 0; dim < dimensions_count; dim++) {
      physical_dims.emplace_back(units_counts[dim]);
    }
  } else {
    std::cout << "[Analytical, main] Topology not defined: " << topology_name
              << std::endl;
    exit(-1);
  }

  // Retrieve cost_model
  auto& cost_model = topology->getCostModel();

  // Instantiate required network, memory, and system layers
  auto queues_per_dim = std::vector<int>(dimensions_count, num_queues_per_dim);

  // link event queue and topology
  Analytical::AnalyticalNetwork::setEventQueue(event_queue);
  Analytical::AnalyticalNetwork::setTopology(topology);
  Analytical::AnalyticalNetwork::setCostModel(&cost_model);

  for (int i = 0; i < npus_count; i++) {
    analytical_networks[i] =
        std::make_unique<Analytical::AnalyticalNetwork>(i, dimensions_count);

    memories[i] = std::make_unique<AstraSim::SimpleMemory>(
        (AstraSim::AstraNetworkAPI*)(analytical_networks[i].get()),
        1,
        500000,
        12.5);

    systems[i] = new AstraSim::Sys(
        analytical_networks[i].get(), // AstraNetworkAPI
        memories[i].get(), // AstraMemoryAPI
        i, // id
        num_passes, // num_passes
        physical_dims, // dimensions
        queues_per_dim, // queues per corresponding dimension
        system_configuration, // system configuration
        workload_configuration, // workload configuration
        comm_scale,
        compute_scale,
        injection_scale, // communication, computation, injection scale
        total_stat_rows,
        stat_row, // total_stat_rows and stat_row
        path, // stat file path
        run_name, // run name
        true, // separate_log
        rendezvous_protocol // randezvous protocol
    );
  }

  // link csv
  auto end_to_env_csv =
      std::make_shared<AstraSim::CSVWriter>(path, "backend_end_to_end.csv");
  auto dimensional_info_csv =
      std::make_shared<AstraSim::CSVWriter>(path, "backend_dim_info.csv");
  auto tutorial_csv =
      std::make_shared<AstraSim::CSVWriter>(path, "tutorial_result.csv");
  if (stat_row == 0) {
    end_to_env_csv->initialize_csv(total_stat_rows + 1, 15);

    end_to_env_csv->write_cell(0, 0, "RunName");
    end_to_env_csv->write_cell(0, 1, "CommsTime");
    end_to_env_csv->write_cell(0, 2, "ComputeTime");
    end_to_env_csv->write_cell(0, 3, "ExposedCommsTime");
    end_to_env_csv->write_cell(0, 4, "Cost");
    end_to_env_csv->write_cell(0, 5, "TotalPayloadSize");
    end_to_env_csv->write_cell(0, 6, "DPCommsTime");
    end_to_env_csv->write_cell(0, 7, "MPCommsTime");
    for (auto dim = 0; dim < 7; dim++) {
      end_to_env_csv->write_cell(
          0, (8 + dim), "PayloadSize_Dim" + std::to_string(dim));
    }

    // fixme: assuming max_dimension is 10
    // fixme: dimensions_count for every topology differs
    auto dimension_csv_rows_count = (total_stat_rows * 10) + 1;
    dimensional_info_csv->initialize_csv(dimension_csv_rows_count, 3);
    dimensional_info_csv->write_cell(0, 0, "RunName");
    dimensional_info_csv->write_cell(0, 1, "DimensionIndex");
    dimensional_info_csv->write_cell(0, 2, "AverageChunkLatency");

    // tutorial
    tutorial_csv->initialize_csv(total_stat_rows + 1, 5);
    tutorial_csv->write_cell(0, 0, "Name");
    tutorial_csv->write_cell(0, 1, "TotalTime(us)");
    tutorial_csv->write_cell(0, 2, "ComputeTime(us)");
    tutorial_csv->write_cell(0, 3, "ExposedCommunicationTime(us)");
    tutorial_csv->write_cell(0, 4, "TotalMessageSize(MB)");
  }

  Analytical::AnalyticalNetwork::setCsvConfiguration(
      path, stat_row, total_stat_rows, end_to_env_csv, dimensional_info_csv, tutorial_csv);

  /**
   * Run Analytical Model
   */
  // Initialize event queue
  for (int i = 0; i < npus_count; i++) {
    systems[i]->workload->fire();
  }

  // Run events
  while (!event_queue->empty()) {
    event_queue->proceed();
  }

  /**
   * Print results
   */
//   auto topology_cost = cost_model.get_network_cost();
//   std::cout << "\n[Analytical, main] Total Cost: $" << topology_cost
//             << std::endl;

  /**
   * Cleanup
   */
  // System class automatically deletes itself, so no need to free systems[i]
  // here. Invoking `free systems[i]` here will trigger segfault (by trying to
  // delete already deleted memory space)

  // terminate program
  return 0;
}
