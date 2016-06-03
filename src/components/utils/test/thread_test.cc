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

#include "gtest/gtest.h"
#include "utils/lock.h"
#include "utils/threads/thread.h"

namespace test {
namespace components {
namespace utils {

using namespace sync_primitives;
using namespace threads;

// TODO(AByzhynar): Add multithreading tests

namespace {

const size_t kThreadStackSize = 32768u;
const std::string kThreadName("Test thread");
const uint32_t kWaitTime = 1000u;

sync_primitives::ConditionalVariable cond_var_;
sync_primitives::Lock test_mutex_;

}  // namespace

class TestDelegate : public threads::ThreadDelegate {
 public:
  TestDelegate() : check_value_(false) {}
  void threadMain() {
    AutoLock test_lock(test_mutex_);
    check_value_ = true;
    cond_var_.NotifyOne();
  }
  void exitThreadMain() {}

  bool check_value() const {
    return check_value_;
  }

 private:
  bool check_value_;
};

class ThreadTest : public ::testing::Test {
 public:
  ThreadTest() : delegate_(NULL), thread_(NULL) {}

  TestDelegate* delegate_;
  Thread* thread_;

 protected:
  void SetUp() {
    delegate_ = new TestDelegate();
    thread_ = CreateThread(kThreadName.c_str(), delegate_);
  }

  void TearDown() {
    DeleteThread(thread_);
    delete delegate_;
  }
};

TEST_F(ThreadTest, CreateThread_ExpectThreadCreated) {
  EXPECT_TRUE(thread_ != NULL);
  EXPECT_EQ(thread_, delegate_->thread());
  EXPECT_EQ(thread_->delegate(), delegate_);
}

TEST_F(ThreadTest, CheckCreatedThreadName_ExpectCorrectName) {
  // Check thread was created with correct name
  EXPECT_EQ(kThreadName, thread_->name());
}

TEST_F(
    ThreadTest,
    StartCreatedThreadWithOptionsJoinableAndMyStackSize_ExpectMyStackSizeStackAndJoinableThreadStarted) {
  AutoLock test_lock(test_mutex_);
  // Start thread with following options (Stack size = 32768 & thread is
  // joinable)
  thread_->start(threads::ThreadOptions(kThreadStackSize));
  // Check thread is joinable
  EXPECT_TRUE(thread_->is_joinable());
  // Check thread stack size is 32768
  EXPECT_EQ(kThreadStackSize, thread_->stack_size());
  cond_var_.WaitFor(test_lock, kWaitTime);
  EXPECT_TRUE(delegate_->check_value());
}

TEST_F(
    ThreadTest,
    StartCreatedThreadWithDefaultOptions_ExpectZeroStackAndJoinableThreadStarted) {
  AutoLock test_lock(test_mutex_);
  // Start thread with default options (Stack size = 0 & thread is joinable)
  thread_->start(threads::ThreadOptions());
  // Check thread is joinable
  EXPECT_TRUE(thread_->is_joinable());
  // Check thread stack size is minimum value. Stack can not be 0
  EXPECT_EQ(Thread::kMinStackSize, thread_->stack_size());
  cond_var_.WaitFor(test_lock, kWaitTime);
  EXPECT_TRUE(delegate_->check_value());
}

// TODO (AN): Test should be reworked because we cannot use
// "test_lock_" and "cond_var_" global variables in detached thread
TEST_F(
    ThreadTest,
    DISABLED_StartThreadWithZeroStackAndDetached_ExpectMinimumStackAndDetachedThreadStarted) {
  AutoLock test_lock(test_mutex_);
  // Start thread with default options (Stack size = 0 & thread is detached)
  thread_->start(threads::ThreadOptions(0, false));
  // Check thread is detached
  EXPECT_FALSE(thread_->is_joinable());
  // Check thread stack size is 0
  EXPECT_EQ(Thread::kMinStackSize, thread_->stack_size());
  cond_var_.WaitFor(test_lock, kWaitTime);
  EXPECT_TRUE(delegate_->check_value());
}

TEST_F(ThreadTest, StartThread_ExpectThreadStarted) {
  AutoLock test_lock(test_mutex_);
  // Start created thread
  EXPECT_TRUE(
      thread_->start(threads::ThreadOptions(threads::Thread::kMinStackSize)));
  cond_var_.WaitFor(test_lock, kWaitTime);
  EXPECT_TRUE(delegate_->check_value());
}

TEST_F(ThreadTest, StartOneThreadTwice_ExpectTheSameThreadStartedTwice) {
  // Arrange
  uint64_t thread1_id;
  uint64_t thread2_id;
  AutoLock test_lock(test_mutex_);
  // Start created thread
  EXPECT_TRUE(
      thread_->start(threads::ThreadOptions(threads::Thread::kMinStackSize)));
  thread1_id = thread_->CurrentId();
  thread_->stop();
  // Try to start thread again
  EXPECT_TRUE(
      thread_->start(threads::ThreadOptions(threads::Thread::kMinStackSize)));
  thread2_id = thread_->CurrentId();
  EXPECT_EQ(thread1_id, thread2_id);
  cond_var_.WaitFor(test_lock, kWaitTime);
  EXPECT_TRUE(delegate_->check_value());
}

}  // namespace utils
}  // namespace components
}  // namespace test
