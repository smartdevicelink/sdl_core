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
#include "utils/lock.h"
#include "utils/threads/thread.h"

namespace test {
namespace components {
namespace utils {

using namespace sync_primitives;
using namespace threads;
using threads::Thread;

using namespace sync_primitives;
using namespace threads;
using threads::Thread;

namespace {
const uint32_t kMaxSize = 20;
const size_t kThreadStackSize = 32768;
const std::string kThreadName("THREAD");
}

class ThreadTestDelegate : public threads::ThreadDelegate {
 public:
  ThreadTestDelegate() : thread_stopped_(false), thread_started_(false) {}

  void threadMain() OVERRIDE {
    {
      AutoLock test_lock(test_mutex_);
      thread_started_ = true;
      cond_var_.Broadcast();
    }

    {
      AutoLock test_lock(test_mutex_);
      while (!thread_stopped_) {
        cond_var_.Wait(test_lock);
      }
    }
  }

  void WaitForThreadStart() {
    AutoLock test_lock(test_mutex_);
    while (!thread_started_) {
      cond_var_.Wait(test_lock);
    }
  }

  bool is_thread_started() const {
    return thread_started_;
  }

  void exitThreadMain() OVERRIDE {
    AutoLock test_lock(test_mutex_);
    thread_stopped_ = true;
    cond_var_.Broadcast();
  }

  void ResetDelegateParams() {
    AutoLock test_lock(test_mutex_);
    thread_stopped_ = false;
    thread_started_ = false;
  }

 private:
  volatile bool thread_stopped_;
  volatile bool thread_started_;
  sync_primitives::ConditionalVariable cond_var_;
  sync_primitives::Lock test_mutex_;
};

class ThreadTest : public testing::Test {
 public:
  ThreadTest() {
    delegate_ = new ThreadTestDelegate();
    thread_ = NULL;
  }

  ~ThreadTest() {
    delete delegate_;
    EXPECT_EQ(NULL, thread_->delegate());
    DeleteThread(thread_);
  }

  void SetUp() OVERRIDE {
    ASSERT_NO_THROW(thread_ = CreateThread(kThreadName.c_str(), delegate_));
  }

  void CheckThreadHasCompleted() {
    delegate_->WaitForThreadStart();
    thread_->join(Thread::JoinOptionStop::kForceStop);
    EXPECT_TRUE(delegate_->is_thread_started());
  }

 protected:
  threads::Thread* thread_;
  ThreadTestDelegate* delegate_;
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
  // Start thread with following options (Stack size = 32768 & thread is
  // joinable)
  thread_->start(threads::ThreadOptions(kThreadStackSize));
  // Check thread is joinable
  EXPECT_TRUE(thread_->is_joinable());
  // Check thread stack size is 32768
  EXPECT_EQ(kThreadStackSize, thread_->stack_size());
  CheckThreadHasCompleted();
}

TEST_F(
    ThreadTest,
    StartCreatedThreadWithDefaultOptions_ExpectZeroStackAndJoinableThreadStarted) {
  // Start thread with default options (Stack size = 0 & thread is joinable)
  thread_->start(threads::ThreadOptions());
  // Check thread is joinable
  EXPECT_TRUE(thread_->is_joinable());
  // Check thread stack size is minimum value. Stack can not be 0
  EXPECT_EQ(Thread::kMinStackSize, thread_->stack_size());
  CheckThreadHasCompleted();
}

TEST_F(ThreadTest, StartOneThreadTwice_ExpectTheSameThreadStartedTwice) {
  // Arrange
  uint64_t thread1_id;
  uint64_t thread2_id;
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
  CheckThreadHasCompleted();
}

TEST_F(ThreadTest, StartAndJoinForce_Success) {
  // Default ThreadOptions means it is joinable
  EXPECT_TRUE(thread_->start(threads::ThreadOptions()));

  // Wait for the threadMain to signal that it has started
  delegate_->WaitForThreadStart();
  EXPECT_TRUE(delegate_->is_thread_started());

  thread_->join(Thread::JoinOptionStop::kForceStop);
  EXPECT_FALSE(thread_->is_running());
}

TEST_F(ThreadTest, StartAndJoinNoForce_Success) {
  EXPECT_TRUE(thread_->start(threads::ThreadOptions()));

  // Wait for the threadMain to signal that it has started
  delegate_->WaitForThreadStart();

  // Signal the delegate it has to finish the execution of threadMain
  delegate_->exitThreadMain();
  thread_->join(Thread::JoinOptionStop::kNoStop);

  EXPECT_FALSE(thread_->is_running());
}

TEST_F(ThreadTest, StartStopStart_Success) {
  EXPECT_TRUE(thread_->start(threads::ThreadOptions()));

  // Wait for the thread to notify it has started execution
  delegate_->WaitForThreadStart();
  EXPECT_TRUE(thread_->is_running());

  // Signal the thread to stop and wait for the execution
  thread_->stop();
  thread_->join(Thread::JoinOptionStop::kNoStop);
  EXPECT_FALSE(thread_->is_running());

  // Reset the parameters of the delegate so thread can be started again
  delegate_->ResetDelegateParams();

  EXPECT_TRUE(thread_->start());

  // Wait for the thread to notify it is running
  delegate_->WaitForThreadStart();
  EXPECT_TRUE(thread_->is_running());

  // Make the thread stop and wait for it
  thread_->join(Thread::JoinOptionStop::kForceStop);
  EXPECT_FALSE(thread_->is_running());
}

TEST_F(ThreadTest, StartJoinStart_Success) {
  EXPECT_TRUE(thread_->start(threads::ThreadOptions()));

  // Wait for the thread to notify it has started execution
  delegate_->WaitForThreadStart();
  EXPECT_TRUE(thread_->is_running());
  // Signal the delegate it has to finish the execution of threadMain
  thread_->join(Thread::JoinOptionStop::kForceStop);
  EXPECT_FALSE(thread_->is_running());

  // Reset the parameters of the delegate so thread can be started again
  delegate_->ResetDelegateParams();

  EXPECT_TRUE(thread_->start());

  // Wait for the thread to notify it has started execution
  delegate_->WaitForThreadStart();
  EXPECT_TRUE(thread_->is_running());

  // Make the thread stop and wait for it
  thread_->join(Thread::JoinOptionStop::kForceStop);
  EXPECT_FALSE(thread_->is_running());
}

}  // namespace utils
}  // namespace components
}  // namespace test
