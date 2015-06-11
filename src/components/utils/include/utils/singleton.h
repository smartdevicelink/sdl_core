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

#ifndef SRC_COMPONENTS_UTILS_INCLUDE_UTILS_SINGLETON_H_
#define SRC_COMPONENTS_UTILS_INCLUDE_UTILS_SINGLETON_H_

#include "utils/lock.h"
#include "utils/memory_barrier.h"
#include "utils/atomic.h"

namespace utils {

namespace deleters {

class DummyDeleter {
 public:
  void grab(void* pointer) {
  }
};

template<typename T>
class Deleter {
 public:
  Deleter() : pointer_(0) {
  }
  ~Deleter() {
    if (pointer_) {
      delete pointer_;
    }
  }
  void grab(T* pointer) {
    pointer_ = pointer;
  }
 private:
  T* pointer_;
};

}  // namespace deleters

template<typename T, class Deleter = deleters::DummyDeleter>
class Singleton {
/**
 * @brief Singleton template
 * Singleton classes must derive from this template specialized with class itself:
 *
 * class MySingleton : public Singleton<MySingleton> {...};
 *
 * All such classes must declare instance() method as friend
 * by adding FRIEND_BASE_SINGLETON_CLASS macro from macro.h to class definition:
 *
 * FRIEND_BASE_SINGLETON_CLASS(MySingleton);
 *
 * Instance of this class (if created) can be deleted by Deleter destructor
 * which is called after main() (or from exit())
 * This requires T destructor to be accessible for Deleter (e.g. public)
 * Deleter template parameter can be specified with any class
 * with public default constructor, destructor and method
 * void grab(T*);
 * However, default Deleter specification does nothing
 *
 * Also instance can be deleted explicitly by calling destroy() method
 *
 * Both instance() and destroy() methods are thread safe
 * but not thread safety between simultaneous calls
 * of instance() and destroy() is cared about
 */
 public:
/**
 * @brief Returns the singleton of class
 */
  static T* instance();
/**
 * @brief Destroys the singleton (if it had been created)
 */
  static void destroy();
/**
 * @brief Checks whether the singleton exists
 */
  static bool exists();

 private:

  static T** instance_pointer();
  static Deleter* deleter();
};

template<typename T, class Deleter>
T* Singleton<T, Deleter>::instance() {
  static sync_primitives::Lock lock;

  T* local_instance;
  atomic_pointer_assign(local_instance, *instance_pointer());
  memory_barrier();

  if (!local_instance) {
    lock.Acquire();
    local_instance = *instance_pointer();
    if (!local_instance) {
      local_instance = new T();
      memory_barrier();
      atomic_pointer_assign(*instance_pointer(), local_instance);
      deleter()->grab(local_instance);
    }
    lock.Release();
  }

  return local_instance;
}

template<typename T, class Deleter>
void Singleton<T, Deleter>::destroy() {
  static sync_primitives::Lock lock;

  T* local_instance;
  atomic_pointer_assign(local_instance, *instance_pointer());
  memory_barrier();

  if (local_instance) {
    lock.Acquire();
    local_instance = *instance_pointer();
    if (local_instance) {
      atomic_pointer_assign(*instance_pointer(), 0);
      memory_barrier();
      delete local_instance;
      deleter()->grab(0);
    }
    lock.Release();
  }
}

template<typename T, class Deleter>
bool Singleton<T, Deleter>::exists() {
  return *instance_pointer() != 0;
}

template<typename T, class Deleter>
T** Singleton<T, Deleter>::instance_pointer() {
  static T* instance = 0;
  return &instance;
}

template<typename T, class Deleter>
Deleter* Singleton<T, Deleter>::deleter() {
  static Deleter deleter;
  return &deleter;
}

}  // namespace utils

#endif  // SRC_COMPONENTS_UTILS_INCLUDE_UTILS_SINGLETON_H_
