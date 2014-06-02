#include <gtest/gtest.h>

#include "statistics_manager_mock.h"
#include "usage_statistics/counter.h"

using namespace testing;

namespace usage_statistics {
namespace test {

TEST(UsageStatisticsTest, TestGlobalCounterIncrementsStatistics) {
  StrictMock<MockStatisticsManager> msm;
  GlobalCounter reboots_counter(&msm, SYNC_REBOOTS);

  EXPECT_CALL(msm, Increment(SYNC_REBOOTS)).Times(1);
  ++reboots_counter;
}

TEST(UsageStatisticsTest, TestGlobalCounterIncrementsStatisticsTwice) {
  StrictMock<MockStatisticsManager> msm;
  GlobalCounter reboots_counter(&msm, SYNC_REBOOTS);

  EXPECT_CALL(msm, Increment(SYNC_REBOOTS)).Times(2);
  ++reboots_counter;
  ++reboots_counter;
}

TEST(UsageStatisticsTest, TestAppCounterIncrementsStatistics) {
  StrictMock<MockStatisticsManager> msm;
  AppCounter user_selections_counter(&msm, "HelloApp", USER_SELECTIONS);

  EXPECT_CALL(msm, Increment("HelloApp", USER_SELECTIONS)).Times(1);
  ++user_selections_counter;
}

TEST(UsageStatisticsTest, TestAppCounterIncrementsStatisticsTwice) {
  StrictMock<MockStatisticsManager> msm;
  AppCounter user_selections_counter(&msm, "HelloApp", USER_SELECTIONS);

  EXPECT_CALL(msm, Increment("HelloApp", USER_SELECTIONS)).Times(2);
  ++user_selections_counter;
  ++user_selections_counter;
}

TEST(UsageStatisticsTest, TestAppInfoUpdates) {
  StrictMock<MockStatisticsManager> msm;
  AppInfo gui_language_info(&msm, "HelloApp", LANGUAGE_GUI);

  EXPECT_CALL(msm, Set("HelloApp", LANGUAGE_GUI, "Klingon")).Times(1);
  gui_language_info.Update("Klingon");
}

TEST(UsageStatisticsTest, TestAppInfoUpdatesTwice) {
  StrictMock<MockStatisticsManager> msm;
  AppInfo gui_language_info(&msm, "HelloApp", LANGUAGE_GUI);

  InSequence s;
  EXPECT_CALL(msm, Set("HelloApp", LANGUAGE_GUI, "Klingon")).Times(1);
  EXPECT_CALL(msm, Set("HelloApp", LANGUAGE_GUI, "UA")).Times(1);
  gui_language_info.Update("Klingon");
  gui_language_info.Update("UA");
}

TEST(UsageStatisticsTest, TestAppStopwatchAutoStopsAndAddsZero) {
  StrictMock<MockStatisticsManager> msm;
  AppStopwatch hmi_full_stopwatch(&msm, "HelloApp");

  EXPECT_CALL(msm, Add("HelloApp", SECONDS_HMI_FULL, 0)).Times(1);

  hmi_full_stopwatch.Start(SECONDS_HMI_FULL);
}

TEST(UsageStatisticsTest, TestAppStopwatchAddsZero) {
  StrictMock<MockStatisticsManager> msm;
  AppStopwatch hmi_full_stopwatch(&msm, "HelloApp");

  InSequence s;
  EXPECT_CALL(msm, Add("HelloApp", SECONDS_HMI_FULL, 0)).Times(1);
  EXPECT_CALL(msm, Add("HelloApp", SECONDS_HMI_BACKGROUND, 0)).Times(1);
  EXPECT_CALL(msm, Add("HelloApp", SECONDS_HMI_FULL, 0)).Times(1);

  hmi_full_stopwatch.Start(SECONDS_HMI_FULL);
  hmi_full_stopwatch.Switch(SECONDS_HMI_BACKGROUND);
  hmi_full_stopwatch.Switch(SECONDS_HMI_FULL);
  hmi_full_stopwatch.Stop();
}

TEST(UsageStatisticsTest, TestAppStopwatchAutoStopsInASecond) {
  StrictMock<MockStatisticsManager> msm;
  AppStopwatch hmi_full_stopwatch(&msm, "HelloApp");

  EXPECT_CALL(msm, Add("HelloApp", SECONDS_HMI_FULL, 1)).Times(1);

  hmi_full_stopwatch.Start(SECONDS_HMI_FULL);
  sleep(1);
}

TEST(UsageStatisticsTest, TestAppStopwatchStopsInTwoSeconds) {
  StrictMock<MockStatisticsManager> msm;
  AppStopwatch hmi_full_stopwatch(&msm, "HelloApp");

  EXPECT_CALL(msm, Add("HelloApp", SECONDS_HMI_NONE, 0)).Times(1);
  EXPECT_CALL(msm, Add("HelloApp", SECONDS_HMI_FULL, 1)).Times(1);
  EXPECT_CALL(msm, Add("HelloApp", SECONDS_HMI_BACKGROUND, 1)).Times(1);

  hmi_full_stopwatch.Start(SECONDS_HMI_NONE);
  hmi_full_stopwatch.Switch(SECONDS_HMI_FULL);
  sleep(1);
  hmi_full_stopwatch.Switch(SECONDS_HMI_BACKGROUND);
  sleep(1);
}


}  // namespace test
}  // namespace usage_statistics
