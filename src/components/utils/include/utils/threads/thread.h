/**
 * \file thread.h
 * \brief
 *
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

#ifndef SRC_COMPONENTS_UTILS_INCLUDE_UTILS_THREADS_THREAD_H_
#define SRC_COMPONENTS_UTILS_INCLUDE_UTILS_THREADS_THREAD_H_

#if defined(OS_WIN)
#include <windows.h>
typedef DWORD PlatformThreadId;
typedef void* PlatformThreadHandle;  // HANDLE
#elif defined(OS_POSIX)
#include <pthread.h>
typedef pthread_t PlatformThreadHandle;
#if defined(OS_LINUX) || defined(OS_OPENBSD)
#include <unistd.h>
typedef pid_t PlatformThreadId;
#elif defined(OS_BSD)
#include <sys/types.h>
typedef lwpid_t PlatformThreadId;
#elif defined(OS_MACOSX)
#include <mach/mach.h>
typedef mach_port_t PlatformThreadId;
#endif
#endif

#include <string>

#include "utils/macro.h"
#include "utils/logger.h"
#include "utils/threads/thread_delegate.h"
#include "utils/threads/thread_options.h"

namespace threads {

/**
 * Non platform specific thread abstraction that establishes a
 * threads::ThreadDelegate on a new thread.
 *
 * ThreadDelegate example:
 * class TestThread : public threads::ThreadDelegate {
 * public:
 *   void threadMain() {
 *     printf("Hello, thread!\n");
 *     sleep(2);
 *   }
 * };
 *
 * Example usage:
 * threads::Thread thread("test thread", new TestThread());
 * thread.startWithOptions(
 *     threads::ThreadOptions(threads::Thread::kMinStackSize));
 * printf("join!\n");
 * thread.join();
 * printf("ok!\n");
 */
class Thread {
 public:
  /**
   * Ctor.
   * @param name - display string to identify the thread.
   * @param delegate - thread procedure delegate. Look for
   * 'threads/thread_delegate.h' for details.
   * NOTE: delegate will be deleted by destructor.
   */
  Thread(const char* name, ThreadDelegate* delegate);

  /**
   * Dtor.
   */
  virtual ~Thread();

  /**
   * Starts the thread.
   * @return true if the thread was successfully started.
   */
  bool start();

  /**
   * Starts the thread. Behaves exactly like Start in addition to
   * allow to override the default options.
   * @param options - thread options. Look for 'threads/thread_options.h'
   * for details.
   * @return true if the thread was successfully started.
   */
  bool startWithOptions(const ThreadOptions& options);

  /**
   * Signals the thread to exit and returns once the thread has exited.
   * After this method returns, the Thread object is completely reset and may
   * be used as if it were newly constructed (i.e., Start may be called again).
   *
   * Stop may be called multiple times and is simply ignored if the thread is
   * already stopped.
   */
  void stop();

  /**
   * Joins with a thread created via the Create function.
   * This function blocks the caller until the designated thread exits.
   * This will invalidate |thread_handle|.
   */
  void join();

  /**
   * Get thread name.
   * @return thread name
   */
  const std::string &thread_name() {
    return name_;
  }

  /**
   * Returns true if the thread has been started, and not yet stopped.
   * When a thread is running, the thread_id_ is non-zero.
   * @return true if the thread has been started, and not yet stopped.
   */
  bool is_running() const {
    return thread_id_ != 0;
  }

  /**
   * Is thread joinable?
   * @return - Returns true if the thread is joinable.
   */
  bool is_joinable() const {
    return thread_options_.is_joinable();
  }

  /**
   * Thread stack size
   * @return thread stack size
   */
  size_t stack_size() const {
    return thread_options_.stack_size();
  }

  /**
   * The native thread handle.
   * @return thread handle.
   */
  PlatformThreadHandle thread_handle() const {
    return thread_handle_;
  }

  /**
   * Thread id.
   * @return return thread id.
   */
  PlatformThreadId thread_id() const {
    return thread_id_;
  }

  /**
   * Thread options.
   * @return thread options.
   */
  const ThreadOptions& thread_options() const {
    return thread_options_;
  }

  /**
   * Minimum size of thread stack for specific platform.
   */
  static size_t kMinStackSize;

 protected:
  std::string name_;
  ThreadDelegate* delegate_;
  PlatformThreadHandle thread_handle_;
  PlatformThreadId thread_id_;
  ThreadOptions thread_options_;

  static log4cxx::LoggerPtr logger_;

 private:
  DISALLOW_COPY_AND_ASSIGN(Thread);
};
}  // namespace threads

#endif  // SRC_COMPONENTS_UTILS_INCLUDE_UTILS_THREADS_THREAD_H_
