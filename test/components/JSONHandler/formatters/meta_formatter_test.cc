/**
 * @file meta_formatter_test.cc
 * @brief implementation of test for class CMetaFormatter
 */
// Copyright (c) 2013, Ford Motor Company
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// Redistributions of source code must retain the above copyright notice, this
// list of conditions and the following disclaimer.
//
// Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following
// disclaimer in the documentation and/or other materials provided with the
// distribution.
//
// Neither the name of the Ford Motor Company nor the names of its contributors
// may be used to endorse or promote products derived from this software
// without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR 'A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.auto

#include "gtest/gtest.h"

#include <iostream>
#include <stdio.h>

#include "JSONHandler/formatters/meta_formatter.h"
#include "JSONHandler/CSmartFactory.hpp"
#include "JSONHandler/formatters/CFormatterJsonSDLRPCv1.hpp"

#include "SmartObjects/CAlwaysTrueSchemaItem.hpp"
#include "SmartObjects/CAlwaysFalseSchemaItem.hpp"
#include "SmartObjects/CArraySchemaItem.hpp"
#include "SmartObjects/CBoolSchemaItem.hpp"
#include "SmartObjects/CObjectSchemaItem.hpp"
#include "SmartObjects/CStringSchemaItem.hpp"
#include "SmartObjects/TEnumSchemaItem.hpp"
#include "SmartObjects/TNumberSchemaItem.hpp"
#include "SmartObjects/TSchemaItemParameter.hpp"

#include "meta_formatter_test_helper.h" 

using test::components::JSONHandler::formatters::CMetaFormatterTestHelper;
namespace testhelper_ns = test::components::JSONHandler::formatters;
namespace formatter_ns = NsSmartDeviceLink::NsJSONHandler::Formatters;
namespace generated_ns = Gen::test::components::JSONHandler2;
namespace strings_ns = NsSmartDeviceLink::NsJSONHandler::strings;
namespace smartobjects_ns = NsSmartDeviceLink::NsSmartObjects;

namespace test { 
namespace components {
namespace JSONHandler {
namespace formatters {
  
TEST_F(CMetaFormatterTestHelper, test_inputObjectIdenticalToSchemaWithAndWithoutMandatoryParams) {
  Json::Value value;  // just a quick workaround to avoid undefined reference to Json
  Json::Reader reader;    // the same thing
  
  NsSmartDeviceLink::NsSmartObjects::CSmartObject object1 =
    factory_.CreateSmartObject(
      generated_ns::FunctionID::RegisterAppInterfaceID,
      generated_ns::messageType::request);

  NsSmartDeviceLink::NsSmartObjects::CSmartObject object2 =
    factory_.CreateSmartObject(
      generated_ns::FunctionID::RegisterAppInterfaceID,
      generated_ns::messageType::request);
  
  NsSmartDeviceLink::NsSmartObjects::CSmartObject result_object1;
  NsSmartDeviceLink::NsSmartObjects::CSmartObject result_object2;
  // get schema
  NsSmartDeviceLink::NsSmartObjects::CSmartSchema schema;  
  bool get_schema_result = factory_.GetSchema(
      generated_ns::FunctionID::RegisterAppInterfaceID,
      generated_ns::messageType::request,
      schema);

  ASSERT_TRUE(get_schema_result);
  
  FillObjectIdenticalToSchema(object1);
  FillObjectIdenticalToSchemaWithoutNoMandatoriesParams(object2);
  
  formatter_ns::CMetaFormatter::CreateObjectByPattern(object1,
      schema, result_object1);
  formatter_ns::CMetaFormatter::CreateObjectByPattern(object2,
      schema, result_object2);

  if (true == kIsPrintOut) { // printing out
    std::string formatted_string;
    formatter_ns::CFormatterJsonSDLRPCv1::toString(object1, formatted_string);
    printf("object1 %s\n", formatted_string.c_str());

    formatter_ns::CFormatterJsonSDLRPCv1::toString(result_object1, formatted_string);
    printf("result_object1 %s\n", formatted_string.c_str());

    formatter_ns::CFormatterJsonSDLRPCv1::toString(object2, formatted_string);
    printf("object2 %s\n", formatted_string.c_str());

    formatter_ns::CFormatterJsonSDLRPCv1::toString(result_object2, formatted_string);
    printf("result_object2 %s\n", formatted_string.c_str());
  }

  CompareObjects(object1, result_object1);
  CompareObjects(result_object1, object1);
  CompareObjects(object2, result_object2);
  CompareObjects(result_object2, object2);

  // enums must be unapplied (converted to string) in order to be compared against strings
  result_object1.getSchema().unapplySchema(result_object1);
  EXPECT_EQ("request",
    result_object1[strings_ns::S_PARAMS][strings_ns::S_MESSAGE_TYPE].asString());
  EXPECT_EQ("RegisterAppInterfaceID",
    result_object1[strings_ns::S_PARAMS][strings_ns::S_FUNCTION_ID].asString());

  result_object2.getSchema().unapplySchema(result_object2);
  EXPECT_EQ("request",
      result_object2[strings_ns::S_PARAMS][strings_ns::S_MESSAGE_TYPE].asString());
  EXPECT_EQ("RegisterAppInterfaceID",
      result_object2[strings_ns::S_PARAMS][strings_ns::S_FUNCTION_ID].asString());
}


TEST_F(CMetaFormatterTestHelper, test_NormalSchemaWithEmptyObject) {
  NsSmartDeviceLink::NsSmartObjects::CSmartObject object;
  NsSmartDeviceLink::NsSmartObjects::CSmartObject result_object;
  NsSmartDeviceLink::NsSmartObjects::CSmartObject expected_object;
  // get schema
  NsSmartDeviceLink::NsSmartObjects::CSmartSchema schema;
  bool get_schema_result = factory_.GetSchema(
      generated_ns::FunctionID::RegisterAppInterfaceID,
      generated_ns::messageType::request,
      schema);

  ASSERT_TRUE(get_schema_result);

  formatter_ns::CMetaFormatter::CreateObjectByPattern(object, schema, result_object);

  FillObjectWithDefaultValues(expected_object);

  CompareObjects(expected_object, result_object);
  CompareObjects(result_object, expected_object);

  if (true == kIsPrintOut) {
    std::string str;
    AnyObjectToJsonString(result_object, str);
    printf("result_object(default) %s", str.c_str());
  }
}

TEST_F(CMetaFormatterTestHelper, test_NormalSchemaWithObjectWithoutSomeMandatoryFields) {
  NsSmartDeviceLink::NsSmartObjects::CSmartObject object;
  NsSmartDeviceLink::NsSmartObjects::CSmartObject result_object;
  // get schema
  NsSmartDeviceLink::NsSmartObjects::CSmartSchema schema;
  bool get_schema_result = factory_.GetSchema(
      generated_ns::FunctionID::RegisterAppInterfaceID,
      generated_ns::messageType::request,
      schema);

  ASSERT_TRUE(get_schema_result);

  FillObjectWithoutSomeMandatoryFields(object);

  formatter_ns::CMetaFormatter::CreateObjectByPattern(object, schema, result_object);

  CompareObjects(object, result_object);
  EXPECT_EQ(0,
    result_object[strings_ns::S_PARAMS][strings_ns::S_CORRELATION_ID].asInt());
  EXPECT_EQ(0,
    result_object[strings_ns::S_MSG_PARAMS]["syncMsgVersion"]["majorVersion"].asInt());
  EXPECT_EQ(0,
    result_object[strings_ns::S_MSG_PARAMS]["syncMsgVersion"]["minorVersion"].asInt());

  if (true == kIsPrintOut) {
    std::string str;
    AnyObjectToJsonString(result_object, str);
    printf("result_object %s", str.c_str());
  }
}

TEST_F(CMetaFormatterTestHelper, test_SimpleEmptyMap) {
  std::map<std::string, smartobjects_ns::CObjectSchemaItem::SMember> schemaMembersMap;
  smartobjects_ns::CSmartSchema map_schema =
    smartobjects_ns::CSmartSchema(
      smartobjects_ns::CObjectSchemaItem::create(schemaMembersMap));

  smartobjects_ns::CSmartObject object;
  NsSmartDeviceLink::NsSmartObjects::CSmartObject result_object_empty_map;

  smartobjects_ns::CSmartObject object_empty_map =
  smartobjects_ns::CSmartObject(smartobjects_ns::SmartType_Map);
  formatter_ns::CMetaFormatter::CreateObjectByPattern(object_empty_map,
      map_schema, result_object_empty_map);
  ASSERT_EQ(smartobjects_ns::SmartType_Map,  result_object_empty_map.getType())
      << "smartObject is not map type";
  ASSERT_EQ(0, result_object_empty_map.length()) << "non empty map";
  
  formatter_ns::CMetaFormatter::CreateObjectByPattern(object,
      map_schema, result_object_empty_map);
  ASSERT_EQ(smartobjects_ns::SmartType_Map,  result_object_empty_map.getType())
      << "smartObject is not map type";
  ASSERT_EQ(0, result_object_empty_map.length()) << "non empty map";
  
  object["field1"] = 0;
  object["field2"] = smartobjects_ns::CSmartObject();
  formatter_ns::CMetaFormatter::CreateObjectByPattern(object,
      map_schema, result_object_empty_map);
  ASSERT_EQ(smartobjects_ns::SmartType_Map,  result_object_empty_map.getType())
      << "smartObject is not map type";
  ASSERT_EQ(0, result_object_empty_map.length()) << "non empty map";

  // fill object with any values. Result must be the same
  FillObjectIdenticalToSchema(object);
  formatter_ns::CMetaFormatter::CreateObjectByPattern(object,
      map_schema, result_object_empty_map);
  ASSERT_EQ(smartobjects_ns::SmartType_Map,  result_object_empty_map.getType())
      << "smartObject is not map type";
  ASSERT_EQ(0, result_object_empty_map.length()) << "non empty map";

  // fill object with any values. Result must be the same
  FillObjectIdenticalToSchemaWithoutNoMandatoriesParams(object);
  formatter_ns::CMetaFormatter::CreateObjectByPattern(object,
      map_schema, result_object_empty_map);
  ASSERT_EQ(smartobjects_ns::SmartType_Map,  result_object_empty_map.getType())
      << "smartObject is not map type";
  ASSERT_EQ(0, result_object_empty_map.length()) << "non empty map";
  
  if (true == kIsPrintOut) {
    std::string str;
    AnyObjectToJsonString(result_object_empty_map, str);
    printf("result_object(empty map) %s", str.c_str());
  }


}

TEST_F(CMetaFormatterTestHelper, test_SimpleEmptyArray) {
  smartobjects_ns::CSmartObject object;
  
  smartobjects_ns::CSmartObject result_object_empty_array;
  smartobjects_ns::CSmartSchema array_schema =
    smartobjects_ns::CSmartSchema(smartobjects_ns::CArraySchemaItem::create());

  smartobjects_ns::CSmartObject object_empty_aray =
    smartobjects_ns::CSmartObject(smartobjects_ns::SmartType_Array);

  formatter_ns::CMetaFormatter::CreateObjectByPattern(object_empty_aray,
      array_schema, result_object_empty_array);
  ASSERT_EQ(smartobjects_ns::SmartType_Array, result_object_empty_array.getType())
      << "smartObject is not array type";
  ASSERT_EQ(0, result_object_empty_array.length())  << "non empty array";

  formatter_ns::CMetaFormatter::CreateObjectByPattern(object,
      array_schema, result_object_empty_array);
  ASSERT_EQ(smartobjects_ns::SmartType_Array, result_object_empty_array.getType())
      << "smartObject is not array type";
  ASSERT_EQ(0, result_object_empty_array.length()) << "non empty array";

  // fill object with any values. Result must be the same
  FillObjectIdenticalToSchema(object);
  formatter_ns::CMetaFormatter::CreateObjectByPattern(object,
      array_schema, result_object_empty_array);
  ASSERT_EQ(smartobjects_ns::SmartType_Array, result_object_empty_array.getType())
      << "smartObject is not array type";
  ASSERT_EQ(0, result_object_empty_array.length()) << "non empty array";

  // fill object with any values. Result must be the same
  FillObjectWithoutSomeMandatoryFields(object);
  formatter_ns::CMetaFormatter::CreateObjectByPattern(object,
      array_schema, result_object_empty_array);
  ASSERT_EQ(smartobjects_ns::SmartType_Array, result_object_empty_array.getType())
      << "smartObject is not array type";
  ASSERT_EQ(0, result_object_empty_array.length()) << "non empty array";

  if (true == kIsPrintOut) {
    std::string str;
    AnyObjectToJsonString(result_object_empty_array, str);
    printf("result_object(empty array) %s", str.c_str());
  }
}

TEST_F(CMetaFormatterTestHelper, testEmptyArrayAndEmptyMapWithOtherParameters) {
  smartobjects_ns::CSmartObject result_object;
  smartobjects_ns::CSmartObject object;
  
  std::map<std::string, smartobjects_ns::CObjectSchemaItem::SMember>
    paramsMembersMap;
  paramsMembersMap[strings_ns::S_FUNCTION_ID] =
    smartobjects_ns::CObjectSchemaItem::SMember(
      smartobjects_ns::TEnumSchemaItem<function_id::EType>::create(
        function_id_items_), true);
  paramsMembersMap[strings_ns::S_MESSAGE_TYPE] =
    smartobjects_ns::CObjectSchemaItem::SMember(
      smartobjects_ns::TEnumSchemaItem<message_type::EType>::create(
        message_type_items_), true);
  paramsMembersMap[strings_ns::S_CORRELATION_ID] =
    smartobjects_ns::CObjectSchemaItem::SMember(
      smartobjects_ns::TNumberSchemaItem<int>::create(0, 100, 55), true);
  paramsMembersMap[strings_ns::S_PROTOCOL_VERSION] =
    smartobjects_ns::CObjectSchemaItem::SMember(
      smartobjects_ns::TNumberSchemaItem<int>::create(1, 2), false);
  paramsMembersMap[strings_ns::S_PROTOCOL_TYPE] =
    smartobjects_ns::CObjectSchemaItem::SMember(
      smartobjects_ns::TNumberSchemaItem<int>::create(), false);

  std::map<std::string, smartobjects_ns::CObjectSchemaItem::SMember>
    schemaMembersMap;
  schemaMembersMap["mandatory_emptyMap1"] =
     smartobjects_ns::CObjectSchemaItem::SMember(
      smartobjects_ns::CObjectSchemaItem::create(
        std::map<std::string, smartobjects_ns::CObjectSchemaItem::SMember>()), true);
  schemaMembersMap["mandatory_emptyMap2"] =
     smartobjects_ns::CObjectSchemaItem::SMember(
      smartobjects_ns::CObjectSchemaItem::create(
        std::map<std::string, smartobjects_ns::CObjectSchemaItem::SMember>()), true);
  schemaMembersMap["mandatory_emptyAray"] =
     smartobjects_ns::CObjectSchemaItem::SMember(
       smartobjects_ns::CArraySchemaItem::create(
         smartobjects_ns::TNumberSchemaItem<int>::create()), true);

  schemaMembersMap["non_mandatory_Array"] =
     smartobjects_ns::CObjectSchemaItem::SMember(
       smartobjects_ns::CArraySchemaItem::create(
         smartobjects_ns::TNumberSchemaItem<int>::create(),
         smartobjects_ns::TSchemaItemParameter<size_t>(1),
         smartobjects_ns::TSchemaItemParameter<size_t>(2)), false);
  schemaMembersMap["mandatory_string"] =
    smartobjects_ns::CObjectSchemaItem::SMember(
      smartobjects_ns::CStringSchemaItem::create(
        500, std::string("defValue")), true);
  schemaMembersMap["non_mandatory_string"] =
    smartobjects_ns::CObjectSchemaItem::SMember(
      smartobjects_ns::CStringSchemaItem::create(
        500, std::string("ignoredDefValue")), false);

  std::map<std::string, smartobjects_ns::CObjectSchemaItem::SMember>
    rootMembersMap;
  rootMembersMap[strings_ns::S_MSG_PARAMS] =
    smartobjects_ns::CObjectSchemaItem::SMember(
      smartobjects_ns::CObjectSchemaItem::create(schemaMembersMap), true);
  rootMembersMap[strings_ns::S_PARAMS] =
    smartobjects_ns::CObjectSchemaItem::SMember(
      smartobjects_ns::CObjectSchemaItem::create(paramsMembersMap), true);

  smartobjects_ns::CSmartSchema schema =
    smartobjects_ns::CSmartSchema(
      smartobjects_ns::CObjectSchemaItem::create(rootMembersMap));

  // set object value
  object[strings_ns::S_PARAMS][strings_ns::S_FUNCTION_ID] = 500;
  object[strings_ns::S_PARAMS][strings_ns::S_PROTOCOL_VERSION] = 11;
  object[strings_ns::S_PARAMS]["new_field"] = "100500 string";

  object[strings_ns::S_MSG_PARAMS]["mandatory_emptyMap1"]["field1"] = 123;
  object[strings_ns::S_MSG_PARAMS]["mandatory_emptyMap1"]["field2"][0] = 100;
  object[strings_ns::S_MSG_PARAMS]["mandatory_emptyMap1"]["field2"][1] = 200;
  object[strings_ns::S_MSG_PARAMS]["non_mandatory_Array"][0] = 100;
  object[strings_ns::S_MSG_PARAMS]["non_mandatory_Array"][1] = 200;
  object[strings_ns::S_MSG_PARAMS]["non_mandatory_Array"][2] = 300;
  object[strings_ns::S_MSG_PARAMS]["non_mandatory_string"] = "some string";
  

  formatter_ns::CMetaFormatter::CreateObjectByPattern(object, schema, result_object);
  if (true == kIsPrintOut) {
    std::string str;
    AnyObjectToJsonString(object, str);
    printf("object %s", str.c_str());
    AnyObjectToJsonString(result_object, str);
    printf("result_object %s", str.c_str());
  }

  EXPECT_EQ(500,
    result_object[strings_ns::S_PARAMS][strings_ns::S_FUNCTION_ID].asInt());
  EXPECT_EQ(-1,
    result_object[strings_ns::S_PARAMS][strings_ns::S_MESSAGE_TYPE].asInt());
  EXPECT_EQ(55,
    result_object[strings_ns::S_PARAMS][strings_ns::S_CORRELATION_ID].asInt());
  EXPECT_EQ(11,
    result_object[strings_ns::S_PARAMS][strings_ns::S_PROTOCOL_VERSION].asLong());

  EXPECT_EQ(smartobjects_ns::SmartType_Map,
    result_object[strings_ns::S_MSG_PARAMS]["mandatory_emptyMap1"].getType());
  EXPECT_EQ(0,
    result_object[strings_ns::S_MSG_PARAMS]["mandatory_emptyMap1"].length());
  EXPECT_EQ(smartobjects_ns::SmartType_Map,
    result_object[strings_ns::S_MSG_PARAMS]["mandatory_emptyMap2"].getType());
  EXPECT_EQ(0,
    result_object[strings_ns::S_MSG_PARAMS]["mandatory_emptyMap2"].length());
  EXPECT_EQ(smartobjects_ns::SmartType_Array,
    result_object[strings_ns::S_MSG_PARAMS]["mandatory_emptyAray"].getType());
  EXPECT_EQ(0,
    result_object[strings_ns::S_MSG_PARAMS]["mandatory_emptyAray"].length());
  EXPECT_EQ(100,
    result_object[strings_ns::S_MSG_PARAMS]["non_mandatory_Array"][0].asInt());
  EXPECT_EQ(200,
    result_object[strings_ns::S_MSG_PARAMS]["non_mandatory_Array"][1].asInt());
  EXPECT_EQ(300,
    result_object[strings_ns::S_MSG_PARAMS]["non_mandatory_Array"][2].asLong());
  EXPECT_EQ(std::string("defValue"),
    result_object[strings_ns::S_MSG_PARAMS]["mandatory_string"].asString());
  EXPECT_EQ(std::string("some string"),
    result_object[strings_ns::S_MSG_PARAMS]["non_mandatory_string"].asString());
}
}}}}

namespace NsSmartDeviceLink {
namespace NsSmartObjects {

template <>
const std::map<testhelper_ns::function_id::EType, std::string> &
smartobjects_ns::TEnumSchemaItem<testhelper_ns::function_id::EType>
  ::getEnumElementsStringRepresentation(void)  {
  static bool isInitialized = false;
  static std::map<testhelper_ns::function_id::EType, std::string>
      enumStringRepresentationMap;

  if (false == isInitialized)  {
    enumStringRepresentationMap.insert(std::make_pair(
      testhelper_ns::function_id::kRegisterAppInterfaceID,
        "RegisterAppInterface"));
    enumStringRepresentationMap.insert(std::make_pair(
      testhelper_ns::function_id::kUnregisterAppInterfaceID,
        "UnregisterAppInterface"));
    enumStringRepresentationMap.insert(std::make_pair(
      testhelper_ns::function_id::kSetGlobalPropertiesID,
        "SetGlobalProperties"));

    isInitialized = true;
  }

  return enumStringRepresentationMap;
}

template <>
const std::map<testhelper_ns::message_type::EType, std::string> &
smartobjects_ns::TEnumSchemaItem<testhelper_ns::message_type::EType>
  ::getEnumElementsStringRepresentation(void) {
  static bool isInitialized = false;
  static std::map<testhelper_ns::message_type::EType, std::string>
    enumStringRepresentationMap;

  if (false == isInitialized)  {
    enumStringRepresentationMap.insert(std::make_pair(
      testhelper_ns::message_type::kRequest, "request"));
    enumStringRepresentationMap.insert(std::make_pair(
      testhelper_ns::message_type::kResponse, "response"));
    enumStringRepresentationMap.insert(std::make_pair(
      testhelper_ns::message_type::kNotification, "notification"));

    isInitialized = true;
  }

  return enumStringRepresentationMap;
}
}}

int main(int argc, char **argv)
{
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
