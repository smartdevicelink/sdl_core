#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "module_timer_test.h"

namespace functional_modules {

TEST(ModuleTimerTest, set_period) {
  ModuleTimer<TestTrackable> timer;
  timer.set_period(10000);
  ModuleTimerTest test(timer);
  EXPECT_EQ(10000u, test.period());
}

TEST(ModuleTimerTest, add_observer) {
  ModuleTimer<TestTrackable> timer;
  ModuleTimerTest test(timer);
  for (size_t i = 0; i < 5; ++i) {
    MockTimerObserver observer;
    timer.AddObserver(&observer);
  }
  EXPECT_EQ(5, test.observers_size());
}

TEST(ModuleTimerTest, remove_observer) {
  ModuleTimer<TestTrackable> timer;
  ModuleTimerTest test(timer);

  MockTimerObserver observer;
  timer.AddObserver(&observer);
  MockTimerObserver observer2;
  timer.AddObserver(&observer2);
  timer.RemoveObserver(&observer);
  EXPECT_EQ(1, test.observers_size());
  MockTimerObserver observer3;
  timer.RemoveObserver(&observer3);
  EXPECT_EQ(1, test.observers_size());
  timer.RemoveObserver(&observer2);
  EXPECT_EQ(0, test.observers_size());
}

TEST(ModuleTimerTest, start) {
  ModuleTimer<TestTrackable> timer;
  ModuleTimerTest test(timer);
  timer.set_period(1);
  MockTimerObserver observer;
  timer.AddObserver(&observer);
  TestTrackable track;
  timer.AddTrackable(track);
  sleep(2);
  EXPECT_CALL(observer, OnTimeoutTriggered(track)).Times(1);
  timer.CheckTimeout();
  EXPECT_EQ(0, test.trackables_size());
  timer.set_period(4);
  timer.AddTrackable(track);
  sleep(2);
  EXPECT_CALL(observer, OnTimeoutTriggered(track)).Times(0);
  timer.CheckTimeout();
  EXPECT_EQ(1, test.trackables_size());
  TestTrackable track2(1);
  timer.AddTrackable(track2);
  timer.AddTrackable(track);
  sleep(2);
  EXPECT_CALL(observer, OnTimeoutTriggered(track2)).Times(1);
  EXPECT_CALL(observer, OnTimeoutTriggered(track)).Times(0);
  timer.CheckTimeout();
  EXPECT_EQ(1, test.trackables_size());
}

TEST(ModuleTimerTest, add_trackable) {
  ModuleTimer<TestTrackable> timer;
  ModuleTimerTest test(timer);
  timer.AddTrackable(TestTrackable());
  ASSERT_EQ(1, test.trackables_size());
  //  adding the same object twice
  timer.AddTrackable(TestTrackable());
  EXPECT_EQ(1, test.trackables_size());
  //  adding another object
  TestTrackable track(3);
  timer.AddTrackable(track);
  ASSERT_EQ(2, test.trackables_size());
  EXPECT_TRUE(test.trackable(track).start_time() - test.current_time() < 1);
  timer.AddTrackable(track);
  sleep(3);
  EXPECT_TRUE(test.current_time() - test.trackable(track).start_time() < 4 &&
              test.current_time() - test.trackable(track).start_time() > 2);
}

TEST(ModuleTimerTest, remove_trackable) {
  ModuleTimer<TestTrackable> timer;
  ModuleTimerTest test(timer);
  EXPECT_EQ(0, test.trackables_size());
  TestTrackable track1;
  TestTrackable track2(1);
  timer.AddTrackable(track1);
  timer.AddTrackable(track2);
  EXPECT_EQ(2, test.trackables_size());
  timer.RemoveTrackable(track2);
  ASSERT_EQ(1, test.trackables_size());
  TestTrackable track3(2);
  timer.RemoveTrackable(track3);
  ASSERT_EQ(1, test.trackables_size());
  timer.RemoveTrackable(track1);
  ASSERT_EQ(0, test.trackables_size());
}

TEST(ModuleTimerTest, notify) {
  ModuleTimer<TestTrackable> timer;
  ModuleTimerTest test(timer);
  timer.set_period(1);
  MockTimerObserver observer;
  timer.AddObserver(&observer);
  TestTrackable track;
  timer.AddTrackable(track);
  TestTrackable track2(2);
  timer.AddTrackable(track2);
  sleep(2);
  EXPECT_CALL(observer, OnTimeoutTriggered(track)).Times(1);
  EXPECT_CALL(observer, OnTimeoutTriggered(track2)).Times(0);
  timer.CheckTimeout();
  EXPECT_EQ(1, test.trackables_size());
}

}  //  namespace functional_modules
