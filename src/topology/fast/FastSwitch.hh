/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#ifndef __FASTSWITCH_HH__
#define __FASTSWITCH_HH__

#include "../TopologyConfig.hh"
#include "FastTopology.hh"

namespace Analytical {
    class FastSwitch : public FastTopology {
    public:
        FastSwitch(TopologyConfigs configs) noexcept;

        ~FastSwitch() noexcept;

        double send(NpuId src, NpuId dest, PayloadSize payload_size) noexcept override;
    };
}

#endif
