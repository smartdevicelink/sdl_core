/*
 * Copyright (c) 2022, Ford Motor Company
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

#include "utils/atomic_object.h"

#include <string>

#include "gtest/gtest.h"

namespace test {
namespace utils {

class AtomicString {
 public:
  AtomicString(const std::string& str) : atomic_str_(str) {}

  void SetValue(const std::string& str) {
    atomic_str_ = str;
  }
  std::string GetValue() const {
    return atomic_str_;
  }

 private:
  sync_primitives::Atomic<std::string> atomic_str_;
};

TEST(AtomicObjectTest, Construct) {
  sync_primitives::atomic_int var(5);
  EXPECT_EQ(5, static_cast<int>(var));

  var = 8;
  EXPECT_EQ(8, static_cast<int>(var));

  sync_primitives::atomic_bool flag = true;

  EXPECT_EQ(true, static_cast<bool>(flag));

  flag = false;
  EXPECT_EQ(false, static_cast<bool>(flag));

  AtomicString atomic_str("string");

  EXPECT_EQ("string", atomic_str.GetValue());

  atomic_str.SetValue("string2");
  EXPECT_EQ("string2", atomic_str.GetValue());
}

}  // namespace utils
}  // namespace test
