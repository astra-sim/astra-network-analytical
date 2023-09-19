/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include <cassert>
#include <iostream>
#include "helper/CommandLineOptParser.hh"

using namespace Congestion;

CommandLineOptParser::CommandLineOptParser() noexcept : parsed(false) {
  // by default, add help option
  description.add_options()("help,h", "Print help message");
}

CommandLineOptParser::~CommandLineOptParser() noexcept = default;

void CommandLineOptParser::parse(int argc, char* argv[]) noexcept {
  // parse argc and argv
  po::store(po::parse_command_line(argc, argv, description), vm);
  po::notify(vm);

  // mark parsed
  parsed = true;
}

void CommandLineOptParser::print_help_message_if_required() {
  // check if parsed
  assert(parsed);

  // if help option is given, print help message and throw exception
  if (vm.count("help") || vm.count("h")) {
    std::cout << description << std::endl;
    throw HelpMessagePrinted();
  }
}
