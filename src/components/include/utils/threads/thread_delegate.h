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

#ifndef SRC_COMPONENTS_INCLUDE_UTILS_THREADS_THREAD_DELEGATE_H_
#define SRC_COMPONENTS_INCLUDE_UTILS_THREADS_THREAD_DELEGATE_H_

#include <pthread.h>

#include "utils/lock.h"

namespace threads {

enum ThreadState {
  kInit = 0,
  kStarted = 1,
  kStopReq = 2
};

class Thread;

/**
 * Thread procedure interface.
 * Look for "threads/thread.h" for example
 */
class ThreadDelegate {
 public:
  ThreadDelegate()
      : state_(kInit),
        thread_(NULL) {
  }
  /**
   * \brief Thread procedure.
   */
  virtual void threadMain() = 0;

  /**
   * Should be called to free all resources allocated in threadMain
   * and exiting threadMain
   * This function should be blocking and return only when threadMain() will be
   * finished in other case segmantation failes are possible
   */
  virtual void exitThreadMain();

  virtual ~ThreadDelegate();

  Thread* thread() const {
    return thread_;
  }

  void set_thread(Thread *thread);

  bool ImproveState(unsigned int to) {
    state_lock_.Lock();
    if ((state_ + 1 == to) || (to == kInit && state_ == kStopReq)) {
      state_ = to;
    }
    state_lock_.Unlock();
    return state_ == to;
  }

  unsigned int state() const {
    return state_;
  }

 private:
  volatile unsigned int state_;
  sync_primitives::SpinMutex state_lock_;
  Thread* thread_;
};

}  // namespace threads
#endif  // SRC_COMPONENTS_INCLUDE_UTILS_THREADS_THREAD_DELEGATE_H_
