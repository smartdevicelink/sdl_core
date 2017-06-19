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

#ifndef SRC_COMPONENTS_INCLUDE_UTILS_THREADS_THREAD_H_
#define SRC_COMPONENTS_INCLUDE_UTILS_THREADS_THREAD_H_

#if defined(OS_POSIX)
#include <pthread.h>
#endif

#include <ostream>
#include <string>

#include "utils/macro.h"
#include "utils/threads/thread_delegate.h"
#include "utils/threads/thread_options.h"
#include "utils/conditional_variable.h"
#include "utils/lock.h"

namespace threads {

#if defined(OS_POSIX)
typedef pthread_t PlatformThreadHandle;
#else
#error Please implement thread for your OS
#endif

/**
 * @brief Non platform specific thread abstraction that establishes a
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

class Thread;
void enqueue_to_join(Thread* thread);

Thread* CreateThread(const char* name, ThreadDelegate* delegate);
void DeleteThread(Thread* thread);

class Thread {
 private:
  const std::string name_;
  // Should be locked to protect delegate_ value
  sync_primitives::Lock delegate_lock_;
  ThreadDelegate* delegate_;
  PlatformThreadHandle handle_;
  ThreadOptions thread_options_;
  // Should be locked to protect isThreadRunning_ and thread_created_ values
  sync_primitives::Lock state_lock_;
  volatile unsigned int isThreadRunning_;
  volatile bool stopped_;
  volatile bool finalized_;
  bool thread_created_;
  // Signalled when Thread::start() is called
  sync_primitives::ConditionalVariable run_cond_;

 public:
  static int count;
  /**
   * @brief Starts the thread.
   * @return true if the thread was successfully started.
   */
  bool start();

  /**
   * @brief Starts the thread. Behaves exactly like \ref start() in addition to
   * allow to override the default options.
   * @param options Thread options. Look for 'threads/thread_options.h'
   * for details.
   * @return true if the thread was successfully started.
   */
  bool start(const ThreadOptions& options);

  sync_primitives::Lock& delegate_lock() {
    return delegate_lock_;
  }

  ThreadDelegate* delegate() const {
    return delegate_;
  }

  void set_delegate(ThreadDelegate* delegate) {
    delegate_ = delegate;
  }

  friend Thread* CreateThread(const char* name, ThreadDelegate* delegate);
  friend void DeleteThread(Thread* thread);

 public:
  // Yield current thread
  static void yield();

  // Get unique ID of currently executing thread
  static PlatformThreadHandle CurrentId();

  // Give thread thread_id a name, helpful for debugging
  static void SetNameForId(const PlatformThreadHandle& thread_id,
                           std::string name);

  /**
   * @brief Signals the thread to exit and returns once the thread has exited.
   * After this method returns, the Thread object is completely reset and may
   * be used as if it were newly constructed (i.e., Start may be called again).
   *
   * Stop may be called multiple times and is simply ignored if the thread is
   * already stopped.
   */
  void stop();

  void join();

  /**
   * @brief Get thread name.
   * @return thread name
   */
  const std::string& name() {
    return name_;
  }

  /**
   * @brief Returns true if the thread has been started, and not yet stopped.
   * When a thread is running, the thread_id_ is non-zero.
   * @return true if the thread has been started, and not yet stopped.
   */
  bool is_running() const {
    return isThreadRunning_;
  }

  void set_running(bool running);

  /**
   * @brief Is thread joinable?
   * @return - Returns true if the thread is joinable.
   */
  bool is_joinable() const {
    return thread_options_.is_joinable();
  }

  /**
   * @brief Thread stack size
   * @return thread stack size
   */
  size_t stack_size() const {
    return thread_options_.stack_size();
  }

  /**
   * @brief The native thread handle.
   * @return thread handle.
   */
  PlatformThreadHandle thread_handle() const {
    return handle_;
  }

  /**
   * @brief Checks if invoked in this Thread context
   * @return True if called from this Thread class, false otherwise
   */
  bool IsCurrentThread() const;

  /**
   * @brief Thread options.
   * @return thread options.
   */
  const ThreadOptions& thread_options() const {
    return thread_options_;
  }

  /**
   * @brief Minimum size of thread stack for specific platform.
   */
  static size_t kMinStackSize;

 protected:
  sync_primitives::ConditionalVariable state_cond_;

 private:
  /**
   * Ctor.
   * @param name - display string to identify the thread.
   * @param delegate - thread procedure delegate. Look for
   * 'threads/thread_delegate.h' for details.
   * LifeCycle thread , otherwise it will be joined in stop method
   * NOTE: delegate will be deleted after thread will be joined
   *       This constructor made private to prevent
   *       Thread object to be created on stack
   */
  Thread(const char* name, ThreadDelegate* delegate);
  virtual ~Thread();
  static void* threadFunc(void* arg);
  static void cleanup(void* arg);
  DISALLOW_COPY_AND_ASSIGN(Thread);
};

}  // namespace threads
#endif  // SRC_COMPONENTS_INCLUDE_UTILS_THREADS_THREAD_H_
