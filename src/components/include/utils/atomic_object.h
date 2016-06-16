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

#ifndef SRC_COMPONENTS_INCLUDE_UTILS_ATOMIC_OBJECT_H_
#define SRC_COMPONENTS_INCLUDE_UTILS_ATOMIC_OBJECT_H_

#include "utils/rwlock.h"
#include "utils/conditional_variable.h"
#include "utils/macro.h"

namespace sync_primitives {

/**
 * @brief Allows to safely change stored value from different threads.
 *
 * The usage example:
 *
 * threads::Atomic<int> i;
 *
 * i = 5; // here SDL is able to guarantee that this value will be safely
 *        // assigned even in multi threaded environment.
 */
template <typename T>
class Atomic {
 public:
  /**
   * @brief Atomic allows to construct atomic object.
   * The operation is not atomic.
   *
   * @param value the value to initialize object with.
   */
  Atomic(const T& value) : value_(value) {}

  /**
   * @brief operator = thread safe setter for stored value.
   *
   * @param val value to assign.
   *
   * @return mofified value.
   */
  T& operator=(const T& val) {
    sync_primitives::AutoWriteLock lock(rw_lock_);
    value_ = val;
    return value_;
  }

  /**
   * @brief operator T thread safe getter
   *
   * return stored value.
   */
  operator T() const {
    sync_primitives::AutoReadLock lock(rw_lock_);
    return value_;
  }

  /**
   * @brief operator T thread safe getter
   *
   * return stored value.
   */
  template <typename U>
  operator U() const {
    sync_primitives::AutoReadLock lock(rw_lock_);
    return static_cast<U>(value_);
  }

 private:
  T value_;
  mutable sync_primitives::RWLock rw_lock_;
};

typedef Atomic<int> atomic_int;
typedef Atomic<int32_t> atomic_int32;
typedef Atomic<uint32_t> atomic_uint32;
typedef Atomic<int64_t> atomic_int64;
typedef Atomic<uint64_t> atomic_uint64;
typedef Atomic<size_t> atomic_size_t;
typedef Atomic<bool> atomic_bool;

}  // namespace sync_primitives

#endif  // SRC_COMPONENTS_INCLUDE_UTILS_ATOMIC_OBJECT_H_
