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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_TEST_STATE_CONTROLLER_INCLUDE_STATISTICS_MANAGER_MOCK_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_TEST_STATE_CONTROLLER_INCLUDE_STATISTICS_MANAGER_MOCK_H_

#include <string>
#include "gmock/gmock.h"
#include "usage_statistics/statistics_manager.h"

namespace test {
namespace components {
namespace state_controller_test {

namespace us = usage_statistics;

class StatisticsManagerMock : public us::StatisticsManager {
 public:
  MOCK_METHOD1(Increment, void (us::GlobalCounterId));
  MOCK_METHOD2(Increment, void (const std::string&, us::AppCounterId));
  MOCK_METHOD3(Set, void (const std::string&, us::AppInfoId, const std::string&));
  MOCK_METHOD3(Add, void (const std::string&, us::AppStopwatchId, int32_t));
};

}  // namespace state_controller_test
}
}

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_TEST_STATE_CONTROLLER_INCLUDE_STATISTICS_MANAGER_MOCK_H_
