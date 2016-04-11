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

#include "SmartFactoryTestHelper.h"
#include "formatters/CSmartFactory.h"
#include "gtest/gtest.h"

namespace test {
namespace components {
namespace formatters {

TEST(CSmartFactoryTest, CreateSmartSchemaKey_ExpectCreated) {
  SmartSchemaKey<FunctionIdTest::eType, MessageTypeTest::eType> test_key(
      FunctionIdTest::Function1, MessageTypeTest::notification);
  EXPECT_EQ(test_key.functionId, FunctionIdTest::Function1);
  EXPECT_EQ(test_key.messageType, MessageTypeTest::notification);
}

TEST(CSmartFactoryTest, CreateSmartFactory_ExpectCreated) {
  CSmartFactoryTest test_factory;
  EXPECT_EQ(9u, test_factory.function_schemes().size());
  EXPECT_EQ(2u, test_factory.structs_schemes().size());
}

TEST(CSmartFactoryTest, CreateSmartObjWithSchema1_ExpectCreatedObjectToCorrespondSmSchema1) {
  CSmartFactoryTest test_factory;
  // Create SmartObject with attached SmartChmema
  SmartObject obj = test_factory.CreateSmartObject(FunctionIdTest::Function1,
                                                   MessageTypeTest::request);
  EXPECT_TRUE(SmartType::SmartType_Map == obj.getType());
  // Adding necessary fields
  obj[S_PARAMS][S_FUNCTION_ID] = FunctionIdTest::Function1;
  obj[S_PARAMS][S_MESSAGE_TYPE] = MessageTypeTest::request;
  obj[S_PARAMS][S_CORRELATION_ID] = 444;
  obj[S_PARAMS][S_PROTOCOL_VERSION] = 1;
  obj[S_PARAMS][S_PROTOCOL_TYPE] = 0;
  obj[S_MSG_PARAMS] = SmartObject(SmartType::SmartType_Map);
  std::set<std::string> keys = obj.enumerate();
  EXPECT_EQ(2u, keys.size());
  EXPECT_EQ(Errors::eType::OK, obj.validate());
  EXPECT_TRUE(obj.isValid());
}

TEST(CSmartFactoryTest, CreateSmartObjWithNotExistedSchema_ExpectCreatedObjectNotValid) {
  CSmartFactoryTest test_factory;
  // Create SmartObject with attached SmartChmema
  SmartObject obj = test_factory.CreateSmartObject(
      FunctionIdTest::Function1, MessageTypeTest::INVALID_ENUM);
  EXPECT_FALSE(SmartType::SmartType_Map == obj.getType());
  EXPECT_TRUE(SmartType::SmartType_Null == obj.getType());
  EXPECT_EQ(Errors::eType::OK, obj.validate());
  EXPECT_TRUE(obj.isValid());
}

TEST(CSmartFactoryTest, CreateSmartObjectWithSchema1_MissedOneField_ExpectCreatedNotCorrespondSmartSchema) {
  CSmartFactoryTest test_factory;
  // Create SmartObject with attached SmartChmema
  SmartObject obj = test_factory.CreateSmartObject(FunctionIdTest::Function1,
                                                   MessageTypeTest::request);
  EXPECT_TRUE(SmartType::SmartType_Map == obj.getType());
  // Adding necessary fields but one field is missed
  obj[S_PARAMS][S_FUNCTION_ID] = FunctionIdTest::Function1;
  obj[S_PARAMS][S_MESSAGE_TYPE] = MessageTypeTest::request;
  obj[S_PARAMS][S_PROTOCOL_VERSION] = 1;
  obj[S_PARAMS][S_PROTOCOL_TYPE] = 0;
  obj[S_MSG_PARAMS] = SmartObject(SmartType::SmartType_Map);
  std::set<std::string> keys = obj.enumerate();
  EXPECT_EQ(2u, keys.size());
  EXPECT_EQ(Errors::eType::MISSING_MANDATORY_PARAMETER, obj.validate());
  EXPECT_FALSE(obj.isValid());
}

TEST(CSmartFactoryTest, CreateSmartObjectWithSchema1_AddOutOfRangeValue_ExpectCreatedNotCorrespondSmartSchema) {
  CSmartFactoryTest test_factory;
  // Create SmartObject with attached SmartChmema
  SmartObject obj = test_factory.CreateSmartObject(FunctionIdTest::Function1,
                                                   MessageTypeTest::request);
  EXPECT_TRUE(SmartType::SmartType_Map == obj.getType());
  // Adding necessary fields but one field is missed
  obj[S_PARAMS][S_FUNCTION_ID] = FunctionIdTest::Function1;
  obj[S_PARAMS][S_MESSAGE_TYPE] = 5;
  obj[S_PARAMS][S_PROTOCOL_VERSION] = 1;
  obj[S_PARAMS][S_PROTOCOL_TYPE] = 0;
  obj[S_PARAMS][S_CORRELATION_ID] = 444;
  obj[S_MSG_PARAMS] = SmartObject(SmartType::SmartType_Map);
  std::set<std::string> keys = obj.enumerate();
  EXPECT_EQ(2u, keys.size());
  EXPECT_EQ(Errors::eType::OUT_OF_RANGE, obj.validate());
  EXPECT_FALSE(obj.isValid());
}

TEST(CSmartFactoryTest, CreateSmartObjectWithSchema1_AddInvalidValue_ExpectCreatedNotCorrespondSmartSchema) {
  CSmartFactoryTest test_factory;
  // Create SmartObject with attached SmartChmema
  SmartObject obj = test_factory.CreateSmartObject(FunctionIdTest::Function1,
                                                   MessageTypeTest::request);
  EXPECT_TRUE(SmartType::SmartType_Map == obj.getType());
  // Adding necessary fields but one field is missed
  obj[S_PARAMS][S_FUNCTION_ID] = FunctionIdTest::Function1;
  obj[S_PARAMS][S_MESSAGE_TYPE] = "return";
  obj[S_PARAMS][S_PROTOCOL_VERSION] = 1;
  obj[S_PARAMS][S_PROTOCOL_TYPE] = 0;
  obj[S_PARAMS][S_CORRELATION_ID] = 444;
  obj[S_MSG_PARAMS] = SmartObject(SmartType::SmartType_Map);
  std::set<std::string> keys = obj.enumerate();
  EXPECT_EQ(2u, keys.size());
  EXPECT_EQ(Errors::eType::INVALID_VALUE, obj.validate());
  EXPECT_FALSE(obj.isValid());
}

TEST(CSmartFactoryTest, CreateSmartObj_AttachSchema1_ExpectCreatedObjectCorrespondsSmSchema1) {
  CSmartFactoryTest test_factory;
  // Create empty SmartObject  without any schema
  SmartObject obj;
  // Adding fields necessary for schema to attach
  obj[S_PARAMS][S_FUNCTION_ID] = FunctionIdTest::Function1;
  obj[S_PARAMS][S_MESSAGE_TYPE] = MessageTypeTest::request;
  // Attach schema to object
  EXPECT_TRUE(test_factory.attachSchema(obj, false));
  EXPECT_TRUE(SmartType::SmartType_Map == obj.getType());
  // Adding necessary fileds to correspond schema
  obj[S_PARAMS][S_CORRELATION_ID] = 444;
  obj[S_PARAMS][S_PROTOCOL_VERSION] = 1;
  obj[S_PARAMS][S_PROTOCOL_TYPE] = 0;
  obj[S_MSG_PARAMS] = SmartObject(SmartType::SmartType_Map);
  std::set<std::string> keys = obj.enumerate();
  EXPECT_EQ(2u, keys.size());
  EXPECT_EQ(Errors::eType::OK, obj.validate());
  EXPECT_TRUE(obj.isValid());
}

TEST(CSmartFactoryTest, CreateSmartObj_AttachSchema1_MissOneField_ExpectCreatedObjectNotCorrespondsSmSchema1) {
  CSmartFactoryTest test_factory;
  // Create empty SmartObject  without any schema
  SmartObject obj;
  // Adding fields necessary for schema to attach
  obj[S_PARAMS][S_FUNCTION_ID] = FunctionIdTest::Function1;
  obj[S_PARAMS][S_MESSAGE_TYPE] = MessageTypeTest::request;
  // Attach schema to object
  EXPECT_TRUE(test_factory.attachSchema(obj, false));
  EXPECT_TRUE(SmartType::SmartType_Map == obj.getType());
  // Adding necessary fileds to correspond schema but 1 field is missing
  obj[S_PARAMS][S_PROTOCOL_VERSION] = 1;
  obj[S_PARAMS][S_PROTOCOL_TYPE] = 0;
  obj[S_MSG_PARAMS] = SmartObject(SmartType::SmartType_Map);
  std::set<std::string> keys = obj.enumerate();
  EXPECT_EQ(2u, keys.size());
  EXPECT_EQ(Errors::eType::MISSING_MANDATORY_PARAMETER, obj.validate());
  EXPECT_FALSE(obj.isValid());
}

TEST(CSmartFactoryTest, CreateSmartObj_AttachNotExistedSchema_ExpectSmSchemaNotAttached) {
  CSmartFactoryTest test_factory;
  // Create empty SmartObject  without any schema
  SmartObject obj;
  // Adding fields necessary for schema to attach but one value is invalid
  obj[S_PARAMS][S_FUNCTION_ID] = FunctionIdTest::Function1;
  obj[S_PARAMS][S_MESSAGE_TYPE] = 10;
  // Attach schema to object
  EXPECT_FALSE(test_factory.attachSchema(obj, false));
  EXPECT_TRUE(SmartType::SmartType_Map == obj.getType());
}

TEST(CSmartFactoryTest, CreateSmartObj_AttachSchema1_AddInvalidValue_ExpectCreatedObjectNotCorrespondsSmSchema1) {
  CSmartFactoryTest test_factory;
  // Create empty SmartObject  without any schema
  SmartObject obj;
  // Adding fields necessary for schema to attach
  obj[S_PARAMS][S_FUNCTION_ID] = FunctionIdTest::Function1;
  obj[S_PARAMS][S_MESSAGE_TYPE] = MessageTypeTest::request;
  // Attach schema to object
  EXPECT_TRUE(test_factory.attachSchema(obj, false));
  EXPECT_TRUE(SmartType::SmartType_Map == obj.getType());
  // Adding necessary fileds to correspond schema but 1 field is missing
  obj[S_PARAMS][S_PROTOCOL_VERSION] = "string";
  obj[S_PARAMS][S_PROTOCOL_TYPE] = 0;
  obj[S_PARAMS][S_CORRELATION_ID] = 444;
  obj[S_MSG_PARAMS] = SmartObject(SmartType::SmartType_Map);
  std::set<std::string> keys = obj.enumerate();
  EXPECT_EQ(2u, keys.size());
  EXPECT_EQ(Errors::eType::INVALID_VALUE, obj.validate());
  EXPECT_FALSE(obj.isValid());
}

TEST(CSmartFactoryTest, CreateSmartObj_AttachSchema1_ExpectCreatedObjectCorrespondsSmSchema) {
  CSmartFactoryTest test_factory;
  // Create empty SmartObject  without any schema
  SmartObject obj;
  // Attach schema to object
  EXPECT_TRUE(test_factory.AttachSchema(StructIdentifiersTest::Common_1, obj));
  obj["text"] = "test";
  obj["position"] = 200;
  std::set<std::string> keys = obj.enumerate();
  EXPECT_EQ(2u, keys.size());
  EXPECT_EQ(Errors::eType::OK, obj.validate());
  EXPECT_TRUE(obj.isValid());
}

TEST(CSmartFactoryTest, CreateSmartObj_AttachSchema1_OneMandatoryFieldMissed_ExpectCreatedObjectNotCorrespondsSmSchema) {
  CSmartFactoryTest test_factory;
  // Create empty SmartObject  without any schema
  SmartObject obj;
  // Attach schema to object
  EXPECT_TRUE(test_factory.AttachSchema(StructIdentifiersTest::Common_1, obj));
  obj["text"] = "test";
  std::set<std::string> keys = obj.enumerate();
  EXPECT_EQ(1u, keys.size());
  EXPECT_EQ(Errors::eType::MISSING_MANDATORY_PARAMETER, obj.validate());
  EXPECT_FALSE(obj.isValid());
}

TEST(CSmartFactoryTest, CreateSmartObj_AttachSchema2_ExpectCreatedObjectCorrespondsSmSchema) {
  CSmartFactoryTest test_factory;
  // Create empty SmartObject  without any schema
  SmartObject obj;
  // Attach schema to object
  EXPECT_TRUE(test_factory.AttachSchema(StructIdentifiersTest::Common_2, obj));
  obj["text"] = "test1";
  obj["position"] = 200;
  std::set<std::string> keys = obj.enumerate();
  EXPECT_EQ(2u, keys.size());
  EXPECT_EQ(Errors::eType::OK, obj.validate());
  EXPECT_TRUE(obj.isValid());
}

TEST(CSmartFactoryTest, CreateSmartObj_AttachSchema_ExpectCreatedObjectCorrespondsSmSchema) {
  CSmartFactoryTest test_factory;
  // Create empty SmartObject  without any schema
  SmartObject obj;
  // Attach schema to object
  EXPECT_TRUE(test_factory.AttachSchema(StructIdentifiersTest::Common_1, obj));
  obj["text"] = "test";
  obj["position"] = 200;
  obj["image"]["text"] = "test2";
  obj["image"]["position"] = 100;
  EXPECT_EQ(Errors::eType::OK, obj["image"].validate());
  std::set<std::string> keys = obj.enumerate();
  EXPECT_EQ(3u, keys.size());
  EXPECT_EQ(Errors::eType::OK, obj.validate());
  EXPECT_TRUE(obj.isValid());
}

TEST(CSmartFactoryTest, CreateSmartObj_WithSchemaFromStructId_ExpectCreatedObjectCorrespondsSmSchema) {
  CSmartFactoryTest test_factory;
  // Create empty SmartObject  with schema correspopnding StructId
  SmartObject obj = test_factory.CreateSmartObject(
      StructIdentifiersTest::Common_1);
  // Add fields
  obj["text"] = "test";
  obj["position"] = 200;
  obj["image"]["text"] = "test2";
  obj["image"]["position"] = 100;
  // Check object "image"
  EXPECT_EQ(Errors::eType::OK, obj["image"].validate());
  std::set<std::string> keys = obj.enumerate();
  EXPECT_EQ(3u, keys.size());
  // Check global object
  EXPECT_EQ(Errors::eType::OK, obj.validate());
  EXPECT_TRUE(obj.isValid());
}

TEST(CSmartFactoryTest, CreateSmartObj_WithSchemaFromStructId_MissedOneField_ExpectCreatedObjectNotCorrespondsSmSchema) {
  CSmartFactoryTest test_factory;
  // Create empty SmartObject  with schema correspopnding StructId
  SmartObject obj = test_factory.CreateSmartObject(
      StructIdentifiersTest::Common_1);
  // Add fields. One missed.
  obj["text"] = "test";
  obj["image"]["text"] = "test2";
  obj["image"]["position"] = 100;
  // Check object "image"
  EXPECT_EQ(Errors::eType::OK, obj["image"].validate());
  std::set<std::string> keys = obj.enumerate();
  EXPECT_EQ(2u, keys.size());
  // Check global object
  EXPECT_EQ(Errors::eType::MISSING_MANDATORY_PARAMETER, obj.validate());
  EXPECT_FALSE(obj.isValid());
}

TEST(CSmartFactoryTest, CreateSmartObj_WithSchemaFromStructId2_ExpectCreatedObjectCorrespondsSmSchema) {
  CSmartFactoryTest test_factory;
  // Create empty SmartObject  with schema correspopnding StructId
  SmartObject obj = test_factory.CreateSmartObject(
      StructIdentifiersTest::Common_2);
  // Add fields
  obj["text"] = "test";
  obj["position"] = 200;
  obj["image"]["text"] = "test2";
  obj["image"]["position"] = 100;
  // Check object "image"
  EXPECT_EQ(Errors::eType::OK, obj["image"].validate());
  std::set<std::string> keys = obj.enumerate();
  EXPECT_EQ(3u, keys.size());
  // Check global object
  EXPECT_EQ(Errors::eType::OK, obj.validate());
  EXPECT_TRUE(obj.isValid());
}

TEST(CSmartFactoryTest, CreateSmartObj_WithSchemaFromStructId2_MissedOneField_ExpectCreatedObjectNotCorrespondsSmSchema) {
  CSmartFactoryTest test_factory;
  // Create empty SmartObject  with schema correspopnding StructId
  SmartObject obj = test_factory.CreateSmartObject(
      StructIdentifiersTest::Common_2);
  // Add fields. One missed.
  obj["text"] = "test";
  obj["image"]["text"] = "test2";
  std::set<std::string> keys = obj.enumerate();
  EXPECT_EQ(2u, keys.size());
  // Check global object
  EXPECT_EQ(Errors::eType::MISSING_MANDATORY_PARAMETER, obj.validate());
  EXPECT_FALSE(obj.isValid());
}

TEST(CSmartFactoryTest, CreateSmartObj_WithSchemaFromStructId2_InvalidValueAdded_ExpectCreatedObjectNotCorrespondsSmSchema) {
  CSmartFactoryTest test_factory;
  // Create empty SmartObject  with schema correspopnding StructId
  SmartObject obj = test_factory.CreateSmartObject(
      StructIdentifiersTest::Common_2);
  // Add fields. One missed.
  obj["text"] = 111;
  obj["position"] = 200;
  obj["image"]["text"] = "test2";
  std::set<std::string> keys = obj.enumerate();
  EXPECT_EQ(3u, keys.size());
  // Check global object
  EXPECT_EQ(Errors::eType::INVALID_VALUE, obj.validate());
  EXPECT_FALSE(obj.isValid());
}

TEST(CSmartFactoryTest, GetSchemaWithSmartFactory_ExpectReceivedSchema) {
  CSmartFactoryTest test_factory;
  CSmartSchema schema;
  EXPECT_TRUE(
      test_factory.GetSchema(FunctionIdTest::Function1,
                             MessageTypeTest::request, schema));
}

TEST(CSmartFactoryTest, GetNotExistedSchemaWithSmartFactory_ExpectNotReceivedSchema) {
  CSmartFactoryTest test_factory;
  CSmartSchema schema;
  EXPECT_FALSE(
      test_factory.GetSchema(FunctionIdTest::Function1,
                             MessageTypeTest::INVALID_ENUM, schema));
}

TEST(CSmartFactoryTest, GetSchemaWithSmartFactoryWithStructId1_ExpectReceivedSchema) {
  CSmartFactoryTest test_factory;
  CSmartSchema schema;
  EXPECT_TRUE(test_factory.GetSchema(StructIdentifiersTest::Common_1, schema));
}

TEST(CSmartFactoryTest, GetSchemaWithSmartFactoryWithStructId2_ExpectReceivedSchema) {
  CSmartFactoryTest test_factory;
  CSmartSchema schema;
  EXPECT_TRUE(test_factory.GetSchema(StructIdentifiersTest::Common_2, schema));
}

TEST(CSmartFactoryTest, GetNotExistedSchemaWithSmartFactoryWithStructId_ExpectNotReceivedSchema) {
  CSmartFactoryTest test_factory;
  CSmartSchema schema;
  EXPECT_FALSE(
      test_factory.GetSchema(StructIdentifiersTest::INVALID_ENUM, schema));
}

}  // namespace formatters
}  // namespace components
}  // namespace test

