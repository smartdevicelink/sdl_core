/*
 * \file matchers.h
 * \brief customers matchers for gmock
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

#ifndef TEST_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_MATCHERS_H_
#define TEST_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_MATCHERS_H_

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "protocol_handler/raw_message.h"
#include "transport_manager/common.h"

using ::testing::MatcherInterface;
using ::testing::MatchResultListener;
using ::testing::Matcher;

using ::protocol_handler::RawMessage;
using ::transport_manager::RawMessageSptr;

namespace test {
namespace components {
namespace transport_manager {

class RawMessageSptrMatcher : public MatcherInterface<RawMessageSptr> {
 public:
  explicit RawMessageSptrMatcher(const unsigned char* data)
      : data_(data), data_size_(0) {}

  virtual bool MatchAndExplain(const RawMessageSptr ptr,
                               MatchResultListener* listener) const {
    unsigned char *d = ptr->data();
    unsigned int count = 0;
    data_size_ = ptr->data_size();
    for(int i = 0; i < ptr->data_size(); ++i){
      if(d[i] == data_[i])
        ++count;
    }
    return count == ptr->data_size();
  }

  virtual void DescribeTo(::std::ostream* os) const {
    *os << "data_ =  " ;
    for(int i = 0; i < data_size_; ++i){
      if(0 != data_[i])
        *os << data_[i];
    }
  }

  virtual void DescribeNegationTo(::std::ostream* os) const {
    *os << "data_ =  " ;
    for(int i = 0; i < data_size_; ++i){
      if (0 != data_[i])
      *os << data_[i];
    }
  }
 private:
  const unsigned char *data_;
  mutable unsigned int data_size_;
};

inline const Matcher<RawMessageSptr> RawMessageSptrEq(const unsigned char* data) {
  return MakeMatcher(new RawMessageSptrMatcher(data));
}

}  // namespace transport_manager
}  // namespace components
}  // namespace test

#endif /* TEST_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_MATCHERS_H_ */
