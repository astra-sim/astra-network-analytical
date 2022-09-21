/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#ifndef __ANALYTICALNETWORK_HH__
#define __ANALYTICALNETWORK_HH__

#include <memory>
#include "../event-queue/EventQueue.hh"
#include "../topology/CostModel.hh"
#include "../topology/Topology.hh"
#include "PayloadSizeTracker.hh"
#include "SendRecvTrackingMap.hh"
#include "astra-sim/system/AstraNetworkAPI.hh"
#include "astra-sim/system/AstraSimDataAPI.hh"
#include "astra-sim/workload/CSVWriter.hh"

namespace Analytical {
class AnalyticalNetwork : public AstraSim::AstraNetworkAPI {
 public:
  /**
   * set event_queue to the given pointer.
   * @param new_event_queue pointer to the new event_queue
   */
  static void setEventQueue(
      const std::shared_ptr<EventQueue>& event_queue_ptr) noexcept;

  /**
   * set topology to the given pointer
   * @param new_topology pointer to the new topology
   */
  static void setTopology(
      const std::shared_ptr<Topology>& topology_ptr) noexcept;

  static void setCostModel(CostModel* const cost_model_ptr) noexcept;

  /**
   * Set static values for backend CSV logging
   * @param stat_path
   * @param stat_row
   * @param total_stat_rows
   * @param end_to_end_csv
   * @param dimensional_info_csv
   */
  static void setCsvConfiguration(
      const std::string& stat_path,
      int stat_row,
      int total_stat_rows,
      std::shared_ptr<AstraSim::CSVWriter> end_to_end_csv,
      std::shared_ptr<AstraSim::CSVWriter> dimensional_info_csv,
      std::shared_ptr<AstraSim::CSVWriter> tutorial_csv) noexcept;

  /**
   * ========================= AstraNetworkAPIs
   * =================================================
   */
  AnalyticalNetwork(int rank, int dims_count) noexcept;

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
      uint64_t count,
      int type,
      int dst,
      int tag,
      AstraSim::sim_request* request,
      void (*msg_handler)(void* fun_arg),
      void* fun_arg) override;

  int sim_recv(
      void* buffer,
      uint64_t count,
      int type,
      int src,
      int tag,
      AstraSim::sim_request* request,
      void (*msg_handler)(void* fun_arg),
      void* fun_arg) override;

  void pass_front_end_report(
      AstraSim::AstraSimDataAPI astraSimDataAPI) override;

  double get_BW_at_dimension(int dim) override;
  /**
   * ===========================================================================================
   */

 private:
  static std::shared_ptr<EventQueue> event_queue;
  static std::shared_ptr<Topology> topology;
  static SendRecvTrackingMap send_recv_tracking_map;
  static CostModel* cost_model;
  static std::shared_ptr<PayloadSizeTracker> payload_size_tracker;

  static std::string stat_path;
  static int stat_row;
  static int total_stat_rows;
  static std::shared_ptr<AstraSim::CSVWriter> end_to_end_csv;
  static std::shared_ptr<AstraSim::CSVWriter> dimensional_info_csv;
  static std::shared_ptr<AstraSim::CSVWriter> tutorial_csv;

  int dims_count;
};
} // namespace Analytical

#endif
