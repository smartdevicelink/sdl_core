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

#include <stdlib.h>
#include <ctime>
#include <memory>
#include "utils/lock.h"
#include "utils/threads/async_runner.h"
#include "utils/conditional_variable.h"



#include "gtest/gtest.h"
#include "gmock/gmock.h"

namespace test {
namespace components {
namespace utils {

using namespace threads;

namespace {
size_t kCheckValue = 0u;
const size_t kDelegatesAmount = 4u;
}

// ThreadDelegate successor
class TestThreadDelegate : public ThreadDelegate {
 public:
  TestThreadDelegate(sync_primitives::ConditionalVariable& cond_var,
                     sync_primitives::Lock& test_lock)
      : cond_var_(cond_var), test_lock_(test_lock) {}

  void threadMain() {
    sync_primitives::AutoLock lock(test_lock_);
    ++kCheckValue;
    cond_var_.NotifyOne();
  }

 protected:
  sync_primitives::ConditionalVariable& cond_var_;
  sync_primitives::Lock& test_lock_;
};

class MockThreadDelegate : public ThreadDelegate {
 public:
  MOCK_METHOD0(threadMain, void());
  MOCK_METHOD0(exitThreadMain, void());
};

class AsyncRunnerTest : public ::testing::Test {
 public:
  AsyncRunnerTest() {
    // Clear global value before test execution
    kCheckValue = 0;
    CreateAsyncRunner();
    CreateThreadsArray();
  }

  ~AsyncRunnerTest() {
    DeleteThreadsArray();
  }

 protected:
  ThreadDelegate** delegates_;
  std::shared_ptr<AsyncRunner> async_runner_;

  void CreateThreadsArray() {
    delegates_ = new ThreadDelegate* [kDelegatesAmount];
  }

  void DeleteThreadsArray() {
    delete[] delegates_;
  }

  void CreateAsyncRunner() {
    async_runner_ = std::make_shared<AsyncRunner>("test");
  }
};

TEST_F(AsyncRunnerTest, ASyncRunManyDelegates_ExpectSuccessfulAllDelegatesRun) {
  // Create Delegates and run
  sync_primitives::ConditionalVariable cond_var;
  sync_primitives::Lock test_lock;
  for (size_t i = 0; i < kDelegatesAmount; ++i) {
    sync_primitives::AutoLock lock(test_lock);
    delegates_[i] = new TestThreadDelegate(cond_var, test_lock);
    async_runner_->AsyncRun(delegates_[i]);
    // Wait for delegate to be run
    cond_var.WaitFor(lock, 1500);
  }
  // Expect all delegates started successfully
  EXPECT_EQ(kDelegatesAmount, kCheckValue);
}

TEST_F(AsyncRunnerTest, StopThenRun_ExpectDelegateNotStarted) {
  // Create Delegate mock
  MockThreadDelegate mock_thread_delegate;
  // Check that delegate was not started due to Stop() called before AsyncRun()
  EXPECT_CALL(mock_thread_delegate, threadMain()).Times(0);
  {
    std::shared_ptr<AsyncRunner> async_runner =
        std::make_shared<AsyncRunner>("test");
    async_runner->Stop();
    async_runner->AsyncRun(&mock_thread_delegate);
  }
}

}  // namespace utils
}  // namespace components
}  // namespace test
