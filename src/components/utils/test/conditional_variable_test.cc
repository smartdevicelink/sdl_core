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

namespace test {
namespace components {
namespace utils {

class ConditionalVariableTest : public ::testing::Test {
 public:
  ConditionalVariableTest()
      : test_value_("initialized"),
        counter_(0) {
  }
  void check_counter();
  void task_one();

  static void* check_counter_helper(void *context) {
    (reinterpret_cast<ConditionalVariableTest *>(context))->check_counter();
    return NULL;
  }

  static void* task_one_helper(void *context) {
    (reinterpret_cast<ConditionalVariableTest *>(context))->task_one();
    return NULL;
  }
 protected:
  std::string test_value_;
  sync_primitives::ConditionalVariable cond_var_;
  sync_primitives::Lock test_mutex_;
  unsigned counter_;
};

// Defines threads behaviour which depends on counter value
void ConditionalVariableTest::check_counter() {
  sync_primitives::AutoLock test_lock(test_mutex_);
  if (counter_ <= 1) {
    counter_++;
    cond_var_.Wait(test_mutex_);  // Mutex unlock & Thread sleeps until Notification
  }
  else if(counter_ == 2) {  // Checking for equal 2 in this specific case. Because we were waiting for 2 threads to be finished
    cond_var_.Broadcast();  // Notify All threads waiting on conditional variable
  }
}

// Tasks for threads to begin with
void ConditionalVariableTest::task_one() {
  sync_primitives::AutoLock test_lock(test_mutex_);
  test_value_ = "changed by thread 1";
  cond_var_.NotifyOne();  // Notify At least one thread waiting on conditional variable
  test_value_ = "changed again by thread 1";
}

TEST_F(ConditionalVariableTest, CheckNotifyOne_OneThreadNotified_ExpectSuccessful) {
  pthread_t thread1;
  sync_primitives::AutoLock test_lock(test_mutex_);
  test_value_ = "changed by main thread";
  const bool thread_created = pthread_create(&thread1,
      NULL,
      &ConditionalVariableTest::task_one_helper,
      this);
  ASSERT_FALSE(thread_created) << "thread1 is not created!";
  test_value_ = "changed twice by main thread";
  cond_var_.WaitFor(test_lock, 2000);
  std::string last_value("changed again by thread 1");
  EXPECT_EQ(last_value, test_value_);
}

TEST_F(ConditionalVariableTest, CheckBroadcast_AllThreadsNotified_ExpectSuccessful) {
  pthread_t thread1;
  pthread_t thread2;
  bool thread_created = pthread_create(&thread1,
      NULL,
      &ConditionalVariableTest::check_counter_helper,
      this);
  ASSERT_FALSE(thread_created) << "thread1 is not created!";
  thread_created = pthread_create(&thread2,
      NULL,
      &ConditionalVariableTest::check_counter_helper,
      this);
  ASSERT_FALSE(thread_created) << "thread2 is not created!";
  check_counter();
  EXPECT_EQ(2u, counter_);
}

TEST_F(ConditionalVariableTest, CheckWaitForWithTimeout1sec_ThreadBlockedForTimeout_ExpectSuccessfulWakeUp) {
  sync_primitives::AutoLock test_lock(test_mutex_);
  sync_primitives::ConditionalVariable::WaitStatus wait_st = cond_var_.WaitFor(test_lock, 1000);
  EXPECT_EQ(sync_primitives::ConditionalVariable::kTimeout, wait_st);
}

}  // namespace utils
}  // namespace components
}  // namespace test

