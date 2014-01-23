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

#include "gtest/gtest.h"
#include "rpc_base/rpc_base.h"


namespace test {
using namespace rpc;

namespace {

enum TestEnum {
  kValue0,
  kValue1,
  kInvalidValue
};

bool IsValidEnum(TestEnum val) {
  return val == kValue0 || val == kValue1;
}

}  // namespace

TEST(ValidatedTypes, TestBooleanDefaultConstructor) {
  Boolean boolean;
  ASSERT_FALSE(boolean.is_valid());
  ASSERT_FALSE(boolean.is_initialized());
  boolean = true;
  bool val = boolean;
  ASSERT_TRUE(val);
  ASSERT_TRUE(boolean.is_initialized());
}

TEST(ValidatedTypes, TestBooleanInitializingConstructor) {
  Boolean true_boolean(true);
  ASSERT_TRUE(true_boolean.is_valid());
  ASSERT_TRUE(true_boolean.is_initialized());
  ASSERT_EQ(true_boolean, true);

  Boolean false_boolean(false);
  ASSERT_TRUE(false_boolean.is_valid());
  ASSERT_TRUE(false_boolean.is_initialized());
  ASSERT_EQ(false_boolean, false);
}

TEST(ValidatedTypes, TestIntegerDefaultConstructor) {
  Integer<int32_t, 4, 100> integer;
  ASSERT_FALSE(integer.is_valid());
  ASSERT_FALSE(integer.is_initialized());
  ASSERT_EQ(integer, 4);
  integer = 5;
  ASSERT_TRUE(integer.is_valid());
  ASSERT_TRUE(integer.is_initialized());
  ASSERT_EQ(integer, 5);
  integer = 700;
  ASSERT_FALSE(integer.is_valid());
  ASSERT_TRUE(integer.is_initialized());
  ASSERT_EQ(integer, 700);
}

TEST(ValidatedTypes, TestIntegerInitializingConstructor) {
  Integer<int32_t, 0, 100> invalid_integer(200);
  ASSERT_FALSE(invalid_integer.is_valid());
  ASSERT_TRUE(invalid_integer.is_initialized());
  ASSERT_EQ(invalid_integer, 200);

  Integer<int32_t, 0, 100> valid_integer(42);
  ASSERT_TRUE(valid_integer.is_valid());
  ASSERT_TRUE(valid_integer.is_initialized());
  ASSERT_EQ(valid_integer, 42);
}

TEST(ValidatedTypes, TestFloatDefaultConstructor) {
  Float<-5, 12> flt;
  ASSERT_FALSE(flt.is_initialized());
  ASSERT_FALSE(flt.is_valid());
  ASSERT_EQ(flt, -5.);

  flt = 12.3;
  ASSERT_TRUE(flt.is_initialized());
  ASSERT_FALSE(flt.is_valid());
  ASSERT_EQ(flt, 12.3);
}

TEST(ValidatedTypes, TestFloatInitializingConstructor) {
  Float<13, 999, 10, 10> flt(4);
  ASSERT_TRUE(flt.is_initialized());
  ASSERT_TRUE(flt.is_valid());
  ASSERT_EQ(flt, 4.);

  flt = 1.2;
  ASSERT_FALSE(flt.is_valid());
  ASSERT_EQ(flt, 1.2);
}

TEST(ValidatedTypes, TestStringDefaultConstructor) {
  String<6> str;
  ASSERT_FALSE(str.is_initialized());
  ASSERT_FALSE(str.is_valid());
  std::string val = str;
  ASSERT_EQ(val, "");
  str = "Test";
  ASSERT_TRUE(str.is_initialized());
  ASSERT_TRUE(str.is_valid());
  val = str;
  ASSERT_EQ(val, "Test");
  str = "Long string";
  ASSERT_TRUE(str.is_initialized());
  ASSERT_FALSE(str.is_valid());
  val = str;
  ASSERT_EQ(val, "Long string");
}

TEST(ValidatedTypes, TestStringInitializingConstructor) {
  String<4> invalid_str("A string");
  ASSERT_TRUE(invalid_str.is_initialized());
  ASSERT_FALSE(invalid_str.is_valid());
}

TEST(ValidatedTypes, TestStringAssignment) {
  String<5> short_str("Short");
  String<10> long_str("A long string");
  short_str = long_str;
  ASSERT_TRUE(short_str.is_initialized());
  ASSERT_FALSE(short_str.is_valid());
}

TEST(ValidatedTypes, TestArray) {
  Array<String<5>, 2, 10> arr;
  ASSERT_FALSE(arr.is_initialized());
  ASSERT_FALSE(arr.is_valid());
  arr.push_back("Text");
  arr.push_back("Dext");
  ASSERT_TRUE(arr.is_valid());
  ASSERT_TRUE(arr.is_initialized());
  arr.push_back("Too long");
  ASSERT_FALSE(arr.is_valid());
}

TEST(ValidatedTypes, TestArrayInitializingConstructor) {
  std::vector<std::string> strings;
  strings.push_back("One");
  strings.push_back("Two");
  Array<String<5>, 2, 10> arr(strings);
  ASSERT_TRUE(arr.is_initialized());
  ASSERT_TRUE(arr.is_valid());
}

TEST(ValidatedTypes, TestEnumConstructor) {
  Enum<TestEnum> te;
  ASSERT_FALSE(te.is_initialized());
  ASSERT_FALSE(te.is_valid());
  te = kValue1;
  ASSERT_TRUE(te.is_initialized());
  ASSERT_TRUE(te.is_valid());
  ASSERT_EQ(te, kValue1);
  te = TestEnum(42);
  ASSERT_TRUE(te.is_initialized());
  ASSERT_FALSE(te.is_valid());
}

TEST(ValidatedTypes, TestMandatoryConstructor) {
  Mandatory< Integer<int8_t, 2, 10> > mandatory_int;
  ASSERT_FALSE(mandatory_int.is_initialized());
  ASSERT_FALSE(mandatory_int.is_valid());
  mandatory_int = 42;
  ASSERT_TRUE(mandatory_int.is_initialized());
  ASSERT_FALSE(mandatory_int.is_valid());
  mandatory_int = 9;
  ASSERT_TRUE(mandatory_int.is_initialized());
  ASSERT_TRUE(mandatory_int.is_valid());
  int val = mandatory_int;
  ASSERT_EQ(val, 9);
}

TEST(ValidatedTypes, TestMandatoryInitializingConstructor) {
  Mandatory< Integer<int8_t, 2, 10> > mandatory_int(15);
  ASSERT_TRUE(mandatory_int.is_initialized());
  ASSERT_FALSE(mandatory_int.is_valid());
  mandatory_int = 3;
  ASSERT_TRUE(mandatory_int.is_initialized());
  ASSERT_TRUE(mandatory_int.is_valid());
  int8_t val = mandatory_int;
  ASSERT_EQ(val, 3);
}

TEST(ValidatedTypes, TestOptionalConstructor) {
  Optional< Integer<int16_t, 3, 15> > optional_int;
  ASSERT_FALSE(optional_int.is_initialized());
  ASSERT_TRUE(optional_int.is_valid());
  *optional_int = 42;
  ASSERT_TRUE(optional_int.is_initialized());
  ASSERT_FALSE(optional_int.is_valid());
  *optional_int = 12;
  ASSERT_TRUE(optional_int.is_initialized());
  ASSERT_TRUE(optional_int.is_valid());
  int readback = *optional_int;
  ASSERT_EQ(readback, 12);
}

TEST(ValidatedTypes, TestOptionalInitializingConstructor) {
  Optional< String<12> > optional_string("Hello world");
  ASSERT_TRUE(optional_string.is_initialized());
  ASSERT_TRUE(optional_string.is_valid());
  std::string value = *optional_string;
  ASSERT_EQ(value, "Hello world");
}

TEST(ValidatedTypes, TestDifferentTypesAssignment) {
  Integer<int8_t, 1, 3> val;
  Integer<int32_t, 5, 90> val2(45);
  val = val2;
  ASSERT_TRUE(val2.is_initialized());
  ASSERT_TRUE(val2.is_valid());
  ASSERT_TRUE(val.is_initialized());
  ASSERT_FALSE(val.is_valid());
}

}  // namespace codegen
