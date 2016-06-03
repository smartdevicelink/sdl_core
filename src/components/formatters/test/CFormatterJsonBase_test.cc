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

#include <string>
#include <algorithm>
#include "json/value.h"
#include "gtest/gtest.h"
#include "json/reader.h"
#include "formatters/CFormatterJsonBase.h"
#include "formatters/generic_json_formatter.h"
#include "utils/json_utils.h"
#include "utils/convert_utils.h"

namespace test {
namespace components {
namespace formatters {

using namespace NsSmartDeviceLink::NsSmartObjects;
using namespace NsSmartDeviceLink::NsJSONHandler::Formatters;

using utils::json::JsonValue;
using utils::json::JsonValueRef;

TEST(CFormatterJsonBaseTest, JSonStringValueToSmartObj_ExpectSuccessful) {
  // Arrange value
  const std::string string_val("test_string");
  const JsonValue json_value(string_val);
  SmartObject object;
  // Convert json to smart object
  const JsonValueRef json_value_ref = json_value;

  CFormatterJsonBase::jsonValueToObj(json_value_ref, object);
  // Check conversion was successful
  EXPECT_EQ(string_val, object.asString());
}

TEST(CFormatterJsonBaseTest, JSonDoubleValueToSmartObj_ExpectSuccessful) {
  // Arrange value
  const double dval = 3.512;
  const JsonValue json_value(dval);
  SmartObject object;
  // Convert json to smart object
  const JsonValueRef json_value_ref = json_value;
  CFormatterJsonBase::jsonValueToObj(json_value_ref, object);
  // Check conversion was successful
  EXPECT_DOUBLE_EQ(dval, object.asDouble());
}

// TODO(OHerasym) : DCHECK on QT platform
TEST(CFormatterJsonBaseTest, JSonMinIntValueToSmartObj_ExpectSuccessful) {
  // Arrange value
  const Json::Int ival = Json::Value::minInt;
  const JsonValue json_value(utils::ConvertInt64ToLongLongInt(ival));
  SmartObject object;
  // Convert json to smart object
  const JsonValueRef json_value_ref = json_value;
  CFormatterJsonBase::jsonValueToObj(json_value_ref, object);
  // Check conversion was successful
  EXPECT_EQ(ival, object.asInt());
}

// TODO(OHerasym) : DCHECK on QT platform
TEST(CFormatterJsonBaseTest, JSonNullIntValueToSmartObj_ExpectSuccessful) {
  // Arrange value
  const Json::Int ival = Json::nullValue;
  const JsonValue json_value(utils::ConvertInt64ToLongLongInt(ival));
  SmartObject object;
  // Convert json to smart object
  const JsonValueRef json_value_ref = json_value;
  CFormatterJsonBase::jsonValueToObj(json_value_ref, object);
  // Check conversion was successful
  EXPECT_EQ(ival, object.asInt());
}

// TODO(OHerasym) : DCHECK on QT platform
TEST(CFormatterJsonBaseTest, JSonSignedMaxIntValueToSmartObj_ExpectSuccessful) {
  // Arrange value
  const Json::Int ival = Json::Value::maxInt;
  const JsonValue json_value(utils::ConvertInt64ToLongLongInt(ival));
  SmartObject object;
  // Convert json to smart object
  const JsonValueRef json_value_ref = json_value;
  CFormatterJsonBase::jsonValueToObj(json_value_ref, object);
  // Check conversion was successful
  EXPECT_EQ(ival, object.asInt());
}

// TODO(OHerasym) : DCHECK on QT platform
TEST(CFormatterJsonBaseTest,
     JSonUnsignedMaxIntValueToSmartObj_ExpectSuccessful) {
  // Arrange value
  const Json::UInt ui_val = Json::Value::maxUInt;
  const JsonValue json_value(utils::ConvertInt64ToLongLongInt(ui_val));
  SmartObject object;
  // Convert json to smart object
  const JsonValueRef json_value_ref = json_value;
  CFormatterJsonBase::jsonValueToObj(json_value_ref, object);
  // Check conversion was successful
  EXPECT_EQ(ui_val, object.asUInt());
}

// TODO(OHerasym) : jsoncpp/src/lib_json/json_value.cpp:1073 assert fails
TEST(CFormatterJsonBaseTest,
     DISABLED_JSonSignedMaxInt64ValueToSmartObj_ExpectSuccess) {
  // Arrange value
  const Json::Int64 ival = Json::Value::maxInt64;
  // Json value from maximum possible signed int
  JsonValue json_value(ival);
  SmartObject object;
  // Convert json to smart object
  JsonValueRef json_value_ref = json_value;
  json_value_ref.Append(json_value);
  CFormatterJsonBase::jsonValueToObj(json_value_ref, object);
  // Check conversion was successful
  EXPECT_EQ(ival, object.asInt());
}

// TODO(OHerasym) : jsoncpp/src/lib_json/json_value.cpp:1073 assert fails
TEST(CFormatterJsonBaseTest,
     DISABLED_JSonUnsignedMaxInt64ValueToSmartObj_ExpectFailed) {
  // Arrange value
  const Json::UInt64 ival = Json::Value::maxUInt64;
  // Json value from max possible unsigned int
  const JsonValue json_value(ival);
  SmartObject object;
  // Convert json to smart object
  JsonValueRef json_value_ref = json_value;
  json_value_ref.Append(json_value);
  CFormatterJsonBase::jsonValueToObj(json_value_ref, object);
  // Check conversion was not successful as there is no such conversion
  EXPECT_EQ(invalid_int64_value, object.asInt());
}

TEST(CFormatterJsonBaseTest, JSonBoolValueToSmartObj_ExpectSuccessful) {
  // Arrange value
  const bool bval1 = true;
  const bool bval2 = false;
  const JsonValue json_value1(bval1);
  const JsonValue json_value2(bval2);
  SmartObject object1;
  SmartObject object2;
  // Convert json to smart object
  const JsonValueRef json_value_ref1 = json_value1;
  const JsonValueRef json_value_ref2 = json_value2;
  CFormatterJsonBase::jsonValueToObj(json_value_ref1, object1);
  CFormatterJsonBase::jsonValueToObj(json_value_ref2, object2);
  // Check conversion was successful
  EXPECT_TRUE(object1.asBool());
  EXPECT_FALSE(object2.asBool());
}

TEST(CFormatterJsonBaseTest, JSonCStringValueToSmartObj_ExpectSuccessful) {
  // Arrange value
  const char* cstr_val = "cstring_test";
  const JsonValue json_value(cstr_val);
  SmartObject object;
  // Convert json to smart object
  const JsonValueRef json_value_ref = json_value;
  CFormatterJsonBase::jsonValueToObj(json_value_ref, object);
  // Check conversion was successful
  EXPECT_STREQ(cstr_val, object.asCharArray());
}

TEST(CFormatterJsonBaseTest, JSonArrayValueToSmartObj_ExpectSuccessful) {
  // Arrange value
  const char* json_array =
      "[\"test1\", \"test2\", \"test3\"]";  // Array in json format

  JsonValue::ParseResult json_value_parse = JsonValue::Parse(json_array);
  const JsonValue json_value = json_value_parse.first;
  SmartObject object;
  const JsonValueRef json_value_ref = json_value;
  CFormatterJsonBase::jsonValueToObj(json_value_ref, object);
  // Check conversion was successful
  EXPECT_TRUE(json_value.IsArray());
  EXPECT_EQ(3u, object.asArray()->size());
  SmartArray* ptr = NULL;  // Smart Array pointer;
  EXPECT_NE(ptr, object.asArray());
}

TEST(CFormatterJsonBaseTest, JSonObjectValueToSmartObj_ExpectSuccessful) {
  // Arrange value
  const char* json_object =
      "{ \"json_test_object\": [\"test1\", \"test2\", \"test3\"], "
      "\"json_test_object2\": [\"test11\", \"test12\", \"test13\" ]}";
  JsonValue::ParseResult json_value_parse = JsonValue::Parse(json_object);
  const JsonValue json_value = json_value_parse.first;
  // Json value from object. Will be initialized later
  SmartObject object;
  // If parsing not successful - no sense to continue
  const JsonValueRef json_value_ref = json_value;
  CFormatterJsonBase::jsonValueToObj(json_value_ref, object);
  // Check conversion was successful
  EXPECT_TRUE(json_value.IsObject());
  // Get keys collection from Smart Object
  std::set<std::string> keys = object.enumerate();
  std::set<std::string>::iterator it1 = keys.begin();
  // Get members names(keys) from Json object
  Json::Value::Members mems = json_value.GetMemberNames();
  std::vector<std::string>::iterator it;
  // Compare sizes
  EXPECT_EQ(mems.size(), keys.size());
  // Sort mems
  std::sort(mems.begin(), mems.end());
  // Full data compare
  for (it = mems.begin(); it != mems.end(); ++it) {
    EXPECT_EQ(*it, *it1);
    ++it1;
  }
  EXPECT_TRUE(it == mems.end() && it1 == keys.end());
}

TEST(CFormatterJsonBaseTest, StringSmartObjectToJSon_ExpectSuccessful) {
  // Arrange value
  const std::string string_val("test_string");
  SmartObject object(string_val);
  const JsonValue json_value(string_val);
  // Json value from string
  // Convert smart object to json
  const JsonValueRef json_value_ref = json_value;
  CFormatterJsonBase::objToJsonValue(object, json_value_ref);
  // Check conversion was successful
  EXPECT_EQ(string_val, json_value.AsString());
}

TEST(CFormatterJsonBaseTest, DoubleSmartObjectToJSon_ExpectSuccessful) {
  // Arrange value
  const double dval = 3.512;
  const JsonValue json_value(dval);
  // Json value from double
  SmartObject object(dval);
  // Convert json to smart object
  const JsonValueRef json_value_ref = json_value;
  CFormatterJsonBase::objToJsonValue(object, json_value_ref);
  // Check conversion was successful
  EXPECT_DOUBLE_EQ(dval, json_value.AsDouble());
}

// TODO(OHerasym) : DCHECK on QT platform
TEST(CFormatterJsonBaseTest, ZeroIntSmartObjectToJSon_ExpectSuccessful) {
  // Arrange value
  const Json::Int ival = Json::nullValue;
  const JsonValue json_value(utils::ConvertInt64ToLongLongInt(ival));
  // Json value from zero int
  SmartObject object(ival);
  // Convert json to smart object
  const JsonValueRef json_value_ref = json_value;
  CFormatterJsonBase::objToJsonValue(object, json_value_ref);
  // Check conversion was successful
  EXPECT_EQ(ival, json_value.AsInt());
}

// TODO(OHerasym) : DCHECK on QT platform
TEST(CFormatterJsonBaseTest, MinIntSmartObjectToJSon_ExpectSuccessful) {
  // Arrange value
  const Json::Int ival = Json::Value::minInt;
  const JsonValue json_value(utils::ConvertInt64ToLongLongInt(ival));
  SmartObject object(ival);
  // Convert json to smart object
  const JsonValueRef json_value_ref = json_value;
  CFormatterJsonBase::objToJsonValue(object, json_value_ref);
  // Check conversion was successful
  EXPECT_EQ(ival, json_value.AsInt());
}

// TODO(OHerasym) : assert (convert <= std::numeric_limits<int32_t>::max())
TEST(CFormatterJsonBaseTest, UnsignedMaxIntSmartObjectToJSon_ExpectSuccessful) {
  // Arrange value
  const Json::UInt ui_val = Json::Value::maxUInt;
  const JsonValue json_value(utils::ConvertInt64ToLongLongInt(ui_val));
  SmartObject object(ui_val);
  // Convert json to smart object
  const JsonValueRef json_value_ref = json_value;
  CFormatterJsonBase::objToJsonValue(object, json_value_ref);
  // Check conversion was successful
  EXPECT_EQ(ui_val, json_value.AsUInt());
}

TEST(CFormatterJsonBaseTest, BoolSmartObjectToJSon_ExpectSuccessful) {
  // Arrange value
  const bool bval1 = true;
  const bool bval2 = false;
  const JsonValue json_value1;
  const JsonValue json_value2;
  SmartObject object1(bval1);
  SmartObject object2(bval2);
  // Convert json to smart object
  const JsonValueRef json_value_ref1 = json_value1;
  const JsonValueRef json_value_ref2 = json_value2;
  CFormatterJsonBase::objToJsonValue(object1, json_value_ref1);
  CFormatterJsonBase::objToJsonValue(object2, json_value_ref2);
  // Check conversion was successful
  EXPECT_TRUE(json_value1.AsBool());
  EXPECT_FALSE(json_value2.AsBool());
}

// TODO(OHerasym() : no CSTRING method in JsonValue
TEST(CFormatterJsonBaseTest, CStringSmartObjectToJSon_ExpectSuccessful) {
  // Arrange value
  const char* cstr_val = "cstring_test";
  const JsonValue json_value(cstr_val);  // Json value from const char*
  SmartObject object(cstr_val);
  // Convert json to smart object
  const JsonValueRef json_value_ref = json_value;
  CFormatterJsonBase::objToJsonValue(object, json_value_ref);
  // Check conversion was successful
  //  EXPECT_STREQ(cstr_val, json_value.asCString());
}

TEST(CFormatterJsonBaseTest, ArraySmartObjectToJSon_ExpectSuccessful) {
  // Arrange value
  const char* json_array =
      "[\"test1\", \"test2\", \"test3\"]";  // Array in json format
  JsonValue::ParseResult json_value_parse_result = JsonValue::Parse(json_array);
  const JsonValue json_value(json_value_parse_result.first);
  JsonValue result;

  SmartObject object;
  // Convert json array to SmartObject
  const JsonValueRef json_value_ref = json_value;
  const JsonValueRef json_value_result = result;
  CFormatterJsonBase::jsonValueToObj(json_value_ref, object);
  // Convert SmartObject to JSon
  CFormatterJsonBase::objToJsonValue(object, json_value_result);
  // Check conversion was successful
  EXPECT_TRUE(result.IsArray());
  EXPECT_EQ(3u, result.Size());
}

TEST(CFormatterJsonBaseTest, JSonObjectValueToObj_ExpectSuccessful) {
  // Arrange value
  const char* json_object =
      "{ \"json_test_object\": [\"test1\", \"test2\", \"test3\"], "
      "\"json_test_object2\": [\"test11\", \"test12\", \"test13\" ]}";
  JsonValue::ParseResult json_value_parse = JsonValue::Parse(json_object);
  const JsonValue json_value(json_value_parse.first);
  JsonValue result;

  SmartObject object;
  // Convert json array to SmartObject
  const JsonValueRef json_value_ref = json_value;
  const JsonValueRef json_value_result = result;
  CFormatterJsonBase::jsonValueToObj(json_value_ref, object);
  // Convert SmartObject to JSon
  CFormatterJsonBase::objToJsonValue(object, json_value_result);
  // Check conversion was successful
  EXPECT_TRUE(result.IsObject());
  // Get keys collection from Smart Object
  std::set<std::string> keys = object.enumerate();
  std::set<std::string>::iterator it1 = keys.begin();
  // Get members names(keys) from Json object
  Json::Value::Members mems = result.GetMemberNames();
  std::vector<std::string>::iterator it;
  // Compare sizes
  EXPECT_EQ(mems.size(), keys.size());
  // Sort mems
  std::sort(mems.begin(), mems.end());
  // Full data compare
  for (it = mems.begin(); it != mems.end(); ++it) {
    EXPECT_EQ(*it, *it1);
    ++it1;
  }
  EXPECT_TRUE(it == mems.end() && it1 == keys.end());
}

}  // namespace formatters
}  // namespace components
}  // namespace test
