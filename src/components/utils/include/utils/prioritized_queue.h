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

#ifndef SRC_COMPONENTS_UTILS_INCLUDE_UTILS_PRIORITIZED_QUEUE_H_
#define SRC_COMPONENTS_UTILS_INCLUDE_UTILS_PRIORITIZED_QUEUE_H_

#include <queue>
#include <vector>

#include "utils/macro.h"

namespace utils {

/*
 * Template queue class that gives out messages respecting their priority
 * Message class must have size_t PriorityOrder() method implemented
 */
template < typename M >
class PrioritizedQueue {
 public:
  typedef M value_type;
  typedef std::vector< std::queue<value_type> > QueuesList;
  PrioritizedQueue()
    : total_size_(0) {
  }
  // All api mimics usual std queue interface
  void push(const value_type& message) {
    size_t message_priority = message.PriorityOrder();
    if (message_priority >= queues_.size())
      queues_.resize(message_priority + 1);
    queues_[message_priority].push(message);
    ++total_size_;
  }
  size_t size() const {
    return total_size_;
  }
  bool empty() const {
    return queues_.empty();
  }
  value_type front() {
    DCHECK(!queues_.empty() && !queues_.back().empty());
    return queues_.back().front();
  }
  void pop() {
    DCHECK(!queues_.empty() && !queues_.back().empty());
    queues_.back().pop();
    --total_size_;
    while (!queues_.empty() && queues_.back().empty())
      queues_.pop_back();
  }
 private:
  QueuesList queues_;
  size_t total_size_;
};

}

#endif // SRC_COMPONENTS_UTILS_INCLUDE_UTILS_
