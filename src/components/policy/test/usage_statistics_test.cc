/* Copyright (c) 2014, Ford Motor Company
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

TEST(UsageStatisticsTest, TestGlobalCounterIncrementsStatistics) {
  MockStatisticsManager* msm = new StrictMock<MockStatisticsManager>();
  GlobalCounter reboots_counter(msm, SYNC_REBOOTS);

  EXPECT_CALL(*msm, Increment(SYNC_REBOOTS)).Times(1);
  ++reboots_counter;
}

TEST(UsageStatisticsTest, TestGlobalCounterIncrementsStatisticsTwice) {
  MockStatisticsManager* msm = new StrictMock<MockStatisticsManager>();
  GlobalCounter reboots_counter(msm, SYNC_REBOOTS);

  EXPECT_CALL(*msm, Increment(SYNC_REBOOTS)).Times(2);
  ++reboots_counter;
  ++reboots_counter;
}

TEST(UsageStatisticsTest, TestAppCounterIncrementsStatistics) {
  MockStatisticsManager* msm = new StrictMock<MockStatisticsManager>();
  AppCounter user_selections_counter(msm, "HelloApp", USER_SELECTIONS);

  EXPECT_CALL(*msm, Increment("HelloApp", USER_SELECTIONS)).Times(1);
  ++user_selections_counter;
}

TEST(UsageStatisticsTest, TestAppCounterIncrementsStatisticsTwice) {
  MockStatisticsManager* msm = new StrictMock<MockStatisticsManager>();
  AppCounter user_selections_counter(msm, "HelloApp", USER_SELECTIONS);

  EXPECT_CALL(*msm, Increment("HelloApp", USER_SELECTIONS)).Times(2);
  ++user_selections_counter;
  ++user_selections_counter;
}

TEST(UsageStatisticsTest, TestAppInfoUpdates) {
  MockStatisticsManager* msm = new StrictMock<MockStatisticsManager>();
  AppInfo gui_language_info(msm, "HelloApp", LANGUAGE_GUI);

  EXPECT_CALL(*msm, Set("HelloApp", LANGUAGE_GUI, "Klingon")).Times(1);
  gui_language_info.Update("Klingon");
}

TEST(UsageStatisticsTest, TestAppInfoUpdatesTwice) {
  MockStatisticsManager* msm = new StrictMock<MockStatisticsManager>();
  AppInfo gui_language_info(msm, "HelloApp", LANGUAGE_GUI);

  InSequence s;
  EXPECT_CALL(*msm, Set("HelloApp", LANGUAGE_GUI, "Klingon")).Times(1);
  EXPECT_CALL(*msm, Set("HelloApp", LANGUAGE_GUI, "UA")).Times(1);
  gui_language_info.Update("Klingon");
  gui_language_info.Update("UA");
}

TEST(UsageStatisticsTest, TestAppStopwatchAutoStopsAndAddsZero) {
  MockStatisticsManager* msm = new StrictMock<MockStatisticsManager>();
  AppStopwatch hmi_full_stopwatch(msm, "HelloApp");

  EXPECT_CALL(*msm, Add("HelloApp", SECONDS_HMI_FULL, 0)).Times(1);

  hmi_full_stopwatch.Start(SECONDS_HMI_FULL);
}

TEST(UsageStatisticsTest, TestAppStopwatchAddsZero) {
  MockStatisticsManager* msm = new StrictMock<MockStatisticsManager>();
  AppStopwatch hmi_full_stopwatch(msm, "HelloApp");

  InSequence s;
  EXPECT_CALL(*msm, Add("HelloApp", SECONDS_HMI_FULL, 0)).Times(1);
  EXPECT_CALL(*msm, Add("HelloApp", SECONDS_HMI_BACKGROUND, 0)).Times(1);
  EXPECT_CALL(*msm, Add("HelloApp", SECONDS_HMI_FULL, 0)).Times(1);

  hmi_full_stopwatch.Start(SECONDS_HMI_FULL);
  hmi_full_stopwatch.Switch(SECONDS_HMI_BACKGROUND);
  hmi_full_stopwatch.Switch(SECONDS_HMI_FULL);
  hmi_full_stopwatch.Stop();
}

TEST(UsageStatisticsTest, TestAppStopwatchAutoStopsInASecond) {
  MockStatisticsManager* msm = new StrictMock<MockStatisticsManager>();
  AppStopwatch hmi_full_stopwatch(msm, "HelloApp");

  EXPECT_CALL(*msm, Add("HelloApp", SECONDS_HMI_FULL, 1)).Times(1);

  hmi_full_stopwatch.Start(SECONDS_HMI_FULL);
  sleep(1);
}

TEST(UsageStatisticsTest, TestAppStopwatchStopsInTwoSeconds) {
  MockStatisticsManager* msm = new StrictMock<MockStatisticsManager>();
  AppStopwatch hmi_full_stopwatch(msm, "HelloApp");

  EXPECT_CALL(*msm, Add("HelloApp", SECONDS_HMI_NONE, 0)).Times(1);
  EXPECT_CALL(*msm, Add("HelloApp", SECONDS_HMI_FULL, 1)).Times(1);
  EXPECT_CALL(*msm, Add("HelloApp", SECONDS_HMI_BACKGROUND, 1)).Times(1);

  hmi_full_stopwatch.Start(SECONDS_HMI_NONE);
  hmi_full_stopwatch.Switch(SECONDS_HMI_FULL);
  sleep(1);
  hmi_full_stopwatch.Switch(SECONDS_HMI_BACKGROUND);
  sleep(1);
}

}  // namespace test
}  // namespace usage_statistics
