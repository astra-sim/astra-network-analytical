/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "NetworkParser.hh"

using namespace NetworkAnalytical;

NetworkParser::NetworkParser(const std::string& path) noexcept
    : dims_count(-1) {
  // initialize values
  npus_counts_per_dim = {};
  bandwidths_per_dim = {};
  latencies_per_dim = {};
  topologies_per_dim = {};

  try {
    // load network config file
    const auto network_config = YAML::LoadFile(path);

    // parse network configs
    parse_network_config_yml(network_config);
  } catch (const YAML::BadFile& e) {
    // loading network config file failed
    std::cerr << e.what() << std::endl;
    std::exit(-1);
  }
}

int NetworkParser::get_dims_count() const noexcept {
  assert(dims_count > 0);

  return dims_count;
}

std::vector<int> NetworkParser::get_npus_counts_per_dim() const noexcept {
  assert(dims_count > 0);
  assert(npus_counts_per_dim.size() == dims_count);

  return npus_counts_per_dim;
}

std::vector<Bandwidth> NetworkParser::get_bandwidths_per_dim() const noexcept {
  assert(dims_count > 0);
  assert(bandwidths_per_dim.size() == dims_count);

  return bandwidths_per_dim;
}

std::vector<Latency> NetworkParser::get_latencies_per_dim() const noexcept {
  assert(dims_count > 0);
  assert(latencies_per_dim.size() == dims_count);

  return latencies_per_dim;
}

std::vector<TopologyBuildingBlock> NetworkParser::get_topologies_per_dim()
    const noexcept {
  assert(dims_count > 0);
  assert(topologies_per_dim.size() == dims_count);

  return topologies_per_dim;
}

void NetworkParser::parse_network_config_yml(
    const YAML::Node& network_config) noexcept {
  // parse topology_per_dim
  const auto read_topology = network_config["topology"];
  for (const auto& topology : read_topology) {
    try {
      auto topology_name = topology.as<std::string>();
      auto topology_dim = NetworkParser::parse_topology_name(topology_name);
      topologies_per_dim.push_back(topology_dim);
    } catch (const YAML::BadConversion& e) {
      std::cerr << "[analytical network] " << e.what() << std::endl;
      std::exit(-1);
    }
  }

  // set dims_count
  dims_count = static_cast<int>(topologies_per_dim.size());

  // parse vector values
  npus_counts_per_dim = parse_vector<int>(network_config["npus_count"]);
  bandwidths_per_dim = parse_vector<Bandwidth>(network_config["bandwidth"]);
  latencies_per_dim = parse_vector<Latency>(network_config["latency"]);

  // check the validity of the parsed network config
  check_validity();
}

TopologyBuildingBlock NetworkParser::parse_topology_name(
    const std::string& topology_name) noexcept {
  assert(!topology_name.empty());

  if (topology_name == "Ring") {
    return TopologyBuildingBlock::Ring;
  }

  if (topology_name == "FullyConnected") {
    return TopologyBuildingBlock::FullyConnected;
  }

  if (topology_name == "Switch") {
    return TopologyBuildingBlock::Switch;
  }

  // shouldn't reach here
  std::cerr << "[analytical network] Topology name " << topology_name
            << " not supported" << std::endl;
  std::exit(-1);
}

void NetworkParser::check_validity() const noexcept {
  // dims_count should match
  if (dims_count != npus_counts_per_dim.size()) {
    std::cerr << "[analytical network] length of npus_count ("
              << npus_counts_per_dim.size()
              << ") doesn't match with dimensions (" << dims_count << ")"
              << std::endl;
    std::exit(-1);
  }

  if (dims_count != bandwidths_per_dim.size()) {
    std::cerr << "[analytical network] length of bandwidth ("
              << bandwidths_per_dim.size()
              << ") doesn't match with dims_count (" << dims_count << ")"
              << std::endl;
    std::exit(-1);
  }

  if (dims_count != latencies_per_dim.size()) {
    std::cerr << "[analytical network] length of latency ("
              << latencies_per_dim.size() << ") doesn't match with dims_count ("
              << dims_count << ")" << std::endl;
    std::exit(-1);
  }

  // npus_count should be all positive
  for (const auto& npus_count : npus_counts_per_dim) {
    if (npus_count <= 1) {
      std::cerr << "[analytical network] npus_count (" << npus_count
                << ") should be larger than 1" << std::endl;
      std::exit(-1);
    }
  }

  // bandwidths should be all positive
  for (const auto& bandwidth : bandwidths_per_dim) {
    if (bandwidth <= 0) {
      std::cerr << "[analytical network] bandwidth (" << bandwidth
                << ") should be larger than 0" << std::endl;
      std::exit(-1);
    }
  }

  // latency should be non-negative
  for (const auto& latency : latencies_per_dim) {
    if (latency < 0) {
      std::cerr << "[analytical network] latency (" << latency
                << ") should be non-negative" << std::endl;
      std::exit(-1);
    }
  }
}

template <typename T>
std::vector<T> NetworkParser::parse_vector(
    const YAML::Node& node) const noexcept {
  auto parsed_vector = std::vector<T>();

  for (const auto& element : node) {
    try {
      auto element_value = element.as<T>();
      parsed_vector.push_back(element_value);
    } catch (const YAML::BadConversion& e) {
      std::cerr << "[analytical network] " << e.what() << std::endl;
      std::exit(-1);
    }
  }
  return parsed_vector;
}
