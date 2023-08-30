/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "NetworkConfigParser.hh"

using namespace Congestion;

NetworkConfigParser::NetworkConfigParser() noexcept : parsed(false) {}

NetworkConfigParser::~NetworkConfigParser() noexcept = default;

void NetworkConfigParser::parse(const std::string& path) {
  // parse if not parsed
  assert(!parsed);

  // load network config file
  network_config = YAML::LoadFile(path);

  // mark as parsed
  parsed = true;
}
