/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "common/Common.hh"
#include "topology/FullyConnected.hh"

using namespace NetworkAnalytical;
using namespace NetworkAnalyticalCongestionUnaware;

int main(int argc, char* argv[]) {
  /// topology settings
  auto npus_count = 16;
  auto bandwidth = 50; // GB/s
  auto latency = 500; // ns

  /// message settings
  auto chunk_size = 1'048'576; // 1 MB

  /// generate topology
  auto topology =
      std::make_shared<FullyConnected>(npus_count, bandwidth, latency);

  /// run sample send-recv
  auto src_id = 0;
  auto dest_id = 7;
  auto comm_delay = topology->send(src_id, dest_id, chunk_size);
  std::cout << "comm_delay: " << comm_delay << std::endl;

  /// terminate
  return 0;
}
