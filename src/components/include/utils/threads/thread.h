/*
 * Copyright (c) 2014, Ford Motor Company
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

namespace threads {

namespace impl {
#if defined(OS_POSIX)
typedef pthread_t PlatformThreadHandle;
#else
#error Please implement thread for your OS
#endif
}

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
class Thread;
Thread* CreateThread(const char* name, ThreadDelegate* delegate);
void DeleteThread(Thread*);

class Thread {
  friend Thread* CreateThread(const char*, ThreadDelegate*);
  friend void DeleteThread(Thread*);
 public:
  /**
   * Class that represents unique in-process thread identifier
   * due to restriction of pthread API it only allows checks
   * for equality to different thread id and no ordering.
   *
   * ostream<< operator is provided for this class which
   * outputs thread name associated to an identifier.
   */
  class Id {
   public:
    explicit Id(const impl::PlatformThreadHandle& id): id_(id) {}
    bool operator==(const Id& that) const;
    impl::PlatformThreadHandle Handle() const { return id_; }
   private:
    impl::PlatformThreadHandle id_;
    friend class Thread;
  };

  // Get unique ID of currently executing thread
  static Id CurrentId();

  // Get name associated with thread identified by thread_id
  static std::string NameFromId(const Id& thread_id);

  // Give thread thread_id a name, helpful for debugging
  static void SetNameForId(const Id& thread_id, const std::string& name);

  /**
   * Starts the thread.
   * @return true if the thread was successfully started.
   */
  bool start();

  ThreadDelegate* delegate() const;

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
   * Get thread name.
   * @return thread name
   */
  const std::string& thread_name() {
    return name_;
  }

  /**
   * Returns true if the thread has been started, and not yet stopped.
   * When a thread is running, the thread_id_ is non-zero.
   * @return true if the thread has been started, and not yet stopped.
   */
  bool is_running() const {
    return isThreadRunning_;
  }

  void set_running(bool running) {
    isThreadRunning_ = running;
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
  impl::PlatformThreadHandle thread_handle() const {
    return thread_handle_;
  }

  /**
   * Thread id.
   * @return return thread id.
   */
  Id thread_id() const {
    return Id(thread_handle());
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
  const std::string name_;
  ThreadDelegate* delegate_;
  impl::PlatformThreadHandle thread_handle_;
  ThreadOptions thread_options_;
  volatile unsigned int isThreadRunning_;

 private:
  /**
   * Ctor.
   * @param name - display string to identify the thread.
   * @param delegate - thread procedure delegate. Look for
   * 'threads/thread_delegate.h' for details.
   * NOTE: delegate will be deleted by destructor.
   *       This constructor made private to prevent
   *       Thread object to be created on stack
   */
  Thread(const char* name, ThreadDelegate* delegate);

  DISALLOW_COPY_AND_ASSIGN(Thread);
  virtual ~Thread() { }
};

inline bool operator!= (const Thread::Id& left, const Thread::Id& right) {
  return !(left == right);
}
std::ostream& operator<<(std::ostream& os, const Thread::Id& thread_id);
}  // namespace threads
#endif  // SRC_COMPONENTS_INCLUDE_UTILS_THREADS_THREAD_H_
