/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#ifndef __FASTALLTOALL_HH__
#define __FASTALLTOALL_HH__

#include "../TopologyConfig.hh"
#include "FastTopology.hh"

namespace Analytical {
    class FastAllToAll : public FastTopology {
    public:
        FastAllToAll(TopologyConfigs configs) noexcept;

        ~FastAllToAll() noexcept;

        double send(NpuId src, NpuId dest, PayloadSize payload_size) noexcept override;
    };
}


#endif
