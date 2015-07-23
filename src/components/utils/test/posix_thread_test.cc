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
#include "threads/thread.h"

namespace test {
namespace components {
namespace utils {

using namespace sync_primitives;
using namespace threads;

// TODO(AByzhynar): Change this to use Gtest class to create all variables for every TEST_F
// TODO(AByzhynar): Add multithreading tests

namespace {
const uint32_t MAX_SIZE = 20;
const size_t MyStackSize = 32768;
const char *threadName("test thread");
const std::string test_thread_name("THREAD");
sync_primitives::ConditionalVariable cond_var_;
sync_primitives::Lock test_mutex_;
};

// ThreadDelegate successor
class TestThreadDelegate : public threads::ThreadDelegate {
 public:
  TestThreadDelegate()
      : check_value_(false) {
  }
  void threadMain() {
    AutoLock test_lock(test_mutex_);
    check_value_ = true;
    cond_var_.NotifyOne();
  }

  bool check_value() const {
    return check_value_;
  }
 private:
  bool check_value_;
};

TEST(PosixThreadTest, CreateThread_ExpectThreadCreated) {
  // Arrange
  threads::Thread *thread = NULL;
  TestThreadDelegate *threadDelegate = new TestThreadDelegate();
  // Create thread
  ASSERT_NO_THROW(thread = CreateThread(threadName, threadDelegate));
  EXPECT_TRUE(thread != NULL);
  EXPECT_EQ(thread, threadDelegate->thread());
  EXPECT_EQ(thread->delegate(), threadDelegate);
  DeleteThread(thread);
  delete threadDelegate;
  // Check Delegate Dtor worked successfully
  EXPECT_EQ(NULL, thread->delegate());
}

TEST(PosixThreadTest, CheckCreatedThreadName_ExpectCorrectName) {
  // Arrange
  threads::Thread *thread = NULL;
  threads::ThreadDelegate *threadDelegate = new TestThreadDelegate();
  // Create thread
  ASSERT_NO_THROW(thread = CreateThread(threadName, threadDelegate));
  // Check thread was created with correct name
  EXPECT_EQ(threadName, thread->name());
  DeleteThread(thread);
  delete threadDelegate;
  EXPECT_EQ(NULL, thread->delegate());
}

TEST(PosixThreadTest, CheckCreatedThreadNameChangeToLongName_ExpectThreadNameReduced) {
  // Arrange
  threads::Thread *thread = NULL;
  TestThreadDelegate *threadDelegate = new TestThreadDelegate();
  AutoLock test_lock(test_mutex_);
  // Create thread
  ASSERT_NO_THROW(thread = CreateThread(threadName, threadDelegate));
  thread->start(threads::ThreadOptions(threads::Thread::kMinStackSize));
  // Rename started thread. Name will be cut to 15 symbols + '\0'
  // This is the limit in current POSIX thread implementation
  thread->SetNameForId(thread->thread_handle(),
                       std::string("new thread with changed name"));
  // Name must be large enough to keep 16 symbols. Read previous comment
  char name[MAX_SIZE];
  int result = pthread_getname_np(thread->thread_handle(), name, sizeof(name));
  if (!result)
    EXPECT_EQ(std::string("new thread with"), std::string(name));
  cond_var_.WaitFor(test_lock, 10000);
  EXPECT_TRUE(threadDelegate->check_value());
  DeleteThread(thread);
  delete threadDelegate;
  EXPECT_EQ(NULL, thread->delegate());
}

TEST(PosixThreadTest, StartCreatedThreadWithOptionsJoinableAndMyStackSize_ExpectMyStackSizeStackAndJoinableThreadStarted) {
  // Arrange
  threads::Thread *thread = NULL;
  TestThreadDelegate *threadDelegate = new TestThreadDelegate();
  AutoLock test_lock(test_mutex_);
  // Create thread
  ASSERT_NO_THROW(thread = CreateThread(threadName, threadDelegate));
  // Start thread with following options (Stack size = 32768 & thread is joinable)
  thread->start(threads::ThreadOptions(MyStackSize));
  // Check thread is joinable
  EXPECT_TRUE(thread->is_joinable());
  // Check thread stack size is 32768
  EXPECT_EQ(MyStackSize, thread->stack_size());
  cond_var_.WaitFor(test_lock, 10000);
  EXPECT_TRUE(threadDelegate->check_value());
  DeleteThread(thread);
  delete threadDelegate;
  EXPECT_EQ(NULL, thread->delegate());
}

TEST(PosixThreadTest, StartCreatedThreadWithDefaultOptions_ExpectZeroStackAndJoinableThreadStarted) {
  // Arrange
  threads::Thread *thread = NULL;
  TestThreadDelegate *threadDelegate = new TestThreadDelegate();
  AutoLock test_lock(test_mutex_);
  // Create thread
  ASSERT_NO_THROW(thread = CreateThread(threadName, threadDelegate));
  // Start thread with default options (Stack size = 0 & thread is joinable)
  thread->start(threads::ThreadOptions());
  // Check thread is joinable
  EXPECT_TRUE(thread->is_joinable());
  // Check thread stack size is minimum value. Stack can not be 0
  EXPECT_EQ(Thread::kMinStackSize, thread->stack_size());
  cond_var_.WaitFor(test_lock, 10000);
  EXPECT_TRUE(threadDelegate->check_value());
  DeleteThread(thread);
  delete threadDelegate;
  EXPECT_EQ(NULL, thread->delegate());
}

TEST(PosixThreadTest, StartThreadWithZeroStackAndDetached_ExpectMinimumStackAndDetachedThreadStarted) {
  // Arrange
  threads::Thread *thread = NULL;
  TestThreadDelegate *threadDelegate = new TestThreadDelegate();
  AutoLock test_lock(test_mutex_);
  // Create thread
  ASSERT_NO_THROW(thread = CreateThread(threadName, threadDelegate));
  // Start thread with default options (Stack size = 0 & thread is detached)
  thread->start(threads::ThreadOptions(0, false));
  // Check thread is detached
  EXPECT_FALSE(thread->is_joinable());
  // Check thread stack size is 0
  EXPECT_EQ(Thread::kMinStackSize, thread->stack_size());
  cond_var_.WaitFor(test_lock, 10000);
  EXPECT_TRUE(threadDelegate->check_value());
  DeleteThread(thread);
  delete threadDelegate;
  EXPECT_EQ(NULL, thread->delegate());
}

TEST(PosixThreadTest, DISABLED_CheckCreatedThreadNameChangeToEmpty_ExpectThreadNameChangedToEmpty) {
  // Arrange
  threads::Thread *thread = NULL;
  TestThreadDelegate *threadDelegate = new TestThreadDelegate();
  AutoLock test_lock(test_mutex_);
  // Create thread
  ASSERT_NO_THROW(thread = CreateThread(threadName, threadDelegate));
  thread->start(threads::ThreadOptions(threads::Thread::kMinStackSize));
  // Rename started thread. Name will be cut to 15 symbols + '\0'
  // This is the limit in current POSIX thread implementation
  thread->SetNameForId(thread->thread_handle(), std::string(""));
  // Name must be large enough to keep 16 symbols. Read previous comment
  char name[MAX_SIZE];
  int result = pthread_getname_np(thread->thread_handle(), name, sizeof(name));
  if (!result) {
    EXPECT_EQ(std::string(""), std::string(name));
  }
  cond_var_.WaitFor(test_lock, 10000);
  EXPECT_TRUE(threadDelegate->check_value());
  DeleteThread(thread);
  delete threadDelegate;
  EXPECT_EQ(NULL, thread->delegate());
}

TEST(PosixThreadTest, CheckCreatedThreadNameChangeToShortName_ExpectThreadNameChangedToShort) {
  // Arrange
  threads::Thread *thread = NULL;
  TestThreadDelegate *threadDelegate = new TestThreadDelegate();
  AutoLock test_lock(test_mutex_);
  // Create thread
  ASSERT_NO_THROW(thread = CreateThread(threadName, threadDelegate));
  // Start created thread
  thread->start(threads::ThreadOptions(threads::Thread::kMinStackSize));
  // Rename started thread. Name will be cut to 15 symbols + '\0'
  // This is the limit in current POSIX thread implementation
  thread->SetNameForId(thread->thread_handle(), test_thread_name);
  // Name must be large enough to keep 16 symbols. Read previous comment
  char name[MAX_SIZE];
  int result = pthread_getname_np(thread->thread_handle(), name, sizeof(name));
  if (!result) {
    EXPECT_EQ(test_thread_name, std::string(name));
  }
  cond_var_.WaitFor(test_lock, 10000);
  EXPECT_TRUE(threadDelegate->check_value());
  DeleteThread(thread);
  delete threadDelegate;
  EXPECT_EQ(NULL, thread->delegate());
}

TEST(PosixThreadTest, StartThread_ExpectThreadStarted) {
  // Arrange
  threads::Thread *thread = NULL;
  TestThreadDelegate *threadDelegate = new TestThreadDelegate();
  AutoLock test_lock(test_mutex_);
  // Create thread
  ASSERT_NO_THROW(thread = CreateThread(threadName, threadDelegate));
  // Start created thread
  EXPECT_TRUE(thread->start(threads::ThreadOptions(threads::Thread::kMinStackSize)));
  cond_var_.WaitFor(test_lock, 10000);
  EXPECT_TRUE(threadDelegate->check_value());
  DeleteThread(thread);
  delete threadDelegate;
  EXPECT_EQ(NULL, thread->delegate());
}

TEST(PosixThreadTest, StartOneThreadTwice_ExpectTheSameThreadStartedTwice) {
  // Arrange
  PlatformThreadHandle thread1_id;
  PlatformThreadHandle thread2_id;
  threads::Thread *thread = NULL;
  TestThreadDelegate *threadDelegate = new TestThreadDelegate();
  AutoLock test_lock(test_mutex_);
  // Create thread
  ASSERT_NO_THROW(thread = CreateThread(threadName, threadDelegate));
  // Start created thread
  EXPECT_TRUE(thread->start(threads::ThreadOptions(threads::Thread::kMinStackSize)));
  thread1_id = thread->CurrentId();
  thread->stop();
  // Try to start thread again
  EXPECT_TRUE(thread->start(threads::ThreadOptions(threads::Thread::kMinStackSize)));
  thread2_id = thread->CurrentId();
  EXPECT_EQ(thread1_id, thread2_id);
  cond_var_.WaitFor(test_lock, 10000);
  EXPECT_TRUE(threadDelegate->check_value());
  DeleteThread(thread);
  delete threadDelegate;
  EXPECT_EQ(NULL, thread->delegate());
}

TEST(PosixThreadTest, StartOneThreadAgainAfterRename_ExpectRenamedThreadStarted) {
  // Arrange
  PlatformThreadHandle thread1_id;
  PlatformThreadHandle thread2_id;
  threads::Thread *thread = NULL;
  TestThreadDelegate *threadDelegate = new TestThreadDelegate();
  AutoLock test_lock(test_mutex_);
  // Create thread
  ASSERT_NO_THROW(thread = CreateThread(threadName, threadDelegate));
  // Start created thread
  EXPECT_TRUE(thread->start(threads::ThreadOptions(threads::Thread::kMinStackSize)));
  thread1_id = thread->CurrentId();
  // Rename started thread. Name will be cut to 15 symbols + '\0'
  // This is the limit in current POSIX thread implementation
  thread->SetNameForId(thread->thread_handle(), test_thread_name);
  // Name must be large enough to keep 16 symbols. Read previous comment
  char name[MAX_SIZE];
  int result = pthread_getname_np(thread->thread_handle(), name, sizeof(name));
  if (!result)
    EXPECT_EQ(test_thread_name, std::string(name));
  // Stop thread
  thread->stop();
  EXPECT_TRUE(thread->start(threads::ThreadOptions(threads::Thread::kMinStackSize)));
  thread2_id = thread->CurrentId();
  // Expect the same thread started with the the same name
  EXPECT_EQ(test_thread_name, std::string(name));
  EXPECT_EQ(thread1_id, thread2_id);
  cond_var_.WaitFor(test_lock, 10000);
  EXPECT_TRUE(threadDelegate->check_value());
  DeleteThread(thread);
  delete threadDelegate;
  EXPECT_EQ(NULL, thread->delegate());
}

}  // namespace utils
}  // namespace components
}  // namespace test
