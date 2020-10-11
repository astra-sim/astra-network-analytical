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

#ifndef __COMMANDLINEPARSER_HH__
#define __COMMANDLINEPARSER_HH__

#include <iostream>
#include <boost/program_options.hpp>

namespace po = boost::program_options;

namespace AnalyticalBackend {
    class CommandLineParser {
    public:
        using ParsingError = po::error;

        /**
         * Command Line Parser constructor
         */
        CommandLineParser() noexcept {
            options_description.add_options()("help", "Prints help message");
        }

        /**
         * Parse the given command line.
         * (This can fail and raise exception of type CommandLineParser::ParsingError)
         * @param argc
         * @param argv
         */
        void parse(int argc, char** argv) noexcept(false);

        /**
         * Prints the help message if --help option is given.
         * After printing the help message, the entire program terminates.
         * (Should be called after `parse` method is done`)
         */
        void print_help_message_if_required() const noexcept;

        /**
         * Add a new command line argument option.
         * (Should be called before `parse` method is called)
         * @tparam T type of argument (e.g., --test=3 -> T is int)
         * @param name name of the command line argument (e.g., --test=3 -> name is "test")
         * @param explanation
         */
        template<typename T>
        void add_command_line_option(const char* name, const char* explanation) noexcept {
            options_description.add_options()(name, po::value<T>(), explanation);
        }

        /**
         * Add a new command line multitoken argument option.
         * (Should be called before `parse` method is called)
         * @tparam T type of argument (e.g., --test=3 -> T is int)
         * @param name name of the command line argument (e.g., --test=3 -> name is "test")
         * @param explanation
         */
        template<typename T>
        void add_command_line_multitoken_option(const char* name, const char* explanation) noexcept {
            options_description.add_options()(name, po::value<T>()->multitoken(), explanation);
        }

        /**
         * Search whether the user set arg_name through the command line.
         * If the user set it, set target_var as the given value.
         * e.g., if arg_name="test" and user put --test=3, then *target_var=3 is invoked.
         * @tparam T type of the argument
         * @param arg_name argument name
         * @param target_var target variable to set
         */
        template<typename T>
        void set_if_defined(const char* arg_name, T* target_var) const noexcept {
            if (variables_map.count(arg_name) > 0) {
                *target_var = variables_map[arg_name].as<T>();
            }
        }

    private:
        /**
         * options description
         */
        po::options_description options_description = po::options_description("Command Line Options");

        /**
         * variables_map that contains parsed result
         */
        po::variables_map variables_map;
    };
}

#endif
