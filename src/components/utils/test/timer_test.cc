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

sync_primitives::Lock shot_lock;
sync_primitives::ConditionalVariable shot_condition;

const std::string kTimerName = "TestTimer";

/*
 * Default timeout used during timer testing.
 * Value should be greater than at least 30 ms
 * to avoid timer firing between two sequental Start/Stop calls
 */
const uint32_t kDefaultTimeoutMs = 30u;
const uint32_t kDefaultTimeoutRestartMs = 45u;

class TestTask : public timer::TimerTask {
 public:
  TestTask() : timer_(NULL), calls_count_(0u) {}

  void set_timer(timer::Timer* timer) {
    timer_ = timer;
  }

  virtual void PerformTimer() const {}

  void run() const OVERRIDE {
    sync_primitives::AutoLock auto_lock(shot_lock);
    ++calls_count_;
    shot_condition.NotifyOne();
    PerformTimer();
  }

  size_t calls_count() const {
    return calls_count_;
  }

 protected:
  mutable timer::Timer* timer_;
  mutable size_t calls_count_;
};

class TestTaskWithStart : public TestTask {
 public:
  void PerformTimer() const OVERRIDE {
    if (timer_) {
      timer_->Start(kDefaultTimeoutRestartMs, timer::kPeriodic);
    }
  }
};

class TestTaskWithStop : public TestTask {
 public:
  void PerformTimer() const OVERRIDE {
    if (timer_) {
      timer_->Stop();
    }
  }
};

}  // namespace

// Start - Stop

TEST(TimerTest, Start_Stop_NoLoop_NoCall) {
  MockTimerTask* mock_task = new MockTimerTask();
  EXPECT_CALL(*mock_task, run()).Times(0);

  timer::Timer timer(kTimerName, mock_task);
  EXPECT_FALSE(timer.is_running());
  EXPECT_EQ(0u, timer.timeout());

  timer.Start(kDefaultTimeoutMs, timer::kSingleShot);
  EXPECT_TRUE(timer.is_running());
  EXPECT_EQ(kDefaultTimeoutMs, timer.timeout());

  timer.Stop();
  EXPECT_FALSE(timer.is_running());
  EXPECT_EQ(0u, timer.timeout());
}

TEST(TimerTest, Start_Stop_Loop_NoCall) {
  MockTimerTask* mock_task = new MockTimerTask();
  EXPECT_CALL(*mock_task, run()).Times(0);

  timer::Timer timer(kTimerName, mock_task);
  EXPECT_FALSE(timer.is_running());
  EXPECT_EQ(0u, timer.timeout());

  timer.Start(kDefaultTimeoutMs, timer::kPeriodic);
  EXPECT_TRUE(timer.is_running());
  EXPECT_EQ(kDefaultTimeoutMs, timer.timeout());

  timer.Stop();
  EXPECT_FALSE(timer.is_running());
  EXPECT_EQ(0u, timer.timeout());
}

TEST(TimerTest, Start_Stop_NoLoop_OneCall) {
  sync_primitives::AutoLock auto_lock(shot_lock);
  TestTask* task = new TestTask();

  timer::Timer timer(kTimerName, task);
  EXPECT_FALSE(timer.is_running());
  EXPECT_EQ(0u, timer.timeout());

  timer.Start(kDefaultTimeoutMs, timer::kSingleShot);
  EXPECT_TRUE(timer.is_running());
  EXPECT_EQ(kDefaultTimeoutMs, timer.timeout());

  // Wait for 1 call
  shot_condition.Wait(shot_lock);
  EXPECT_FALSE(timer.is_running());

  timer.Stop();
  EXPECT_FALSE(timer.is_running());
  EXPECT_EQ(0u, timer.timeout());

  EXPECT_EQ(1u, task->calls_count());
}

TEST(TimerTest, Start_Stop_Loop_3Calls) {
  const size_t loops_count = 3u;

  sync_primitives::AutoLock auto_lock(shot_lock);
  TestTask* task = new TestTask();

  timer::Timer timer(kTimerName, task);
  EXPECT_FALSE(timer.is_running());
  EXPECT_EQ(0u, timer.timeout());

  timer.Start(kDefaultTimeoutMs, timer::kPeriodic);
  EXPECT_TRUE(timer.is_running());
  EXPECT_EQ(kDefaultTimeoutMs, timer.timeout());

  // Wait for 3 calls
  for (size_t i = 0; i < loops_count; ++i) {
    shot_condition.Wait(shot_lock);
    EXPECT_TRUE(timer.is_running());
  }

  timer.Stop();
  EXPECT_FALSE(timer.is_running());
  EXPECT_EQ(0u, timer.timeout());

  EXPECT_EQ(loops_count, task->calls_count());
}

// Restart

TEST(TimerTest, Restart_NoLoop_NoCall) {
  MockTimerTask* mock_task = new MockTimerTask();
  EXPECT_CALL(*mock_task, run()).Times(0);

  timer::Timer timer(kTimerName, mock_task);

  timer.Start(kDefaultTimeoutMs, timer::kSingleShot);
  EXPECT_TRUE(timer.is_running());
  EXPECT_EQ(kDefaultTimeoutMs, timer.timeout());

  timer.Start(kDefaultTimeoutRestartMs, timer::kSingleShot);
  EXPECT_TRUE(timer.is_running());
  EXPECT_EQ(kDefaultTimeoutRestartMs, timer.timeout());
}

TEST(TimerTest, Restart_Loop_NoCall) {
  MockTimerTask* mock_task = new MockTimerTask();
  EXPECT_CALL(*mock_task, run()).Times(0);

  timer::Timer timer(kTimerName, mock_task);

  timer.Start(kDefaultTimeoutMs, timer::kPeriodic);
  EXPECT_TRUE(timer.is_running());
  EXPECT_EQ(kDefaultTimeoutMs, timer.timeout());

  timer.Start(kDefaultTimeoutRestartMs, timer::kPeriodic);
  EXPECT_TRUE(timer.is_running());
  EXPECT_EQ(kDefaultTimeoutRestartMs, timer.timeout());
}

TEST(TimerTest, Restart_Loop_3Calls) {
  const size_t loops_count = 3u;

  sync_primitives::AutoLock auto_lock(shot_lock);
  TestTask* task = new TestTask();
  timer::Timer timer(kTimerName, task);

  timer.Start(kDefaultTimeoutMs, timer::kPeriodic);
  EXPECT_TRUE(timer.is_running());
  EXPECT_EQ(kDefaultTimeoutMs, timer.timeout());

  // Wait for 3 calls
  for (size_t i = 0; i < loops_count; ++i) {
    shot_condition.Wait(shot_lock);
  }
  timer.Start(kDefaultTimeoutRestartMs, timer::kPeriodic);
  EXPECT_TRUE(timer.is_running());
  EXPECT_EQ(kDefaultTimeoutRestartMs, timer.timeout());

  EXPECT_EQ(loops_count, task->calls_count());
}

// Restart from call

TEST(TimerTest, Restart_NoLoop_FromCall) {
  sync_primitives::AutoLock auto_lock(shot_lock);
  TestTask* task = new TestTaskWithStart();
  timer::Timer timer(kTimerName, task);
  task->set_timer(&timer);

  timer.Start(kDefaultTimeoutMs, timer::kSingleShot);
  EXPECT_TRUE(timer.is_running());
  EXPECT_EQ(kDefaultTimeoutMs, timer.timeout());

  // Wait for 1 calls
  shot_condition.Wait(shot_lock);

  EXPECT_TRUE(timer.is_running());
  EXPECT_EQ(kDefaultTimeoutRestartMs, timer.timeout());

  EXPECT_EQ(1u, task->calls_count());
}

TEST(TimerTest, Restart_Loop_FromCall) {
  sync_primitives::AutoLock auto_lock(shot_lock);
  TestTask* task = new TestTaskWithStart();
  timer::Timer timer(kTimerName, task);
  task->set_timer(&timer);

  timer.Start(kDefaultTimeoutMs, timer::kPeriodic);
  EXPECT_TRUE(timer.is_running());
  EXPECT_EQ(kDefaultTimeoutMs, timer.timeout());

  // Wait for 1 calls
  shot_condition.Wait(shot_lock);

  EXPECT_TRUE(timer.is_running());
  EXPECT_EQ(kDefaultTimeoutRestartMs, timer.timeout());

  EXPECT_EQ(1u, task->calls_count());
}

// Stop from call

TEST(TimerTest, Stop_Loop_FromCall) {
  sync_primitives::AutoLock auto_lock(shot_lock);
  TestTask* task = new TestTaskWithStop();
  timer::Timer timer(kTimerName, task);
  task->set_timer(&timer);

  timer.Start(kDefaultTimeoutMs, timer::kPeriodic);
  EXPECT_TRUE(timer.is_running());
  EXPECT_EQ(kDefaultTimeoutMs, timer.timeout());

  // Wait for 1 calls
  shot_condition.Wait(shot_lock);

  EXPECT_FALSE(timer.is_running());
  EXPECT_EQ(0u, timer.timeout());

  EXPECT_EQ(1u, task->calls_count());
}

// Delete running

TEST(TimerTest, Delete_Running_NoLoop) {
  MockTimerTask* mock_task = new MockTimerTask();
  EXPECT_CALL(*mock_task, run()).Times(0);

  timer::Timer* timer = new timer::Timer(kTimerName, mock_task);
  EXPECT_FALSE(timer->is_running());
  EXPECT_EQ(0u, timer->timeout());

  timer->Start(kDefaultTimeoutMs, timer::kSingleShot);
  EXPECT_TRUE(timer->is_running());
  EXPECT_EQ(kDefaultTimeoutMs, timer->timeout());

  delete timer;
}

}  // namespace timer_test
}  // namespace components
}  // namespace test
