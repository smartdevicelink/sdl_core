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

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "utils/logger2.h"

#include "utils/logger/log4cxxlogger.h"
#include "utils/logger/logger_impl.h"

namespace test {
namespace components {
namespace utils_test {

// using namespace ::logger;
using ::testing::_;

class LoggerMainTest : public testing::Test {
 public:
};

CREATE_LOGGERPTR("main test")

TEST(LoggerMainTest, Try_log) {
  logger::Log4CXXLogger logger("log4cxx.properties");

  logger::Logger<logger::Log4CXXLogger>::instance().Init(
      &logger);  // move logger_ to Logger instance

  LOG_WITH_LEVEL2(logger::LogLevel::DEBUG_LEVEL, "Hello");
  LOG_WITH_LEVEL2(logger::LogLevel::ERROR_LEVEL, "Hello");
  logger::Logger<logger::Log4CXXLogger>::instance().DeInit();
}

}  // namespace utils_test
}  // namespace components
}  // namespace test
