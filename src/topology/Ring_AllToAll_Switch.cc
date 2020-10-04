#include <iostream>
#include "Ring_AllToAll_Switch.hh"

using namespace AnalyticalBackend;

Ring_AllToAll_Switch::Ring_AllToAll_Switch(std::vector<TopologyConfiguration> configurations,
                                           std::vector<int> nodes_per_dims) noexcept :
        configurations(configurations), nodes_per_dims(nodes_per_dims) { }

Topology::Latency Ring_AllToAll_Switch::simulateSend(NodeID src, NodeID dest, PayloadSize payload_size) noexcept {
    // get dimension
    auto tiles_per_gpu = nodes_per_dims[0];
    auto tiles_per_node = nodes_per_dims[1] * tiles_per_gpu;

    // translate nodeID into address by dimension
    auto src_node = src / tiles_per_node;
    auto src_gpu = (src % tiles_per_node) / tiles_per_gpu;
    auto src_tile = src % tiles_per_gpu;

    auto dest_node = dest / tiles_per_node;
    auto dest_gpu = (dest % tiles_per_node) / tiles_per_gpu;
    auto dest_tile = dest % tiles_per_gpu;

    // Check which network to use
    auto hops_count = -1;
    auto config_dim = -1;
    if (src_node != dest_node) {
        // use switch
        config_dim = 2;
        hops_count = 2;
    } else if (src_gpu != dest_gpu) {
        // use all-to-all
        config_dim = 1;
        hops_count = 1;
    } else if (src_tile != dest_tile) {
        // use ring
        config_dim = 0;

        // compute hops_count
        auto nodes_count = nodes_per_dims[0];
        auto half_nodes_count = nodes_count / 2;

        // compute clockwise distance
        auto clockwise_distance = (dest - src) % nodes_count;
        if (clockwise_distance < 0) {
            // distance negative means src->dest passes node 0: compensate this by adding nodes_count
            clockwise_distance = clockwise_distance + nodes_count;
        }

        // counterclockwise distance may be shorter
        if (clockwise_distance <= half_nodes_count) {
            // clockwise is shorter
            hops_count = clockwise_distance;
        } else {
            // counterclockwise is shorter
            hops_count = (nodes_count - clockwise_distance);
        }

        assert(hops_count >= 0 && "[Ring] Routing error: hops count cannot be negative");
    } else {
        // src and dest same: should not occur
        std::cout << "[Ring_AllToAll_Switch]: Same src and dest given" << std::endl;
        exit(-1);
    }

    // compute communication latency
    auto configuration = configurations[config_dim];

    auto comm_latency = hops_count * configuration.getLinkLatency();  // Link delay
    comm_latency += payload_size / configuration.getBandwidth();  // Serialization delay
    comm_latency += configuration.getRouterLatency();  // Switch delay
    comm_latency += 2 * configuration.getNIC_Latency();  // NIC delay when nic is enabled

    // compute hbm delay
    auto hbm_latency = configuration.getHBM_Latency();  // HBM delay
    hbm_latency += payload_size / configuration.getHBM_Bandwidth();  // HBM serialization delay
    hbm_latency *= configuration.getHBM_Scale();  // Scale HBM by scaling factor

    auto latency = std::max((int)comm_latency, (int)hbm_latency);
    assert(latency >= 0 && "[Ring_AllToAll_Switch] Latency error: latency cannot be negative");

    return latency;
}
