/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#pragma once

#include "topology/Topology.hh"

namespace Congestion {

/**
 * A 2D Mesh topology.
 */
class Mesh2D final : public Topology {
 public:
  /**
   * Constructor.
   *
   * @param width width of the mesh
   * @param height height of the mesh
   * @param bandwidth bandwidth of link
   * @param latency latency of link
   */
  Mesh2D(int width, int height, Bandwidth bandwidth, Latency latency) noexcept;

  /**
   * Destructor
   */
  ~Mesh2D() noexcept;

  /**
   * Implementation of route function.
   */
  Route route(NodeId src, NodeId dest) const noexcept override;

 private:
  /// width of mesh
  int width;

  /// height of mesh
  int height;
};

} // namespace Congestion
