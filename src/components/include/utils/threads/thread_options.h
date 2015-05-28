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

#ifndef SRC_COMPONENTS_INCLUDE_UTILS_THREADS_THREAD_OPTIONS_H_
#define SRC_COMPONENTS_INCLUDE_UTILS_THREADS_THREAD_OPTIONS_H_

#include <cstdio>

namespace threads {

/**
 * @brief Startup options for thread.
 * Look for "threads/thread.h" for example
 */
class ThreadOptions {
 public:
  /**
   * Ctor
   * @param stack_size - thread stack size. If stack size less than
   * threads::Thread::kMinStackSize the default value is used.
   * 0 - default stack size for current platform.
   * @param is_joinable - is thread joinable?
   */
  explicit ThreadOptions(size_t stack_size = 0, bool is_joinable = true)
      : stack_size_(stack_size),
        is_joinable_(is_joinable) {
  }

  /**
   * Dtor.
   */
  virtual ~ThreadOptions() {
  }

  /**
   * Copy ctor.
   * @param options - new options.
   */
  ThreadOptions(const ThreadOptions& options) {
    *this = options;
  }

  /**
   * Assign operator.
   * @param options - new options.
   * @return new options.
   */
  ThreadOptions& operator=(const ThreadOptions& options) {
    stack_size_ = options.stack_size();
    is_joinable_ = options.is_joinable();
    return *this;
  }

  /**
   * Stack size.
   * @return Stack size for thread.
   */
  size_t stack_size() const {
    return stack_size_;
  }

  /**
   * Is thread joinable?
   * @return - Returns true if the thread is joinable.
   */
  bool is_joinable() const {
    return is_joinable_;
  }

  void is_joinable(bool val) {
    is_joinable_ = val;
  }

 protected:
  size_t stack_size_;
  bool is_joinable_;
};

}  // namespace threads
#endif  // SRC_COMPONENTS_INCLUDE_UTILS_THREADS_THREAD_OPTIONS_H_
