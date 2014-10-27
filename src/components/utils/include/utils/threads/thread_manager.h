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

#ifndef SRC_COMPONENTS_UTILS_INCLUDE_UTILS_THREADS_THREAD_MANAGER_H_
#define SRC_COMPONENTS_UTILS_INCLUDE_UTILS_THREADS_THREAD_MANAGER_H_

#include "utils/threads/thread.h"

#include <pthread.h>

#include <map>
#include <string>
#include <set>
#include <list>

#include "utils/macro.h"
#include "utils/singleton.h"
#include "utils/lock.h"
#include "utils/conditional_variable.h"
#include "utils/message_queue.h"
#include "utils/threads/thread.h"
#include "utils/threads/thread_delegate.h"

namespace threads {
  class Thread;

/*
 * This class is here currently to remember names associated to threads.
 * It manages raw impl::PlatformHandles because Thread::Id's do not provide
 * comparison operator. Current linux implementation relies on fact that
 * pthread_t is just an integer and every thread has single unique value
 * associated with it.
 * OS provides it's own facilities to name threads but
 */
class ThreadManager : public utils::Singleton<ThreadManager> {
 public:
  struct ThreadDesc {
    pthread_t handle;
    ThreadDelegate* delegate;
  };
  ThreadManager() {}
  MessageQueue<ThreadDesc> threads_to_terminate;
 private:
  DISALLOW_COPY_AND_ASSIGN(ThreadManager);

  FRIEND_BASE_SINGLETON_CLASS(ThreadManager);
};

} // namespace threads

#endif // SRC_COMPONENTS_UTILS_INCLUDE_UTILS_THREADS_THREAD_MANAGER_H_
