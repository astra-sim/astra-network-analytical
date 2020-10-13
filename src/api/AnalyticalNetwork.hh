/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#ifndef __ANALYTICALNETWORK_HH__
#define __ANALYTICALNETWORK_HH__

#include <memory>
#include "../event-queue/EventQueue.hh"
#include "../topology/Topology.hh"
#include "SendRecvTrackingMap.hh"
#include "astra-sim/system/AstraNetworkAPI.hh"

namespace Analytical {
class AnalyticalNetwork : public AstraSim::AstraNetworkAPI {
 public:
  /**
   * set event_queue to the given pointer.
   * @param new_event_queue pointer to the new event_queue
   */
  static void set_event_queue(
      const std::shared_ptr<EventQueue>& event_queue_ptr) noexcept;

  /**
   * set topology to the given pointer
   * @param new_topology pointer to the new topology
   */
  static void set_topology(
      const std::shared_ptr<Topology>& topology_ptr) noexcept;

  /**
   * ========================= AstraNetworkAPIs
   * =================================================
   */
  explicit AnalyticalNetwork(int rank) : AstraSim::AstraNetworkAPI(rank) {}

  int sim_comm_size(AstraSim::sim_comm comm, int* size) override;

  int sim_finish() override;

  double sim_time_resolution() override;

  int sim_init(AstraSim::AstraMemoryAPI* MEM) override;

  AstraSim::timespec_t sim_get_time() override;

  void sim_schedule(
      AstraSim::timespec_t delta,
      void (*fun_ptr)(void* fun_arg),
      void* fun_arg) override;

  int sim_send(
      void* buffer,
      int count,
      int type,
      int dst,
      int tag,
      AstraSim::sim_request* request,
      void (*msg_handler)(void* fun_arg),
      void* fun_arg) override;

  int sim_recv(
      void* buffer,
      int count,
      int type,
      int src,
      int tag,
      AstraSim::sim_request* request,
      void (*msg_handler)(void* fun_arg),
      void* fun_arg) override;
  /**
   * ===========================================================================================
   */

 private:
  static std::shared_ptr<EventQueue> event_queue;
  static std::shared_ptr<Topology> topology;
  static SendRecvTrackingMap send_recv_tracking_map;
};
} // namespace Analytical

#endif
