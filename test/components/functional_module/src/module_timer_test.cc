#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "utils/macro.h"
#include "functional_module/module_timer.h"

namespace functional_modules {

class TestTrackable : Trackable {
 public:
  explicit TestTrackable(TimeUnit interval = 0)
    : custorm_interval_(interval) {}

  virtual TimeUnit custorm_interval() const {
    return custorm_interval_;
  }
  virtual TimeUnit start_time() const {
    return start_time_;
  }
  virtual void set_start_time(TimeUnit start_time) {
    start_time_ = start_time;
  }

  bool operator<(const TestTrackable& other) const {
    return custorm_interval_ < other.custorm_interval_;
  }
 private:
  TimeUnit start_time_;
  TimeUnit custorm_interval_;
};

class ModuleTimerTest {
 public:
  ModuleTimerTest(ModuleTimer<TestTrackable>& timer)
    : timer_(timer) {}
  TimeUnit period() const {
    return timer_.period_;
  }
  int observers_size() const {
    return timer_.observers_.size();
  }
  bool keeps_running() const {
    return timer_.keep_running_;
  }
  int trackables_size() const {
    return timer_.trackables_.size();
  }
  TestTrackable trackable(const TestTrackable& track) const {
    return *timer_.trackables_.find(track);
  }
  TimeUnit current_time() const {
    return timer_.CurrentTime();
  }
 private:
  ModuleTimer<TestTrackable>& timer_;
};

class MockTimerObserver : public TimerObserver<TestTrackable> {
 public:
  MOCK_METHOD1(OnTimeoutTriggered, void(const TestTrackable& expired));
};


TEST(ModuleTimerTest, set_period) {
  ModuleTimer<TestTrackable> timer;
  timer.set_period(10000);
  ModuleTimerTest test(timer);
  EXPECT_EQ(10000, test.period());
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

/*TEST(ModuleTimerTest, start) {
  ModuleTimer<TestTrackable> timer;
  ModuleTimerTest test(timer);
  EXPECT_FALSE(test.keeps_running());
  timer.Start();
  ASSERT_TRUE(test.keeps_running());
}

TEST(ModuleTimerTest, stop) {
  ModuleTimer<TestTrackable> timer;
  ModuleTimerTest test(timer);
  timer.Stop();
  EXPECT_FALSE(test.keeps_running());
  timer.Start();
  timer.Stop();
  ASSERT_FALSE(test.keeps_running());
}*/

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
  EXPECT_TRUE(
    test.trackable(track).start_time() - test.current_time() < 1);
  timer.AddTrackable(track);
  printf("%d\n", test.current_time());
  sleep(3);
  printf("%d\n", test.current_time());
  EXPECT_TRUE(
    test.trackable(track).start_time() - test.current_time() < 4000 &&
    test.trackable(track).start_time() - test.current_time() > 2000);
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
  //ModuleTimer<TestTrackable>
}

}   //  namespace functional_modules
