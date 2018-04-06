/*
 * Copyright (c) 2017, Ford Motor Company
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

#ifndef SRC_COMPONENTS_INCLUDE_TEST_UTILS_TEST_ASYNC_WAITER_H_
#define SRC_COMPONENTS_INCLUDE_TEST_UTILS_TEST_ASYNC_WAITER_H_

#include <stdint.h>

#include "utils/lock.h"
#include "utils/conditional_variable.h"

namespace test {

/**
 * @brief The TestAsyncWaiter class
 * Tool with condition variable style interface
 * to be used in unit tests for asynchronous calls waiting.
 * Could wait specified number of calls, waiting for timeout is also supported
 *
 * Usage example:
 * TEST() {
 *   TestAsyncWaiter waiter;
 *   EXPECT_CALL(mock, InterestingCall())
 *       .Times(n)
 *       .WillRepeatedly(NotifyTestAsyncWaiter(&waiter));
 *   EXPECT_TRUE(waiter.WaitFor(n, 1000));
 * }
 */
class TestAsyncWaiter {
 public:
  TestAsyncWaiter() : notified_(false), count_(0), lock_(), cond_var_() {}

  /**
   * @brief WaitFor
   * Waits for specified number of notifications but not longer
   * than given timeout for each notification
   * @param wait_count Number of notifications to wait for
   * @param milliseconds Timeout in milliseconds
   * @return True if specified number of notifications have been received,
   * false otherwise (return by timeout)
   */
  bool WaitFor(const uint32_t wait_count, const uint32_t milliseconds) {
    sync_primitives::AutoLock auto_lock(lock_);
    while (count_ < wait_count) {
      notified_ = false;
      if (sync_primitives::ConditionalVariable::kTimeout ==
          cond_var_.WaitFor(auto_lock, milliseconds)) {
        return false;
      }
    }
    return true;
  }

  /**
   * @brief Notify
   * Notifies async waiter
   */
  void Notify() {
    sync_primitives::AutoLock auto_lock(lock_);
    notified_ = true;
    ++count_;
    cond_var_.Broadcast();
  }

 private:
  bool notified_;
  uint32_t count_;
  sync_primitives::Lock lock_;
  sync_primitives::ConditionalVariable cond_var_;
};

ACTION_P(NotifyTestAsyncWaiter, test_async_waiter) {
  test_async_waiter->Notify();
}

}  // namespace test
#endif  // SRC_COMPONENTS_INCLUDE_TEST_UTILS_TEST_ASYNC_WAITER_H_
