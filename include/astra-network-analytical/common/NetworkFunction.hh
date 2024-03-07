/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#pragma once

#include <vector>
#include "common/Type.hh"

namespace NetworkAnalytical {

/**
 * Convert bandwidth from GB/s to B/ns.
 * Here, 1 GB = 2^30 B
 * 1 s = 10^9 ns
 *
 * @param bw_GBps bandwidth in GB/s
 * @return translated bandwidth in B/ns
 */
[[nodiscard]] Bandwidth bw_GBps_to_Bpns(Bandwidth bw_GBps) noexcept;

/**
 * Translate NPU ID to multi-dim network address.
 * e.g., if npu_id is 47 and npus_count_per_dim is [2, 8, 4],
 * the address is [1, 7, 2].
 *
 * @param npu_id id of the npu to translate
 * @param npus_count_per_dim number of NPUs per each dimension
 * @return translated multi-dim address
 */
[[nodiscard]] MultiDimAddress translate_id_to_address(
    DeviceId npu_id,
    const std::vector<int>& npus_count_per_dim) noexcept;

/**
 * Translate multi-dim network address to NPU ID.
 * e.g., if the address is [1, 7, 2] and npus_count_per_dim is [2, 8, 4],
 * the NPU ID is 47.
 *
 * @param address multi-dim address to translate
 * @param npus_count_per_dim number of NPUs per each dimension
 * @return translated NPU ID
 */
[[nodiscard]] DeviceId translate_address_to_id(
    const MultiDimAddress& address,
    const std::vector<int>& npus_count_per_dim) noexcept;

} // namespace NetworkAnalytical
