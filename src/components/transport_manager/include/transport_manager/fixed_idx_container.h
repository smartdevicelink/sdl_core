/*
 * \file fixed_idx_container.h
 * \brief 
 *
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

#ifndef FIXED_IDX_CONTAINER_H_
#define FIXED_IDX_CONTAINER_H_

template<class T>
class pointer_container;

template<>
template<class T>
class pointer_container<T*>
{
  T **items_;
  size_t size_;
  size_t capacity_;
 public:
  pointer_container() {
    capacity_ = 64;
    size_ = 0;
    items_ = new T*[capacity_];
  }
  ~pointer_container() {
    delete[] items_;
  }
  int add(T *item) {
    if (size_ >= capacity_) {
      for (T **c = items_; c < items_ + size_; ++c) {
        if (*c == nullptr) {
          *c = item;
          return c - items_;
        }
      }
      capacity_ = size_ * 1.41;
      T** newItems = new T*[capacity_];
      std::copy(items_, items_ + size_, newItems);
      std::fill(newItems + size_, newItems + capacity_, nullptr);
      delete[] items_;
      items_ = newItems;
    }
    items_[size_] = item;
    return size_++;
  }
  void remove(const int idx) {
    items_[idx] = nullptr;
  }
  T* operator[] (int idx) {
    return items_[idx];
  }
  int size() const{
    return size_;
  }
};

#endif /* FIXED_IDX_CONTAINER_H_ */
