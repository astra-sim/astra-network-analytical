/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#pragma once

#include <boost/program_options.hpp>
#include <cassert>
#include <optional>
#include <string>
#include "type/Type.hh"

namespace po = boost::program_options;

namespace Congestion {

/**
 * @brief Command line options parser
 */
class CommandLineOptParser {
 public:
  /**
   * @brief Constructor
   */
  CommandLineOptParser() noexcept;

  /**
   * @brief Destructor
   */
  ~CommandLineOptParser() noexcept;

  /**
   * @brief Parse command line options
   * @param argc Number of arguments
   * @param argv Arguments array
   */
  void parse(int argc, char* argv[]) noexcept;

  /**
   * @brief Print help message if --help or -h is given as an option
   * Must be called after parse()
   * @throw HelpMessagePrinted if --help or -h is given
   */
  void print_help_message_if_required();

  /**
   * @brief Add an command-line option
   * @tparam T datatype of the option
   * @param name name of the option
   * @param opt_description description of the option
   * @param multi_token OptType::Single if one value, OptType::Multi if
   * std::vector
   */
  template <typename T>
  void define_option(
      const std::string& name,
      const std::string& opt_description,
      OptType multi_token = OptType::Single) noexcept {
    // define option before parsing
    assert(!parsed);

    if (multi_token == OptType::Single) {
      // add single token option
      description.add_options()(
          name.c_str(), po::value<T>(), opt_description.c_str());
    } else {
      // add multitoken option
      description.add_options()(
          name.c_str(),
          po::value<std::vector<T>>()->multitoken(),
          opt_description.c_str());
    }
  }

  /**
   * @brief Define an option with default value
   * @tparam T datatype of the option
   * @param name name of the option
   * @param opt_description description of the option
   * @param default_value default value of the option
   */
  template <typename T>
  void define_option(
      const std::string& name,
      const std::string& opt_description,
      T default_value) noexcept {
    // define option before parsing
    assert(!parsed);

    // add option (single-token) with default value
    description.add_options()(
        name.c_str(),
        po::value<T>()->default_value(default_value),
        opt_description.c_str());
  }

  /**
   * @brief Get the value of an option
   * @tparam T datatype of the option (must be the same as the one defined)
   * @param name name of the option
   * @return value of the option
   * @throw ArgNotSpecifiedError if the option is not specified
   */
  template <typename T>
  T get_value(const std::string& name) {
    // get the value after parsing
    assert(parsed);

    if (vm.count(name) <= 0) {
      // throw exception if the option is not specified
      throw OptNotSpecifiedError(name);
    }

    // return the value
    return vm[name].as<T>();
  }

  /**
   * @brief Get the value of an option, if one exists
   * Return std::nullopt if the option is not specified
   * @tparam T datatype of the option (must be the same as the one defined)
   * @param name name of the option
   * @return value of the option if one exists, std::nullopt otherwise
   */
  template <typename T>
  std::optional<T> get_optional_value(const std::string& name) noexcept {
    // get the value after parsing
    assert(parsed);

    if (vm.count(name) <= 0) {
      // return nullopt if the option is not specified
      return std::nullopt;
    }

    // return the value
    return vm[name].as<T>();
  }

 private:
  /// Description of the options
  po::options_description description = {"Available Options"};

  /// Parsed options
  po::variables_map vm;

  /// Whether the options are parsed
  bool parsed;
};

} // namespace Congestion
