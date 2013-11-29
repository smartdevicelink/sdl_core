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

#include "utils/threads/thread_manager.h"

#include <sstream>

#if defined(OS_LINUX)
#include <sys/syscall.h>
#include <unistd.h>
#endif

namespace threads {
namespace impl {
using namespace std;
using namespace sync_primitives;

namespace {

const char* kUnknownName = "UnknownThread";

log4cxx::LoggerPtr g_logger =
    log4cxx::LoggerPtr(log4cxx::Logger::getLogger("Utils"));

#if defined(OS_LINUX)
bool IsMainThread() {
  return syscall(__NR_gettid) == getpid();
}

void SetSystemThreadName(PlatformThreadHandle thread, const std::string& thread_name) {
  // Avoid renaming main thread because it renames whole process on linux
  if (IsMainThread())
     return;

  // System thread names has a restriction of 16 chars plus zero terminator on linux
  string sort_name = thread_name.length() < 16 ? thread_name
                                               : thread_name.substr(0, 15);
  int rv = pthread_setname_np(thread, sort_name.c_str());
  if (rv != 0) {
    LOG4CXX_ERROR(g_logger, "Failed to set system thread name for " << sort_name);
  }
}
#else
void SetSystemThreadName(PlatformThreadHandle  thread, const std::string& thread_name) {
}
#endif

} // namespace

//static
ThreadManager* ThreadManager::instance_ = 0;

// static
ThreadManager& ThreadManager::instance() {
  if (!instance_) {
    instance_ = new ThreadManager;
  }
  return *instance_;
}

// static
void ThreadManager::Shutdown() {
  delete instance_;
  instance_ = NULL;
}

ThreadManager::ThreadManager() {
  names_.insert(kUnknownName);
}

ThreadManager::~ThreadManager() {
}

void ThreadManager::RegisterName(PlatformThreadHandle id, const string& name) {
  AutoLock auto_lock(state_lock_);
  if (names_.count(name) == 0) {
    names_.insert(name);
    pair<IdNamesMap::iterator, bool> inserted =
        id_names_.insert(make_pair(id, name));
    if (inserted.second) {
      SetSystemThreadName(id, name);
    } else {
      LOG4CXX_ERROR(g_logger, "Trying to register thread name " << name
                           <<", but it is already registered with name "
                           <<inserted.first->second);
    }
  } else {
    LOG4CXX_ERROR(g_logger, "Ignoring duplicate thread name: " + name);
  }
}

string ThreadManager::GetName(PlatformThreadHandle id) const {
  AutoLock auto_lock(state_lock_);
  IdNamesMap::const_iterator found = id_names_.find(id);
  if (found != id_names_.end()) {
    return found->second;
  } else {
    LOG4CXX_WARN(g_logger, "Thread doesn't have associated name");
    stringstream ss;
    ss<<kUnknownName<<id;
    return ss.str();
  }
}

void ThreadManager::Unregister(PlatformThreadHandle id) {
  AutoLock auto_lock(state_lock_);
  string name = id_names_[id];
  names_.erase(name);
  id_names_.erase(id);
}


} // namespace impl
} // namespace threads
