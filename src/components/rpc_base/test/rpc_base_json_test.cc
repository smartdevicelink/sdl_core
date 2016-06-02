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
#include "json/value.h"
#include "rpc_base/rpc_base.h"
#include "utils/convert_utils.h"

namespace test {
namespace components {
namespace rpc_base_test {

using namespace rpc;
using utils::json::JsonValue;
using utils::json::JsonValueRef;

namespace {
enum TestEnum { kValue0, kValue1, kInvalidValue };

bool EnumFromJsonString(const std::string& value, TestEnum* enm) {
  if (value == "kValue0") {
    *enm = kValue0;
    return true;
  } else if (value == "kValue1") {
    *enm = kValue1;
    return true;
  } else {
    return false;
  }
}

}  // namespace

TEST(ValidatedTypesJson, BooleanFromJsonTest) {
  JsonValue json_value(true);
  JsonValueRef val = json_value;
  Boolean boolean(val);
  ASSERT_TRUE(boolean.is_initialized());
  ASSERT_TRUE(boolean.is_valid());
  ASSERT_EQ(boolean, true);
  JsonValue readback = boolean.ToJsonValue();
  ASSERT_TRUE(readback.IsBool());
  ASSERT_EQ(readback.AsBool(), true);
}

TEST(ValidatedTypesJson, BooleanNullTest) {
  JsonValue json_value(utils::json::ValueType::NULL_VALUE);
  JsonValueRef val = json_value;
  Boolean boolean(val);
  ASSERT_TRUE(boolean.is_initialized());
  ASSERT_FALSE(boolean.is_valid());
}

TEST(ValidatedTypesJson, BooleanAbsentValueTest) {
  JsonValue* json_value = NULL;
  Boolean boolean(*json_value);
  ASSERT_FALSE(boolean.is_initialized());
  ASSERT_FALSE(boolean.is_valid());
}

// TODO(OHerasym) : dcheck Unsupported type inside QVariant
TEST(ValidatedTypesJson, DISABLED_BooleanFromInvalidJsonTest) {
  JsonValue inv(utils::ConvertInt64ToLongLongInt(7));
  Boolean boolean(inv);
  ASSERT_TRUE(boolean.is_initialized());
  ASSERT_FALSE(boolean.is_valid());
}

TEST(ValidatedTypesJson, DISABLED_IntegerFromJsonTest) {
  JsonValue json_value(utils::ConvertInt64ToLongLongInt(42));
  JsonValueRef int_val = json_value;
  Integer<int32_t, -5, 192> integer(int_val);
  ASSERT_TRUE(integer.is_initialized());
  ASSERT_TRUE(integer.is_valid());
  JsonValue readback = integer.ToJsonValue();
  ASSERT_TRUE(readback.IsInt());
  ASSERT_EQ(readback.AsInt(), 42);
}

TEST(ValidatedTypesJson, IntegerNullTest) {
  JsonValue json_value;
  Integer<int32_t, -5, 192> integer(json_value);
  ASSERT_TRUE(integer.is_initialized());
  ASSERT_FALSE(integer.is_valid());
}

TEST(ValidatedTypesJson, IntegerAbsentValueTest) {
  JsonValue* json_value = NULL;
  Integer<int32_t, -5, 192> integer(*json_value);
  ASSERT_FALSE(integer.is_initialized());
  ASSERT_FALSE(integer.is_valid());
}

TEST(ValidatedTypesJson, IntegerFromOverflowingJsonTest) {
  JsonValue json_value(static_cast<double>(0xFFFFFFFFFFll));
  Integer<int32_t, -5, 192> integer(json_value);
  ASSERT_TRUE(integer.is_initialized());
  ASSERT_FALSE(integer.is_valid());
}

TEST(ValidatedTypesJson, IntegerFromInvalidJsonTest) {
  JsonValue json_value("Hello");
  JsonValueRef ref = json_value;
  Integer<int8_t, -3, 15> integer(ref);
  ASSERT_TRUE(integer.is_initialized());
  ASSERT_FALSE(integer.is_valid());
}

TEST(ValidatedTypesJson, DISABLED_IntegerFromOutOfRangeValueTest) {
  JsonValue json_value(utils::ConvertInt64ToLongLongInt(500));
  JsonValueRef ref = json_value;
  Integer<int8_t, 0, 100> integer(ref);
  ASSERT_TRUE(integer.is_initialized());
  ASSERT_FALSE(integer.is_valid());
}

TEST(ValidatedTypesJson, FloatFromJsonTest) {
  JsonValue float_value(4.2);
  JsonValueRef ref = float_value;
  Float<1, 7> flt(ref);
  ASSERT_TRUE(flt.is_initialized());
  ASSERT_TRUE(flt.is_valid());
  JsonValue readback = flt.ToJsonValue();
  ASSERT_TRUE(readback.IsDouble());
  ASSERT_EQ(readback.AsDouble(), 4.2);
}

TEST(ValidatedTypesJson, FloatNullTest) {
  Float<1, 7> flt(utils::json::ValueType::NULL_VALUE);
  ASSERT_TRUE(flt.is_initialized());
  ASSERT_FALSE(flt.is_valid());
}

TEST(ValidatedTypesJson, FloatAbsentValueTest) {
  JsonValue* novalue = NULL;
  Float<1, 7> flt(*novalue);
  ASSERT_FALSE(flt.is_initialized());
  ASSERT_FALSE(flt.is_valid());
}

TEST(ValidatedTypesJson, FloatFromInvalidJsonTest) {
  JsonValue str_val("Hello");
  Float<-5, 3> flt(str_val);
  ASSERT_TRUE(flt.is_initialized());
  ASSERT_FALSE(flt.is_valid());
}

TEST(ValidatedTypesJson, StringNullTest) {
  JsonValue json_value(utils::json::ValueType::NULL_VALUE);
  JsonValueRef ref = json_value;
  String<1, 42> str(ref);
  ASSERT_TRUE(str.is_initialized());
  ASSERT_FALSE(str.is_valid());
}

TEST(ValidatedTypesJson, DISABLED_StringFromInvalidJsonTest) {
  JsonValue json_value(utils::ConvertInt64ToLongLongInt(42));
  JsonValueRef ref = json_value;
  String<1, 500> str(ref);
  ASSERT_TRUE(str.is_initialized());
  ASSERT_FALSE(str.is_valid());
}

TEST(ValidatedTypesJson, StringAbsentValueTest) {
  JsonValue* json_value = NULL;
  String<1, 500> str(*json_value);
  ASSERT_FALSE(str.is_initialized());
  ASSERT_FALSE(str.is_valid());
}

TEST(ValidatedTypesJson, StringFromToLongJsonString) {
  JsonValue json_value("Too long string");
  JsonValueRef ref = json_value;
  String<1, 5> str(ref);
  ASSERT_TRUE(str.is_initialized());
  ASSERT_FALSE(str.is_valid());
}

TEST(ValidatedTypesJson, EnumNullTest) {
  JsonValue json_value(utils::json::ValueType::NULL_VALUE);
  JsonValueRef ref = json_value;
  Enum<TestEnum> enm(ref);
  ASSERT_TRUE(enm.is_initialized());
  ASSERT_FALSE(enm.is_valid());
}

TEST(ValidatedTypesJson, EnumAbsentValueTest) {
  JsonValue* json_value = NULL;
  Enum<TestEnum> enm(*json_value);
  ASSERT_FALSE(enm.is_initialized());
  ASSERT_FALSE(enm.is_valid());
}

TEST(ValidatedTypesJson, EnumFromInvalidJsonTest) {
  JsonValue json_value("Random string");
  JsonValueRef ref = json_value;
  Enum<TestEnum> enm(ref);
  ASSERT_TRUE(enm.is_initialized());
  ASSERT_FALSE(enm.is_valid());
}

TEST(ValidatedTypesJson, ArrayFromJsonTest) {
  JsonValue array_value;
  array_value.Append(JsonValue("haha"));
  array_value.Append(JsonValue("hoho"));
  JsonValueRef ref = array_value;
  Array<String<1, 32>, 2, 5> arr(ref);
  ASSERT_TRUE(arr.is_initialized());
  ASSERT_TRUE(arr.is_valid());
  JsonValue readback = arr.ToJsonValue();
  ASSERT_TRUE(readback.IsArray());
  ASSERT_EQ(readback.Size(), array_value.Size());
}

TEST(ValidatedTypesJson, MandatoryArrayNullTest) {
  JsonValue json_value(utils::json::ValueType::NULL_VALUE);
  JsonValueRef ref = json_value;
  Array<String<1, 32>, 2, 5> arr(ref);
  ASSERT_TRUE(arr.is_initialized());
  ASSERT_FALSE(arr.is_valid());
}

TEST(ValidatedTypesJson, ArrayAbsentValueTest) {
  JsonValue* json_value = NULL;
  JsonValueRef ref = *json_value;
  Array<String<1, 32>, 2, 5> arr(ref);
  ASSERT_FALSE(arr.is_initialized());
  ASSERT_FALSE(arr.is_valid());
}

TEST(ValidatedTypesJson, MandatoryMapNullTest) {
  const JsonValue json_value(utils::json::ValueType::NULL_VALUE);
  const JsonValueRef ref = json_value;
  Map<String<1, 32>, 2, 5> map(ref);
  ASSERT_TRUE(map.is_initialized());
  ASSERT_FALSE(map.is_valid());
}

TEST(ValidatedTypesJson, OptionalMapAbsentValueTest) {
  JsonValue* json_value = NULL;
  JsonValueRef ref = *json_value;
  Optional<Map<String<1, 32>, 0, 5> > map(ref);
  ASSERT_FALSE(map.is_initialized());
  ASSERT_TRUE(map.is_valid());
}

TEST(ValidatedTypesJson, ArrayJsonTest) {
  JsonValue hello("Hello");
  JsonValue world("World");

  JsonValue array_value;
  array_value.Append(hello);
  array_value.Append(world);
  JsonValueRef ref = array_value;
  Array<Integer<int8_t, 0, 32>, 2, 4> int_array(ref);
  ASSERT_TRUE(int_array.is_initialized());
  ASSERT_TRUE(int_array.is_valid());
  ASSERT_EQ(int_array.size(), array_value.Size());
}

TEST(ValidatedTypesJson, ArrayFromNonArrayJsonTest) {
  JsonValue json_value("Hello");
  JsonValueRef ref = json_value;
  Array<Integer<int8_t, 0, 32>, 0, 4> int_array(ref);
  ASSERT_TRUE(int_array.is_initialized());
  ASSERT_FALSE(int_array.is_valid());
  ASSERT_TRUE(int_array.empty());
}

TEST(ValidatedTypesJson, MapFromNonArrayJsonTest) {
  const JsonValue json_value("Hello");
  const JsonValueRef ref = json_value;
  Map<Integer<int8_t, 0, 32>, 0, 4> int_map(ref);
  ASSERT_TRUE(int_map.is_initialized());
  ASSERT_FALSE(int_map.is_valid());
  ASSERT_TRUE(int_map.empty());
}

TEST(ValidatedTypesJson, OptionalBoolFromJsonTest) {
  JsonValue json_value(true);
  JsonValueRef ref = json_value;
  Optional<Boolean> optional_bool;
  *optional_bool = Boolean(ref);
  ASSERT_TRUE(optional_bool.is_initialized());
  ASSERT_TRUE(optional_bool.is_valid());
  JsonValue readback = optional_bool->ToJsonValue();
  ASSERT_TRUE(readback.IsBool());
  ASSERT_EQ(readback.AsBool(), true);
}

TEST(ValidatedTypesJson, OptionalBoolFromAbsentValueTest) {
  JsonValue* json_value = NULL;
  JsonValueRef ref = *json_value;
  Optional<Boolean> optional_bool;
  *optional_bool = Boolean(ref);
  ASSERT_FALSE(optional_bool.is_initialized());
  // It is ok for Optional value to be absent
  ASSERT_TRUE(optional_bool.is_valid());
}

TEST(ValidatedTypesJson, OptionalBoolFromNullValueTest) {
  JsonValue json_value(utils::json::ValueType::NULL_VALUE);
  JsonValueRef ref = json_value;
  Optional<Boolean> optional_bool;
  *optional_bool = Boolean(ref);
  ASSERT_TRUE(optional_bool.is_initialized());
  // Optional values should not be absent
  ASSERT_FALSE(optional_bool.is_valid());
}

TEST(ValidatedTypesJson, NullableIntFromNullValueTest) {
  JsonValue json_value(utils::json::ValueType::NULL_VALUE);
  JsonValueRef ref = json_value;
  Nullable<Integer<int8_t, 1, 15> > nullable_int(ref);
  ASSERT_TRUE(nullable_int.is_initialized());
  ASSERT_TRUE(nullable_int.is_valid());
  ASSERT_TRUE(nullable_int.is_null());
}

TEST(ValidatedTypesJson, DISABLED_NullableIntFromNonNullValueTest) {
  JsonValue json_value(utils::ConvertInt64ToLongLongInt(3));
  JsonValueRef json = json_value;
  Nullable<Integer<int8_t, 1, 15> > nullable_int(json);
  ASSERT_TRUE(nullable_int.is_initialized());
  ASSERT_TRUE(nullable_int.is_valid());
  ASSERT_FALSE(nullable_int.is_null());
  ASSERT_EQ(3, nullable_int);
}

TEST(ValidatedTypesJson, NullableIntFromAbsentValueTest) {
  JsonValue* json_value = NULL;
  Nullable<Integer<int8_t, 1, 15> > nullable_int(*json_value);
  ASSERT_FALSE(nullable_int.is_initialized());
  ASSERT_FALSE(nullable_int.is_valid());
  ASSERT_FALSE(nullable_int.is_null());
}

TEST(ValidatedTypesJson, DISABLED_OptionalIntFromJsonTest) {
  JsonValue json_value(utils::ConvertInt64ToLongLongInt(42));
  JsonValueRef int_value = json_value;
  Optional<Integer<int64_t, 42, 43> > optional_int;
  *optional_int = Integer<int64_t, 42, 43>(int_value);
  ASSERT_TRUE(optional_int.is_initialized());
  ASSERT_TRUE(optional_int.is_valid());
  JsonValue readback = optional_int->ToJsonValue();
  ASSERT_TRUE(readback.IsInt());
  ASSERT_EQ(readback.AsInt(), 42);
}

}  // namespace rpc_base_test
}  // namespace components
}  // namespace test
