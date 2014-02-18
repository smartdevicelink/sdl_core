/**
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
#include "utils/lock.h"

#include <map>
#include <string>
#include <set>

#include "utils/macro.h"
#include "utils/singleton.h"

namespace threads {
namespace impl {

/*
 * Generates short and unique names for unnamed threads
 * and remembers association between thread handle and that short name
 */
class UnnamedThreadRegistry {
 public:
  UnnamedThreadRegistry();
  ~UnnamedThreadRegistry();
  /*
   * Returns a name for given unnamed thread id.
   * If id is first seen, new name is generated and memorized
   * If id is already known, previously generated name is returned
   */
  std::string GetUniqueName(PlatformThreadHandle id);
 private:
  typedef std::map<PlatformThreadHandle, std::string> IdNameMap;
  IdNameMap id_number_;
  int32_t last_thread_number_;
  sync_primitives::Lock state_lock_;
};

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
  // Name a thread. Should be called only once for every thread.
  // Threads can't be renamed
  void RegisterName(PlatformThreadHandle id, const std::string& name);

  // Get a name for previously registered thread
  std::string GetName(PlatformThreadHandle id) const;

  // Forget a name of (possibly destroyed) thread
  // Make sure to call it after thread is finished
  // Because thread id's can be recycled
  void Unregister(PlatformThreadHandle id);
 private:
  ThreadManager();
  ~ThreadManager();

 private:
  typedef std::set<std::string> NamesSet;
  typedef std::map<PlatformThreadHandle, std::string> IdNamesMap;
  // Set of thread names for fast checking if name is unique
  NamesSet names_;
  // Map from system handle to the thread name
  IdNamesMap id_names_;
  mutable sync_primitives::Lock state_lock_;

  // Generator of shorter sequental names for unnamed threads
  // Has to memorize every generated name this is why it is mutable
  mutable UnnamedThreadRegistry unnamed_thread_namer_;

 private:
  DISALLOW_COPY_AND_ASSIGN(ThreadManager);

  FRIEND_BASE_SINGLETON_CLASS_INSTANCE(ThreadManager);
};

} // namespace impl
} // namespace threads

#endif // SRC_COMPONENTS_UTILS_INCLUDE_UTILS_THREADS_THREAD_MANAGER_H_
