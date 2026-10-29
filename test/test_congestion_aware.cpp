// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2026 Advanced Micro Devices, Inc.

#include <astra-network-analytical/common/EventQueue.h>
#include <astra-network-analytical/common/NetworkParser.h>
#include <astra-network-analytical/common/Type.h>
#include <astra-network-analytical/congestion_aware/Chunk.h>
#include <astra-network-analytical/congestion_aware/Helper.h>
#include <gtest/gtest.h>

using namespace NetworkAnalytical;
using namespace NetworkAnalyticalCongestionAware;

class TestNetworkAnalyticalCongestionAware : public ::testing::Test {
  protected:
    void SetUp() override {
        // set event queue
        event_queue = std::make_shared<EventQueue>();
        Topology::set_event_queue(event_queue);

        // set chunk size
        chunk_size = 1'048'576;  // 1 MB
    }

    std::shared_ptr<EventQueue> event_queue;

    static void callback(void* const arg) {}

    ChunkSize chunk_size;
};

TEST_F(TestNetworkAnalyticalCongestionAware, Ring) {
    /// setup
    const auto network_parser = NetworkParser("../../input/Ring.yml");
    const auto topology = construct_topology(network_parser);

    /// message settings
    auto route = topology->route(1, 4);
    auto chunk = std::make_unique<Chunk>(chunk_size, route, callback, nullptr);

    // send a chunk
    topology->send(std::move(chunk));

    /// Run simulation
    while (!event_queue->finished()) {
        event_queue->proceed();
    }

    /// test
    const auto simulation_time = event_queue->get_current_time();
    EXPECT_EQ(simulation_time, 60'093'750);
}

TEST_F(TestNetworkAnalyticalCongestionAware, FullyConnected) {
    /// setup
    const auto network_parser = NetworkParser("../../input/FullyConnected.yml");
    const auto topology = construct_topology(network_parser);

    /// message settings
    auto route = topology->route(1, 4);
    auto chunk = std::make_unique<Chunk>(chunk_size, route, callback, nullptr);

    // send a chunk
    topology->send(std::move(chunk));

    /// Run simulation
    while (!event_queue->finished()) {
        event_queue->proceed();
    }

    /// test
    const auto simulation_time = event_queue->get_current_time();
    EXPECT_EQ(simulation_time, 20'031'250);
}

TEST_F(TestNetworkAnalyticalCongestionAware, Switch) {
    /// setup
    const auto network_parser = NetworkParser("../../input/Switch.yml");
    const auto topology = construct_topology(network_parser);

    /// message settings
    auto route = topology->route(1, 4);
    auto chunk = std::make_unique<Chunk>(chunk_size, route, callback, nullptr);

    // send a chunk
    topology->send(std::move(chunk));

    /// Run simulation
    while (!event_queue->finished()) {
        event_queue->proceed();
    }

    /// test
    const auto simulation_time = event_queue->get_current_time();
    EXPECT_EQ(simulation_time, 40'062'500);
}

TEST_F(TestNetworkAnalyticalCongestionAware, AllGatherOnRing) {
    /// setup
    const auto network_parser = NetworkParser("../../input/Ring.yml");
    const auto topology = construct_topology(network_parser);
    const auto npus_count = topology->get_npus_count();

    /// message settings
    const auto chunk_size = 1'048'576;  // 1 MB

    /// Run All-Gather
    for (int i = 0; i < npus_count; i++) {
        for (int j = 0; j < npus_count; j++) {
            if (i == j) {
                continue;
            }

            // crate a chunk
            auto route = topology->route(i, j);
            auto* event_queue_ptr = static_cast<void*>(event_queue.get());
            auto chunk =
                std::make_unique<Chunk>(chunk_size, route, callback, nullptr);

            // send a chunk
            topology->send(std::move(chunk));
        }
    }

    /// Run simulation
    while (!event_queue->finished()) {
        event_queue->proceed();
    }

    /// test
    const auto simulation_time = event_queue->get_current_time();
    EXPECT_EQ(simulation_time, 704'125'000);
}
