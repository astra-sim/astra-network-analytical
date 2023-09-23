/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "common/Common.hh"
#include "common/event-queue/EventQueue.hh"
#include "network/Chunk.hh"
#include "topology/Ring.hh"

using namespace NetworkAnalytical;
using namespace NetworkAnalyticalCongestionAware;

void chunk_arrived_callback(void* event_queue_ptr) {
  //  typecast event_queue_ptr
  auto event_queue = static_cast<EventQueue*>(event_queue_ptr);

  // print chunk arrival time
  auto current_time = event_queue->get_current_time();
  std::cout << "A chunk arrived at destination at time: " << current_time
            << " ns" << std::endl;
}

int main() {
  /// Instantiate shared resources
  auto event_queue = std::make_shared<EventQueue>();
  Topology::set_event_queue(event_queue);

  /// Setup topology
  // topology configuration
  auto npus_count = 16;
  auto bandwidth = 50; // GB/s ~ B/ns
  auto latency = 500; // ns
  auto chunk_size = 1048576; // Bytes

  // instantiate nodes
  auto nodes = std::vector<std::shared_ptr<Node>>();
  for (auto i = 0; i < npus_count; i++) {
    nodes.push_back(std::make_shared<Node>(i));
  }

  // instantiate Ring topology
  auto topology = std::make_shared<Ring>(npus_count, bandwidth, latency, true);

  /// Run All-Gather
  for (int i = 0; i < npus_count; i++) {
    for (int j = 0; j < npus_count; j++) {
      if (i == j) {
        continue;
      }

      // crate a chunk
      auto route = std::move(topology->route(i, j));
      auto* event_queue_ptr = static_cast<void*>(event_queue.get());
      auto chunk = std::make_unique<Chunk>(
          chunk_size, route, chunk_arrived_callback, event_queue_ptr);

      // send a chunk
      topology->send(std::move(chunk));
    }
  }

  /// Run simulation
  while (!event_queue->finished()) {
    event_queue->proceed();
  }

  /// Print simulation result
  auto finish_time = event_queue->get_current_time();
  std::cout << "Simulation finished at time: " << finish_time << " ns"
            << std::endl;

  return 0;
}
