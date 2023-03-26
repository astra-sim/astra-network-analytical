/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include <vector>
#include "api/AstraCongestionApi.hh"
#include "astra-sim/system/Sys.hh"
#include "astra-sim/system/memory/SimpleMemory.hh"
#include "astra-sim/workload/CSVWriter.hh"
#include "event-queue/EventQueue.hh"
#include "helper/CommandLineOptParser.hh"
#include "helper/NetworkConfigParser.hh"
#include "topology/FullyConnected.hh"
#include "topology/Ring.hh"
#include "topology/Switch.hh"
#include "topology/Topology.hh"

using namespace Congestion;

int main(int argc, char* argv[]) {
  /// Create command line parser
  auto command_line_parser = CommandLineOptParser();

  // add command line options
  // file path
  command_line_parser.define_option<std::string>(
      "network-configuration", "Network Configuration File Path");
  command_line_parser.define_option<std::string>(
      "system-configuration", "system Configuration File Path");
  command_line_parser.define_option<std::string>(
      "workload-configuration", "Workload Configuration File Path");

  // run-related names and paths
  command_line_parser.define_option<std::string>("run-name", "Run Name");
  command_line_parser.define_option<std::string>(
      "path", "Result directory path");
  command_line_parser.define_option<int>("total-stat-rows", "Total stat rows");
  command_line_parser.define_option<int>("stat-row", "Current stat row");

  // scale configuration
  command_line_parser.define_option<float>(
      "comm-scale", "Communication scale", 1.0);
  command_line_parser.define_option<float>(
      "compute-scale", "Communication scale", 1.0);
  command_line_parser.define_option<float>(
      "injection-scale", "Communication scale", 1.0);
  command_line_parser.define_option<int>(
      "num-passes", "Number of run passes", 1);
  command_line_parser.define_option<bool>(
      "rendezvous-protocol", "Rendezvous Protocol", false);

  // topology-related options that could be overridden
  command_line_parser.define_option<int>(
      "npus-count", "Npus count", OptType::Multi);
  command_line_parser.define_option<double>(
      "bandwidth", "Bandwidth of each dim", OptType::Multi);
  command_line_parser.define_option<double>(
      "latency", "Latency of each dim", OptType::Multi);

  // parse command line
  command_line_parser.parse(argc, argv);

  // print help message
  try {
    command_line_parser.print_help_message_if_required();
  } catch (const HelpMessagePrinted&) {
    return 1;
  }

  // get required command line option values
  auto network_file_path = std::string();
  auto system_file_path = std::string();
  auto workload_file_path = std::string();

  auto run_name = std::string();
  auto path = std::string();

  auto comm_scale = 0.0f;
  auto compute_scale = 0.0f;
  auto injection_scale = 0.0f;
  auto num_passes = -1;
  auto total_stat_rows = -1;
  auto stat_row = -1;
  auto rendezvous_protocol = false;

  // first, parse network file
  // parse network file path
  try {
    network_file_path =
        command_line_parser.get_value<std::string>("network-configuration");
  } catch (const OptNotSpecifiedError& e) {
    std::cout << e.what() << std::endl;
    return 2;
  }

  // parse network configuration file
  auto network_config_parser = NetworkConfigParser();
  try {
    network_config_parser.parse(network_file_path);
  } catch (const YAML::BadFile& e) {
    std::cout << e.what() << std::endl;
    return 3;
  } catch (const YAML::ParserException& e) {
    std::cout << e.what() << std::endl;
    return 4;
  }

  // get network configuration values
  auto topology_name = std::string();
  auto npus_count_dim = std::vector<int>();
  auto bandwidth = std::vector<double>();
  auto latency = std::vector<double>();
  try {
    topology_name = network_config_parser.get_value<std::string>("topology");
    npus_count_dim =
        network_config_parser.get_value<std::vector<int>>("npus_count");
    bandwidth =
        network_config_parser.get_value<std::vector<double>>("bandwidth");
    latency = network_config_parser.get_value<std::vector<double>>("latency");
  } catch (const OptNotSpecifiedError& e) {
    std::cout << e.what() << std::endl;
    return 5;
  }

  // parse command line options
  try {
    system_file_path =
        command_line_parser.get_value<std::string>("system-configuration");
    workload_file_path =
        command_line_parser.get_value<std::string>("workload-configuration");

    run_name = command_line_parser.get_value<std::string>("run-name");
    path = command_line_parser.get_value<std::string>("path");

    comm_scale = command_line_parser.get_value<float>("comm-scale");
    compute_scale = command_line_parser.get_value<float>("compute-scale");
    injection_scale = command_line_parser.get_value<float>("injection-scale");
    num_passes = command_line_parser.get_value<int>("num-passes");
    total_stat_rows = command_line_parser.get_value<int>("total-stat-rows");
    stat_row = command_line_parser.get_value<int>("stat-row");
    rendezvous_protocol =
        command_line_parser.get_value<bool>("rendezvous-protocol");
  } catch (const OptNotSpecifiedError& e) {
    std::cout << e.what() << std::endl;
    return 2;
  }

  // override topology-related options if specified
  auto npus_count_opt =
      command_line_parser.get_optional_value<std::vector<int>>("npus-count");
  auto bandwidth_opt =
      command_line_parser.get_optional_value<std::vector<double>>("bandwidth");
  auto latency_opt =
      command_line_parser.get_optional_value<std::vector<double>>("latency");

  if (npus_count_opt.has_value()) {
    npus_count_dim = npus_count_opt.value();
  }
  if (bandwidth_opt.has_value()) {
    bandwidth = bandwidth_opt.value();
  }
  if (latency_opt.has_value()) {
    latency = latency_opt.value();
  }

  /// Update BW from GB/s to B/ns
  for (auto i = 0; i < bandwidth.size(); i++) {
    std::cout << bandwidth[i] << std::endl;
    bandwidth[i] = bandwidth[i] * 1.073741824;
    std::cout << bandwidth[i] << std::endl;
  }

  /// Set tutorial csv
  auto tutorial_csv =
      std::make_shared<AstraSim::CSVWriter>(path, "tutorial_result.csv");
  // tutorial_csv->initialize_csv(total_stat_rows + 1, 5);
  tutorial_csv->write_cell(0, 0, "Name");
  tutorial_csv->write_cell(0, 1, "TotalTime(us)");
  tutorial_csv->write_cell(0, 2, "ComputeTime(us)");
  tutorial_csv->write_cell(0, 3, "ExposedCommunicationTime(us)");
  tutorial_csv->write_cell(0, 4, "TotalMessageSize(MB)");
  AstraCongestionApi::setCsvConfiguration(
      stat_row, total_stat_rows, tutorial_csv);

  /// Instantiate shared resources
  auto event_queue = std::make_shared<EventQueue>();

  /// Setup topology
  Topology::link_event_queue(event_queue);

  // create topology
  std::shared_ptr<Topology> topology;
  auto npus_count = -1;
  if (topology_name == "Ring") {
    npus_count = npus_count_dim[0];
    std::make_shared<Ring>(npus_count, bandwidth[0], latency[0], true);
  } else if (topology_name == "Switch") {
    npus_count = npus_count_dim[0];
    topology = std::make_shared<Switch>(npus_count, bandwidth[0], latency[0]);
  } else if (topology_name == "FullyConnected") {
    npus_count = npus_count_dim[0];
    topology =
        std::make_shared<FullyConnected>(npus_count, bandwidth[0], latency[0]);
  } else {
    std::cout << "Unknown topology name: " << topology_name << std::endl;
    return 6;
  }

  /// Setup Common resources
  Link::link_event_queue(event_queue);
  AstraCongestionApi::link_event_queue(event_queue);
  AstraCongestionApi::link_topology(topology);

  /// Create ASTRA-sim related resources
  std::shared_ptr<Congestion::AstraCongestionApi> congestion_apis[npus_count];
  std::shared_ptr<AstraSim::SimpleMemory> memories[npus_count];
  AstraSim::Sys* systems[npus_count];

  auto queues_per_dim = std::vector<int>();
  for (int i = 0; i < npus_count_dim.size(); i++) {
    queues_per_dim.push_back(1);
  }

  for (int i = 0; i < npus_count; i++) {
    congestion_apis[i] = std::make_shared<AstraCongestionApi>(i);

    memories[i] = std::make_shared<AstraSim::SimpleMemory>(
        (AstraSim::AstraNetworkAPI*)(congestion_apis[i].get()),
        1,
        500000,
        12.5);

    systems[i] = new AstraSim::Sys(
        congestion_apis[i].get(),
        memories[i].get(),
        i,
        num_passes,
        npus_count_dim,
        queues_per_dim,
        system_file_path,
        workload_file_path,
        comm_scale,
        compute_scale,
        injection_scale,
        total_stat_rows,
        stat_row,
        path,
        run_name,
        true,
        rendezvous_protocol);
  }

  /// Run ASTRA-sim simulation
  for (int i = 0; i < npus_count; i++) {
    systems[i]->workload->fire();
  }

  while (!event_queue->finished()) {
    event_queue->proceed();
  }

  /// terminate simulation
  return 0;
}
