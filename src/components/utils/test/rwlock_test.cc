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
#include "utils/rwlock.h"
#include "utils/threads/thread.h"
#include "utils/threads/thread_delegate.h"

namespace test {
namespace components {
namespace utils_test {

using sync_primitives::RWLock;

class RWlockTest : public ::testing::Test {
 public:
  void ThreadsDispatcher(threads::ThreadDelegate* func) {
    for (uint8_t i = 0; i < kNum_threads_; ++i) {
      thread[i] = threads::CreateThread("test thread", func);
      thread[i]->start();
      ASSERT_TRUE(thread[i]->CurrentId());
    }
    for (uint8_t i = 0; i < kNum_threads_; ++i) {
      thread[i]->join();
      threads::DeleteThread(thread[i]);
    }
  }

  static void ReadLock() {
    test_rwlock.AcquireForReading();
    test_rwlock.ReleaseForReading();
  }

  static void ExpectReadLockFail() {
    bool temp = test_rwlock.TryAcquireForReading();
    EXPECT_FALSE(temp);
    if (temp) {
      test_rwlock.ReleaseForReading();
    }
  }

  static void ExpectWriteLockFail() {
    bool temp = test_rwlock.TryAcquireForWriting();
    EXPECT_FALSE(temp);
    if (temp) {
      test_rwlock.ReleaseForWriting();
    }
  }

 protected:
  static RWLock test_rwlock;
  enum { kNum_threads_ = 4 };
  static threads::Thread* thread[kNum_threads_];
};

RWLock RWlockTest::test_rwlock;
threads::Thread* RWlockTest::thread[kNum_threads_];

class ReadLockDelegate : public threads::ThreadDelegate {
 public:
  void threadMain() {
    RWlockTest::ReadLock();
  }
  void exitThreadMain() {}
};

class ExpectReadLockFailDelegate : public threads::ThreadDelegate {
 public:
  void threadMain() {
    RWlockTest::ExpectReadLockFail();
  }
  void exitThreadMain() {}
};

class ExpectWriteLockFailDelegate : public threads::ThreadDelegate {
 public:
  void threadMain() {
    RWlockTest::ExpectWriteLockFail();
  }
  void exitThreadMain() {}
};

TEST_F(RWlockTest, AcquireForReading_ExpectAccessForReading) {
  // Lock rw lock for reading
  test_rwlock.AcquireForReading();
  // Try to lock rw lock for reading again
  test_rwlock.AcquireForReading();
  // Creating kNumThreads threads, starting them with callback function, waits
  // until all of them finished
  ThreadsDispatcher(new ReadLockDelegate());
  // Releasing RW locks
  test_rwlock.ReleaseForReading();
  test_rwlock.ReleaseForReading();
}

TEST_F(RWlockTest, AcquireForReading_ExpectNoAccessForWriting) {
  // Lock rw lock for reading
  test_rwlock.AcquireForReading();
  // Try to lock rw lock for writing
  EXPECT_FALSE(test_rwlock.TryAcquireForWriting());
  // Creating kNumThreads threads, starting them with callback function, waits
  // until all of them finished
  ThreadsDispatcher(new ExpectWriteLockFailDelegate());
  test_rwlock.ReleaseForReading();
}

TEST_F(RWlockTest, AcquireForWriting_ExpectNoAccessForReading) {
  // Lock rw lock for writing
  test_rwlock.AcquireForWriting();
  // Try to lock rw lock for reading
  EXPECT_FALSE(test_rwlock.TryAcquireForReading());
  // Creating kNumThreads threads, starting them with callback function, waits
  // until all of them finished
  ThreadsDispatcher(new ExpectReadLockFailDelegate());
  test_rwlock.ReleaseForWriting();
}

TEST_F(RWlockTest, AcquireForWriting_ExpectNoMoreAccessForWriting) {
  // Lock rw lock for writing
  test_rwlock.AcquireForWriting();
  // Try to lock rw lock for reading
  EXPECT_FALSE(test_rwlock.TryAcquireForWriting());
  // Creating kNumThreads threads, starting them with callback function, waits
  // until all of them finished
  ThreadsDispatcher(new ExpectWriteLockFailDelegate());
  test_rwlock.ReleaseForWriting();
}

}  // namespace utils_test
}  // namespace components
}  // namespace test
