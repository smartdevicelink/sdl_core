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

namespace test {
namespace components {
namespace utils {

using sync_primitives::Lock;

TEST(LockPosixTest, DefaultCtorTest_ExpectNonRecursiveMutexCreated) {
  // Create Lock object
  Lock test_mutex;
  // Lock mutex
  test_mutex.Acquire();
  // Check if created mutex is non-recursive
  EXPECT_FALSE(test_mutex.Try());
  // Release mutex before destroy
  test_mutex.Release();
}

TEST(LockPosixTest, CtorTestWithFalseArgument_ExpectNonRecursiveMutexCreated) {
  // Create Lock object
  Lock test_mutex(false);
  // Lock mutex
  test_mutex.Acquire();
  // Check if created mutex is non-recursive
  EXPECT_FALSE(test_mutex.Try());
  // Release mutex before destroy
  test_mutex.Release();
}

TEST(LockPosixTest, CtorTestWithTrueArgument_ExpectRecursiveMutexCreated) {
  // Create Lock object
  Lock test_mutex(true);
  // Lock mutex
  test_mutex.Acquire();
  // Check if created mutex is recursive
  EXPECT_TRUE(test_mutex.Try());
  // Release mutex before destroy
  test_mutex.Release();
  test_mutex.Release();
}

TEST(LockPosixTest, AcquireMutex_ExpectMutexLocked) {
  // Create Lock object (non-recursive mutex)
  Lock test_mutex;
  // Lock mutex
  test_mutex.Acquire();
  // Try to lock it again. If locked expect false
  EXPECT_FALSE(test_mutex.Try());
  test_mutex.Release();
}

TEST(LockPosixTest, ReleaseMutex_ExpectMutexReleased) {
  // Create Lock object (non-recursive mutex)
  Lock test_mutex;
  // Lock mutex
  test_mutex.Acquire();
  // Release mutex
  test_mutex.Release();
  // Try to lock it again. If released expect true
  EXPECT_TRUE(test_mutex.Try());
  test_mutex.Release();
}

TEST(LockPosixTest, TryLockNonRecursiveMutex_ExpectMutexNotLockedTwice) {
  // Create Lock object (non-recursive mutex)
  Lock test_mutex;
  // Lock mutex
  test_mutex.Try();
  // Try to lock it again. If locked expect false
  EXPECT_FALSE(test_mutex.Try());
  test_mutex.Release();
}

TEST(LockPosixTest, TryLockRecursiveMutex_ExpectMutexLockedTwice) {
  // Create Lock object (recursive mutex)
  Lock test_mutex(true);
  // Lock mutex
  test_mutex.Try();
  // Try to lock it again. Expect true and internal counter increase
  EXPECT_TRUE(test_mutex.Try());
  // Release mutex twice as was locked twice.
  // Every Release() will decrement internal counter
  test_mutex.Release();
  test_mutex.Release();
}

}  // namespace utils
}  // namespace components
}  // namespace test
