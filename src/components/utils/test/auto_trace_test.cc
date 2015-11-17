/*
 * Copyright (c) 2015, Ford Motor Company
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

#include "gtest/gtest.h"
#include "utils/auto_trace.h"
#include "logger.h"
#include <fstream>

namespace test {
namespace components {
namespace utils {

using namespace ::logger;

CREATE_LOGGERPTR_GLOBAL(logger_, "AutoTraceTestLog");

void Preconditions() {
  //delete file with previous logs
  const char* file_name = "AutoTraceTestLogFile.log";
  std::remove(file_name);
}

void InitLogger() {
  INIT_LOGGER("log4cxx.properties");
}

void CreateDeleteAutoTrace(const std::string & testlog) {
  LOG4CXX_AUTO_TRACE(logger_);
  LOG4CXX_DEBUG(logger_, testlog);
}

bool CheckTraceInFile(const std::string & testlog) {

  bool isLogFound = false;
  std::string line;

  std::ifstream file_log("AutoTraceTestLogFile.log");

  if (file_log.is_open()) {
    while (getline(file_log, line)) {
      std::size_t found = line.find(testlog);
      std::size_t founddebug = line.find("DEBUG");
      if ((found != std::string::npos) && (founddebug != std::string::npos)) {
        isLogFound = true;
        break;
      }
    }
    file_log.close();
  } else {
    std::cout << "file cannot be opened \n";
  }
  return isLogFound;
}

void DeinitLogger() {
  DEINIT_LOGGER();
}

//TODO(VVeremjova) APPLINK-12832 Logger does not write debug information in file
TEST(AutoTraceTest, DISABLED_Basic) {
  const std::string testlog =
      "Test trace is working!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
  Preconditions();
  InitLogger();
  CreateDeleteAutoTrace(testlog);
  DeinitLogger();

  ASSERT_TRUE(CheckTraceInFile(testlog));
}

}  // namespace utils
}  // namespace components
}  // namespace test
