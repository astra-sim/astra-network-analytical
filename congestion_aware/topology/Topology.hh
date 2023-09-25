/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#pragma once

#include "common/event-queue/EventQueue.hh"
#include "congestion_aware/network/Chunk.hh"
#include "congestion_aware/network/Device.hh"

using namespace NetworkAnalytical;

namespace NetworkAnalyticalCongestionAware {

class Topology {
 public:
  /**
   * Set the event queue of the Link class.
   *
   * @param event_queue The event queue to be linked.
   */
  static void set_event_queue(std::shared_ptr<EventQueue> event_queue) noexcept;

  /**
   * Instantiate a topology object.
   */
  Topology() noexcept;

  /**
   * Construct the route from src to dest.
   * Route is a list of std::shared_ptr<e>, includinDevice both src and dest.
   *
   * e.g., route(0, 3) = [0, 1, 2, 3]
   *
   * @param src src device id
   * @param dest dest device id
   *
   * @return route from src to dest
   */
  [[nodiscard]] virtual Route route(DeviceId src, DeviceId dest)
      const noexcept = 0;

  /**
   * Initiate the transmission of a chunk.
   *
   * @param chunk chunk to be transmitted
   */
  void send(std::unique_ptr<Chunk> chunk) noexcept;

  [[nodiscard]] int get_npus_count() const noexcept;

  [[nodiscard]] int get_devices_count() const noexcept;

 protected:
  /// number of total devices (including switches) in the topology
  int devices_count;

  /// number of NPUs in the topology
  int npus_count;

  /// vector of Device instances in the topology
  std::vector<std::shared_ptr<Device>> devices;

  void instantiate_devices() noexcept;

  /**
   * Connect src -> dest with the given bandwidth and latency.
   * (i.e., a `Link` gets constructed between the two npus)
   *
   * if bidirectional=true, dest -> src connection is also established.
   *
   * @param src src device id
   * @param dest dest device id
   * @param bandwidth bandwidth of link
   * @param latency latency of link
   * @param bidirectional true if connection is bidirectional, false otherwise
   */
  void connect(
      DeviceId src,
      DeviceId dest,
      Bandwidth bandwidth,
      Latency latency,
      bool bidirectional = true) noexcept;
};

} // namespace NetworkAnalyticalCongestionAware
