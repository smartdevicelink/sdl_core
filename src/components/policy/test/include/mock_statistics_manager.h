/* Copyright (c) 2013, Ford Motor Company
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
#ifndef SRC_COMPONENTS_POLICY_TEST_POLICY_INCLUDE_MOCK_STATISTICS_MANAGER_H_
#define SRC_COMPONENTS_POLICY_TEST_POLICY_INCLUDE_MOCK_STATISTICS_MANAGER_H_

#include <string>

#include "gmock/gmock.h"

#include "usage_statistics/statistics_manager.h"

namespace usage_statistics {
namespace test {

class MockStatisticsManager: public StatisticsManager {
 public:
  MOCK_METHOD1(Increment, void(GlobalCounterId type));
  MOCK_METHOD2(Increment, void(const std::string& app_id, AppCounterId type));
  MOCK_METHOD3(Set, void(const std::string& app_id,
                         AppInfoId type,
                         const std::string& value));
  MOCK_METHOD3(Add, void(const std::string& app_id,
                         AppStopwatchId type,
                         int32_t timespan_seconds));
};

}  // namespace test
}  // namespace usage_statistics

#endif  // SRC_COMPONENTS_POLICY_TEST_POLICY_INCLUDE_MOCK_STATISTICS_MANAGER_H_
