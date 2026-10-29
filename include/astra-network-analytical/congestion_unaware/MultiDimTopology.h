// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2026 Advanced Micro Devices, Inc.

#pragma once

#include <astra-network-analytical/common/Type.h>
#include <astra-network-analytical/congestion_unaware/BasicTopology.h>
#include <astra-network-analytical/congestion_unaware/Topology.h>
#include <memory>

using namespace NetworkAnalytical;

namespace NetworkAnalyticalCongestionUnaware {

/**
 * MultiDimTopology implements multi-dimensional network topologies
 * which can be constructed by stacking up multiple BasicTopology instances.
 */
class MultiDimTopology : public Topology {
  public:
    /**
     * Constructor.
     */
    MultiDimTopology() noexcept;

    int get_topology_dim(int src, int dst) {
        const auto src_address = translate_address(src);
        const auto dest_address = translate_address(dst);
        return get_dim_to_transfer(src_address, dest_address);
    }
    /**
     * Implement the send method of Topology.
     */
    [[nodiscard]] EventTime send(DeviceId src,
                                 DeviceId dest,
                                 ChunkSize chunk_size) const noexcept override;

    [[nodiscard]] EventTime receive(
        DeviceId src,
        DeviceId dest,
        ChunkSize chunk_size) const noexcept override;
    /**
     * Add a dimension to the multi-dimensional topology.
     *
     * @param topology BasicTopology instance to be added.
     */
    void append_dimension(
        std::unique_ptr<BasicTopology> basic_topology) noexcept;

  private:
    /// Each NPU ID can be broken down into multiple dimensions.
    /// for example, if the topology size is [2, 8, 4] and the NPU ID is 31,
    /// then the NPU ID can be broken down into [1, 7, 1].
    using MultiDimAddress = std::vector<DeviceId>;

    /// BasicTopology instances per dimension.
    std::vector<std::unique_ptr<BasicTopology>> topology_per_dim;

    /**
     * Translate the multi-dimensional address into an NPU ID.
     * Keep same name as reverse mapping for simplicity
     *
     * @param npu_addr MultiDimAddress of the NPU
     * @return the same NPU in device id representation
     */
    [[nodiscard]] DeviceId translate_address(
        const MultiDimAddress npu_addr) const noexcept;

    /**
     * Translate the NPU ID into a multi-dimensional address.
     *
     * @param npu_id id of the NPU
     * @return the same NPU in multi-dimensional address representation
     */
    [[nodiscard]] MultiDimAddress translate_address(
        const DeviceId npu_id) const noexcept;

    /**
     * Given src and dest address in multi-dimensional form,
     * return the dimension where the transfer should happen.
     * i.e., the dimension where the src and dest addresses differ.
     *
     * @param src_address src NPU ID in multi-dimensional form
     * @param dest_address dest NPU ID in multi-dimensional form
     * @return the dimension where the transfer should happen
     */
    [[nodiscard]] int get_dim_to_transfer(
        const MultiDimAddress& src_address,
        const MultiDimAddress& dest_address) const noexcept;
};

}  // namespace NetworkAnalyticalCongestionUnaware
