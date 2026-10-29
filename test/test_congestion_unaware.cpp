// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2026 Advanced Micro Devices, Inc.

#include <astra-network-analytical/common/NetworkParser.h>
#include <astra-network-analytical/common/Type.h>
#include <astra-network-analytical/congestion_unaware/Helper.h>
#include <gtest/gtest.h>

using namespace NetworkAnalytical;
using namespace NetworkAnalyticalCongestionUnaware;

class TestNetworkAnalyticalCongestionUnaware : public ::testing::Test {
  protected:
    void SetUp() override {
        // set chunk size
        chunk_size = 1'048'576;  // 1 MB
    }

    ChunkSize chunk_size;
};

TEST_F(TestNetworkAnalyticalCongestionUnaware, Ring) {
    // create network
    const auto network_parser = NetworkParser("../../input/Ring.yml");
    const auto topology = construct_topology(network_parser);

    // run communication
    const auto comm_delay = topology->send(1, 4, chunk_size);
    EXPECT_EQ(comm_delay, 21'031'250);
}

TEST_F(TestNetworkAnalyticalCongestionUnaware, FullyConnected) {
    // create network
    const auto network_parser = NetworkParser("../../input/FullyConnected.yml");
    const auto topology = construct_topology(network_parser);

    // run communication
    const auto comm_delay = topology->send(1, 4, chunk_size);
    EXPECT_EQ(comm_delay, 20'031'250);
}

TEST_F(TestNetworkAnalyticalCongestionUnaware, Switch) {
    // create network
    const auto network_parser = NetworkParser("../../input/Switch.yml");
    const auto topology = construct_topology(network_parser);

    // run communication
    const auto comm_delay = topology->send(1, 4, chunk_size);
    EXPECT_EQ(comm_delay, 20'531'250);
}

TEST_F(TestNetworkAnalyticalCongestionUnaware, Ring_FullyConnected_Switch) {
    // create network
    const auto network_parser =
        NetworkParser("../../input/Ring_FullyConnected_Switch.yml");
    const auto topology = construct_topology(network_parser);

    // run on dim 1
    const auto comm_delay_dim1 = topology->send(0, 1, chunk_size);
    EXPECT_EQ(comm_delay_dim1, 4'932'812);

    // run on dim 2
    const auto comm_delay_dim2 = topology->send(37, 41, chunk_size);
    EXPECT_EQ(comm_delay_dim2, 10'265'625);

    // run on dim 3
    const auto comm_delay_dim3 = topology->send(26, 42, chunk_size);
    EXPECT_EQ(comm_delay_dim3, 23'531'250);
}
