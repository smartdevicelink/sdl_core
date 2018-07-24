/**
 * Copyright (c) 2014, Ford Motor Company
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of the Ford Motor Company nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <algorithm>
#include <iostream>
#include <iterator>
#include <set>
#include <sstream>

#include "getopt.h"

#include "cppgen/cpp_api_code_generator.h"
#include "model/api.h"
#include "model/model_filter.h"
#include "pugixml.hpp"
#include "utils/safeformat.h"

using std::cout;
using std::cerr;

/*
 * Parsed command line options
 */
struct Options {
  char* interface_xml;
  bool  auto_generate_function_ids;
  bool  generate_json_code;
  std::set<std::string> requested_interfaces;
  std::set<std::string> excluded_scopes;
  bool  avoid_unsigned;
  int   minimum_word_size;
  Options()
      : interface_xml(NULL),
        auto_generate_function_ids(false),
        generate_json_code(false),
        avoid_unsigned(false),
        minimum_word_size(8) {
  }
};

void Usage() {
  cout << "Interface generator\n"
       << "Usage: intergen -f <xml_file> <options>\n"
       << "Options are:\n"
       << "  -f <xml_file>       Specifies interface definition input xml file\n"
       << "  -i <interface_name> Specifies interface name from given xml file.\n"
       << "                      Must be given as a lower_case_identifier.\n"
       << "                      Option can occur multiple times to select\n"
       << "                      multiple interfaces.\n"
       << "  -s <scope_name>     Excludes entities marked with given scope from\n"
       << "                      generated code. Can occur multiple times.\n"
       << "  -a                  Automatically generates function ID enum.\n"
       << "  -U                  Avoid unsigned integers in generated types\n"
       << "  -w <word_bits>      Minimal word size (integer size in bits) in generated types\n"
       << "  -j                  Generate json serialization code\n"
       << "  -d                  Generate d-bus serialization code\n";
}

int main(int argc, char* argv[]) {
  if (argc == 1) {
    Usage();
    return EXIT_FAILURE;
  }
  Options options;
  const char* opts = "ajdUf:i:s:w:";
  for (int opt = getopt(argc, argv, opts); opt != -1;
      opt = getopt(argc, argv, opts)) {
    switch (opt) {
      case 'i': {
        options.requested_interfaces.insert(optarg);
        break;
      }
      case 'f': {
        if (options.interface_xml == NULL) {
          options.interface_xml = optarg;
        } else {
          cerr << "Option 'f' specified multiple times" << '\n';
        }
        break;
      }
      case 's': {
        options.excluded_scopes.insert(optarg);
        break;
      }
      case 'a': {
        options.auto_generate_function_ids = true;
        break;
      }
      case 'U': {
        options.avoid_unsigned = true;
        break;
      }
      case 'w': {
        static const int kValidWidths[] = {8, 16, 32, 64};
        std::set<int> valid_widths(kValidWidths, kValidWidths + 4);

        std::stringstream wordwidth(optarg);
        if (!(wordwidth>>options.minimum_word_size)
            || valid_widths.count(options.minimum_word_size) == 0) {
          cerr << "Invalid word size provided, valid values are "
                  "8, 16, 32 or 64" << '\n';
          return EXIT_FAILURE;
        }
        break;
      }
      case 'j': {
        options.generate_json_code = true;
        break;
      }
      default: {
        cerr << "Invalid option: '" << opt << "'" << '\n';
        return EXIT_FAILURE;
      }
    }
  }

  if (options.interface_xml == NULL) {
    cerr << "Interface definition file must be specified" << '\n';
    return EXIT_FAILURE;
  }
  pugi::xml_document doc;
  pugi::xml_parse_result result = doc.load_file(options.interface_xml);
  if (result) {
    codegen::ModelFilter model_filter(options.excluded_scopes);
    codegen::API api(&model_filter, options.auto_generate_function_ids);
    if (api.init(doc)) {
      codegen::CppApiCodeGenerator cpp_code_generator(&api);
      std::set<std::string> bad =
          cpp_code_generator.Generate(
            codegen::Preferences(options.minimum_word_size,
                                 options.avoid_unsigned,
                                 options.generate_json_code,
                                 options.requested_interfaces));
      if (bad.empty()) {
        return EXIT_SUCCESS;
      } else {
        cerr << "Failed to generate interfaces: " << '\n';
        std::copy(bad.begin(), bad.end(),
                  std::ostream_iterator<std::string>(cerr, "\n"));
        cerr << '\n';
      }
    } else {
      cerr << "Failed to process xml" << '\n';
    }
  } else {
    std::cerr << "Failed to load xml file " << options.interface_xml << ": "
              << result.description() << '\n';
  }
  return EXIT_FAILURE;
}
