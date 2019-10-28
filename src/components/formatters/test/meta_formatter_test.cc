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

#include "formatters/meta_formatter.h"
#include "formatters/meta_formatter_test_helper.h"
#include "gtest/gtest.h"

namespace test {
namespace components {
namespace formatters {

TEST_F(CMetaFormatterTestHelper,
       inputObjectIdenticalToSchemaWithAndWithoutMandatoryParams) {
  Json::Value value;
  Json::Reader reader;
  CSmartFactory<FunctionIDTest::eType,
                MessageTypeTest::eType,
                StructIdentifiers::eType>
      factory_;

  SmartObject object1 = factory_.CreateSmartObject(
      FunctionIDTest::RegisterAppInterface, MessageTypeTest::request);

  SmartObject object2 = factory_.CreateSmartObject(
      FunctionIDTest::RegisterAppInterface, MessageTypeTest::request);

  SmartObject result_object1;
  SmartObject result_object2;
  // Get schema
  CSmartSchema schema;
  schema = initSchemaForMetaFormatter();

  FillObjectIdenticalToSchema(object1);
  FillObjectIdenticalToSchemaWithoutNoMandatoriesParams(object2);
  bool creationresult;
  creationresult =
      CMetaFormatter::CreateObjectByPattern(object1, schema, result_object1);
  EXPECT_TRUE(creationresult);
  creationresult =
      CMetaFormatter::CreateObjectByPattern(object2, schema, result_object2);
  EXPECT_TRUE(creationresult);

  CompareObjects(object1, result_object1);
  CompareObjects(object2, result_object2);

  // Enums must be unapplied (converted to string) in order to be compared
  // against strings
  result_object1.getSchema().unapplySchema(result_object1);
  EXPECT_EQ("request", result_object1[S_PARAMS][S_MESSAGE_TYPE].asString());
  EXPECT_EQ("RegisterAppInterface",
            result_object1[S_PARAMS][S_FUNCTION_ID].asString());

  result_object2.getSchema().unapplySchema(result_object2);
  EXPECT_EQ("request", result_object2[S_PARAMS][S_MESSAGE_TYPE].asString());
  EXPECT_EQ("RegisterAppInterface",
            result_object2[S_PARAMS][S_FUNCTION_ID].asString());
}

TEST_F(CMetaFormatterTestHelper, NormalSchemaWithEmptyObject) {
  SmartObject object;
  SmartObject result_object;
  SmartObject expected_object;

  // Get schema
  CSmartSchema schema = initSchemaForMetaFormatter();
  bool create_object_result =
      CMetaFormatter::CreateObjectByPattern(object, schema, result_object);
  EXPECT_TRUE(create_object_result);

  FillObjectWithDefaultValues(expected_object);

  CompareObjects(expected_object, result_object);
}

TEST_F(CMetaFormatterTestHelper,
       NormalSchemaWithObjectWithoutSomeMandatoryFields) {
  SmartObject object;
  SmartObject result_object;

  // Get schema
  CSmartSchema schema = initSchemaForMetaFormatter();
  FillObjectWithoutSomeMandatoryFields(object);

  CMetaFormatter::CreateObjectByPattern(object, schema, result_object);

  CompareObjects(object, result_object);
  EXPECT_EQ(0, result_object[S_PARAMS][S_CORRELATION_ID].asInt());
  EXPECT_EQ(
      0, result_object[S_MSG_PARAMS]["syncMsgVersion"]["majorVersion"].asInt());
  EXPECT_EQ(
      0, result_object[S_MSG_PARAMS]["syncMsgVersion"]["minorVersion"].asInt());
  EXPECT_EQ(
      0, result_object[S_MSG_PARAMS]["syncMsgVersion"]["patchVersion"].asInt());
}

TEST_F(CMetaFormatterTestHelper, ObjectWithEmptyMap) {
  std::map<std::string, SMember> schemaMembersMap;
  CSmartSchema map_schema =
      CSmartSchema(CObjectSchemaItem::create(schemaMembersMap));

  SmartObject object;
  SmartObject result_object_empty_map;

  SmartObject object_empty_map = SmartObject(SmartType_Map);
  CMetaFormatter::CreateObjectByPattern(
      object_empty_map, map_schema, result_object_empty_map);
  EXPECT_EQ(SmartType_Map, result_object_empty_map.getType())
      << "smartObject is not map type";
  EXPECT_EQ(0u, result_object_empty_map.length()) << "non empty map";

  CMetaFormatter::CreateObjectByPattern(
      object, map_schema, result_object_empty_map);
  EXPECT_EQ(SmartType_Map, result_object_empty_map.getType())
      << "smartObject is not map type";
  EXPECT_EQ(0u, result_object_empty_map.length()) << "non empty map";

  object["field1"] = 0;
  object["field2"] = SmartObject();
  CMetaFormatter::CreateObjectByPattern(
      object, map_schema, result_object_empty_map);
  EXPECT_EQ(SmartType_Map, result_object_empty_map.getType())
      << "smartObject is not map type";
  EXPECT_EQ(0u, result_object_empty_map.length()) << "non empty map";

  // Fill object with any values. Result must be the same
  FillObjectIdenticalToSchema(object);
  CMetaFormatter::CreateObjectByPattern(
      object, map_schema, result_object_empty_map);
  EXPECT_EQ(SmartType_Map, result_object_empty_map.getType())
      << "smartObject is not map type";
  EXPECT_EQ(0u, result_object_empty_map.length()) << "non empty map";

  // Fill object with any values. Result must be the same
  FillObjectIdenticalToSchemaWithoutNoMandatoriesParams(object);
  CMetaFormatter::CreateObjectByPattern(
      object, map_schema, result_object_empty_map);
  EXPECT_EQ(SmartType_Map, result_object_empty_map.getType())
      << "smartObject is not map type";
  EXPECT_EQ(0u, result_object_empty_map.length()) << "non empty map";
}

TEST_F(CMetaFormatterTestHelper, ObjectWithEmptyArray) {
  SmartObject object;

  SmartObject result_object_empty_array;
  CSmartSchema array_schema = CSmartSchema(CArraySchemaItem::create());

  SmartObject object_empty_aray = SmartObject(SmartType_Array);

  CMetaFormatter::CreateObjectByPattern(
      object_empty_aray, array_schema, result_object_empty_array);
  EXPECT_EQ(SmartType_Array, result_object_empty_array.getType())
      << "smartObject is not array type";
  EXPECT_EQ(0u, result_object_empty_array.length()) << "non empty array";

  CMetaFormatter::CreateObjectByPattern(
      object, array_schema, result_object_empty_array);
  EXPECT_EQ(SmartType_Array, result_object_empty_array.getType())
      << "smartObject is not array type";
  EXPECT_EQ(0u, result_object_empty_array.length()) << "non empty array";

  // Fill object with any values. Result must be the same
  FillObjectIdenticalToSchema(object);
  CMetaFormatter::CreateObjectByPattern(
      object, array_schema, result_object_empty_array);
  EXPECT_EQ(SmartType_Array, result_object_empty_array.getType())
      << "smartObject is not array type";
  EXPECT_EQ(0u, result_object_empty_array.length()) << "non empty array";

  // Fill object with any values. Result must be the same
  FillObjectWithoutSomeMandatoryFields(object);
  CMetaFormatter::CreateObjectByPattern(
      object, array_schema, result_object_empty_array);
  EXPECT_EQ(SmartType_Array, result_object_empty_array.getType())
      << "smartObject is not array type";
  EXPECT_EQ(0u, result_object_empty_array.length()) << "non empty array";
}

TEST_F(CMetaFormatterTestHelper,
       ObjectWithEmptyArrayAndEmptyMapWithOtherParameters) {
  // Arrange
  SmartObject result_object;
  SmartObject object;

  std::map<std::string, SMember> paramsMembersMap;

  paramsMembersMap[S_FUNCTION_ID] = SMember(
      TEnumSchemaItem<FunctionIDTest::eType>::create(function_id_items_), true);

  paramsMembersMap[S_MESSAGE_TYPE] = SMember(
      TEnumSchemaItem<MessageTypeTest::eType>::create(message_type_items_),
      true);

  paramsMembersMap[S_CORRELATION_ID] =
      SMember(TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0),
                                             TSchemaItemParameter<int>(100),
                                             TSchemaItemParameter<int>(55)),
              true);

  paramsMembersMap[S_PROTOCOL_VERSION] =
      SMember(TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(1),
                                             TSchemaItemParameter<int>(2)),
              false);
  paramsMembersMap[S_PROTOCOL_TYPE] =
      SMember(TNumberSchemaItem<int>::create(), false);

  std::map<std::string, SMember> schemaMembersMap;

  schemaMembersMap["mandatory_emptyMap1"] = SMember(
      CObjectSchemaItem::create(std::map<std::string, SMember>()), true);

  schemaMembersMap["mandatory_emptyMap2"] = SMember(
      CObjectSchemaItem::create(std::map<std::string, SMember>()), true);

  schemaMembersMap["mandatory_emptyAray"] =
      SMember(CArraySchemaItem::create(TNumberSchemaItem<int>::create()), true);

  schemaMembersMap["non_mandatory_Array"] =
      SMember(CArraySchemaItem::create(TNumberSchemaItem<int>::create(),
                                       TSchemaItemParameter<size_t>(1),
                                       TSchemaItemParameter<size_t>(2)),
              false);

  schemaMembersMap["mandatory_string"] = SMember(
      CStringSchemaItem::create(TSchemaItemParameter<size_t>(0),
                                TSchemaItemParameter<size_t>(500),
                                TSchemaItemParameter<std::string>("defValue")),
      true);

  schemaMembersMap["non_mandatory_string"] =
      SMember(CStringSchemaItem::create(
                  TSchemaItemParameter<size_t>(0),
                  TSchemaItemParameter<size_t>(500),
                  TSchemaItemParameter<std::string>("ignoredDefValue")),
              false);

  std::map<std::string, SMember> rootMembersMap;
  rootMembersMap[S_MSG_PARAMS] =
      SMember(CObjectSchemaItem::create(schemaMembersMap), true);
  rootMembersMap[S_PARAMS] =
      SMember(CObjectSchemaItem::create(paramsMembersMap), true);

  CSmartSchema schema = CSmartSchema(CObjectSchemaItem::create(rootMembersMap));

  // Set object value
  object[S_PARAMS][S_FUNCTION_ID] = 500;
  object[S_PARAMS][S_PROTOCOL_VERSION] = 11;
  object[S_PARAMS]["new_field"] = "100500 string";

  object[S_MSG_PARAMS]["mandatory_emptyMap1"]["field1"] = 123;
  object[S_MSG_PARAMS]["mandatory_emptyMap1"]["field2"][0] = 100;
  object[S_MSG_PARAMS]["mandatory_emptyMap1"]["field2"][1] = 200;
  object[S_MSG_PARAMS]["non_mandatory_Array"][0] = 100;
  object[S_MSG_PARAMS]["non_mandatory_Array"][1] = 200;
  object[S_MSG_PARAMS]["non_mandatory_Array"][2] = 300;
  object[S_MSG_PARAMS]["non_mandatory_string"] = "some string";

  CMetaFormatter::CreateObjectByPattern(object, schema, result_object);

  // Assert
  EXPECT_EQ(500, result_object[S_PARAMS][S_FUNCTION_ID].asInt());
  EXPECT_EQ(-1, result_object[S_PARAMS][S_MESSAGE_TYPE].asInt());
  EXPECT_EQ(55, result_object[S_PARAMS][S_CORRELATION_ID].asInt());
  EXPECT_EQ(11u, result_object[S_PARAMS][S_PROTOCOL_VERSION].asUInt());

  EXPECT_EQ(SmartType_Map,
            result_object[S_MSG_PARAMS]["mandatory_emptyMap1"].getType());
  EXPECT_EQ(0u, result_object[S_MSG_PARAMS]["mandatory_emptyMap1"].length());
  EXPECT_EQ(SmartType_Map,
            result_object[S_MSG_PARAMS]["mandatory_emptyMap2"].getType());
  EXPECT_EQ(0u, result_object[S_MSG_PARAMS]["mandatory_emptyMap2"].length());
  EXPECT_EQ(SmartType_Array,
            result_object[S_MSG_PARAMS]["mandatory_emptyAray"].getType());
  EXPECT_EQ(0u, result_object[S_MSG_PARAMS]["mandatory_emptyAray"].length());
  EXPECT_EQ(100, result_object[S_MSG_PARAMS]["non_mandatory_Array"][0].asInt());
  EXPECT_EQ(200, result_object[S_MSG_PARAMS]["non_mandatory_Array"][1].asInt());
  EXPECT_EQ(300u,
            result_object[S_MSG_PARAMS]["non_mandatory_Array"][2].asUInt());
  EXPECT_EQ(std::string("defValue"),
            result_object[S_MSG_PARAMS]["mandatory_string"].asString());
  EXPECT_EQ(std::string("some string"),
            result_object[S_MSG_PARAMS]["non_mandatory_string"].asString());
}

}  // namespace formatters
}  // namespace components
}  // namespace test
