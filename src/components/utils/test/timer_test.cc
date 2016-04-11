/*
 * Copyright (c) 2016, Ford Motor Company
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
#include "utils/timer.h"
#include "utils/mock_timer_task.h"
#include "utils/conditional_variable.h"
#include "utils/lock.h"
#include "utils/macro.h"
#include "utils/timer_task_impl.h"

namespace test {
namespace components {
namespace timer_test {
namespace {

sync_primitives::Lock test_lock;
sync_primitives::ConditionalVariable lock_monitor;
const uint32_t kDefaultTimeout = 30u;
const std::string kTimerName = "test_timer";

class TestTask : public timer::TimerTask {
 public:
  TestTask() : calls_count_(0u) {}

  void run() const OVERRIDE {
    sync_primitives::AutoLock auto_lock(test_lock);
    ++calls_count_;
    lock_monitor.NotifyOne();
  }

  uint GetCallsCount() const {
    return calls_count_;
  }

 private:
  mutable uint calls_count_;
};

class FakeClassWithTimer {
 public:
  FakeClassWithTimer()
      : calls_count_(0u)
      , internal_timer_("test_timer",
                        new timer::TimerTaskImpl<FakeClassWithTimer>(
                            this, &FakeClassWithTimer::OnTimer)) {
    internal_timer_.Start(kDefaultTimeout, false);
  }

  void OnTimer() {
    sync_primitives::AutoLock auto_lock_(test_lock);
    internal_timer_.Stop();
    ++calls_count_;
    lock_monitor.NotifyOne();
  }

  bool IsTimerRunning() const {
    return internal_timer_.is_running();
  }

  uint GetCallsCount() const {
    return calls_count_;
  }

 private:
  uint calls_count_;
  timer::Timer internal_timer_;
};
}  // namespace

class TimerTest : public testing::Test {
 protected:
  void SetUp() OVERRIDE {
    timeout_ = kDefaultTimeout;
    single_shot_ = true;
    // Will be destroyed in Timer Destructor
    test_task_ = new MockTimerTask();
  }
  bool single_shot_;
  MockTimerTask* test_task_;
  uint32_t timeout_;
};

TEST_F(TimerTest, Start_ZeroTimeout_CorrectTimeout) {
  // Preconditions
  timer::Timer test_timer(kTimerName, test_task_);
  // Actions
  test_timer.Start(0u, single_shot_);
  // Expects
  EXPECT_EQ(0u, test_timer.timeout());

  test_timer.Stop();
}

TEST_F(TimerTest, Start_NoLoop_OneCall) {
  // Preconditions
  test_lock.Acquire();
  TestTask* task = new TestTask();
  timer::Timer test_timer(kTimerName, task);
  // Actions
  test_timer.Start(timeout_, single_shot_);
  ASSERT_TRUE(test_timer.is_running());
  // Wait for call
  lock_monitor.Wait(test_lock);
  test_lock.Release();
  EXPECT_FALSE(test_timer.is_running());
  EXPECT_EQ(1u, task->GetCallsCount());
}

TEST_F(TimerTest, Start_Loop_3Calls) {
  // Preconditions
  uint loops_count = 3u;
  single_shot_ = false;
  test_lock.Acquire();
  TestTask* task = new TestTask();
  timer::Timer test_timer(kTimerName, task);
  // Actions
  test_timer.Start(timeout_, single_shot_);
  for (uint i = loops_count; i; --i) {
    lock_monitor.Wait(test_lock);
  }
  test_lock.Release();
  test_timer.Stop();

  EXPECT_EQ(loops_count, task->GetCallsCount());
}

TEST_F(TimerTest, Start_Runned_RunnedWithNewTimeout) {
  // Preconditions
  timer::Timer test_timer(kTimerName, test_task_);
  // Actions
  test_timer.Start(timeout_, single_shot_);
  // Expects
  ASSERT_EQ(timeout_, test_timer.timeout());
  ASSERT_TRUE(test_timer.is_running());
  // Actions
  timeout_ = 1000u;
  test_timer.Start(timeout_, single_shot_);
  // Expects
  ASSERT_EQ(timeout_, test_timer.timeout());
  ASSERT_TRUE(test_timer.is_running());
}

TEST_F(TimerTest, Start_NotRunned_RunnedWithNewTimeout) {
  // Preconditions
  timer::Timer test_timer(kTimerName, test_task_);
  // Expects
  ASSERT_EQ(0u, test_timer.timeout());
  ASSERT_FALSE(test_timer.is_running());
  // Actions
  timeout_ = 1000u;
  test_timer.Start(timeout_, single_shot_);
  // Expects
  ASSERT_EQ(timeout_, test_timer.timeout());
  ASSERT_TRUE(test_timer.is_running());
}

TEST_F(TimerTest, Stop_FirstLoop_NoCall) {
  // Preconditions
  timer::Timer test_timer(kTimerName, test_task_);
  // Expects
  EXPECT_CALL(*test_task_, run()).Times(0);
  // Actions
  test_timer.Start(10000u, single_shot_);
  test_timer.Stop();
}

TEST_F(TimerTest, Stop_SecondLoop_OneCall) {
  // Preconditions
  test_lock.Acquire();
  TestTask* task = new TestTask();
  timer::Timer test_timer(kTimerName, task);
  // Actions
  test_timer.Start(timeout_, single_shot_);
  ASSERT_TRUE(test_timer.is_running());
  // Wait for Starting second loop
  lock_monitor.Wait(test_lock);
  test_timer.Stop();
  test_lock.Release();
  // Expects
  EXPECT_EQ(1u, task->GetCallsCount());
}

TEST_F(TimerTest, IsRunning_Started_True) {
  // Preconditions
  timer::Timer test_timer(kTimerName, test_task_);
  // Actions
  test_timer.Start(timeout_, single_shot_);
  // Expects
  EXPECT_TRUE(test_timer.is_running());
}

TEST_F(TimerTest, IsRunning_Stoped_False) {
  // Preconditions
  timer::Timer test_timer(kTimerName, test_task_);
  // Actions
  test_timer.Start(timeout_, single_shot_);
  ASSERT_TRUE(test_timer.is_running());
  test_timer.Stop();
  // Expects
  EXPECT_FALSE(test_timer.is_running());
}

TEST_F(TimerTest, IsRunning_Suspended_FalseAndOneCall) {
  // Preconditions
  test_lock.Acquire();
  FakeClassWithTimer fake_class;
  // Expects
  ASSERT_EQ(0u, fake_class.GetCallsCount());
  ASSERT_TRUE(fake_class.IsTimerRunning());
  // Wait for end of loop
  lock_monitor.Wait(test_lock);
  test_lock.Release();
  // Expects
  ASSERT_EQ(1u, fake_class.GetCallsCount());
  ASSERT_FALSE(fake_class.IsTimerRunning());
}

}  // namespace timer_test
}  // namespace components
}  // namespace test
