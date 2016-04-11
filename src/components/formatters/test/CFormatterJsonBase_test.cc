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

namespace test {
namespace components {
namespace formatters {

using namespace NsSmartDeviceLink::NsSmartObjects;
using namespace NsSmartDeviceLink::NsJSONHandler::Formatters;

TEST(CFormatterJsonBaseTest, JSonStringValueToSmartObj_ExpectSuccessful) {
  // Arrange value
  std::string string_val("test_string");
  Json::Value json_value(string_val);  // Json value from string
  SmartObject object;
  // Convert json to smart object
  CFormatterJsonBase::jsonValueToObj(json_value, object);
  // Check conversion was successful
  EXPECT_EQ(string_val, object.asString());
}

TEST(CFormatterJsonBaseTest, JSonDoubleValueToSmartObj_ExpectSuccessful) {
  // Arrange value
  double dval = 3.512;
  Json::Value json_value(dval);  // Json value from double
  SmartObject object;
  // Convert json to smart object
  CFormatterJsonBase::jsonValueToObj(json_value, object);
  // Check conversion was successful
  EXPECT_DOUBLE_EQ(dval, object.asDouble());
}

TEST(CFormatterJsonBaseTest, JSonMinIntValueToSmartObj_ExpectSuccessful) {
  // Arrange value
  Json::Int ival = Json::Value::minInt;
  Json::Value json_value(ival);  // Json value from possible minimum signed int
  SmartObject object;
  // Convert json to smart object
  CFormatterJsonBase::jsonValueToObj(json_value, object);
  // Check conversion was successful
  EXPECT_EQ(ival, object.asInt());
}

TEST(CFormatterJsonBaseTest, JSonNullIntValueToSmartObj_ExpectSuccessful) {
  // Arrange value
  Json::Int ival = Json::nullValue;
  Json::Value json_value(ival);  // Json value from null int value
  SmartObject object;
  // Convert json to smart object
  CFormatterJsonBase::jsonValueToObj(json_value, object);
  // Check conversion was successful
  EXPECT_EQ(ival, object.asInt());
}

TEST(CFormatterJsonBaseTest, JSonSignedMaxIntValueToSmartObj_ExpectSuccessful) {
  // Arrange value
  Json::Int ival = Json::Value::maxInt;
  Json::Value json_value(ival);  // Json value from maximum possible signed int
  SmartObject object;
  // Convert json to smart object
  CFormatterJsonBase::jsonValueToObj(json_value, object);
  // Check conversion was successful
  EXPECT_EQ(ival, object.asInt());
}

TEST(CFormatterJsonBaseTest, JSonUnsignedMaxIntValueToSmartObj_ExpectSuccessful) {
  // Arrange value
  Json::UInt ui_val = Json::Value::maxUInt;
  Json::Value json_value(ui_val);  // Json value from maximum possible unsigned int
  SmartObject object;
  // Convert json to smart object
  CFormatterJsonBase::jsonValueToObj(json_value, object);
  // Check conversion was successful
  EXPECT_EQ(ui_val, object.asUInt());
}

TEST(CFormatterJsonBaseTest, JSonSignedMaxInt64ValueToSmartObj_ExpectSuccess) {
  // Arrange value
  Json::Int64 ival = Json::Value::maxInt64;
  Json::Value json_value(ival);  // Json value from maximum possible signed int
  SmartObject object;
  // Convert json to smart object
  CFormatterJsonBase::jsonValueToObj(json_value, object);
  // Check conversion was successful
  EXPECT_EQ(ival, object.asInt());
}

TEST(CFormatterJsonBaseTest, JSonUnsignedMaxInt64ValueToSmartObj_ExpectFailed) {
  // Arrange value
  Json::UInt64 ival = Json::Value::maxUInt64;
  Json::Value json_value(ival);  // Json value from max possible unsigned int
  SmartObject object;
  // Convert json to smart object
  CFormatterJsonBase::jsonValueToObj(json_value, object);
  // Check conversion was not successful as there is no such conversion
  EXPECT_EQ(invalid_int64_value, object.asInt());
}

TEST(CFormatterJsonBaseTest, JSonBoolValueToSmartObj_ExpectSuccessful) {
  // Arrange value
  bool bval1 = true;
  bool bval2 = false;
  Json::Value json_value1(bval1);  // Json value from bool
  Json::Value json_value2(bval2);  // Json value from bool
  SmartObject object1;
  SmartObject object2;
  // Convert json to smart object
  CFormatterJsonBase::jsonValueToObj(json_value1, object1);
  CFormatterJsonBase::jsonValueToObj(json_value2, object2);
  // Check conversion was successful
  EXPECT_TRUE(object1.asBool());
  EXPECT_FALSE(object2.asBool());
}

TEST(CFormatterJsonBaseTest, JSonCStringValueToSmartObj_ExpectSuccessful) {
  // Arrange value
  const char* cstr_val = "cstring_test";
  Json::Value json_value(cstr_val);  // Json value from const char*
  SmartObject object;
  // Convert json to smart object
  CFormatterJsonBase::jsonValueToObj(json_value, object);
  // Check conversion was successful
  EXPECT_STREQ(cstr_val, object.asCharArray());
}

TEST(CFormatterJsonBaseTest, JSonArrayValueToSmartObj_ExpectSuccessful) {
  // Arrange value
  const char* json_array = "[\"test1\", \"test2\", \"test3\"]";  // Array in json format
  Json::Value json_value;  // Json value from array. Will be initialized later
  SmartObject object;
  Json::Reader reader;  // Json reader - Needed for correct parsing
  // Parse array to json value
  ASSERT_TRUE(reader.parse(json_array, json_value));
  // Convert json array to SmartObject
  CFormatterJsonBase::jsonValueToObj(json_value, object);
  // Check conversion was successful
  EXPECT_TRUE(json_value.isArray());
  EXPECT_EQ(3u, object.asArray()->size());
  SmartArray *ptr = NULL;  // Smart Array pointer;
  EXPECT_NE(ptr, object.asArray());
}

TEST(CFormatterJsonBaseTest, JSonObjectValueToSmartObj_ExpectSuccessful) {
  // Arrange value
  const char* json_object =
      "{ \"json_test_object\": [\"test1\", \"test2\", \"test3\"], "
      "\"json_test_object2\": [\"test11\", \"test12\", \"test13\" ]}";
  Json::Value json_value;  // Json value from object. Will be initialized later
  SmartObject object;
  Json::Reader reader;  // Json reader - Needed for correct parsing
  ASSERT_TRUE(reader.parse(
      json_object,
      json_value));  // If parsing not successful - no sense to continue
  CFormatterJsonBase::jsonValueToObj(json_value, object);
  // Check conversion was successful
  EXPECT_TRUE(json_value.isObject());
  EXPECT_TRUE(json_value.type() == Json::objectValue);
  // Get keys collection from Smart Object
  std::set<std::string> keys = object.enumerate();
  std::set<std::string>::iterator it1 = keys.begin();
  // Get members names(keys) from Json object
  Json::Value::Members mems = json_value.getMemberNames();
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
  std::string string_val("test_string");
  SmartObject object(string_val);
  Json::Value json_value;  // Json value from string
  // Convert smart object to json
  CFormatterJsonBase::objToJsonValue(object, json_value);
  // Check conversion was successful
  EXPECT_EQ(string_val, json_value.asString());
}

TEST(CFormatterJsonBaseTest, DoubleSmartObjectToJSon_ExpectSuccessful) {
  // Arrange value
  double dval = 3.512;
  Json::Value json_value;  // Json value from double
  SmartObject object(dval);
  // Convert json to smart object
  CFormatterJsonBase::objToJsonValue(object, json_value);
  // Check conversion was successful
  EXPECT_DOUBLE_EQ(dval, json_value.asDouble());
}

TEST(CFormatterJsonBaseTest, ZeroIntSmartObjectToJSon_ExpectSuccessful) {
  // Arrange value
  Json::Int ival = Json::nullValue;
  Json::Value json_value;  // Json value from zero int
  SmartObject object(ival);
  // Convert json to smart object
  CFormatterJsonBase::objToJsonValue(object, json_value);
  // Check conversion was successful
  EXPECT_EQ(ival, json_value.asInt());
}

TEST(CFormatterJsonBaseTest, MinIntSmartObjectToJSon_ExpectSuccessful) {
  // Arrange value
  Json::Int ival = Json::Value::minInt;
  Json::Value json_value;  // Json value from mimimum possible signed int
  SmartObject object(ival);
  // Convert json to smart object
  CFormatterJsonBase::objToJsonValue(object, json_value);
  // Check conversion was successful
  EXPECT_EQ(ival, json_value.asInt());
}

TEST(CFormatterJsonBaseTest, UnsignedMaxIntSmartObjectToJSon_ExpectSuccessful) {
  // Arrange value
  Json::UInt ui_val = Json::Value::maxUInt;
  Json::Value json_value;  // Json value from maximum unsigned int
  SmartObject object(ui_val);
  // Convert json to smart object
  CFormatterJsonBase::objToJsonValue(object, json_value);
  // Check conversion was successful
  EXPECT_EQ(ui_val, json_value.asUInt());
}

TEST(CFormatterJsonBaseTest, BoolSmartObjectToJSon_ExpectSuccessful) {
  // Arrange value
  bool bval1 = true;
  bool bval2 = false;
  Json::Value json_value1;  // Json value from bool
  Json::Value json_value2;  // Json value from bool
  SmartObject object1(bval1);
  SmartObject object2(bval2);
  // Convert json to smart object
  CFormatterJsonBase::objToJsonValue(object1, json_value1);
  CFormatterJsonBase::objToJsonValue(object2, json_value2);
  // Check conversion was successful
  EXPECT_TRUE(json_value1.asBool());
  EXPECT_FALSE(json_value2.asBool());
}

TEST(CFormatterJsonBaseTest, CStringSmartObjectToJSon_ExpectSuccessful) {
  // Arrange value
  const char* cstr_val = "cstring_test";
  Json::Value json_value;  // Json value from const char*
  SmartObject object(cstr_val);
  // Convert json to smart object
  CFormatterJsonBase::objToJsonValue(object, json_value);
  // Check conversion was successful
  EXPECT_STREQ(cstr_val, json_value.asCString());
}

TEST(CFormatterJsonBaseTest, ArraySmartObjectToJSon_ExpectSuccessful) {
  // Arrange value
  const char* json_array = "[\"test1\", \"test2\", \"test3\"]";  // Array in json format
  Json::Value json_value;  // Json value from array. Will be initialized later
  Json::Value result;  // Json value from array. Will be initialized later
  SmartObject object;
  Json::Reader reader;  // Json reader - Needed for correct parsing
  // Parse array to json value
  ASSERT_TRUE(reader.parse(json_array, json_value));  // Convert json array to SmartObject
  // Convert json array to SmartObject
  CFormatterJsonBase::jsonValueToObj(json_value, object);
  // Convert SmartObject to JSon
  CFormatterJsonBase::objToJsonValue(object, result);
  // Check conversion was successful
  EXPECT_TRUE(result.isArray());
  EXPECT_EQ(3u, result.size());
}

TEST(CFormatterJsonBaseTest, JSonObjectValueToObj_ExpectSuccessful) {
  // Arrange value
  const char* json_object =
      "{ \"json_test_object\": [\"test1\", \"test2\", \"test3\"], "
      "\"json_test_object2\": [\"test11\", \"test12\", \"test13\" ]}";
  Json::Value
      json_value;      // Json value from json object. Will be initialized later
  Json::Value result;  // Json value from Smart object. Will keep conversion result
  SmartObject object;
  Json::Reader reader;  // Json reader - Needed for correct parsing
  // Parse json object to correct json value
  ASSERT_TRUE(reader.parse(json_object, json_value));  // If parsing not successful - no sense to continue
  // Convert json array to SmartObject
  CFormatterJsonBase::jsonValueToObj(json_value, object);
  // Convert SmartObject to JSon
  CFormatterJsonBase::objToJsonValue(object, result);
  // Check conversion was successful
  EXPECT_TRUE(result.isObject());
  EXPECT_TRUE(result.type() == Json::objectValue);
  EXPECT_TRUE(result == json_value);
  // Get keys collection from Smart Object
  std::set<std::string> keys = object.enumerate();
  std::set<std::string>::iterator it1 = keys.begin();
  // Get members names(keys) from Json object
  Json::Value::Members mems = result.getMemberNames();
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
