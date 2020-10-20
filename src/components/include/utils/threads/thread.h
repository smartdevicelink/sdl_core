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

#include "utils/conditional_variable.h"
#include "utils/lock.h"
#include "utils/macro.h"
#include "utils/threads/thread_delegate.h"
#include "utils/threads/thread_options.h"

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

Thread* CreateThread(const char* name, ThreadDelegate* delegate);
void DeleteThread(Thread* thread);

class Thread {
 public:
  /**
   * @brief ThreadCommand is used to command the thread
   * kThreadCommandNone - no command, used to indicate that there is no pending
   * command
   * kThreadCommandRun - commands thread to run (do another iteration)
   * kThreadCommandFinalize - informs thread that must exit
   */
  enum ThreadCommand {
    kThreadCommandNone,  // must be first
    kThreadCommandRun,
    kThreadCommandFinalize  // keep last
    // in case of new commands - update/check threadFunc()
  };

  /**
   * @brief ThreadState informs outside world about its state
   * kThreadStateError - if pthread_create returned an error
   * kThreadStateNone - there is no thread at all
   * kThreadStateIdle - the thread is in state idle
   * kThreadStateRunning - thread is in state running (executing delegates
   * threadMain())
   * kThreadStateCompleted - thread completed
   */
  enum ThreadState {
    kThreadStateError = -1,
    kThreadStateNone,
    kThreadStateIdle,
    kThreadStateRunning,
    kThreadStateCompleted
  };

  /**
   * @brief ThreadStopOption
   * kThreadStopDelegate - executing delegates exitThreadMain and
   * move thread to kThreadStateIdle
   * kThreadSoftStop - executing kThreadStopDelegate and
   * move thread to kThreadStateCompleted
   * kThreadForceStop - executing kThreadSoftStop,
   * if necessary pthread_cancel or pthread_exit and
   * move thread to kThreadStateCompleted
   */
  enum ThreadStopOption {
    kThreadStopDelegate,
    kThreadSoftStop,
    kThreadForceStop
  };

  /**
   * @brief ThreadJoinOption
   * kThreadJoinDelegate - waiting for finish threadMain
   * kThreadJoinThread - waiting for finish threadFunc
   */
  enum ThreadJoinOption { kThreadJoinDelegate, kThreadJoinThread };

  friend Thread* CreateThread(const char* name, ThreadDelegate* delegate);
  friend void DeleteThread(Thread* thread);

  /**
   * @brief Starts the thread.
   * @return true if the thread was successfully started.
   */
  bool Start();

  /**
   * @brief Starts the thread. Behaves exactly like \ref start() in addition to
   * allow to override the default options.
   * @param options Thread options. Look for 'threads/thread_options.h'
   * for details.
   * @return true if the thread was successfully started.
   */
  bool Start(const ThreadOptions& options);

  /**
   * @brief Signals the thread to exit and returns once the thread has exited.
   * After this method returns, the Thread object is completely reset and may
   * be used as if it were newly constructed (i.e., Start may be called again).
   *
   * Stop may be called multiple times and is simply ignored if the thread is
   * already stopped.
   *
   * Stop will wait for delegate exit
   */
  bool Stop(const ThreadStopOption stop_option);

  /**
   * @brief Blocks the current thread until
   * the fucntion identified by join_option finishes execution.
   * If that fucntion has already terminated, then
   * Join returns immediately.
   * @param join_option - specify function to wait
   */
  void Join(const ThreadJoinOption join_option);

  ThreadDelegate* GetDelegate() const {
    return delegate_;
  }

  void SetDelegate(ThreadDelegate* delegate) {
    Stop(kThreadStopDelegate);
    delegate_ = delegate;
  }

  /**
   * @brief Causes the calling thread to relinquish the CPU.  The
   * thread is moved to the end of the queue for its static priority and a
   * new thread gets to run.
   */
  static void SchedYield();

  // Get unique ID of currently executing thread
  static PlatformThreadHandle CurrentId();

  // Give thread thread_id a name, helpful for debugging
  static void SetNameForId(const PlatformThreadHandle& thread_id,
                           std::string name);

  /**
   * @brief Get thread name.
   * @return thread name
   */
  const std::string& GetThreadName() {
    return name_;
  }

  /**
   * @brief Returns true if the thread has been started, and not yet stopped.
   * When a thread is running, the thread_id_ is non-zero.
   * @return true if the thread has been started, and not yet stopped.
   */
  bool IsRunning() {
    sync_primitives::AutoLock auto_lock(state_lock_);
    return kThreadStateRunning == thread_state_;
  }

  /**
   * @brief Is thread joinable?
   * @return - Returns true if the thread is joinable.
   */
  bool IsJoinable() const {
    return thread_options_.is_joinable();
  }

  /**
   * @brief Thread stack size
   * @return thread stack size
   */
  size_t StackSize() const {
    return thread_options_.stack_size();
  }

  /**
   * @brief The native thread handle.
   * @return thread handle.
   */
  PlatformThreadHandle ThreadHandle() const {
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
  const ThreadOptions& GetThreadOptions() const {
    return thread_options_;
  }

  /**
   * @brief Minimum size of thread stack for specific platform.
   */
  static size_t kMinStackSize;

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

  /**
   * @brief Initializes the thread attributes and
   * set thread options into attributes
   * @param thread_options - thread options
   * @return pthread_attr_t - initialized the thread attributes
   */
  pthread_attr_t SetThreadCreationAttributes(ThreadOptions* thread_options);

  /**
   * @brief Executing delegates exitThreadMain and move thread to
   * kThreadStateIdle. That funciton is not thread safe.
   * @param auto_lock - Locked object is used to wait
   * thread iteration completion
   * @return true if delegate has been successfully stopped,
   * false otherwise
   */
  bool StopDelegate(sync_primitives::AutoLock& auto_lock);

  /**
   * @brief Executing StopDelegate and run kThreadCommandFinalize command,
   * move thread to kThreadStateCompleted,
   * that funciton is not thread safe
   * @param auto_lock - Locked object used for waiting
   * of the last iteration in thread
   * @return true if thread has been successfully stopped,
   * false otherwise
   */
  bool StopSoft(sync_primitives::AutoLock& auto_lock);

  /**
   * @brief Executing StopSoft, if necessary pthread_cancel or pthread_exit
   * and move thread to kThreadStateCompleted,
   * that funciton is not thread safe
   * @param auto_lock - Locked object used for waiting
   * of the last iteration in thread
   */
  void StopForce(sync_primitives::AutoLock& auto_lock);

  /**
   * @brief Waiting finished iteration in thread,
   * that funciton is not thread safe
   * @param auto_lock - Locked object using for waiting
   * finishing iteration in thread
   */
  void JoinDelegate(sync_primitives::AutoLock& auto_lock);

  const std::string name_;
  ThreadDelegate* delegate_;
  PlatformThreadHandle handle_;
  ThreadOptions thread_options_;
  // Should be locked to protect thread state
  sync_primitives::Lock state_lock_;
  sync_primitives::ConditionalVariable state_cond_;

  /**
   * @brief Used to request actions from worker thread.
   */
  volatile ThreadCommand thread_command_;

  /**
   * @brief Used from worker thread to inform about its status.
   */
  volatile ThreadState thread_state_;

#ifdef BUILD_TESTS
  FRIEND_TEST(PosixThreadTest,
              StartThreadWithNullPtrDelegate_ExpectThreadStateNone);
  FRIEND_TEST(PosixThreadTest,
              StartThreadExecutingThreadMain_ExpectThreadStateRunning);
  FRIEND_TEST(
      PosixThreadTest,
      StartThreadExecutingThreadMainCallStopDelegate_ExpectThreadStateIdle);
  FRIEND_TEST(
      PosixThreadTest,
      StartThreadExecutingThreadMainCallForceStop_ExpectThreadStateCompleted);
#endif
};

}  // namespace threads
#endif  // SRC_COMPONENTS_INCLUDE_UTILS_THREADS_THREAD_H_
