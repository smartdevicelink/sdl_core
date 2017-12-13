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
#ifndef SRC_COMPONENTS_INCLUDE_UTILS_DATA_ACCESSOR_H_
#define SRC_COMPONENTS_INCLUDE_UTILS_DATA_ACCESSOR_H_

#include <memory>
#include "utils/lock.h"

// This class is for thread-safe access to data
// It creates a reference-counted shared pointer to an AutoLock object
// When the refcount reaches 0, the AutoLock is deleted, and unlocks the lock
template <class T>
struct DataAccessorImpl {
  DataAccessorImpl(T& data, sync_primitives::Lock& lock)
    : lockptr_(std::make_shared<sync_primitives::AutoLock>(lock))
    , data_(data) {
  }
  T& GetData() const {
    return data_;
  }
 DataAccessorImpl(const DataAccessorImpl&) = default;
 DataAccessorImpl(DataAccessorImpl&&) = default;
 private:
  std::shared_ptr<sync_primitives::AutoLock> lockptr_;
  T& data_;
};

template<class T> using DataAccessor = DataAccessorImpl<const T>;
template<class T> using NonConstDataAccessor = DataAccessorImpl<T>;

#endif  // SRC_COMPONENTS_INCLUDE_UTILS_DATA_ACCESSOR_H_
