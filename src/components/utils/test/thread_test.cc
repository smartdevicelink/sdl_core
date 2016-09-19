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

// TODO(AByzhynar): Add multithreading tests

namespace {

const size_t kThreadStackSize = 32768u;
const std::string kThreadName("Test thread");
const uint32_t kWaitTime = 1000u;

sync_primitives::ConditionalVariable cond_var_thread_start;
sync_primitives::ConditionalVariable cond_var_thread_exit;
sync_primitives::Lock test_mutex_thread_start;
sync_primitives::Lock test_mutex_thread_exit;

}  // namespace

class TestDelegate : public threads::ThreadDelegate {
 public:
  TestDelegate() : check_value_(false) {}
  void threadMain() OVERRIDE {
    AutoLock test_lock(test_mutex_thread_start);
    check_value_ = true;
    cond_var_thread_start.NotifyOne();
  }

  void exitThreadMain() OVERRIDE {}

  bool check_value() const {
    return check_value_;
  }

 private:
  bool check_value_;
};

template <class Delegate>
void WaitForThreadStart(Delegate delegate) {
  {
    AutoLock test_lock(test_mutex_thread_start);
    while (!delegate->notify_running_) {
      cond_var_thread_start.Wait(test_lock);
    }
  }
}

class InfiniteMainTestDelegate : public threads::ThreadDelegate {
 public:
  InfiniteMainTestDelegate()
      : notified_(false), notify_running_(false), check_value_(false) {}

  void threadMain() OVERRIDE {
    {
      AutoLock test_lock(test_mutex_thread_start);
      check_value_ = true;
      notify_running_ = true;
      cond_var_thread_start.Broadcast();
    }

    {
      AutoLock test_lock(test_mutex_thread_exit);
      while (!notified_) {
        cond_var_thread_exit.Wait(test_lock);
      }
    }
  }

  bool check_value() const {
    return check_value_;
  }

  void exitThreadMain() OVERRIDE {
    notified_ = true;
    {
      AutoLock test_lock(test_mutex_thread_exit);
      cond_var_thread_exit.Broadcast();
    }
  }

  void SignalWakeUp() {
    notified_ = true;
    {
      AutoLock test_lock(test_mutex_thread_exit);
      cond_var_thread_exit.Broadcast();
    }
  }

  volatile bool notified_;
  volatile bool notify_running_;

 private:
  bool check_value_;
};

class DelayedExitTestDelegate : public threads::ThreadDelegate {
 public:
  DelayedExitTestDelegate()
      : notified_main_(false), notified_exit_(false), notify_running_(false) {}

  void threadMain() OVERRIDE {
    {
      AutoLock test_lock(test_mutex_thread_start);
      notify_running_ = true;
      cond_var_thread_start.Broadcast();
    }

    {
      AutoLock test_lock(test_mutex_thread_exit);
      while (!notified_main_) {
        cond_var_thread_exit.Wait(test_lock);
      }
    }

    {
      AutoLock test_lock(test_mutex_thread_start);
      notify_running_ = true;
      cond_var_thread_start.Broadcast();
    }
    notified_exit_ = true;
    {
      AutoLock test_lock(test_mutex_thread_exit);
      cond_var_thread_exit.Broadcast();
    }
  }

  void exitThreadMain() OVERRIDE {
    notified_main_ = true;
    {
      AutoLock test_lock(test_mutex_thread_exit);
      cond_var_thread_exit.Broadcast();
    }

    {
      AutoLock test_lock(test_mutex_thread_exit);
      while (!notified_exit_) {
        cond_var_thread_exit.Wait(test_lock);
      }
    }
  }

  void SignalWakeUp() {
    notified_exit_ = true;
    {
      AutoLock test_lock(test_mutex_thread_exit);
      cond_var_thread_exit.Broadcast();
    }
  }

  volatile bool notified_main_;
  volatile bool notified_exit_;
  volatile bool notify_running_;
};

class ThreadOptionsTest : public ::testing::Test {
 public:
  ThreadOptionsTest() : delegate_(NULL), thread_(NULL) {}

  TestDelegate* delegate_;
  Thread* thread_;

 protected:
  void SetUp() {
    delegate_ = new TestDelegate();
    thread_ = CreateThread(kThreadName.c_str(), delegate_);
  }

  void TearDown() {
    delete delegate_;
    DeleteThread(thread_);
  }
};

TEST_F(ThreadOptionsTest, CreateThread_ExpectThreadCreated) {
  EXPECT_TRUE(thread_ != NULL);
  EXPECT_EQ(thread_, delegate_->thread());
  EXPECT_EQ(thread_->delegate(), delegate_);
}

TEST_F(ThreadOptionsTest, CheckCreatedThreadName_ExpectCorrectName) {
  // Check thread was created with correct name
  EXPECT_EQ(kThreadName, thread_->name());
}

TEST_F(
    ThreadOptionsTest,
    StartCreatedThreadWithOptionsJoinableAndMyStackSize_ExpectMyStackSizeStackAndJoinableThreadStarted) {
  AutoLock test_lock(test_mutex_thread_start);
  // Start thread with following options (Stack size = 32768 & thread is
  // joinable)
  thread_->start(threads::ThreadOptions(kThreadStackSize));
  // Check thread is joinable
  EXPECT_TRUE(thread_->is_joinable());
  // Check thread stack size is 32768
  EXPECT_EQ(kThreadStackSize, thread_->stack_size());
  cond_var_thread_start.WaitFor(test_lock, kWaitTime);
  EXPECT_TRUE(delegate_->check_value());
}

TEST_F(
    ThreadOptionsTest,
    StartCreatedThreadWithDefaultOptions_ExpectZeroStackAndJoinableThreadStarted) {
  AutoLock test_lock(test_mutex_thread_start);
  // Start thread with default options (Stack size = 0 & thread is joinable)
  thread_->start(threads::ThreadOptions());
  // Check thread is joinable
  EXPECT_TRUE(thread_->is_joinable());
  // Check thread stack size is minimum value. Stack can not be 0
  EXPECT_EQ(Thread::kMinStackSize, thread_->stack_size());
  cond_var_thread_start.WaitFor(test_lock, kWaitTime);
  EXPECT_TRUE(delegate_->check_value());
}

// TODO (AN): Test should be reworked because we cannot use
// "test_lock_" and "cond_var_" global variables in detached thread
TEST_F(
    ThreadOptionsTest,
    StartThreadWithZeroStackAndDetached_ExpectMinimumStackAndDetachedThreadStarted) {
  AutoLock test_lock(test_mutex_thread_start);
  // Start thread with default options (Stack size = 0 & thread is detached)
  thread_->start(threads::ThreadOptions(0, false));
  // Check thread is detached
  EXPECT_FALSE(thread_->is_joinable());
  // Check thread stack size is 0
  EXPECT_EQ(Thread::kMinStackSize, thread_->stack_size());
  cond_var_thread_start.WaitFor(test_lock, kWaitTime);
  EXPECT_TRUE(delegate_->check_value());
}

TEST_F(ThreadOptionsTest, StartThread_ExpectThreadStarted) {
  AutoLock test_lock(test_mutex_thread_start);
  // Start created thread
  EXPECT_TRUE(
      thread_->start(threads::ThreadOptions(threads::Thread::kMinStackSize)));
  cond_var_thread_start.WaitFor(test_lock, kWaitTime);
  EXPECT_TRUE(delegate_->check_value());
}

TEST_F(ThreadOptionsTest, StartOneThreadTwice_ExpectTheSameThreadStartedTwice) {
  // Arrange
  uint64_t thread1_id;
  uint64_t thread2_id;
  AutoLock test_lock(test_mutex_thread_start);
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
  cond_var_thread_start.WaitFor(test_lock, kWaitTime);
  EXPECT_TRUE(delegate_->check_value());
}

TEST(ThreadExecutionFlowTest, StartAndJoinForce_Success) {
  AutoLock test_lock(test_mutex_thread_start);

  // Create a delegate with infinite threadMain
  InfiniteMainTestDelegate* delegate = new InfiniteMainTestDelegate();
  Thread* thread = CreateThread(kThreadName.c_str(), delegate);

  // Default ThreadOptions means it is joinable
  EXPECT_TRUE(thread->start(threads::ThreadOptions()));
  EXPECT_TRUE(thread->is_joinable());

  // Wait for the threadMain to signal that it has started
  cond_var_thread_start.Wait(test_lock);
  EXPECT_TRUE(delegate->check_value());

  thread->join(Thread::JoinOptionStop::kForceStop);
  EXPECT_FALSE(thread->is_running());

  delete delegate;
  DeleteThread(thread);
}

TEST(ThreadExecutionFlowTest, StartAndJoinNoForce_Success) {
  AutoLock test_lock(test_mutex_thread_start);

  // Create a delegate with infinite threadMain
  InfiniteMainTestDelegate* delegate = new InfiniteMainTestDelegate();
  Thread* thread = CreateThread(kThreadName.c_str(), delegate);
  EXPECT_TRUE(thread->start(threads::ThreadOptions()));
  EXPECT_TRUE(thread->is_joinable());

  // Wait for the threadMain to signal that it has started
  cond_var_thread_start.Wait(test_lock);

  // Signal the delegate it has to finish the execution of threadMain
  delegate->SignalWakeUp();
  thread->join(Thread::JoinOptionStop::kNoStop);

  EXPECT_FALSE(thread->is_running());
  EXPECT_TRUE(delegate->check_value());

  delete delegate;
  DeleteThread(thread);
}

TEST(ThreadExecutionFlowTest, StartStopStart_Success) {
  // Create a delegate with infinite threadMain
  InfiniteMainTestDelegate* delegate = new InfiniteMainTestDelegate();
  Thread* thread = CreateThread(kThreadName.c_str(), delegate);
  EXPECT_TRUE(thread->start(threads::ThreadOptions()));
  EXPECT_TRUE(thread->is_joinable());

  // Wait for the thread to notify it has started execution
  WaitForThreadStart(delegate);
  EXPECT_TRUE(thread->is_running());

  // Signal the thread to stop and wait for the execution
  thread->stop();
  thread->join(Thread::JoinOptionStop::kNoStop);
  EXPECT_FALSE(thread->is_running());

  // Reset the parameters of the delegate so thread can be started again
  delegate->notified_ = false;
  delegate->notify_running_ = false;
  EXPECT_TRUE(thread->start());

  // Wait for the thread to notify it is running
  WaitForThreadStart(delegate);
  EXPECT_TRUE(thread->is_running());

  // Make the thread stop and wait for it
  delegate->SignalWakeUp();
  thread->join(Thread::JoinOptionStop::kNoStop);
  EXPECT_FALSE(thread->is_running());

  delete delegate;
  DeleteThread(thread);
}

TEST(ThreadExecutionFlowTest, StartJoinStart_Success) {
  // Create a delegate with infinite threadMain
  InfiniteMainTestDelegate* delegate = new InfiniteMainTestDelegate();
  Thread* thread = CreateThread(kThreadName.c_str(), delegate);
  EXPECT_TRUE(thread->start(threads::ThreadOptions()));
  EXPECT_TRUE(thread->is_joinable());

  // Wait for the thread to notify it has started execution
  WaitForThreadStart(delegate);
  EXPECT_TRUE(thread->is_running());
  // Signal the delegate it has to finish the execution of threadMain
  delegate->notified_ = true;
  {
    AutoLock test_lock(test_mutex_thread_exit);
    cond_var_thread_exit.Broadcast();
  }
  thread->join(Thread::JoinOptionStop::kNoStop);
  EXPECT_FALSE(thread->is_running());

  // Reset the parameters of the delegate so thread can be started again
  delegate->notified_ = false;
  delegate->notify_running_ = false;

  EXPECT_TRUE(thread->start());

  // Wait for the thread to notify it has started execution
  WaitForThreadStart(delegate);
  EXPECT_TRUE(thread->is_running());

  // Signal the delegate it has to finish the execution of threadMain
  delegate->SignalWakeUp();
  thread->join(Thread::JoinOptionStop::kNoStop);
  EXPECT_FALSE(thread->is_running());

  delete delegate;
  DeleteThread(thread);
}

TEST(ThreadExecutionFlowTest, DeleteJoinable_Success) {
  // Create a delegate with delayed exit of threadMain
  DelayedExitTestDelegate* delegate = new DelayedExitTestDelegate();
  Thread* thread = CreateThread(kThreadName.c_str(), delegate);
  EXPECT_TRUE(thread->start(threads::ThreadOptions()));
  EXPECT_TRUE(thread->is_joinable());

  // Wait for the thread to notify it has started execution
  WaitForThreadStart(delegate);

  // Reset the parameters of the delegate
  delegate->notify_running_ = false;
  EXPECT_TRUE(thread->is_running());
  DeleteThread(thread);

  // Wait for threadMain to notify it is exiting
  WaitForThreadStart(delegate);
  EXPECT_TRUE(delegate->notified_exit_);

  // Creating a new dummy thread so the delegate can be deleted
  Thread* dummy_thread = CreateThread(kThreadName.c_str(), delegate);
  delegate->set_thread(dummy_thread);
  delete delegate;
  DeleteThread(dummy_thread);
}

}  // namespace utils
}  // namespace components
}  // namespace test
