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

#include "getopt.h"

#include "cppgen/cpp_api_code_generator.h"
#include "model/api.h"
#include "pugixml.hpp"
#include "utils/safeformat.h"

using std::cout;
using std::cerr;

/*
 * Parsed command line options
 */
struct Options {
  char* interface_xml;
  std::set<std::string> requested_interfaces;
  Options()
      : interface_xml(NULL) {
  }
};

void Usage() {
  cout << "Interface generator" << '\n'
       << "Usage: intergen -f <interface definition xml>"
       << " -i <interface_name> [-i <another_interface_name>]" << '\n';
}

int main(int argc, char* argv[]) {
  Options options;
  const char* opts = "f:i:";
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
  if (options.requested_interfaces.empty()) {
    cerr << "At least one interface must be specified" << '\n';
    return EXIT_FAILURE;
  }

  pugi::xml_document doc;
  pugi::xml_parse_result result = doc.load_file(options.interface_xml);
  if (result) {
    codegen::API api;
    if (api.init(doc)) {
      codegen::CppApiCodeGenerator cpp_code_generator(&api);
      std::set<std::string> bad = cpp_code_generator.Generate(
          options.requested_interfaces);
      if (bad.empty()) {
        return EXIT_SUCCESS;
      } else {
        cerr << "Failed to generate interfaces: " << '\n';
        std::copy(bad.begin(), bad.end(),
                  std::ostream_iterator<std::string>(cerr, "\n"));
        cerr << '\n';
      }
    } else {
      cerr << "Failed to initialize" << '\n';
    }
  } else {
    std::cerr << "Failed to load xml file " << options.interface_xml << ": "
              << result.description() << '\n';
  }
  return EXIT_FAILURE;
}
