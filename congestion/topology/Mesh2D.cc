/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "Mesh2D.hh"

using namespace Congestion;

Mesh2D::Mesh2D(
    int width,
    int height,
    Bandwidth bandwidth,
    Latency latency) noexcept
    : width(width), height(height), Topology(width * height) {
  // assert width and height are valid
  assert(width > 0);
  assert(height > 0);

  assert(npus_count > 0);

  // assert bandwidth and latency are valid
  assert(bandwidth > 0);
  assert(latency >= 0);

  // connect npus width-wise
  for (int h = 0; h < height; h++) {
    for (int w = 0; w < width - 1; w++) {
      // get npu ids
      auto src = (h * width) + w;
      auto dest = (h * width) + (w + 1);

      // connect npus
      connect(src, dest, bandwidth, latency, true);
    }
  }

  // connect npus height-wise
  for (int w = 0; w < width; w++) {
    for (int h = 0; h < height - 1; h++) {
      // get npu ids
      auto src = (h * width) + w;
      auto dest = ((h + 1) * width) + w;

      // connect npus
      connect(src, dest, bandwidth, latency, true);
    }
  }
}

Mesh2D::~Mesh2D() noexcept = default;

Route Mesh2D::route(NodeId src, NodeId dest) const noexcept {
  // assert npus are in valid range
  assert(0 <= src && src < npus_count);
  assert(0 <= dest && dest < npus_count);

  // assert src and dest are different
  assert(src != dest);

  // get (w, h) coordinates of src and dest
  auto src_w = src % width;
  auto src_h = src / width;
  auto dest_w = dest % width;
  auto dest_h = dest / width;

  // using xy routing
  auto w_dir = (src_w < dest_w) ? 1 : -1;
  auto h_dir = (src_h < dest_h) ? 1 : -1;

  // construct route
  auto route = Route();
  route.push_back(npus[src]);

  // width-wise movement
  while (src_w != dest_w) {
    src_w += w_dir;

    auto current_npu = (src_h * width) + src_w;
    route.push_back(npus[current_npu]);
  }

  // height-wise movement
  while (src_h != dest_h) {
    src_h += h_dir;

    auto current_npu = (src_h * width) + src_w;
    route.push_back(npus[current_npu]);
  }

  // push destination npu
  route.push_back(npus[dest]);

  return route;
}
