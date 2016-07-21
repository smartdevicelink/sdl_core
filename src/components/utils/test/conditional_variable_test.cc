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

#include <iostream>

#include "gtest/gtest.h"

#include "utils/lock.h"
#include "utils/macro.h"
#include "utils/conditional_variable.h"
#include "utils/threads/thread.h"
#include "utils/threads/thread_delegate.h"

namespace test {
namespace components {
namespace utils_test {

class ConditionalVariableTest : public ::testing::Test {
 public:
  ConditionalVariableTest() {}
  /**
   * @brief Defines threads behaviour which depends on counter value
   */
  static void CheckCounter();
  /**
   * @brief Tasks for threads to begin with
   */
  static void TaskOne();

 protected:
  static std::string test_value_;
  static sync_primitives::ConditionalVariable cond_var_;
  static sync_primitives::Lock test_mutex_;
  static unsigned counter_;
};

unsigned ConditionalVariableTest::counter_ = 0;
std::string ConditionalVariableTest::test_value_;
sync_primitives::Lock ConditionalVariableTest::test_mutex_;
sync_primitives::ConditionalVariable ConditionalVariableTest::cond_var_;

class TaskOneDelegate : public threads::ThreadDelegate {
 public:
  void threadMain() {
    ConditionalVariableTest::TaskOne();
  }
  void exitThreadMain() {}
};

class CheckCounterDelegate : public threads::ThreadDelegate {
 public:
  void threadMain() {
    ConditionalVariableTest::CheckCounter();
  }
  void exitThreadMain() {}
};

void ConditionalVariableTest::CheckCounter() {
  sync_primitives::AutoLock test_lock(test_mutex_);
  if (counter_ <= 1) {
    counter_++;
    cond_var_.Wait(
        test_mutex_);  // Mutex unlock & Thread sleeps until Notification
  } else if (counter_ == 2) {  // Checking for equal 2 in this specific case.
                               // Because we were waiting for 2 threads to be
                               // finished
    cond_var_
        .Broadcast();  // Notify All threads waiting on conditional variable
  }
}

void ConditionalVariableTest::TaskOne() {
  sync_primitives::AutoLock test_lock(test_mutex_);
  test_value_ = "changed by thread 1";
  cond_var_.NotifyOne();  // Notify At least one thread waiting on conditional
                          // variable
  test_value_ = "changed again by thread 1";
}

TEST_F(ConditionalVariableTest,
       CheckNotifyOne_OneThreadNotified_ExpectSuccessful) {
  threads::Thread* thread =
      threads::CreateThread("test thread", new TaskOneDelegate());
  sync_primitives::AutoLock test_lock(test_mutex_);
  thread->start();
  test_value_ = "changed by main thread";
  ASSERT_TRUE(thread->CurrentId()) << "thread1 is not created!";
  test_value_ = "changed twice by main thread";
  cond_var_.WaitFor(test_lock, 2000);
  std::string last_value("changed again by thread 1");
  EXPECT_EQ(last_value, test_value_);
  thread->join(threads::Thread::kForceStop);
  threads::DeleteThread(thread);
}

TEST_F(ConditionalVariableTest,
       CheckBroadcast_AllThreadsNotified_ExpectSuccessful) {
  threads::Thread* thread1 =
      threads::CreateThread("test thread", new CheckCounterDelegate());
  thread1->start();
  ASSERT_TRUE(thread1->CurrentId()) << "thread1 is not created!";
  threads::Thread* thread2 =
      threads::CreateThread("test thread", new CheckCounterDelegate());
  thread2->start();
  ASSERT_TRUE(thread2->CurrentId()) << "thread2 is not created!";
  CheckCounter();
  EXPECT_EQ(2u, counter_);
  threads::DeleteThread(thread1);
  threads::DeleteThread(thread2);
}

TEST_F(
    ConditionalVariableTest,
    CheckWaitForWithTimeout1sec_ThreadBlockedForTimeout_ExpectSuccessfulWakeUp) {
  sync_primitives::AutoLock test_lock(test_mutex_);
  sync_primitives::ConditionalVariable::WaitStatus wait_st =
      cond_var_.WaitFor(test_lock, 1000);
  EXPECT_EQ(sync_primitives::ConditionalVariable::kTimeout, wait_st);
}

}  // namespace utils_test
}  // namespace components
}  // namespace test
