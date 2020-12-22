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
#include "helper/CommandLineParser.hh"
#include "helper/NetworkConfigParser.hh"
#include "helper/json.hh"
#include "topology/CostModel.hh"
#include "topology/Topology.hh"
#include "topology/TopologyConfig.hh"
#include "topology/detailed/DetailedAllToAll.hh"
#include "topology/detailed/DetailedRing.hh"
#include "topology/detailed/DetailedSwitch.hh"
#include "topology/detailed/DetailedTorus2D.hh"
#include "topology/fast/FastAllToAll.hh"
#include "topology/fast/FastRing.hh"
#include "topology/fast/FastRing_AllToAll_Switch.hh"
#include "topology/fast/FastSwitch.hh"
#include "topology/fast/FastTorus2D.hh"
#include "topology/hierarchical/HierarchicalTopology.hh"
#include "topology/hierarchical/HierarchicalTopologyConfig.hh"

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

  // Parse command line arguments
  try {
    cmd_parser.parse(argc, argv);
  } catch (const Analytical::CommandLineParser::ParsingError& e) {
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
    std::cout
        << "[Analytical, function main] Network configuration file path not given!"
        << std::endl;
    exit(-1);
  }

  // parse configuration.json file
  auto network_parser = Analytical::NetworkConfigParser(network_configuration);
  auto topology_name = network_parser.get<std::string>("topology-name");
  auto dimensions_count = network_parser.get<int>("dimensions-count");
  auto units_counts = network_parser.get<std::vector<int>>("units-count");
  cmd_parser.set_if_defined("units-count", &units_counts);
  auto link_latencies = network_parser.get<std::vector<double>>("link-latency");
  auto link_bandwidths =
      network_parser.get<std::vector<double>>("link-bandwidth");
  auto nic_latencies = network_parser.get<std::vector<double>>("nic-latency");
  auto router_latencies =
      network_parser.get<std::vector<double>>("router-latency");
  auto hbm_latencies = network_parser.get<std::vector<double>>("hbm-latency");
  auto hbm_bandwidths =
      network_parser.get<std::vector<double>>("hbm-bandwidth");
  auto hbm_scales = network_parser.get<std::vector<double>>("hbm-scale");

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

  // cost model
  auto cost_model = Analytical::CostModel();

  // topology configuration for each dimension
  auto topology_configs = Analytical::Topology::TopologyConfigs();
  for (int i = 0; i < dimensions_count; i++) {
    topology_configs.emplace_back(
        units_counts[i], // NPUs count
        link_latencies[i], // link latency (ns)
        link_bandwidths[i], // link bandwidth (GB/s) = (B/ns)
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
    auto links_count_per_dim = network_parser.parseLinksCountPerDim();
    cmd_parser.set_if_defined("links-count", &links_count_per_dim);
    auto hierarchy_config = Analytical::HierarchicalTopologyConfig(
        dimensions_count, topologies_per_dim, links_count_per_dim);

    topology = std::make_shared<Analytical::HierarchicalTopology>(
        topology_configs, hierarchy_config, cost_model);
    for (int dim = 0; dim < dimensions_count; dim++) {
      physical_dims.emplace_back(units_counts[dim]);
    }
  } else if (topology_name == "Switch") {
    assert(
        dimensions_count == 1 &&
        "[main] Switch is the given topology but dimension != 1");

    if (network_parser.useFastVersion()) {
      topology = std::make_shared<Analytical::FastSwitch>(
          topology_configs, cost_model);
    } else {
      topology = std::make_shared<Analytical::DetailedSwitch>(
          topology_configs, cost_model);
    }
    physical_dims.emplace_back(npus_count);
  } else if (topology_name == "AllToAll") {
    assert(
        dimensions_count == 1 &&
        "[main] AllToAll is the given topology but dimension != 1");

    if (network_parser.useFastVersion()) {
      topology = std::make_shared<Analytical::FastAllToAll>(
          topology_configs, cost_model);
    } else {
      topology = std::make_shared<Analytical::DetailedAllToAll>(
          topology_configs, cost_model);
    }
    physical_dims.emplace_back(npus_count);
  } else if (topology_name == "Torus2D") {
    assert(
        dimensions_count == 2 &&
        "[main] Torus2D is the given topology but dimension != 2");

    if (network_parser.useFastVersion()) {
      topology = std::make_shared<Analytical::FastTorus2D>(
          topology_configs, cost_model);
    } else {
      topology = std::make_shared<Analytical::DetailedTorus2D>(
          topology_configs, cost_model);
    }

    physical_dims.emplace_back(units_counts[0]);
    physical_dims.emplace_back(units_counts[1]);
  } else if (topology_name == "Ring") {
    assert(
        dimensions_count == 1 &&
        "[main] Ring is the given topology but dimension != 1");

    if (network_parser.useFastVersion()) {
      topology =
          std::make_shared<Analytical::FastRing>(topology_configs, cost_model);
    } else {
      topology = std::make_shared<Analytical::DetailedRing>(
          topology_configs, cost_model);
    }
    physical_dims.emplace_back(npus_count);
  } else if (topology_name == "Ring_AllToAll_Switch") {
    assert(
        dimensions_count == 3 &&
        "[main] Ring_AllToAll_Switch is the given topology but dimension != 3");

    if (network_parser.useFastVersion()) {
      topology = std::make_shared<Analytical::FastRing_AllToAll_Switch>(
          topology_configs, cost_model);
    } else {
      // non-fast version
      // TODO: implement this
      std::cout << "Detailed version not implemented yet" << std::endl;
      exit(-1);
    }
    physical_dims.emplace_back(units_counts[0]);
    physical_dims.emplace_back(units_counts[1]);
    physical_dims.emplace_back(units_counts[2]);
  } else {
    std::cout << "[Main] Topology not defined: " << topology_name << std::endl;
    exit(-1);
  }

  // Instantiate required network, memory, and system layers
  auto queues_per_dim = std::vector<int>(dimensions_count, num_queues_per_dim);

  for (int i = 0; i < npus_count; i++) {
    analytical_networks[i] = std::make_unique<Analytical::AnalyticalNetwork>(i);

    memories[i] = std::make_unique<AstraSim::SimpleMemory>(
        (AstraSim::AstraNetworkAPI*)(analytical_networks[i].get()),
        500,
        270,
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

  // link event queue and topology
  Analytical::AnalyticalNetwork::set_event_queue(event_queue);
  Analytical::AnalyticalNetwork::set_topology(topology);

  // link csv
  auto end_to_env_csv =
      std::make_shared<AstraSim::CSVWriter>(path, "backend_end_to_end.csv");
  auto dimensional_info_csv =
      std::make_shared<AstraSim::CSVWriter>(path, "backend_dim_info.csv");
  if (stat_row == 0) {
    end_to_env_csv->initialize_csv(total_stat_rows + 1, 4);
    end_to_env_csv->write_cell(0, 0, "Run name");
    end_to_env_csv->write_cell(0, 1, "Running time");
    end_to_env_csv->write_cell(0, 2, "Compute time");
    end_to_env_csv->write_cell(0, 3, "Exposed comm time");

    // fixme: assuming max_dimension is 10
    // fixme: dimensions_count for every topology differs
    auto dimension_csv_rows_count = (total_stat_rows * 10) + 1;
    dimensional_info_csv->initialize_csv(dimension_csv_rows_count, 3);
    dimensional_info_csv->write_cell(0, 0, "Run name");
    dimensional_info_csv->write_cell(0, 1, "Dimension index");
    dimensional_info_csv->write_cell(0, 2, "Average chunk latency");
  }
  Analytical::AnalyticalNetwork::set_csv_configuration(
      path, stat_row, total_stat_rows, end_to_env_csv, dimensional_info_csv);

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
  cost_model.computeCost();

  /**
   * Cleanup
   */
  // System class automatically deletes itself, so no need to free systems[i]
  // here. Invoking `free systems[i]` here will trigger segfault (by trying to
  // delete already deleted memory space)

  // terminate program
  return 0;
}
