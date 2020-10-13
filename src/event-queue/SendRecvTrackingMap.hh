/******************************************************************************
Copyright (c) 2020 Georgia Institute of Technology
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

Author : William Won (william.won@gatech.edu)
*******************************************************************************/

#ifndef __SENDRECVTRACKINGMAP_HH__
#define __SENDRECVTRACKINGMAP_HH__

#include <map>
#include <tuple>
#include "SendRecvTrackingMapValue.hh"
#include "astra-sim/system/AstraNetworkAPI.hh"

namespace Analytical {
class SendRecvTrackingMap {
 public:
  /**
   * Check whether send operation with given key entry exists.
   * @param tag
   * @param src
   * @param dest
   * @param count
   * @return true if send operation exists, false if not
   */
  bool has_send_operation(int tag, int src, int dest, int count) const noexcept;

  /**
   * Check whether recv operation with given key entry exists.
   * @param tag
   * @param src
   * @param dest
   * @param count
   * @return true if recv operation exists, false if not
   */
  bool has_recv_operation(int tag, int src, int dest, int count) const noexcept;

  /**
   * Remove the send operation entry with given key, and return send_finish_time
   * value. Assertion: send_operation with given key must exist.
   * @param tag
   * @param src
   * @param dest
   * @param count
   * @return send_finish_time value
   */
  AstraSim::timespec_t pop_send_finish_time(
      int tag,
      int src,
      int dest,
      int count) noexcept;

  /**
   * Remove the recv operation entry with given key, and return
   * recv_event_handler value. Assertion: recv operation with given key must
   * exist.
   * @param tag
   * @param src
   * @param dest
   * @param count
   * @return recv_event_handler
   */
  Event pop_recv_event_handler(int tag, int src, int dest, int count) noexcept;

  /**
   * Insert a new send operation with given key.
   *      Assertion: given key entry should not exist in the map
   * @param tag
   * @param src
   * @param dest
   * @param count
   * @param send_finish_time send_finish_time to write into the table
   */
  void insert_send(
      int tag,
      int src,
      int dest,
      int count,
      AstraSim::timespec_t send_finish_time) noexcept;

  /**
   * Insert a new recv operation with given key.
   *      Assertion: given key entry should not exist in the map.
   * @param tag
   * @param src
   * @param dest
   * @param count
   * @param fun_ptr recv event handler
   * @param fun_arg recv event handler argument
   */
  void insert_recv(
      int tag,
      int src,
      int dest,
      int count,
      void (*fun_ptr)(void*),
      void* fun_arg) noexcept;

  /**
   * For debugging purpose: print status for debugging.
   */
  void print() const noexcept;

 private:
  /**
   * tag, src, dest, count
   */
  typedef std::tuple<int, int, int, int> Key;

  /**
   * Send and recv tracking map
   * (tag, src, dest, count) -> Value
   */
  std::map<Key, SendRecvTrackingMapValue> send_recv_tracking_map;
};
} // namespace Analytical

#endif
