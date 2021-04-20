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

namespace threads {

using namespace sync_primitives;

namespace {
const uint32_t kMaxSize = 20;
const size_t kStackSize = 32768;
const char* kThreadName = "test thread";
const std::string kRenamedThreadName("THREAD");
};  // namespace

// ThreadDelegate successor
class TestThreadDelegate : public ThreadDelegate {
 public:
  TestThreadDelegate(const bool idle = false)
      : idle_(idle), check_value_(false) {}
  void threadMain() override {
    AutoLock test_lock(test_mutex_);
    check_value_ = true;
    cond_var_.NotifyOne();
    if (idle_) {
      cond_var_.WaitFor(test_lock, 5000);
      cond_var_.NotifyOne();
    }
  }

  void exitThreadMain() override {
    NotifyOne();
  }

  bool IsChangedValue() const {
    return check_value_;
  }

  bool WaitFor(AutoLock& auto_lock, const uint32_t& milliseconds) {
    return cond_var_.WaitFor(auto_lock, milliseconds);
  }

  void NotifyOne() {
    cond_var_.NotifyOne();
  }

  Lock& GetLock() {
    return test_mutex_;
  }

 private:
  bool idle_;
  bool check_value_;
  sync_primitives::Lock test_mutex_;
  sync_primitives::ConditionalVariable cond_var_;
};

class PosixThreadTest : public ::testing::Test {
 public:
  PosixThreadTest() : thread_delegate_(new TestThreadDelegate()) {
    thread_ = CreateThread(kThreadName, thread_delegate_);
  }

  ~PosixThreadTest() {
    thread_->Stop(Thread::kThreadForceStop);
    delete thread_delegate_;
    DeleteThread(thread_);
  };

  void ReplaceThreadDelegate(TestThreadDelegate* thread_delegate) {
    EXPECT_TRUE(nullptr != thread_);
    delete thread_->GetDelegate();
    EXPECT_EQ(nullptr, thread_->GetDelegate());

    thread_->SetDelegate(thread_delegate);
    EXPECT_EQ(thread_delegate, thread_->GetDelegate());
  }

 protected:
  TestThreadDelegate* thread_delegate_;
  Thread* thread_;
};

TEST_F(PosixThreadTest, CreateThread_ExpectThreadCreated) {
  EXPECT_TRUE(NULL != thread_);
  EXPECT_EQ(thread_, thread_delegate_->thread());
  EXPECT_EQ(thread_delegate_, thread_->GetDelegate());

  delete thread_delegate_;
  thread_delegate_ = nullptr;
  // Check Delegate Dtor worked successfully
  EXPECT_EQ(NULL, thread_->GetDelegate());
}

TEST_F(PosixThreadTest, CheckCreatedThreadName_ExpectCorrectName) {
  EXPECT_TRUE(NULL != thread_);
  EXPECT_EQ(thread_, thread_delegate_->thread());
  EXPECT_EQ(thread_delegate_, thread_->GetDelegate());
  // Check thread was created with correct name
  EXPECT_EQ(kThreadName, thread_->GetThreadName());
}

TEST_F(PosixThreadTest,
       CheckCreatedThreadNameChangeToLongName_ExpectThreadNameReduced) {
  {
    auto& lock = thread_delegate_->GetLock();
    AutoLock test_lock(lock);
    thread_->Start(ThreadOptions(Thread::kMinStackSize));
    // Rename started thread. Name will be cut to 15 symbols + '\0'
    // This is the limit in current POSIX thread implementation
    thread_->SetNameForId(thread_->ThreadHandle(),
                          std::string("new thread with changed name"));
    // Name must be large enough to keep 16 symbols. Read previous comment
    char name[kMaxSize];
    int result =
        pthread_getname_np(thread_->ThreadHandle(), name, sizeof(name));
    if (!result) {
      EXPECT_EQ(std::string("new thread with"), std::string(name));
    }

    thread_delegate_->WaitFor(test_lock, 10000);
  }

  EXPECT_TRUE(thread_delegate_->IsChangedValue());
}

TEST_F(
    PosixThreadTest,
    StartCreatedThreadWithOptionsJoinableAndUserStackSize_ExpectUserStackSizeStackAndJoinableThreadStarted) {
  {
    auto& lock = thread_delegate_->GetLock();
    AutoLock test_lock(lock);
    // Start thread with following options (Stack size = 32768 & thread is
    // joinable)
    thread_->Start(ThreadOptions(kStackSize));
    // Check thread is joinable
    EXPECT_TRUE(thread_->IsJoinable());
    // Check thread stack size is 32768
    EXPECT_EQ(kStackSize, thread_->StackSize());
    thread_delegate_->WaitFor(test_lock, 10000);
  }

  EXPECT_TRUE(thread_delegate_->IsChangedValue());
}

TEST_F(
    PosixThreadTest,
    StartCreatedThreadWithDefaultOptions_ExpectZeroStackAndJoinableThreadStarted) {
  {
    auto& lock = thread_delegate_->GetLock();
    AutoLock test_lock(lock);
    // Start thread with default options (Stack size = 0 & thread is joinable)
    thread_->Start(ThreadOptions());
    // Check thread is joinable
    EXPECT_TRUE(thread_->IsJoinable());
    // Check thread stack size is minimum value. Stack can not be 0
    EXPECT_EQ(Thread::kMinStackSize, thread_->StackSize());
    thread_delegate_->WaitFor(test_lock, 10000);
  }

  EXPECT_TRUE(thread_delegate_->IsChangedValue());
}

TEST_F(
    PosixThreadTest,
    StartThreadWithZeroStackAndDetached_ExpectMinimumStackAndDetachedThreadStarted) {
  {
    auto& lock = thread_delegate_->GetLock();
    AutoLock test_lock(lock);
    // Start thread with default options (Stack size = 0 & thread is detached)
    thread_->Start(ThreadOptions(0, false));
    // Check thread is detached
    EXPECT_FALSE(thread_->IsJoinable());
    // Check thread stack size is 0
    EXPECT_EQ(Thread::kMinStackSize, thread_->StackSize());
    thread_delegate_->WaitFor(test_lock, 10000);
  }

  EXPECT_TRUE(thread_delegate_->IsChangedValue());
}

TEST_F(PosixThreadTest,
       CheckCreatedThreadNameChangeToEmpty_ExpectThreadNameChangedToEmpty) {
  {
    auto& lock = thread_delegate_->GetLock();
    AutoLock test_lock(lock);
    thread_->Start(ThreadOptions(Thread::kMinStackSize));
    // Rename started thread. Name will be cut to 15 symbols + '\0'
    // This is the limit in current POSIX thread implementation
    thread_->SetNameForId(thread_->ThreadHandle(), std::string(""));
    // Name must be large enough to keep 16 symbols. Read previous comment
    char name[kMaxSize];
    int result =
        pthread_getname_np(thread_->ThreadHandle(), name, sizeof(name));
    if (!result) {
      EXPECT_TRUE(std::string(name).empty());
    }
    thread_delegate_->WaitFor(test_lock, 10000);
  }

  EXPECT_TRUE(thread_delegate_->IsChangedValue());
}

TEST_F(PosixThreadTest,
       CheckCreatedThreadNameChangeToShortName_ExpectThreadNameChangedToShort) {
  {
    auto& lock = thread_delegate_->GetLock();
    AutoLock test_lock(lock);
    // Start created thread
    thread_->Start(ThreadOptions(Thread::kMinStackSize));
    // Rename started thread. Name will be cut to 15 symbols + '\0'
    // This is the limit in current POSIX thread implementation
    thread_->SetNameForId(thread_->ThreadHandle(), kRenamedThreadName);
    // Name must be large enough to keep 16 symbols. Read previous comment
    char name[kMaxSize];
    int result =
        pthread_getname_np(thread_->ThreadHandle(), name, sizeof(name));
    if (!result) {
      EXPECT_EQ(kRenamedThreadName, std::string(name));
    }
    thread_delegate_->WaitFor(test_lock, 10000);
  }

  EXPECT_TRUE(thread_delegate_->IsChangedValue());
}

TEST_F(PosixThreadTest, StartThread_ExpectThreadStarted) {
  {
    auto& lock = thread_delegate_->GetLock();
    AutoLock test_lock(lock);
    // Start created thread
    EXPECT_TRUE(thread_->Start(ThreadOptions(Thread::kMinStackSize)));
    thread_delegate_->WaitFor(test_lock, 10000);
  }

  EXPECT_TRUE(thread_delegate_->IsChangedValue());
}

TEST_F(PosixThreadTest, StartOneThreadTwice_ExpectTheSameThreadStartedTwice) {
  {
    auto& lock = thread_delegate_->GetLock();
    AutoLock test_lock(lock);
    // Start created thread
    EXPECT_TRUE(thread_->Start(ThreadOptions(Thread::kMinStackSize)));
    auto thread1_id = thread_->CurrentId();
    thread_delegate_->WaitFor(test_lock, 10000);
    thread_->Join(Thread::kThreadJoinDelegate);
    // Try to start thread again
    EXPECT_TRUE(thread_->Start(ThreadOptions(Thread::kMinStackSize)));
    auto thread2_id = thread_->CurrentId();
    EXPECT_EQ(thread1_id, thread2_id);
    thread_delegate_->WaitFor(test_lock, 10000);
  }

  EXPECT_TRUE(thread_delegate_->IsChangedValue());
}

TEST_F(PosixThreadTest,
       StartOneThreadAgainAfterRename_ExpectRenamedThreadStarted) {
  {
    auto& lock = thread_delegate_->GetLock();
    AutoLock test_lock(lock);
    // Start created thread
    EXPECT_TRUE(thread_->Start(ThreadOptions(Thread::kMinStackSize)));
    auto thread1_id = thread_->CurrentId();
    // Rename started thread. Name will be cut to 15 symbols + '\0'
    // This is the limit in current POSIX thread implementation
    thread_->SetNameForId(thread_->ThreadHandle(), kThreadName);
    // Name must be large enough to keep 16 symbols. Read previous comment
    char name[kMaxSize];
    int result =
        pthread_getname_np(thread_->ThreadHandle(), name, sizeof(name));
    if (!result) {
      EXPECT_EQ(kThreadName, std::string(name));
    }
    thread_delegate_->WaitFor(test_lock, 10000);
    thread_->Join(Thread::kThreadJoinDelegate);
    EXPECT_TRUE(thread_->Start(ThreadOptions(Thread::kMinStackSize)));
    auto thread2_id = thread_->CurrentId();
    // Expect the same thread started with the the same name
    EXPECT_EQ(kThreadName, std::string(name));
    EXPECT_EQ(thread1_id, thread2_id);
    thread_delegate_->WaitFor(test_lock, 10000);
  }

  EXPECT_TRUE(thread_delegate_->IsChangedValue());
}

TEST_F(PosixThreadTest, StartThreadWithNullPtrDelegate_ExpectThreadStateNone) {
  thread_->SetDelegate(nullptr);
  EXPECT_TRUE(nullptr != thread_);
  EXPECT_EQ(nullptr, thread_->GetDelegate());

  EXPECT_FALSE(thread_->Start());
  EXPECT_FALSE(thread_->IsRunning());
  EXPECT_EQ(Thread::kThreadStateNone, thread_->thread_state_);
}

TEST_F(PosixThreadTest,
       StartThreadExecutingThreadMain_ExpectThreadStateRunning) {
  const bool cycled_thread_main = true;
  thread_delegate_ = new TestThreadDelegate(cycled_thread_main);
  ReplaceThreadDelegate(thread_delegate_);

  {
    auto& lock = thread_delegate_->GetLock();
    AutoLock auto_lock(lock);
    EXPECT_TRUE(thread_->Start());
    thread_delegate_->WaitFor(auto_lock, 5000);
  }

  EXPECT_TRUE(thread_->IsRunning());
  EXPECT_EQ(Thread::kThreadStateRunning, thread_->thread_state_);
}

TEST_F(PosixThreadTest,
       StartThreadExecutingThreadMainCallStopDelegate_ExpectThreadStateIdle) {
  const bool cycled_thread_main = true;
  thread_delegate_ = new TestThreadDelegate(cycled_thread_main);
  ReplaceThreadDelegate(thread_delegate_);

  {
    auto& lock = thread_delegate_->GetLock();
    AutoLock auto_lock(lock);
    EXPECT_TRUE(thread_->Start());
    thread_delegate_->WaitFor(auto_lock, 5000);
  }

  thread_->Stop(Thread::kThreadStopDelegate);

  EXPECT_FALSE(thread_->IsRunning());
  EXPECT_EQ(Thread::kThreadStateIdle, thread_->thread_state_);
}

TEST_F(PosixThreadTest,
       StartThreadExecutingThreadMainCallForceStop_ExpectThreadStateCompleted) {
  const bool cycled_thread_main = true;
  thread_delegate_ = new TestThreadDelegate(cycled_thread_main);
  ReplaceThreadDelegate(thread_delegate_);

  {
    auto& lock = thread_delegate_->GetLock();
    AutoLock auto_lock(lock);
    EXPECT_TRUE(thread_->Start());
    thread_delegate_->WaitFor(auto_lock, 5000);
  }

  thread_->Stop(Thread::kThreadForceStop);
  thread_->Join(Thread::kThreadJoinThread);

  EXPECT_FALSE(thread_->IsRunning());
  EXPECT_EQ(Thread::kThreadStateCompleted, thread_->thread_state_);
}

}  // namespace threads
