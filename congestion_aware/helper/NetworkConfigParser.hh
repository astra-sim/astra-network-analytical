/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#pragma once

#include <yaml-cpp/yaml.h>
#include <cassert>
#include "type/Type.hh"

namespace Congestion {

/**
 * @brief NetworkConfigParser parses network config file (in yaml format).
 */
class NetworkConfigParser {
 public:
  /**
   * @brief Constructor.
   */
  NetworkConfigParser() noexcept;

  /**
   * @brief Destructor.
   */
  ~NetworkConfigParser() noexcept;

  /**
   * @brief Parse network config file.
   * @param path path to the network config file (in yaml format)
   * @throw YAML::BadFile if the file doesn't exist
   * @throw YAML::ParserException if the file is not in yaml format
   */
  void parse(const std::string& path);

  /**
   * @brief Get value from network config.
   * @tparam T type of the value
   * @param name name of the value
   * @return return the value if it exists
   * @throw OptNotSpecifiedError if the value doesn't exist
   */
  template <typename T>
  T get_value(const std::string& name) {
    // get value after parsing
    assert(parsed);

    if (!network_config[name]) {
      // if value doesn't exist, throw an error
      throw OptNotSpecifiedError(
          "Value " + name + " not found in network config");
    }

    // return the value
    return network_config[name].as<T>();
  }

 private:
  /// parsed network config
  YAML::Node network_config;

  /// mark if parsed
  bool parsed;
};

} // namespace Congestion
