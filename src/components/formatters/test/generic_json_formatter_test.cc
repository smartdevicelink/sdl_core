/*
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
#include "formatters/generic_json_formatter.h"

namespace test {
namespace components {
namespace formatters {

TEST(GenericJsonFormatter, ToString) {
  namespace smartobj = NsSmartDeviceLink::NsSmartObjects;
  namespace formatters = NsSmartDeviceLink::NsJSONHandler::Formatters;

  smartobj::SmartObject obj;
  std::string result;

  formatters::GenericJsonFormatter::ToString(obj, result);
  ASSERT_STREQ("null\n", result.c_str());

  obj = true;
  formatters::GenericJsonFormatter::ToString(obj, result);
  ASSERT_STREQ("true\n", result.c_str());

  obj = 10;
  formatters::GenericJsonFormatter::ToString(obj, result);
  ASSERT_STREQ("10\n", result.c_str());

  obj = 15.2;
  formatters::GenericJsonFormatter::ToString(obj, result);
  ASSERT_STREQ("15.20\n", result.c_str());

  obj = 'c';
  formatters::GenericJsonFormatter::ToString(obj, result);
  ASSERT_STREQ("\"c\"\n", result.c_str());

  obj[0] = 1;
  obj[1] = true;
  obj[2] = "string";
  formatters::GenericJsonFormatter::ToString(obj, result);
  ASSERT_STREQ("[ 1, true, \"string\" ]\n", result.c_str());

  obj["intField"] = 100500;
  obj["stringField"] = "s";
  obj["subobject"]["boolField"] = false;
  obj["subobject"]["arrayField"][0] = 0;
  obj["subobject"]["arrayField"][1] = 'c';
  obj["subobject"]["arrayField"][2][0] = 10.0;
  formatters::GenericJsonFormatter::ToString(obj, result);
  ASSERT_STREQ("{\n"
               "   \"intField\" : 100500,\n"
               "   \"stringField\" : \"s\",\n"
               "   \"subobject\" : {\n"
               "      \"arrayField\" : [\n"
               "         0,\n"
               "         \"c\",\n"
               "         [ 10.0 ]\n"
               "      ],\n"
               "      \"boolField\" : false\n"
               "   }\n"
               "}\n", result.c_str());
}

TEST(GenericJsonFormatter, FromString) {
  namespace smartobj = NsSmartDeviceLink::NsSmartObjects;
  namespace formatters = NsSmartDeviceLink::NsJSONHandler::Formatters;

  smartobj::SmartObject result;

  ASSERT_FALSE(formatters::GenericJsonFormatter::FromString("", result));
  ASSERT_FALSE(formatters::GenericJsonFormatter::FromString("\"str", result));
  ASSERT_FALSE(formatters::GenericJsonFormatter::FromString("[10", result));
  ASSERT_FALSE(formatters::GenericJsonFormatter::FromString("{10}", result));

  ASSERT_TRUE(formatters::GenericJsonFormatter::FromString("null", result));
  ASSERT_EQ(smartobj::SmartType_Null, result.getType());

  ASSERT_TRUE(formatters::GenericJsonFormatter::FromString("true", result));
  ASSERT_EQ(smartobj::SmartType_Boolean, result.getType());
  ASSERT_EQ(true, result.asBool());

  ASSERT_TRUE(formatters::GenericJsonFormatter::FromString("1", result));
  ASSERT_EQ(smartobj::SmartType_Integer, result.getType());
  ASSERT_EQ(1, result.asInt());

  ASSERT_TRUE(formatters::GenericJsonFormatter::FromString("0.5", result));
  ASSERT_EQ(smartobj::SmartType_Double, result.getType());
  ASSERT_DOUBLE_EQ(0.5, result.asDouble());

  ASSERT_TRUE(formatters::GenericJsonFormatter::FromString("\"str\"", result));
  ASSERT_EQ(smartobj::SmartType_String, result.getType());
  ASSERT_STREQ("str", result.asString().c_str());

  ASSERT_TRUE(formatters::GenericJsonFormatter::FromString("[true, null, 10]",
                                                           result));
  ASSERT_EQ(smartobj::SmartType_Array, result.getType());
  ASSERT_EQ(smartobj::SmartType_Boolean, result.getElement(0U).getType());
  ASSERT_EQ(true, result.getElement(0U).asBool());
  ASSERT_EQ(smartobj::SmartType_Null, result.getElement(1U).getType());
  ASSERT_EQ(smartobj::SmartType_Integer, result.getElement(2U).getType());
  ASSERT_EQ(10, result.getElement(2U).asInt());

  ASSERT_TRUE(
    formatters::GenericJsonFormatter::FromString("{"
                                                 " \"intField\": 100500,"
                                                 " \"subobject\": {"
                                                 "  \"arrayField\": [1, null],"
                                                 "  \"strField\": \"str\""
                                                 " }"
                                                 "}",
                                                 result));
  ASSERT_EQ(smartobj::SmartType_Map, result.getType());
  ASSERT_EQ(smartobj::SmartType_Integer,
            result.getElement("intField").getType());
  ASSERT_EQ(100500, result.getElement("intField").asInt());
  ASSERT_EQ(smartobj::SmartType_Map, result.getElement("subobject").getType());
  ASSERT_EQ(smartobj::SmartType_Array,
            result.getElement("subobject").getElement("arrayField").getType());
  ASSERT_EQ(smartobj::SmartType_Integer,
            result.getElement("subobject").getElement("arrayField").getElement(0U).getType());
  ASSERT_EQ(1, result.getElement("subobject").getElement("arrayField").getElement(0U).asInt());
  ASSERT_EQ(smartobj::SmartType_Null,
            result.getElement("subobject").getElement("arrayField").getElement(1U).getType());
  ASSERT_EQ(smartobj::SmartType_String,
            result.getElement("subobject").getElement("strField").getType());
  ASSERT_STREQ(
    "str",
    result.getElement("subobject").getElement("strField").asString().c_str());
}

} // formatters
} // components
} // test
