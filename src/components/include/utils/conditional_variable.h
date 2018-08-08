/*
 * Copyright (c) 2013, Ford Motor Company
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
#ifndef SRC_COMPONENTS_INCLUDE_UTILS_CONDITIONAL_VARIABLE_H_
#define SRC_COMPONENTS_INCLUDE_UTILS_CONDITIONAL_VARIABLE_H_

#include <stdint.h>
#include <boost/thread/condition_variable.hpp>

#include "utils/lock.h"
#include "utils/macro.h"

namespace sync_primitives {

/*
 * Conditional variable wrapper
 * Conditional variable is a thing that can be waited on
 * Wait is finished when other thread puts that thing in a signaled state
 * (or when timeout is over).
 * Data that is conditionally accessed should be protected by
 * a Lock and that lock must be taken before starting to Wait.
 * When wait is performed, Lock is temporarly released.
 * When wait is finished, Lock is captured back.
 * WARNING: Beware of Spurious wakeups
 * http://en.wikipedia.org/wiki/Spurious_wakeup
 * Thread can wake up from wait spuriously, without conditional
 * variable being actually set by other thread. This means
 * additional check should be made right after thread awakening
 * and if check fails thread should continue waiting.
 *
 * while(!DataReady()) cond_var.Wait(auto_lock);
 *
 */
class ConditionalVariable {
 public:
  enum WaitStatus { kNoTimeout, kTimeout };
  ConditionalVariable();
  ~ConditionalVariable();
  // Wakes up single thread that is waiting on this conditional variable
  void NotifyOne();
  // Wakes up all waiting threads
  void Broadcast();

  // Wait forever or up to milliseconds time limit
  bool Wait(AutoLock& auto_lock);
  bool Wait(BaseLock& lock);
  WaitStatus WaitFor(AutoLock& auto_lock, uint32_t milliseconds);

 private:
  boost::condition_variable_any cond_var_;

 private:
  DISALLOW_COPY_AND_ASSIGN(ConditionalVariable);
};

}  // namespace sync_primitives
#endif  // SRC_COMPONENTS_INCLUDE_UTILS_CONDITIONAL_VARIABLE_H_
