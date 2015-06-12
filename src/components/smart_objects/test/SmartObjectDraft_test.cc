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

#include <string>
#include <vector>

#include "gmock/gmock.h"

#include "smart_objects/smart_object.h"

using ::testing::ElementsAre;
using ::testing::ContainerEq;

namespace test {
namespace components {
namespace SmartObjects {
namespace SmartObjectDraftTest {

using NsSmartDeviceLink::NsSmartObjects::SmartObject;
using NsSmartDeviceLink::NsSmartObjects::SmartType;

TEST(SmartObjectsDraftTest, primitive_types) {
  SmartObject obj;

  obj = true;
  ASSERT_TRUE(obj.asBool());
  ASSERT_EQ(SmartType::SmartType_Boolean, obj.getType());

  obj = 5;
  ASSERT_EQ(5, obj.asInt());
  ASSERT_EQ(SmartType::SmartType_Integer, obj.getType());

  obj = 'A';
  ASSERT_DOUBLE_EQ('A', obj.asChar());
  ASSERT_EQ(SmartType::SmartType_Character, obj.getType());

  obj = "Test";
  ASSERT_EQ(std::string("Test"), obj.asString());
  ASSERT_EQ(SmartType::SmartType_String, obj.getType());

  obj = 6.0;
  ASSERT_DOUBLE_EQ(6.0, obj.asDouble());
  ASSERT_EQ(SmartType::SmartType_Double, obj.getType());
}

TEST(SmartObjectsDraftTest, test_map_access) {
  SmartObject obj;

  obj["aa"] = true;
  ASSERT_TRUE(obj["aa"].asInt());
  ASSERT_EQ(SmartType::SmartType_Map, obj.getType());

  obj["aa"]["fds"]["Fsdf"] = 123;
  ASSERT_EQ(123, obj["aa"]["fds"]["Fsdf"].asInt());
  ASSERT_EQ(SmartType::SmartType_Map, obj.getType());
}

TEST(SmartObjectsDraftTest, test_array_access) {
  SmartObject obj;

  obj[0] = 5;
  obj[-1] = 6;  //  Appending new item to array

  ASSERT_EQ(5, obj[0].asInt());
  ASSERT_EQ(6, obj[1].asInt());
  ASSERT_EQ(SmartType::SmartType_Array, obj.getType());
}

TEST(SmartObjectsDraftTest, test_public_interface) {
  SmartObject obj;

  // ---- INTEGER ---- //
  obj = 1;
  ASSERT_EQ(1, obj.asInt());

  // ---- unsigned int ---- //
  obj = static_cast<unsigned int>(100);
  ASSERT_EQ(100u, obj.asUInt());

  // ---- DOUBLE ---- //
  obj = 3.14;
  ASSERT_EQ(3.14, obj.asDouble());

  // ---- CHAR ---- //
  obj = 'a';
  ASSERT_EQ('a', obj.asChar());

  // ---- BOOL ---- //
  obj = true;
  ASSERT_TRUE(obj.asBool());

  // ---- CHAR* ---- //
  obj = "Hello, world";
  ASSERT_EQ(std::string("Hello, world"), obj.asString());

  // ---- STD::STRING ---- //
  obj = std::string("Hello, world");
  ASSERT_EQ(std::string("Hello, world"), obj.asString());

  // ---- Binary ---- //
  NsSmartDeviceLink::NsSmartObjects::SmartBinary binaryData;
  binaryData.push_back('\0');
  binaryData.push_back('a');
  obj = binaryData;
  ASSERT_THAT(obj.asBinary(), ElementsAre('\0', 'a'));

  // ---- ARRAY ---- //
  obj[0] = 1;
  obj[1] = true;
  obj[2] = 'a';
  obj[3] = 3.14;

  ASSERT_EQ(1, obj[0].asInt());
  ASSERT_TRUE(obj[1].asBool());
  ASSERT_EQ('a', obj[2].asChar());
  ASSERT_EQ(3.14, obj[3].asDouble());

  // ---- DEEP ARRAY ---- //
  obj[0] = 1;
  obj[1][0] = 3.14;
  obj[1][1][0] = true;

  ASSERT_EQ(1, obj[0].asInt());
  ASSERT_EQ(3.14, obj[1][0].asDouble());
  ASSERT_TRUE(obj[1][1][0].asBool());

  // ---- MAP ---- //
  obj["name"] = "My name";
  obj["count"] = 10;
  obj["isValid"] = true;

  ASSERT_EQ(std::string("My name"), obj["name"].asString());
  ASSERT_EQ(10, obj["count"].asInt());
  ASSERT_TRUE(obj["isValid"].asBool());

  // ---- DEEP MAP ---- //
  obj["request"]["name"] = "My Request";
  obj["request"]["id"] = 123;
  obj["response"]["name"] = "My Response";
  obj["response"]["id"] = 456;
  obj["we"]["need"]["to"]["go"]["deeper"] = true;

  ASSERT_EQ(std::string("My Request"), obj["request"]["name"].asString());
  ASSERT_EQ(123, obj["request"]["id"].asInt());
  ASSERT_EQ(std::string("My Response"), obj["response"]["name"].asString());
  ASSERT_EQ(456, obj["response"]["id"].asInt());
  ASSERT_TRUE(obj["we"]["need"]["to"]["go"]["deeper"].asBool());
}

TEST(SmartObjectsDraftTest, test_helper_methods) {
  SmartObject obj;

  // ---- INTEGER ---- //
  obj = 1;
  ASSERT_EQ(1, obj.asInt());

  // ---- unsigned int ---- //
  obj = static_cast<unsigned int>(100);
  ASSERT_EQ(100u, obj.asUInt());

  // ---- DOUBLE ---- //
  obj = 3.14;
  ASSERT_EQ(3.14, obj.asDouble());
  //TEST_COMPONENTS_SMART_OBJECTS_SMARTOBJECTDRAFTTEST_H_
  // ---- CHAR ---- //
  obj = 'a';
  ASSERT_EQ('a', obj.asChar());

  // ---- BOOL ---- //
  obj = true;
  ASSERT_TRUE(obj.asBool());

  // ---- STD::STRING ---- //
  obj = std::string("Hello, world");
  ASSERT_EQ(std::string("Hello, world"), obj.asString());

  // ---- Binary ---- //
  NsSmartDeviceLink::NsSmartObjects::SmartBinary binaryData;
  binaryData.push_back('\0');
  binaryData.push_back('a');
  obj = binaryData;
  ASSERT_THAT(obj.asBinary(), ElementsAre('\0', 'a'));
}

TEST(SmartObjectsDraftTest, compare_empty_objects_by_types) {
  ASSERT_EQ(SmartObject(), SmartObject());

  std::vector<SmartType> smart_types;
  smart_types.push_back(SmartType::SmartType_Null);
  smart_types.push_back(SmartType::SmartType_Boolean);
  smart_types.push_back(SmartType::SmartType_Integer);
  smart_types.push_back(SmartType::SmartType_Character);
  smart_types.push_back(SmartType::SmartType_String);
  smart_types.push_back(SmartType::SmartType_Double);
  smart_types.push_back(SmartType::SmartType_Map);
  smart_types.push_back(SmartType::SmartType_Array);
  smart_types.push_back(SmartType::SmartType_Binary);
  smart_types.push_back(SmartType::SmartType_Invalid);

  for (size_t i = 0u; i < smart_types.size(); ++i) {
    const SmartType type_i = smart_types[i];
    for (size_t j = 0u; j < smart_types.size(); ++j) {
      const SmartType type_j = smart_types[i];
      if (type_i == type_j) {
        ASSERT_EQ(SmartObject(type_i), SmartObject(type_j));
        ASSERT_EQ(SmartObject(type_j), SmartObject(type_i));
      } else {
        ASSERT_NE(SmartObject(type_i), SmartObject(type_j));
        ASSERT_EQ(SmartObject(type_j), SmartObject(type_i));
      }
    }
  }
}

TEST(SmartObjectsDraftTest, compare_integer_type) {
  SmartObject value = SmartObject(0xFFFFF);
  SmartObject same_value = SmartObject(static_cast<int64_t>(value.asInt()));

  ASSERT_EQ(value, same_value);
  ASSERT_EQ(same_value, value);

  SmartObject other_value = SmartObject(0x00000);

  ASSERT_NE(value, other_value);
  ASSERT_NE(other_value, value);

  ASSERT_NE(other_value, same_value);
  ASSERT_NE(same_value, other_value);

  ASSERT_NE(value, SmartObject());
  ASSERT_NE(other_value, SmartObject());
  ASSERT_NE(same_value, SmartObject());
}

TEST(SmartObjectsDraftTest, compare_double_type) {
  SmartObject value = SmartObject(6.0);
  SmartObject same_value = SmartObject(6.0);

  ASSERT_EQ(value, same_value);
  ASSERT_EQ(same_value, value);

  SmartObject other_value = SmartObject(6.0000001);

  ASSERT_NE(value, other_value);
  ASSERT_NE(other_value, value);

  ASSERT_NE(other_value, same_value);
  ASSERT_NE(same_value, other_value);

  ASSERT_NE(value, SmartObject());
  ASSERT_NE(other_value, SmartObject());
  ASSERT_NE(same_value, SmartObject());
}

TEST(SmartObjectsDraftTest, compare_bool_type) {
  SmartObject value = SmartObject(true);
  SmartObject same_value = SmartObject(true);

  ASSERT_EQ(value, same_value);
  ASSERT_EQ(same_value, value);

  SmartObject other_value = SmartObject(false);

  ASSERT_NE(value, other_value);
  ASSERT_NE(other_value, value);

  ASSERT_NE(other_value, same_value);
  ASSERT_NE(same_value, other_value);

  ASSERT_NE(value, SmartObject());
  ASSERT_NE(other_value, SmartObject());
  ASSERT_NE(same_value, SmartObject());
}

TEST(SmartObjectsDraftTest, compare_string_type) {
  SmartObject value = SmartObject("Test string");
  SmartObject same_value = SmartObject(std::string("Test string"));

  ASSERT_EQ(value, same_value);
  ASSERT_EQ(same_value, value);

  SmartObject other_value = SmartObject("Other string");

  ASSERT_NE(value, other_value);
  ASSERT_NE(other_value, value);

  ASSERT_NE(other_value, same_value);
  ASSERT_NE(same_value, other_value);

  ASSERT_NE(value, SmartObject());
  ASSERT_NE(other_value, SmartObject());
  ASSERT_NE(same_value, SmartObject());
}

TEST(SmartObjectsDraftTest, compare_map_type) {
  SmartObject value;
  value["KEY1"] = "VALUE1";
  value["KEY2"] = 0;
  value["KEY3"] = false;

  SmartObject same_value;
  same_value["KEY1"] = "VALUE1";
  same_value["KEY2"] = 0;
  same_value["KEY3"] = false;

  ASSERT_EQ(value, same_value);
  ASSERT_EQ(same_value, value);

  SmartObject other_value;
  other_value["KEY1"] = "VALUE1";
  other_value["KEY2"] = 0;
  // no KEY3 field

  SmartObject other_value2;
  other_value2["KEY1"] = "VALUE1";
  other_value2["KEY2"] = 0;
  // other ype of KEY3 field
  other_value2["KEY3"] = "VALUE3";

  ASSERT_NE(value, other_value);
  ASSERT_NE(other_value, value);

  ASSERT_NE(value, other_value2);
  ASSERT_NE(other_value2, value);

  ASSERT_NE(value, SmartObject());
  ASSERT_NE(same_value, SmartObject());
  ASSERT_NE(other_value, SmartObject());
  ASSERT_NE(other_value2, SmartObject());
}
// TODO(Ezamakhov): add test for conversion string/int/double
}// namespace SmartObjectDraftTest
}  // namespace SmartObjects
}  // namespace components
}  // namespace test
