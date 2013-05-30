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
// POSSIBILITY OF SUCH DAMAGE.

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <iostream>
#include "json/json.h"

#include "JSONHandler/formatters/CFormatterJsonSDLRPCv2.hpp"
#include "JSONHandler/CSmartFactory.hpp"

#include "SmartObjects/CArraySchemaItem.hpp"
#include "SmartObjects/CBoolSchemaItem.hpp"
#include "SmartObjects/CObjectSchemaItem.hpp"
#include "SmartObjects/CStringSchemaItem.hpp"
#include "SmartObjects/TEnumSchemaItem.hpp"
#include "SmartObjects/TNumberSchemaItem.hpp"
#include "SmartObjects/TSchemaItemParameter.hpp"

#include "CFormatterTestHelper.hpp"


namespace test { namespace components { namespace JSONHandler { namespace formatters {

namespace function_id {
/**
  * @brief Enumeration FunctionID.
  */
enum Type {
  /**
    * @brief INVALID_ENUM.
    */
  INVALID_ENUM = -1,

  /**
    * @brief RegisterAppInterface.
    */
  kRegisterAppInterface,

  /**
    * @brief UnregisterAppInterface.
    */
  kUnregisterAppInterface,

  /**
    * @brief SetGlobalProperties.
    */
  kSetGlobalProperties,
};
} //namespace FunctionID

namespace message_type {
/**
  * @brief Enumeration messageType.
  */
enum Type {
  /**
    * @brief INVALID_ENUM.
    */
  INVALID_ENUM = -1,

  /**
    * @brief request.
    */
  kRequest,

  /**
    * @brief response.
    */
  kResponse,

  /**
    * @brief notification.
    */
  kNotification
};
} //namespace messageType


  
    using namespace NsSmartDeviceLink::NsJSONHandler::strings;

    typedef NsSmartDeviceLink::NsJSONHandler::Formatters::CFormatterJsonSDLRPCv2 FormatterV2;

    TEST_F(CFormatterTestHelper, test_fromObjToSDLRPCv2AndBack)
    {
        Json::Value value;  // just a quick workaround to avoid undefined reference to Json

        std::string str;
        NsSmartDeviceLink::NsSmartObjects::CSmartObject srcObj;
        NsSmartDeviceLink::NsSmartObjects::CSmartObject dstObj;

        fillTestObject(srcObj);
        srcObj[S_PARAMS][S_PROTOCOL_VERSION] = 2;

        // SmartObjects --> JSON
        NsSmartDeviceLink::NsJSONHandler::Formatters::CFormatterJsonSDLRPCv2::toString(srcObj, str);

        //std::cout << str << std::endl;

        // JSON --> SmartObjects
        NsSmartDeviceLink::NsJSONHandler::Formatters::CFormatterJsonSDLRPCv2::
            fromString<std::string, std::string>(str, dstObj, "some function", "request", 12);


        compareObjects(srcObj, dstObj);
    }

    TEST_F(CFormatterTestHelper, test_fromJsonSDLRPCv2)
    {
        std::string str = "\
        {\
            \"appID\": \"APP ID\",\
            \"appName\": \"APP NAME\",\
            \"appType\": [\"SYSTEM\", \"COMMUNICATION\"],\
            \"hmiDisplayLanguageDesired\": \"RU-RU\",\
            \"isMediaApplication\": true,\
            \"languageDesired\": \"EN-US\",\
            \"ngnMediaScreenAppName\": \"SCREEN NAME\",\
            \"syncMsgVersion\": {\
                \"majorVersion\": 2,\
                \"minorVersion\": 10\
            },\
            \"ttsName\": [{\
                \"text\": \"ABC\",\
                \"type\": \"TEXT\"\
            }],\
            \"vrSynonyms\": [\"Synonym 1\", \"Synonym 2\"]\
        }";

        NsSmartDeviceLink::NsSmartObjects::CSmartObject obj;
        bool result;

        result = NsSmartDeviceLink::NsJSONHandler::Formatters::CFormatterJsonSDLRPCv2::
                fromString<std::string, std::string>(str, obj, "some name", "request", 12);

        ASSERT_TRUE(result) << "Error parsing JSON string";

        ASSERT_EQ("some name", (std::string)obj[S_PARAMS][S_FUNCTION_ID]);
        ASSERT_EQ(12, (int)obj[S_PARAMS][S_CORRELATION_ID]);
        ASSERT_EQ(2, (int)obj[S_PARAMS][S_PROTOCOL_VERSION]);
        ASSERT_EQ(10, (int)obj[S_MSG_PARAMS]["syncMsgVersion"]["minorVersion"]);
        ASSERT_EQ("TEXT", (std::string)obj[S_MSG_PARAMS]["ttsName"][0]["type"]);
        ASSERT_EQ("Synonym 2", (std::string)obj[S_MSG_PARAMS]["vrSynonyms"][1]);
    }

    TEST_F(CFormatterTestHelper, test_SDLRPCv2_EmptyMapArrayTest) {
      using namespace NsSmartDeviceLink::NsSmartObjects;

      CSmartObject srcObj, dstObj;
      std::string str;

      fillTestObject(srcObj);
      srcObj[S_PARAMS][S_PROTOCOL_VERSION] = 2;
      srcObj[S_PARAMS][S_MESSAGE_TYPE] = 0;
      srcObj[S_PARAMS][S_CORRELATION_ID] = 0;
      srcObj[S_PARAMS][S_FUNCTION_ID] = 0;
      srcObj[S_MSG_PARAMS]["EmptyArray"] = CSmartObject(SmartType_Array);
      srcObj[S_MSG_PARAMS]["EmptyMap"] = CSmartObject(SmartType_Map);
      srcObj[S_MSG_PARAMS]["AnotherEmptyArray"] = srcObj[S_MSG_PARAMS]["EmptyArray"];
      srcObj[S_MSG_PARAMS]["AnotherEmptyMap"]  =  srcObj[S_MSG_PARAMS]["EmptyMap"];

      // SmartObjects --> JSON
      FormatterV2::toString(srcObj, str);

      // JSON --> SmartObjects
      FormatterV2::fromString<int, int>(str, dstObj, 0, 0, 0);

      compareObjects(srcObj, dstObj);
    }

namespace so = NsSmartDeviceLink::NsSmartObjects;
namespace mf = NsSmartDeviceLink::NsJSONHandler::Formatters::
    meta_formatter_error_code;

TEST_F(CFormatterTestHelper, test_SDLRPCv2_MetaFormatToString_Empty) {
  std::string result;
  FormatterV2::tMetaFormatterErrorCode error_code;

  so::CSmartObject empty_object;
  so::CSmartSchema empty_schema;

  error_code = FormatterV2::MetaFormatToString(empty_object,
                                               empty_schema,
                                               result);

  std::string expected_result("\"\"\n");
  
  ASSERT_EQ(expected_result, result) <<
      "Unexpected result string";

  ASSERT_FALSE(mf::kErrorOk & error_code) <<
      "Result must not be OK in such case";

  ASSERT_TRUE(mf::kErrorObjectIsNotFunction & error_code) <<
      "Object must be not be detected as function";

  ASSERT_TRUE(mf::kErrorSchemaIsNotFunction & error_code) <<
      "Target schema does not define a function";

  ASSERT_FALSE(mf::kErrorFailedCreateObjectBySchema & error_code) <<
      "This creation shold not fail because of valid empty schema";

  so::CSmartObject object(so::SmartType_Map);

  object["1"] = 1;
  object["2"] = "two";
  object["3"] = false;

  error_code = FormatterV2::MetaFormatToString(object,
                                               empty_schema,
                                               result);

  ASSERT_EQ(expected_result, result) <<
      "Unexpected result string";

  ASSERT_FALSE(mf::kErrorOk & error_code) <<
      "Result must not be OK in such case";

  ASSERT_TRUE(mf::kErrorObjectIsNotFunction & error_code) <<
      "Object must be not be detected as function";

  ASSERT_TRUE(mf::kErrorSchemaIsNotFunction & error_code) <<
      "Target schema does not define a function";

  ASSERT_FALSE(mf::kErrorFailedCreateObjectBySchema & error_code) <<
      "This creation shold not fail because of valid empty schema";

  so::CSmartObject function_object(so::SmartType_Map);

  function_object[S_PARAMS][S_FUNCTION_ID] =
      function_id::kUnregisterAppInterface;
  function_object[S_PARAMS][S_MESSAGE_TYPE] = message_type::kRequest;
  function_object[S_MSG_PARAMS]["Some data"] = "Some data";

  error_code = FormatterV2::MetaFormatToString(function_object,
                                               empty_schema,
                                               result);

  ASSERT_EQ(expected_result, result) <<
      "Unexpected result string";

  ASSERT_FALSE(mf::kErrorOk & error_code) <<
      "Result must not be OK in such case";

  ASSERT_FALSE(mf::kErrorObjectIsNotFunction & error_code) <<
      "Object must be detected as function";

  ASSERT_TRUE(mf::kErrorSchemaIsNotFunction & error_code) <<
      "Target schema does not define a function";

  ASSERT_FALSE(mf::kErrorFailedCreateObjectBySchema & error_code) <<
      "This creation shold not fail because of valid empty schema";

  std::map<std::string, so::CObjectSchemaItem::SMember>
      non_function_schema_members_map;

  non_function_schema_members_map["StringField"] =
      so::CObjectSchemaItem::SMember(
          so::CStringSchemaItem::create(
              so::TSchemaItemParameter<size_t>(1000),
              so::TSchemaItemParameter<std::string>()),
          false);
  non_function_schema_members_map["IntField"] =
      so::CObjectSchemaItem::SMember(so::TNumberSchemaItem<int>::create(
          1, 20, 10), true);
  non_function_schema_members_map["BoolField"] =
      so::CObjectSchemaItem::SMember(
          so::CBoolSchemaItem::create(
              so::TSchemaItemParameter<bool>()),
          true);

  so::CSmartSchema non_function_schema(so::CObjectSchemaItem::create(
      non_function_schema_members_map));

  error_code = FormatterV2::MetaFormatToString(empty_object,
                                               non_function_schema,
                                               result);

  ASSERT_EQ(expected_result, result) <<
      "Unexpected result string";

  ASSERT_FALSE(mf::kErrorOk & error_code) <<
      "Result must not be OK in such case";

  ASSERT_TRUE(mf::kErrorObjectIsNotFunction & error_code) <<
      "Object must not be detected as function";

  ASSERT_TRUE(mf::kErrorSchemaIsNotFunction & error_code) <<
      "Target schema does not define a function";

  ASSERT_FALSE(mf::kErrorFailedCreateObjectBySchema & error_code) <<
      "This creation shold not fail because of valid empty schema";
}

TEST_F(CFormatterTestHelper, test_SDLRPCv2_MetaFormatToString_Valid) {
  std::string result;
  FormatterV2::tMetaFormatterErrorCode error_code;

  so::CSmartObject empty_object;

  std::set<function_id::Type> function_id_items;
  function_id_items.insert(function_id::kRegisterAppInterface);
  function_id_items.insert(function_id::kUnregisterAppInterface);
  function_id_items.insert(function_id::kSetGlobalProperties);

  std::set<message_type::Type> message_type_items;
  message_type_items.insert(message_type::kRequest);
  message_type_items.insert(message_type::kResponse);
  message_type_items.insert(message_type::kNotification);

  std::map<std::string, so::CObjectSchemaItem::SMember>
      test_struct_members;

  test_struct_members["mandatory_int_field"] =
      so::CObjectSchemaItem::SMember(so::TNumberSchemaItem<int>::create(
          1, 20, 15), true);

  test_struct_members["mandatory_string_field"] =
      so::CObjectSchemaItem::SMember(
          so::CStringSchemaItem::create(
              500,
              std::string("Mandatory text")),
          true);

  test_struct_members["non_mandatory_string_field"] =
      so::CObjectSchemaItem::SMember(
          so::CStringSchemaItem::create(
              500,
              std::string("Non-mandatory text")),
          false);

  std::map<std::string, so::CObjectSchemaItem::SMember>
      test_non_mandatory_struct_members;

  test_non_mandatory_struct_members["non_mandatory_int_field"] =
  so::CObjectSchemaItem::SMember(so::TNumberSchemaItem<int>::create(
      1, 20, 15), false);

  std::map<std::string, so::CObjectSchemaItem::SMember> schema_members;

  schema_members["mandatory_auto_default_string"] =
      so::CObjectSchemaItem::SMember(so::CStringSchemaItem::create(100),
                                      true);

  schema_members["non_mandatory_auto_default_string"] =
      so::CObjectSchemaItem::SMember(so::CStringSchemaItem::create(100),
                                      false);

  schema_members["mandatory_manual_default_string"] =
      so::CObjectSchemaItem::SMember(
          so::CStringSchemaItem::create(500,
                                        std::string("String")),
          true);

  schema_members["non_mandatory_manual_default_string"] =
      so::CObjectSchemaItem::SMember(
          so::CStringSchemaItem::create(500,
                                        std::string("String")),
          false);

  schema_members["mandatory_auto_default_int"] =
      so::CObjectSchemaItem::SMember(so::TNumberSchemaItem<int>::create(
          1, 200), true);

  schema_members["mandatory_manual_default_int"] =
      so::CObjectSchemaItem::SMember(so::TNumberSchemaItem<int>::create(
          1, 20, 10), true);

  schema_members["non_mandatory_manual_default_int"] =
      so::CObjectSchemaItem::SMember(so::TNumberSchemaItem<int>::create(
          1, 20, 10), false);

  schema_members["non_mandatory_auto_default_int"] =
      so::CObjectSchemaItem::SMember(so::TNumberSchemaItem<int>::create(
          1, 200), false);

  schema_members["non_mandatory_auto_default_float"] =
      so::CObjectSchemaItem::SMember(so::TNumberSchemaItem<double>::create(
          0.0, 100.0), false);

  schema_members["non_mandatory_manual_default_float"] =
      so::CObjectSchemaItem::SMember(so::TNumberSchemaItem<double>::create(
          0.0, 100.0, 33.33), false);

  schema_members["mandatory_auto_default_float"] =
      so::CObjectSchemaItem::SMember(so::TNumberSchemaItem<double>::create(
          0.0, 100.0), false);

  schema_members["mandatory_manual_default_float"] =
      so::CObjectSchemaItem::SMember(so::TNumberSchemaItem<double>::create(
          0.0, 100.0, 33.33), false);

  schema_members["non_mandatory_auto_default_bool"] =
      so::CObjectSchemaItem::SMember(so::CBoolSchemaItem::create(), false);

  schema_members["non_mandatory_manual_default_bool"] =
      so::CObjectSchemaItem::SMember(so::CBoolSchemaItem::create(true),
                                      false);

  schema_members["mandatory_auto_default_bool"] =
      so::CObjectSchemaItem::SMember(so::CBoolSchemaItem::create(), true);

  schema_members["mandatory_manual_default_bool"] =
      so::CObjectSchemaItem::SMember(so::CBoolSchemaItem::create(true),
                                      true);

  schema_members["mandatory_auto_default_enum"] =
      so::CObjectSchemaItem::SMember(
          so::TEnumSchemaItem<message_type::Type>::create(
              message_type_items),
          true);

  schema_members["mandatory_manual_default_enum"] =
      so::CObjectSchemaItem::SMember(
          so::TEnumSchemaItem<message_type::Type>::create(
              message_type_items,
              message_type::kRequest),
          true);

  schema_members["non_mandatory_auto_default_enum"] =
      so::CObjectSchemaItem::SMember(
          so::TEnumSchemaItem<message_type::Type>::create(
              message_type_items),
          false);

  schema_members["non_mandatory_manual_default_enum"] =
      so::CObjectSchemaItem::SMember(
          so::TEnumSchemaItem<message_type::Type>::create(
              message_type_items,
              message_type::kRequest),
          false);

  schema_members["non_mandatory_struct"] =
      so::CObjectSchemaItem::SMember(so::CObjectSchemaItem::create(
          test_struct_members), false);

  schema_members["mandatory_struct"] =
      so::CObjectSchemaItem::SMember(so::CObjectSchemaItem::create(
          test_struct_members), true);

  schema_members["mandatory_struct_nm"] =
      so::CObjectSchemaItem::SMember(so::CObjectSchemaItem::create(
          test_non_mandatory_struct_members), true);

  schema_members["non_mandatory_struct_nm"] =
      so::CObjectSchemaItem::SMember(so::CObjectSchemaItem::create(
          test_non_mandatory_struct_members), false);

  schema_members["non_mandatory_non_empty_array"] =
      so::CObjectSchemaItem::SMember(so::CArraySchemaItem::create(
          so::TNumberSchemaItem<int>::create()), false);

  schema_members["mandatory_empty_array"] =
      so::CObjectSchemaItem::SMember(so::CArraySchemaItem::create(
          so::TNumberSchemaItem<int>::create(0, 0, 0)), true);

  schema_members["mandatory_empty_map"] =
      so::CObjectSchemaItem::SMember(so::CObjectSchemaItem::create(
          std::map<std::string, so::CObjectSchemaItem::SMember>()), true);

  schema_members["non_mandatory_empty_map"] =
      so::CObjectSchemaItem::SMember(so::CObjectSchemaItem::create(
          std::map<std::string, so::CObjectSchemaItem::SMember>()), false);

  std::map<std::string, so::CObjectSchemaItem::SMember>
      function_params_members;

  function_params_members[S_FUNCTION_ID] =
      so::CObjectSchemaItem::SMember(
          so::TEnumSchemaItem<function_id::Type>::create(function_id_items),
          true);
  function_params_members[S_MESSAGE_TYPE] =
      so::CObjectSchemaItem::SMember(
          so::TEnumSchemaItem<message_type::Type>::create(
              message_type_items), true);
  function_params_members[S_CORRELATION_ID] =
      so::CObjectSchemaItem::SMember(
          so::TNumberSchemaItem<int>::create(), true);
  function_params_members[S_PROTOCOL_VERSION] =
      so::CObjectSchemaItem::SMember(
          so::TNumberSchemaItem<int>::create(), true);
  function_params_members[S_PROTOCOL_TYPE] =
      so::CObjectSchemaItem::SMember(
          so::TNumberSchemaItem<int>::create(), true);

  std::map<std::string, so::CObjectSchemaItem::SMember>
      functiom_root_members;

  functiom_root_members[S_MSG_PARAMS] =
      so::CObjectSchemaItem::SMember(so::CObjectSchemaItem::create(
          schema_members), true);
  functiom_root_members[S_PARAMS] =
      so::CObjectSchemaItem::SMember(so::CObjectSchemaItem::create(
          function_params_members), true);

  so::CSmartSchema function_schema(
      so::CObjectSchemaItem::create(functiom_root_members));

  std::string expected_result1(
      "{\n"
      "   \"mandatory_auto_default_bool\" : false,\n"
      "   \"mandatory_auto_default_enum\" : -1,\n"
      "   \"mandatory_auto_default_int\" : 0,\n"
      "   \"mandatory_auto_default_string\" : \"\",\n"
      "   \"mandatory_empty_array\" : [],\n"
      "   \"mandatory_empty_map\" : {},\n"
      "   \"mandatory_manual_default_bool\" : true,\n"
      "   \"mandatory_manual_default_enum\" : \"request\",\n"
      "   \"mandatory_manual_default_int\" : 10,\n"
      "   \"mandatory_manual_default_string\" : \"String\",\n"
      "   \"mandatory_struct\" : {\n"
      "      \"mandatory_int_field\" : 15,\n"
      "      \"mandatory_string_field\" : \"Mandatory text\"\n"
      "   },\n"
      "   \"mandatory_struct_nm\" : {}\n"
      "}\n");

  error_code = FormatterV2::MetaFormatToString(empty_object,
                                               function_schema,
                                               result);
  
  ASSERT_EQ(expected_result1, result) <<
      "Invalid result JSON string";

  ASSERT_FALSE(mf::kErrorOk & error_code) <<
      "Result must not be OK in such case";

  ASSERT_TRUE(mf::kErrorObjectIsNotFunction & error_code) <<
      "Object must not be detected as function";

  ASSERT_FALSE(mf::kErrorSchemaIsNotFunction & error_code) <<
      "Target schema defines a function";

  ASSERT_FALSE(mf::kErrorFailedCreateObjectBySchema & error_code) <<
      "This creation shold not fail because of valid schema";

  so::CSmartObject function_object(so::SmartType_Map);

  function_object[S_PARAMS][S_MESSAGE_TYPE] = message_type::kResponse;
  function_object[S_PARAMS][S_FUNCTION_ID] =
      function_id::kRegisterAppInterface;
  function_object[S_PARAMS][S_PROTOCOL_VERSION] = 13;

  error_code = FormatterV2::MetaFormatToString(function_object,
                                               function_schema,
                                               result);

  ASSERT_EQ(expected_result1, result) <<
      "Invalid result JSON string";

  ASSERT_FALSE(mf::kErrorOk & error_code) <<
      "Result must not be OK in such case";

  ASSERT_TRUE(mf::kErrorObjectIsNotFunction & error_code) <<
      "Object must not be detected as function";

  ASSERT_FALSE(mf::kErrorSchemaIsNotFunction & error_code) <<
      "Target schema defines a function";

  ASSERT_FALSE(mf::kErrorFailedCreateObjectBySchema & error_code) <<
      "This creation shold not fail because of valid schema";

  function_object[S_MSG_PARAMS]["non_schema_int_field"] = 13;
  function_object[S_MSG_PARAMS]["non_schema_string_field"] = "xxx";
  function_object[S_PARAMS]["Noise"] = "bzzzz";

  error_code = FormatterV2::MetaFormatToString(function_object,
                                               function_schema,
                                               result);

  ASSERT_EQ(expected_result1, result) <<
      "Invalid result JSON string";

  ASSERT_FALSE(mf::kErrorOk & error_code) <<
      "Result must not be OK in such case";

  ASSERT_FALSE(mf::kErrorObjectIsNotFunction & error_code) <<
      "Object must be detected as function";

  ASSERT_FALSE(mf::kErrorSchemaIsNotFunction & error_code) <<
      "Target schema defines a function";

  ASSERT_FALSE(mf::kErrorFailedCreateObjectBySchema & error_code) <<
      "This creation shold not fail because of valid schema";

  function_object[S_MSG_PARAMS]["mandatory_auto_default_int"] = 25;
  function_object[S_MSG_PARAMS]["non_mandatory_auto_default_int"] = 100;
  function_object[S_MSG_PARAMS]["non_mandatory_struct_nm"]
      ["non_mandatory_int_field"] = 3;
  function_object[S_MSG_PARAMS]["mandatory_empty_map"]["x"] = 0;
  function_object[S_MSG_PARAMS]["mandatory_empty_map"]["y"] = 0;
  function_object[S_MSG_PARAMS]["non_mandatory_non_empty_array"][0] = 1;
  function_object[S_MSG_PARAMS]["non_mandatory_non_empty_array"][1] = 2;
  function_object[S_MSG_PARAMS]["non_mandatory_non_empty_array"][2] = 3;
  function_object[S_MSG_PARAMS]["mandatory_empty_array"][0] = 0;
  function_object[S_MSG_PARAMS]["mandatory_empty_array"][1] = 1;
  function_object[S_MSG_PARAMS]["mandatory_empty_array"][2] = 2;

  std::string expected_result2(
      "{\n"
      "   \"mandatory_auto_default_bool\" : false,\n"
      "   \"mandatory_auto_default_enum\" : -1,\n"
      "   \"mandatory_auto_default_int\" : 25,\n"
      "   \"mandatory_auto_default_string\" : \"\",\n"
      "   \"mandatory_empty_array\" : [ 0, 1, 2 ],\n"
      "   \"mandatory_empty_map\" : {},\n"
      "   \"mandatory_manual_default_bool\" : true,\n"
      "   \"mandatory_manual_default_enum\" : \"request\",\n"
      "   \"mandatory_manual_default_int\" : 10,\n"
      "   \"mandatory_manual_default_string\" : \"String\",\n"
      "   \"mandatory_struct\" : {\n"
      "      \"mandatory_int_field\" : 15,\n"
      "      \"mandatory_string_field\" : \"Mandatory text\"\n"
      "   },\n"
      "   \"mandatory_struct_nm\" : {},\n"
      "   \"non_mandatory_auto_default_int\" : 100,\n"
      "   \"non_mandatory_non_empty_array\" : [ 1, 2, 3 ],\n"
      "   \"non_mandatory_struct_nm\" : {\n"
      "      \"non_mandatory_int_field\" : 3\n"
      "   }\n"
      "}\n");

  error_code = FormatterV2::MetaFormatToString(function_object,
                                               function_schema,
                                               result);

  ASSERT_EQ(expected_result2, result) <<
      "Invalid result JSON string";

  ASSERT_FALSE(mf::kErrorOk & error_code) <<
      "Result must not be OK in such case";

  ASSERT_FALSE(mf::kErrorObjectIsNotFunction & error_code) <<
      "Object must be detected as function";

  ASSERT_FALSE(mf::kErrorSchemaIsNotFunction & error_code) <<
      "Target schema defines a function";

  ASSERT_FALSE(mf::kErrorFailedCreateObjectBySchema & error_code) <<
      "This creation shold not fail because of valid schema";
}
}}}}

namespace NsSmartDeviceLink {
namespace NsSmartObjects {

namespace func_id = test::components::JSONHandler::formatters::function_id;

template <>
const std::map<func_id::Type, std::string> &
TEnumSchemaItem<func_id::Type>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<func_id::Type, std::string> enum_string_representation;

  if (!is_initialized) {
    enum_string_representation.insert(std::make_pair(
        func_id::kRegisterAppInterface, "RegisterAppInterface"));
    enum_string_representation.insert(std::make_pair(
        func_id::kUnregisterAppInterface, "UnregisterAppInterface"));
    enum_string_representation.insert(std::make_pair(
        func_id::kSetGlobalProperties, "SetGlobalProperties"));

    is_initialized = true;
  }

  return enum_string_representation;
}

namespace msg_type = test::components::JSONHandler::formatters::message_type;

template <>
const std::map<msg_type::Type, std::string> &
TEnumSchemaItem<msg_type::Type>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<msg_type::Type, std::string> enum_string_representation;

  if (!is_initialized) {
    enum_string_representation.insert(std::make_pair(
        msg_type::kRequest, "request"));
    enum_string_representation.insert(std::make_pair(
        msg_type::kResponse, "response"));
    enum_string_representation.insert(std::make_pair(
        msg_type::kNotification, "notification"));

    is_initialized = true;
  }

  return enum_string_representation;
}
} // namespace NsSmartObjects
} // namespace NsSmartDeviceLink

int main(int argc, char **argv)
{
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}

