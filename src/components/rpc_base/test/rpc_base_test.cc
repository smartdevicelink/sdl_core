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
#include "json/writer.h"
#include "rpc_base/gtest_support.h"
#include "rpc_base/rpc_base.h"

namespace test {
namespace components {
namespace rpc_base_test {
using namespace rpc;

namespace {

enum TestEnum { kValue0, kValue1, kInvalidValue };

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
  ASSERT_RPCTYPE_VALID(true_boolean);
  ASSERT_TRUE(true_boolean.is_initialized());
  ASSERT_EQ(true_boolean, true);

  Boolean false_boolean(false);
  ASSERT_RPCTYPE_VALID(false_boolean);
  ASSERT_TRUE(false_boolean.is_initialized());
  ASSERT_EQ(false_boolean, false);
}

TEST(ValidatedTypes, TestIntegerDefaultConstructor) {
  Integer<int32_t, 4, 100> integer;
  ASSERT_FALSE(integer.is_valid());
  ASSERT_FALSE(integer.is_initialized());
  ASSERT_EQ(integer, 4);
  integer = 5;
  ASSERT_RPCTYPE_VALID(integer);
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
  ASSERT_RPCTYPE_VALID(valid_integer);
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
  ASSERT_RPCTYPE_VALID(flt);
  ASSERT_EQ(flt, 4.);

  flt = 1.2;
  ASSERT_FALSE(flt.is_valid());
  ASSERT_EQ(flt, 1.2);
}

TEST(ValidatedTypes, TestStringDefaultConstructor) {
  String<1, 6> str;
  ASSERT_FALSE(str.is_initialized());
  ASSERT_FALSE(str.is_valid());
  std::string val = str;
  ASSERT_EQ(val, "");
  str = "Test";
  ASSERT_TRUE(str.is_initialized());
  ASSERT_RPCTYPE_VALID(str);
  val = str;
  ASSERT_EQ(val, "Test");
  str = "Long string";
  ASSERT_TRUE(str.is_initialized());
  ASSERT_FALSE(str.is_valid());
  val = str;
  ASSERT_EQ(val, "Long string");
}

TEST(ValidatedTypes, TestStringLengthRange) {
  String<4, 10> str;
  ASSERT_FALSE(str.is_initialized());
  ASSERT_FALSE(str.is_valid());
  str = "Hello";
  ASSERT_TRUE(str.is_initialized());
  ASSERT_RPCTYPE_VALID(str);
  str = "Sh";
  ASSERT_TRUE(str.is_initialized());
  ASSERT_FALSE(str.is_valid());
}

TEST(ValidatedTypes, TestStringInitializingConstructor) {
  String<1, 4> invalid_str("A string");
  ASSERT_TRUE(invalid_str.is_initialized());
  ASSERT_FALSE(invalid_str.is_valid());
}

TEST(ValidatedTypes, TestStringAssignment) {
  String<1, 5> short_str("Short");
  String<1, 10> long_str("A long string");
  short_str = long_str;
  ASSERT_TRUE(short_str.is_initialized());
  ASSERT_FALSE(short_str.is_valid());
}

TEST(ValidatedTypes, TestArray) {
  Array<String<1, 5>, 2, 10> arr;
  ASSERT_FALSE(arr.is_initialized());
  ASSERT_FALSE(arr.is_valid());
  arr.push_back("Text");
  arr.push_back("Dext");
  ASSERT_RPCTYPE_VALID(arr);
  ASSERT_TRUE(arr.is_initialized());
  arr.push_back("Too long");
  ASSERT_FALSE(arr.is_valid());
}

TEST(ValidatedTypes, TestArrayInitializingConstructor) {
  std::vector<std::string> strings;
  strings.push_back("One");
  strings.push_back("Two");
  Array<String<1, 5>, 2, 10> arr(strings);
  ASSERT_TRUE(arr.is_initialized());
  ASSERT_RPCTYPE_VALID(arr);
}

TEST(ValidatedTypes, TestOptionalEmptyArray) {
  Optional<Array<Integer<int8_t, 0, 10>, 0, 5> > ai;
  ASSERT_RPCTYPE_VALID(ai);
  ASSERT_FALSE(ai.is_initialized());
  Json::FastWriter fw;
  std::string serialized = fw.write(ai.ToJsonValue());
  ASSERT_EQ(serialized, "[]\n");
}

TEST(ValidatedTypes, TestMandatoryEmptyArray) {
  Array<Integer<int8_t, 0, 10>, 0, 5> ai;
  ASSERT_FALSE(ai.is_valid());
  ASSERT_FALSE(ai.is_initialized());
  Json::FastWriter fw;
  std::string serialized = fw.write(ai.ToJsonValue());
  ASSERT_EQ(serialized, "[]\n");
}

TEST(ValidatedTypes, TestMap) {
  Map<String<1, 6>, 2, 10> map;
  ASSERT_FALSE(map.is_initialized());
  ASSERT_FALSE(map.is_valid());
  map["a"] = "Hello";
  map["b"] = "World";
  ASSERT_TRUE(map.is_initialized());
  ASSERT_RPCTYPE_VALID(map);
  map["c"] = "Too long";
  ASSERT_FALSE(map.is_valid());
}

TEST(ValidatedTypes, TestMapInitializingConstructor) {
  std::map<std::string, std::string> init_map;
  init_map["a"] = "Hello";
  init_map["b"] = "World";
  Map<String<1, 6>, 2, 10> map(init_map);
  ASSERT_TRUE(map.is_initialized());
  ASSERT_RPCTYPE_VALID(map);
}

TEST(ValidatedTypes, TestEmptyMandatoryMap) {
  Map<Integer<int8_t, 0, 10>, 0, 5> im;
  ASSERT_FALSE(im.is_valid());
  ASSERT_FALSE(im.is_initialized());
  Json::FastWriter fw;
  std::string serialized = fw.write(im.ToJsonValue());
  ASSERT_EQ(serialized, "{}\n");
}

TEST(ValidatedTypes, TestEnumConstructor) {
  Enum<TestEnum> te;
  ASSERT_FALSE(te.is_initialized());
  ASSERT_FALSE(te.is_valid());
  te = kValue1;
  ASSERT_TRUE(te.is_initialized());
  ASSERT_RPCTYPE_VALID(te);
  ASSERT_EQ(te, kValue1);
  te = TestEnum(42);
  ASSERT_TRUE(te.is_initialized());
  ASSERT_FALSE(te.is_valid());
}

TEST(ValidatedTypes, TestNullableConstructor) {
  Nullable<Integer<int8_t, 2, 10> > nullable_int;
  ASSERT_FALSE(nullable_int.is_initialized());
  ASSERT_FALSE(nullable_int.is_null());
  ASSERT_FALSE(nullable_int.is_valid());
  nullable_int = 5;
  ASSERT_TRUE(nullable_int.is_initialized());
  ASSERT_FALSE(nullable_int.is_null());
  ASSERT_RPCTYPE_VALID(nullable_int);
  nullable_int.set_to_null();
  ASSERT_TRUE(nullable_int.is_initialized());
  ASSERT_TRUE(nullable_int.is_null());
  ASSERT_RPCTYPE_VALID(nullable_int);
}

TEST(ValidatedTypes, TestOptionalNullableConstructor) {
  Optional<Nullable<Integer<int8_t, 2, 10> > > optional_nullable_int;
  ASSERT_FALSE(optional_nullable_int.is_initialized());
  ASSERT_FALSE(optional_nullable_int->is_null());
  ASSERT_RPCTYPE_VALID(optional_nullable_int);
  ASSERT_FALSE(optional_nullable_int);

  *optional_nullable_int = 9;
  ASSERT_TRUE(optional_nullable_int.is_initialized());
  ASSERT_FALSE(optional_nullable_int->is_null());
  ASSERT_RPCTYPE_VALID(optional_nullable_int);
  ASSERT_EQ(9, *optional_nullable_int);
  ASSERT_TRUE(optional_nullable_int);

  optional_nullable_int->set_to_null();
  ASSERT_TRUE(optional_nullable_int.is_initialized());
  ASSERT_TRUE(optional_nullable_int->is_null());
  ASSERT_RPCTYPE_VALID(optional_nullable_int);
}

TEST(ValidatedTypes, TestOptionalConstructor) {
  Optional<Integer<int16_t, 3, 15> > optional_int;
  ASSERT_FALSE(optional_int.is_initialized());
  ASSERT_RPCTYPE_VALID(optional_int);
  *optional_int = 42;
  ASSERT_TRUE(optional_int.is_initialized());
  ASSERT_FALSE(optional_int.is_valid());
  *optional_int = 12;
  ASSERT_TRUE(optional_int.is_initialized());
  ASSERT_RPCTYPE_VALID(optional_int);
  int readback = *optional_int;
  ASSERT_EQ(readback, 12);
}

TEST(ValidatedTypes, TestOptionalInitializingConstructor) {
  Optional<String<1, 12> > optional_string("Hello world");
  ASSERT_TRUE(optional_string.is_initialized());
  ASSERT_RPCTYPE_VALID(optional_string);
  std::string value = *optional_string;
  ASSERT_EQ(value, "Hello world");
}

TEST(ValidatedTypes, TestDifferentTypesAssignment) {
  Integer<int8_t, 1, 3> val;
  Integer<int32_t, 5, 90> val2(45);
  val = val2;
  ASSERT_TRUE(val2.is_initialized());
  ASSERT_RPCTYPE_VALID(val2);
  ASSERT_TRUE(val.is_initialized());
  ASSERT_FALSE(val.is_valid());
}

TEST(ValidatedTypes, ReportUninitializedIntType) {
  Integer<int8_t, 1, 3> val;
  ASSERT_FALSE(val.is_valid());
  ValidationReport report("val");
  val.ReportErrors(&report);
  ASSERT_EQ("val: value is not initialized\n", PrettyFormat(report));
}

TEST(ValidatedTypes, ReportIncorrectInitializedIntType) {
  Integer<int8_t, 1, 3> val(5);
  ASSERT_FALSE(val.is_valid());
  ValidationReport report("val");
  val.ReportErrors(&report);
  ASSERT_EQ("val: value initialized incorrectly\n", PrettyFormat(report));
}

TEST(ValidatedTypes, ReportUninitializedOptionalType) {
  Optional<Integer<int8_t, 1, 3> > val;
  ASSERT_RPCTYPE_VALID(val);
  ValidationReport report("val");
  val.ReportErrors(&report);
  ASSERT_EQ("", PrettyFormat(report));
}

TEST(ValidatedTypes, ReportIncorrectInitializedOptionalType) {
  Optional<Integer<int8_t, 1, 3> > val(5);
  ASSERT_FALSE(val.is_valid());
  ValidationReport report("val");
  val.ReportErrors(&report);
  ASSERT_EQ("val: value initialized incorrectly\n", PrettyFormat(report));
}

TEST(ValidatedTypes, ReportUninitializedNullableIntType) {
  Nullable<Integer<int8_t, 1, 3> > val;
  ASSERT_FALSE(val.is_valid());
  ValidationReport report("val");
  val.ReportErrors(&report);
  ASSERT_EQ("val: value is not initialized\n", PrettyFormat(report));
}

TEST(ValidatedTypes, ReportNullInitializedNullableIntType) {
  Nullable<Integer<int8_t, 1, 3> > val;
  val.set_to_null();
  ASSERT_RPCTYPE_VALID(val);
  ValidationReport report("val");
  val.ReportErrors(&report);
  ASSERT_EQ("", PrettyFormat(report));
}

TEST(ValidatedTypes, ReportNoninitializedIntArray) {
  Array<Enum<TestEnum>, 1, 3> array;
  ASSERT_FALSE(array.is_valid());
  ValidationReport report("array");
  array.ReportErrors(&report);
  ASSERT_EQ("array: object is not initialized\n", PrettyFormat(report));
}

TEST(ValidatedTypes, ReportIncorrectlyInitializedIntArray1) {
  Array<Integer<int8_t, 1, 10>, 1, 3> array;
  array.push_back(11);
  ASSERT_FALSE(array.is_valid());
  ValidationReport report("array");
  array.ReportErrors(&report);
  ASSERT_EQ("array[0]: value initialized incorrectly\n", PrettyFormat(report));
}

TEST(ValidatedTypes, ReportIncorrectlyInitializedIntArray2) {
  Array<Integer<int8_t, 1, 10>, 1, 3> array;
  array.push_back(1);
  array.push_back(2);
  array.push_back(3);
  array.push_back(4);
  ASSERT_FALSE(array.is_valid());
  ValidationReport report("array");
  array.ReportErrors(&report);
  ASSERT_EQ("array: array has invalid size\n", PrettyFormat(report));
}

TEST(ValidatedTypes, ReportIncorrectlyInitializedArray3) {
  Array<Integer<int8_t, 1, 10>, 1, 3> array;
  array.push_back(1);
  array.push_back(2);
  array.push_back(42);
  array.push_back(4);
  ValidationReport report("array");
  array.ReportErrors(&report);
  ASSERT_EQ(
      "array: array has invalid size\n"
      "array[2]: value initialized incorrectly\n",
      PrettyFormat(report));
}

TEST(ValidatedTypes, ReportUninitializedMap) {
  Map<Integer<int8_t, 1, 10>, 1, 3> map;
  ValidationReport report("map");
  map.ReportErrors(&report);
  ASSERT_EQ("map: object is not initialized\n", PrettyFormat(report));
}

TEST(ValidatedTypes, ReportIncorrectlyInitializedMap1) {
  Map<Integer<int8_t, 1, 10>, 1, 3> map;
  map["aha"] = 42;
  ValidationReport report("map");
  map.ReportErrors(&report);
  ASSERT_EQ("map[\"aha\"]: value initialized incorrectly\n",
            PrettyFormat(report));
}

TEST(ValidatedTypes, ReportIncorrectlyInitializedMap2) {
  Map<Integer<int8_t, 1, 10>, 1, 3> map;
  map["aha"] = 3;
  map["haha"] = 12;
  map["muhahaha"] = 17;
  map["muhahaha"] = 22;
  ValidationReport report("map");
  map.ReportErrors(&report);
  ASSERT_EQ(
      "map[\"haha\"]: value initialized incorrectly\n"
      "map[\"muhahaha\"]: value initialized incorrectly\n",
      PrettyFormat(report));
}

}  // namespace rpc_base_test
}  // namespace components
}  // namespace test
