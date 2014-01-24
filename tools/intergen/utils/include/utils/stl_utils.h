/**
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

#ifndef STL_UTILS_H_
#define STL_UTILS_H_

/*
 * Common helper functions that work with STL types
 */

#include <cassert>
#include <map>
#include <set>

namespace utils {

/*
 * Collects all the keys from given map |m|
 */
template<typename K, typename V>
std::set<K> MapKeys(const std::map<K, V> m) {
  std::set<K> keys;
  for (typename std::map<K, V>::const_iterator i = m.begin(), end = m.end();
      i != end; ++i) {
    keys.insert(i->first);
  }
  return keys;
}

/*
 * Automatcally deletes all the values |cont| elements are pointing to.
 */
template<typename T>
class StdContainerDeleter {
 public:
  explicit StdContainerDeleter(T* cont)
      : cont_(cont) {
  }
  ~StdContainerDeleter() {
    for (typename T::iterator i = cont_->begin(), end = cont_->end(); i != end;
        ++i) {
      delete *i;
    }
  }
 private:
  T* cont_;
};

/*
 * Automatcally deletes all map values |cont| elements are pointing to.
 */
template<typename T>
  class StdMapDeleter {
public:
  explicit StdMapDeleter(T* cont)
      : cont_(cont) {
  }
  ~StdMapDeleter() {
    for (typename T::iterator i = cont_->begin(), end = cont_->end(); i != end;
         ++i) {
      delete i->second;
    }
  }
private:
  T* cont_;
};



}

#endif /* STL_UTILS_H_ */
