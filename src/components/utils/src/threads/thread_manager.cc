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

#include "utils/threads/thread_manager.h"
#include "utils/threads/thread_delegate.h"
#include "utils/lock.h"
#include "utils/conditional_variable.h"
#include "utils/threads/thread.h"
#include "utils/logger.h"

#include <pthread.h>

#include <sstream>
#include <list>

#if defined(OS_LINUX)
#include <sys/syscall.h>
#include <unistd.h>
#endif

namespace threads {
namespace impl {
using namespace std;
using namespace sync_primitives;

CREATE_LOGGERPTR_GLOBAL(logger_, "Utils")

namespace {
#ifdef OS_WIN32
void findThreadById(const std::vector<PlatformThreadStruct> &mapNames, 
	PlatformThreadHandle id, 
	std::vector<PlatformThreadStruct>::const_iterator &findIter)
{
	for (findIter = mapNames.begin(); findIter != mapNames.end(); findIter++){
		if (pthread_equal(findIter->id, id)){
			return;
		}
	}
	findIter = mapNames.end();
}
#endif
  const char* kUnknownName = "UnnamedThread";
}

UnnamedThreadRegistry::UnnamedThreadRegistry() {
}

UnnamedThreadRegistry::~UnnamedThreadRegistry() {
}

std::string UnnamedThreadRegistry::GetUniqueName(PlatformThreadHandle id) {
  AutoLock auto_lock(state_lock_);
#ifdef OS_WIN32
  IdNameMap::iterator found;
  findThreadById(id_number_, id, found);
#else
  IdNameMap::iterator found = id_number_.find(id);
#endif
  if (found != id_number_.end()) {
#ifdef OS_WIN32
	  return found->name;
#else
    return found->second;
#endif
  } else {
    ++last_thread_number_;
    std::stringstream namestream;
    namestream<<kUnknownName<<last_thread_number_;
#ifdef OS_WIN32
	found->name = namestream.str();
	return found->name;
#else
    return id_number_[id] = namestream.str();
#endif
  }
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
#ifdef OS_WIN32
	IdNamesMap::const_iterator findIter;
	findThreadById(id_names_, id, findIter);
	if (findIter == id_names_.end()){
		PlatformThreadStruct pt;
		pt.id = id;
		pt.name = name;
		id_names_.push_back(pt);
	}else{
		LOG4CXX_ERROR(logger_, "Trying to register thread name " << name
			<< ", but it is already registered with name "
			<< findIter->name);
	}
#else
	pair<IdNamesMap::iterator, bool> inserted =
		id_names_.insert(make_pair(id, name));
    if (!inserted.second) {
      LOG4CXX_ERROR(logger_, "Trying to register thread name " << name
                           <<", but it is already registered with name "
                           <<inserted.first->second);
    }
#endif
  } else {
    LOG4CXX_ERROR(logger_, "Ignoring duplicate thread name: " + name);
  }
}

string ThreadManager::GetName(PlatformThreadHandle id) const {
  AutoLock auto_lock(state_lock_);
#ifdef OS_WIN32
  IdNamesMap::const_iterator found;
  findThreadById(id_names_, id, found);
#else
  IdNamesMap::const_iterator found = id_names_.find(id);
#endif
  if (found != id_names_.end()) {
#ifdef OS_WIN32
	  return found->name;
#else
    return found->second;
#endif
  } else {
    LOG4CXX_WARN(logger_, "Thread doesn't have associated name");
    return unnamed_thread_namer_.GetUniqueName(id);
  }
}

void ThreadManager::Unregister(PlatformThreadHandle id) {
  AutoLock auto_lock(state_lock_);
#ifdef OS_WIN32
  IdNamesMap::iterator findIter;
  findThreadById(id_names_, id, findIter);
	if (id_names_.end() != findIter){
		string name = findIter->name;
#else
	string name = id_names_[id];
#endif
	names_.erase(name);
#ifdef OS_WIN32
		id_names_.erase(findIter);
	}
#else
  id_names_.erase(id);
#endif
}

} // namespace impl
} // namespace threads
