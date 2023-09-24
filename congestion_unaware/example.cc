/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "common/Common.hh"
#include "congestion_unaware/basic-topology/FullyConnected.hh"
#include "congestion_unaware/basic-topology/Ring.hh"
#include "congestion_unaware/basic-topology/Switch.hh"
#include "congestion_unaware/multi-dim-topology/MultiDimTopology.hh"

using namespace NetworkAnalytical;
using namespace NetworkAnalyticalCongestionUnaware;

int main() {
  /// Create a 3D topology
  auto topology = std::make_shared<MultiDimTopology>();

  auto first_dim = std::make_unique<Ring>(2, 200, 50);
  auto second_dim = std::make_unique<FullyConnected>(8, 100, 500);
  auto third_dim = std::make_unique<Switch>(4, 50, 2000);

  topology->add_dim(std::move(first_dim));
  topology->add_dim(std::move(second_dim));
  topology->add_dim(std::move(third_dim));

  /// message settings
  auto chunk_size = 1'048'576; // 1 MB

  /// Print topology information
  std::cout << "Nodes Count: " << topology->get_nodes_count() << std::endl;

  /// Run sample send-recv
  auto comm_delay = topology->send(3, 19, chunk_size);
  std::cout << "comm_delay: " << comm_delay << std::endl;

  /// terminate
  return 0;
}
