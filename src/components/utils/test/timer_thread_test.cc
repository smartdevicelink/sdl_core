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

#include <pthread.h>
#include <iostream>

#include "lock.h"
#include "macro.h"

#include "gtest/gtest.h"
#include "utils/conditional_variable.h"
#include "utils/timer_thread.h"

namespace test {
namespace components {
namespace utils {

using namespace timer;
using namespace sync_primitives;

class TimerThreadTest : public ::testing::Test {
 public:
  TimerThreadTest()
      : check_val(0),
        val1(3),
        val2(4),
        wait_val(3000) {
  }

  void function() {
    AutoLock alock(lock_);
    ++check_val;
    condvar_.NotifyOne();
  }

 protected:
  uint32_t check_val;
  Lock lock_;
  ConditionalVariable condvar_;
  const uint32_t val1;
  const uint32_t val2;
  const uint32_t wait_val;
};

TEST_F(TimerThreadTest, StartTimerThreadWithTimeoutOneSec_ExpectSuccessfullInvokeCallbackFuncOnTimeout) {
  // Create Timer with TimerDeleagate
  TimerThread<TimerThreadTest> timer("Test", this, &TimerThreadTest::function,
                                     false);
  AutoLock alock(lock_);
  EXPECT_EQ(0u, check_val);
  timer.start(100);
  condvar_.WaitFor(alock, wait_val);
  EXPECT_EQ(1u, check_val);
}

TEST_F(TimerThreadTest, StartTimerThreadWithTimeoutOneSecInLoop_ExpectSuccessfullInvokeCallbackFuncOnEveryTimeout) {
  // Create Timer with TimerLooperDeleagate
  TimerThread<TimerThreadTest> timer("Test", this, &TimerThreadTest::function,
                                     true);
  AutoLock alock(lock_);
  EXPECT_EQ(0u, check_val);
  timer.start(100);
  while (check_val < val2) {
    condvar_.WaitFor(alock, wait_val);
  }
  // Check callback function was called 4 times
  EXPECT_EQ(val2, check_val);
}

TEST_F(TimerThreadTest, StopStartedTimerThreadWithTimeoutOneSecInLoop_ExpectSuccessfullStop) {
  // Create Timer with TimerLooperDeleagate
  TimerThread<TimerThreadTest> timer("Test", this, &TimerThreadTest::function,
                                     true);
  AutoLock alock(lock_);
  EXPECT_EQ(0u, check_val);
  timer.start(100);
  // Stop timer on 3rd second
  while (check_val < val2) {
    if (check_val == val1) {
      timer.stop();
      break;
    }
    condvar_.WaitFor(alock, wait_val);
  }
  EXPECT_EQ(val1, check_val);
}

TEST_F(TimerThreadTest, ChangeTimeoutForStartedTimerThreadWithTimeoutOneSecInLoop_ExpectSuccessfullStop) {
  // Create Timer with TimerLooperDeleagate
  TimerThread<TimerThreadTest> timer("Test", this, &TimerThreadTest::function,
                                     true);
  AutoLock alock(lock_);
  EXPECT_EQ(0u, check_val);
  timer.start(100);
  // Change timer timeout
  while (check_val < val2) {
    if (check_val == val1) {
      timer.updateTimeOut(200);
    }
    condvar_.WaitFor(alock, wait_val);
  }
  EXPECT_EQ(val2, check_val);
}

TEST_F(TimerThreadTest, CheckStartedTimerIsRunning_ExpectTrue) {
  // Create Timer with TimerLooperDeleagate
  TimerThread<TimerThreadTest> timer("Test", this, &TimerThreadTest::function,
                                     true);
  AutoLock alock(lock_);
  EXPECT_EQ(0u, check_val);
  timer.start(100);
  // Change timer timeout on 3rd second
  while (check_val < val1) {
    condvar_.WaitFor(alock, wait_val);
    // Check start is running
    EXPECT_TRUE(timer.isRunning());
  }
  EXPECT_EQ(val1, check_val);
}

}  // namespace utils
}  // namespace components
}  // namespace test

