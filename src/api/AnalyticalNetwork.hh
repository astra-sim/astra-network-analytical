/******************************************************************************
Copyright (c) 2020 Georgia Institute of Technology
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

Author : William Won (william.won@gatech.edu)
*******************************************************************************/

#ifndef __ANALYTICALNETWORK_HH__
#define __ANALYTICALNETWORK_HH__

#include <memory>
#include "../event-queue/EventQueue.hh"
#include "../event-queue/SendRecvTrackingMap.hh"
#include "../topology/Topology.hh"
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
