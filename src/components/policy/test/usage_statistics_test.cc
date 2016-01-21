/* Copyright (c) 2015, Ford Motor Company
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

#include "mock_statistics_manager.h"
#include "usage_statistics/counter.h"

using ::testing::StrictMock;
using ::testing::InSequence;

namespace usage_statistics {
namespace test {

// TEST(A, B_C_D) { ... }
// A - What you test
// B - What you do
// C - Input data
// D - Expected result

TEST(StatisticsManagerIncrementMethod1Arg, GlobalCounterOverloadedIncrement_CallONCE_StatisticsManagerIncrementCalledONCE) {
  // Arrange
  MockStatisticsManager* msm = new StrictMock<MockStatisticsManager>();
  GlobalCounter reboots_counter(msm, SYNC_REBOOTS);

  // Assert
  EXPECT_CALL(*msm, Increment(SYNC_REBOOTS));

  // Act
  ++reboots_counter;
}

TEST(StatisticsManagerIncrementMethod1Arg, GlobalCounterOverloadedIncrement_CallTWICE_StatisticsManagerIncrementCalledTWICE) {
  // Arrange
  MockStatisticsManager* msm = new StrictMock<MockStatisticsManager>();
  GlobalCounter reboots_counter(msm, SYNC_REBOOTS);

  // Assert
  EXPECT_CALL(*msm, Increment(SYNC_REBOOTS)).Times(2);

  // Act
  ++reboots_counter;
  ++reboots_counter;
}

TEST(StatisticsManagerIncrementMethod2Args, AppCounterOverloadedIncrement_CallONCE_StatisticsManagerIncrementCalledONCE) {
  // Arrange
  MockStatisticsManager* msm = new StrictMock<MockStatisticsManager>();
  AppCounter user_selections_counter(msm, "HelloApp", USER_SELECTIONS);

  // Assert
  EXPECT_CALL(*msm, Increment("HelloApp", USER_SELECTIONS));

  // Act
  ++user_selections_counter;
}

TEST(StatisticsManagerIncrementMethod2Args, AppCounterOverloadedIncrement_CallTWICE_StatisticsManagerIncrementCalledTWICE) {
  // Arrange
  MockStatisticsManager* msm = new StrictMock<MockStatisticsManager>();
  AppCounter user_selections_counter(msm, "HelloApp", USER_SELECTIONS);

  // Assert
  EXPECT_CALL(*msm, Increment("HelloApp", USER_SELECTIONS)).Times(2);

  // Act
  ++user_selections_counter;
  ++user_selections_counter;
}
//---
TEST(StatisticsManagerSetMethod, AppInfoUpdateMethod_CallONCE_StatisticsManagerSetMethodCalledONCE) {
  // Arrange
  MockStatisticsManager* msm = new StrictMock<MockStatisticsManager>();
  AppInfo gui_language_info(msm, "HelloApp", LANGUAGE_GUI);

  // Assert
  EXPECT_CALL(*msm, Set("HelloApp", LANGUAGE_GUI, "Klingon"));

  // Act
  gui_language_info.Update("Klingon");
}

TEST(StatisticsManagerSetMethod, AppInfoUpdateMethod_CallTWICE_StatisticsManagerSetMethodCalledTWICE) {
  // Arrange
  MockStatisticsManager* msm = new StrictMock<MockStatisticsManager>();
  AppInfo gui_language_info(msm, "HelloApp", LANGUAGE_GUI);

  // Assert
  EXPECT_CALL(*msm, Set("HelloApp", LANGUAGE_GUI, "Klingon"));
  EXPECT_CALL(*msm, Set("HelloApp", LANGUAGE_GUI, "UA"));

  // Act
  gui_language_info.Update("Klingon");
  gui_language_info.Update("UA");
}


TEST(StatisticsManagerAddMethod, AppStopwatchStartMethod_CallONCE_StatisticsManagerAddMethodCalledONCE) {
  // Arrange
  MockStatisticsManager* msm = new StrictMock<MockStatisticsManager>();
  const std::uint32_t time_out = 1;
  AppStopwatch hmi_full_stopwatch(msm, "HelloApp", time_out);

  hmi_full_stopwatch.Start(SECONDS_HMI_FULL);
  // Assert
  EXPECT_CALL(*msm, Add("HelloApp", SECONDS_HMI_FULL, time_out));

  // Act
  hmi_full_stopwatch.WriteTime();
}

TEST(StatisticsManagerAddMethod, AppStopwatchSwitchMethod_Call_StatisticsManagerAddMethodCalled) {
  // Arrange
  MockStatisticsManager* msm = new StrictMock<MockStatisticsManager>();
  AppStopwatch hmi_full_stopwatch(msm, "HelloApp");
  hmi_full_stopwatch.Start(SECONDS_HMI_FULL);

  hmi_full_stopwatch.Switch(SECONDS_HMI_FULL);
  // Assert
  EXPECT_CALL(*msm, Add("HelloApp", SECONDS_HMI_FULL, 60));

  // Act
  hmi_full_stopwatch.WriteTime();

}

TEST(StatisticsManagerAddMethod, AppStopwatchSwitchMethod_CallAnd1SecSleepAfter_StatisticsManagerAddMethodCalledWith1SecTimespan) {
  // Arrange
  MockStatisticsManager* msm = new StrictMock<MockStatisticsManager>();
  const std::uint32_t time_out = 1;
  AppStopwatch hmi_full_stopwatch(msm, "HelloApp", time_out);

  // Act
  hmi_full_stopwatch.Start(SECONDS_HMI_NONE);
  // Assert
  EXPECT_CALL(*msm, Add("HelloApp", SECONDS_HMI_NONE, time_out));
  // Act
  hmi_full_stopwatch.WriteTime();

  hmi_full_stopwatch.Switch(SECONDS_HMI_BACKGROUND);
  // Assert
  EXPECT_CALL(*msm, Add("HelloApp", SECONDS_HMI_BACKGROUND, time_out));
  // Act
  hmi_full_stopwatch.WriteTime();
}
}  // namespace test
}  // namespace usage_statistics
