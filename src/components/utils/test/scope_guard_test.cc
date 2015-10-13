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

#include "gmock/gmock.h"
#include "utils/scope_guard.h"
#include "utils/macro.h"

namespace test {
namespace components {
namespace utils {

using ::utils::ScopeGuard;
using ::utils::MakeGuard;
using ::utils::MakeObjGuard;
using ::testing::Mock;

class TestObject {
 public:
  MOCK_METHOD0(function_to_call, void());
  MOCK_METHOD1(function_to_call_with_param, void(void*));
};

namespace {
static int call_with_param_count;
void dealloc(char* ptr) {
  delete ptr;
  ++call_with_param_count;
}
}

TEST(ScopeGuardTest, CallFreeFunctionWithParam) {
  {
    call_with_param_count = 0;
    char* ptr = new char;
    ScopeGuard guard = MakeGuard(dealloc, ptr);
    UNUSED(guard);
  }
  EXPECT_EQ(1, call_with_param_count);
}

TEST(ScopeGuardTest, CallObjectFunction) {
  TestObject obj;
  Mock::AllowLeak(&obj);  // Google tests bug
  EXPECT_CALL(obj, function_to_call()).Times(1);
  {
    ScopeGuard guard = MakeObjGuard(obj, &TestObject::function_to_call);
    UNUSED(guard);
  }
}

TEST(ScopeGuardTest, CallObjectFunctionWithParam) {
  TestObject obj;
  EXPECT_CALL(obj, function_to_call_with_param(&obj)).Times(1);
  {
    ScopeGuard guard =
        MakeObjGuard(obj, &TestObject::function_to_call_with_param, &obj);
    UNUSED(guard);
  }
}

TEST(ScopeGuardTest, DismissCallFreeFunctionWithParam) {
  {
    call_with_param_count = 0;
    char* ptr = new char;
    ScopeGuard guard = MakeGuard(dealloc, ptr);
    guard.Dismiss();
  }
  EXPECT_EQ(0, call_with_param_count);
}

TEST(ScopeGuardTest, DismissCallObjectFunction) {
  TestObject obj;
  EXPECT_CALL(obj, function_to_call()).Times(0);
  {
    ScopeGuard guard = MakeObjGuard(obj, &TestObject::function_to_call);
    guard.Dismiss();
  }
}

TEST(ScopeGuardTest, DismissCallObjectFunctionWithParam) {
  TestObject obj;
  EXPECT_CALL(obj, function_to_call_with_param(&obj)).Times(0);
  {
    ScopeGuard guard =
        MakeObjGuard(obj, &TestObject::function_to_call_with_param, &obj);
    guard.Dismiss();
  }
}

}  // namespace utils
}  // components
}  // namesapce test
