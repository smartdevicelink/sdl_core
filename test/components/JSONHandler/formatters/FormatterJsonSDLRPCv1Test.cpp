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

#include "JSONHandler/formatters/CFormatterJsonSDLRPCv1.hpp"
#include "json/json.h"
#include "JSONHandler/CSmartFactory.hpp"

#include "CFormatterTestHelper.hpp"

namespace test { namespace components { namespace JSONHandler { namespace Formatters {

    // The code below is cut and paste from the generated code just to provide necessary environment for string <-> enum conversion.
    namespace FunctionID
    {
          /**
           * @brief Enumeration FunctionID.
           */
          enum eType
          {
              /**
               * @brief INVALID_ENUM.
               */
              INVALID_ENUM = -1,

              /**
               * @brief RegisterAppInterface.
               */
              RegisterAppInterface,

              /**
               * @brief UnregisterAppInterface.
               */
              UnregisterAppInterface,

              /**
               * @brief SetGlobalProperties.
               */
              SetGlobalProperties,

              // ...
          };
      }

      namespace messageType
      {
          /**
           * @brief Enumeration messageType.
           */
          enum eType
          {
              /**
               * @brief INVALID_ENUM.
               */
              INVALID_ENUM = -1,

              /**
               * @brief request.
               */
              request,

              /**
               * @brief response.
               */
              response,

              /**
               * @brief notification.
               */
              notification
          };
     }
    // end of cut and pasted code

    using namespace NsSmartDeviceLink::NsJSONHandler::strings;
    typedef NsSmartDeviceLink::NsJSONHandler::Formatters::CFormatterJsonSDLRPCv1 FormatterV1;

    TEST_F(CFormatterTestHelper, test_fromObjToSDLRPCv1AndBack)
    {
        Json::Value value;  // just a quick workaround to avoid undefined reference to Json
        Json::Reader reader;    // the same thing

        std::string str;
        NsSmartDeviceLink::NsSmartObjects::CSmartObject srcObj;
        NsSmartDeviceLink::NsSmartObjects::CSmartObject dstObj;

        fillTestObject(srcObj);
        srcObj[S_PARAMS][S_FUNCTION_ID] = "UnregisterAppInterface"; // a hack, it shouldn't be a string in the SmartObject
        srcObj[S_PARAMS][S_MESSAGE_TYPE] = "request";               // the same thing
        srcObj[S_PARAMS][S_PROTOCOL_VERSION] = 1;

        // SmartObjects --> JSON
        FormatterV1::toString(srcObj, str);

        //std::cout << str << std::endl;

        // JSON --> SmartObjects
        int result = FormatterV1::fromString<FunctionID::eType, messageType::eType>(str, dstObj);

        ASSERT_EQ(FormatterV1::kSuccess, result) << "Error parsing JSON string";

        srcObj[S_PARAMS][S_FUNCTION_ID] = FunctionID::UnregisterAppInterface;   // write the proper enum values
        srcObj[S_PARAMS][S_MESSAGE_TYPE] = messageType::request;

        compareObjects(srcObj, dstObj);
    }

    TEST_F(CFormatterTestHelper, test_fromJsonSDLRPCv1)
    {
        std::string str = "\
        {\
            \"request\": {\
                \"name\" : \"SetGlobalProperties\",\
                \"correlationID\": 11,\
                \"parameters\": {\
                    \"syncMsgVersion\": \"version\",\
                    \"appName\": \"some app name\",\
                    \"ttsName\": [{\
                        \"text\": \"ABC\",\
                        \"type\": \"TEXT\"\
                    }],\
                   \"vrSynonyms\": [\"Synonym 1\", \"Synonym 2\"]\
                }\
            }\
        }";

        NsSmartDeviceLink::NsSmartObjects::CSmartObject obj;
        int result = FormatterV1::fromString<FunctionID::eType, messageType::eType>(str, obj);

        ASSERT_EQ(FormatterV1::kSuccess, result) << "Error parsing JSON string";

        ASSERT_EQ(FunctionID::SetGlobalProperties, (int)obj[S_PARAMS][S_FUNCTION_ID]);
        ASSERT_EQ(messageType::request, (int)obj[S_PARAMS][S_MESSAGE_TYPE]);
        ASSERT_EQ(11, (int)obj[S_PARAMS][S_CORRELATION_ID]);
        ASSERT_EQ("version", (std::string)obj[S_MSG_PARAMS]["syncMsgVersion"]);
        ASSERT_EQ("some app name", (std::string)obj[S_MSG_PARAMS]["appName"]);
        ASSERT_EQ("some app name", (std::string)obj[S_MSG_PARAMS]["appName"]);
        ASSERT_EQ(1, (int)obj[S_PARAMS][S_PROTOCOL_VERSION]);
        ASSERT_EQ("TEXT", (std::string)obj[S_MSG_PARAMS]["ttsName"][0]["type"]);
        ASSERT_EQ("Synonym 2", (std::string)obj[S_MSG_PARAMS]["vrSynonyms"][1]);
    }

  TEST_F(CFormatterTestHelper, test_SDLRPCv1_ErrorParsing)
  {
    std::string str = "\
    {\
        \"request\": {\
            \"name\" : \"SetGlobalProperties\",\
            \"correlationID\": 11,\
            \"parameters\": {\
                \"appName\": \"some app name\
            }\
        }\
    }";

    NsSmartDeviceLink::NsSmartObjects::CSmartObject obj;
    int result = FormatterV1::fromString<FunctionID::eType, messageType::eType>(str, obj);

    ASSERT_TRUE(FormatterV1::kParsingError & result) << "Wrong error code";
    ASSERT_TRUE(FormatterV1::kMessageTypeNotFound & result) << "MessageType error code is not set";
    ASSERT_TRUE(FormatterV1::kFunctionIdNotFound & result) << "FunctionID error code is not set";
    ASSERT_TRUE(FormatterV1::kCorrelationIdNotFound & result) << "CorrelationID error code is not set";
  }

  TEST_F(CFormatterTestHelper, test_SDLRPCv1_MessageTypeError)
  {
    std::string str1 = "\
    {\
        \"information\": {\
            \"name\" : \"SetGlobalProperties\",\
            \"correlationID\": 11,\
            \"parameters\": {\
                \"appName\": \"some app name\"\
            }\
        }\
    }";

    NsSmartDeviceLink::NsSmartObjects::CSmartObject obj1;
    int result = FormatterV1::fromString<FunctionID::eType, messageType::eType>(str1, obj1);

    ASSERT_FALSE(FormatterV1::kParsingError & result) << "Wrong error code";
    ASSERT_TRUE(FormatterV1::kMessageTypeNotFound & result) << "MessageType error code is not set";
    ASSERT_TRUE(FormatterV1::kFunctionIdNotFound & result) << "FunctionID error code is not set";
    ASSERT_TRUE(FormatterV1::kCorrelationIdNotFound & result) << "CorrelationID error code is not set";

    std::string str2 = "\
    {\
        \"name\" : \"SetGlobalProperties\",\
        \"correlationID\": 11,\
        \"parameters\": {\
            \"appName\": \"some app name\"\
        }\
    }";

    NsSmartDeviceLink::NsSmartObjects::CSmartObject obj2;
    result = FormatterV1::fromString<FunctionID::eType, messageType::eType>(str1, obj2);

    ASSERT_FALSE(FormatterV1::kParsingError & result) << "Wrong error code";
    ASSERT_TRUE(FormatterV1::kMessageTypeNotFound & result) << "MessageType error code is not set";
    ASSERT_TRUE(FormatterV1::kFunctionIdNotFound & result) << "FunctionID error code is not set";
    ASSERT_TRUE(FormatterV1::kCorrelationIdNotFound & result) << "CorrelationID error code is not set";
  }

  TEST_F(CFormatterTestHelper, test_SDLRPCv1_FunctionID_Error)
  {
    std::string str1 = "\
    {\
        \"request\": {\
            \"not a name\" : \"SetGlobalProperties\",\
            \"correlationID\": 11,\
            \"parameters\": {\
                \"appName\": \"some app name\"\
            }\
        }\
    }";

    NsSmartDeviceLink::NsSmartObjects::CSmartObject obj1;
    int result = FormatterV1::fromString<FunctionID::eType, messageType::eType>(str1, obj1);

    ASSERT_FALSE(FormatterV1::kParsingError & result) << "Wrong error code";
    ASSERT_FALSE(FormatterV1::kMessageTypeNotFound & result) << "MessageType error code is set";
    ASSERT_TRUE(FormatterV1::kFunctionIdNotFound & result) << "FunctionID error code is not set";
    ASSERT_FALSE(FormatterV1::kCorrelationIdNotFound & result) << "CorrelationID error code is set";
    ASSERT_EQ(11, (int)obj1[S_PARAMS][S_CORRELATION_ID]) << "Wrong CorrelationID";
    ASSERT_EQ(FunctionID::INVALID_ENUM, (int)obj1[S_PARAMS][S_FUNCTION_ID]);
    ASSERT_EQ(messageType::request, (int)obj1[S_PARAMS][S_MESSAGE_TYPE]);

    std::string str2 = "\
    {\
        \"request\": {\
            \"name\" : \"UnknownName\",\
            \"correlationID\": 12,\
            \"parameters\": {\
                \"appName\": \"some app name\"\
            }\
        }\
    }";

    NsSmartDeviceLink::NsSmartObjects::CSmartObject obj2;
    result = FormatterV1::fromString<FunctionID::eType, messageType::eType>(str2, obj2);

    ASSERT_FALSE(FormatterV1::kParsingError & result) << "Wrong error code";
    ASSERT_FALSE(FormatterV1::kMessageTypeNotFound & result) << "MessageType error code is not set";
    ASSERT_TRUE(FormatterV1::kFunctionIdNotFound & result) << "FunctionID error code is not set";
    ASSERT_FALSE(FormatterV1::kCorrelationIdNotFound & result) << "CorrelationID error code is set";
    ASSERT_EQ(12, (int)obj2[S_PARAMS][S_CORRELATION_ID]) << "Wrong CorrelationID";
    ASSERT_EQ(FunctionID::INVALID_ENUM, (int)obj2[S_PARAMS][S_FUNCTION_ID]);
    ASSERT_EQ(messageType::request, (int)obj2[S_PARAMS][S_MESSAGE_TYPE]);
  }

  TEST_F(CFormatterTestHelper, test_SDLRPCv1_CorrelationID_Error)
  {
    std::string str1 = "\
    {\
        \"request\": {\
            \"name\" : \"SetGlobalProperties\",\
            \"NotACorrelationID\": 13,\
            \"parameters\": {\
                \"appName\": \"some app name\"\
            }\
        }\
    }";

    NsSmartDeviceLink::NsSmartObjects::CSmartObject obj1;
    int result = FormatterV1::fromString<FunctionID::eType, messageType::eType>(str1, obj1);

    ASSERT_FALSE(FormatterV1::kParsingError & result) << "Wrong error code";
    ASSERT_FALSE(FormatterV1::kMessageTypeNotFound & result) << "MessageType error code is set";
    ASSERT_FALSE(FormatterV1::kFunctionIdNotFound & result) << "FunctionID error code is set";
    ASSERT_TRUE(FormatterV1::kCorrelationIdNotFound & result) << "CorrelationID error code is not set";
    ASSERT_EQ(-1, (int)obj1[S_PARAMS][S_CORRELATION_ID]) << "Wrong CorrelationID";
    ASSERT_EQ(FunctionID::SetGlobalProperties, (int)obj1[S_PARAMS][S_FUNCTION_ID]);
    ASSERT_EQ(messageType::request, (int)obj1[S_PARAMS][S_MESSAGE_TYPE]);
  }

  TEST_F(CFormatterTestHelper, test_SDLRPCv1_CombinationError)
  {
    std::string str1 = "\
    {\
        \"response\": {\
            \"name\" : \"UnKnownName\",\
            \"NotACorrelationID\": 10,\
            \"parameters\": {\
                \"appName\": \"some app name\"\
            }\
        }\
    }";

    NsSmartDeviceLink::NsSmartObjects::CSmartObject obj1;
    int result = FormatterV1::fromString<FunctionID::eType, messageType::eType>(str1, obj1);

    ASSERT_FALSE(FormatterV1::kParsingError & result) << "Wrong error code";
    ASSERT_FALSE(FormatterV1::kMessageTypeNotFound & result) << "MessageType error code is set";
    ASSERT_TRUE(FormatterV1::kFunctionIdNotFound & result) << "FunctionID error code is not set";
    ASSERT_TRUE(FormatterV1::kCorrelationIdNotFound & result) << "CorrelationID error code is not set";
    ASSERT_EQ(-1, (int)obj1[S_PARAMS][S_CORRELATION_ID]) << "Wrong CorrelationID";
    ASSERT_EQ(FunctionID::INVALID_ENUM, (int)obj1[S_PARAMS][S_FUNCTION_ID]);
    ASSERT_EQ(messageType::response, (int)obj1[S_PARAMS][S_MESSAGE_TYPE]);
  }

  TEST_F(CFormatterTestHelper, test_SDLRPCv1_NotificationCorrelationId)
  {
    std::string str1 = "\
    {\
        \"notification\": {\
            \"name\" : \"SetGlobalProperties\",\
            \"NotACorrelationID\": 10,\
            \"parameters\": {\
                \"appName\": \"some app name\"\
            }\
        }\
    }";

    NsSmartDeviceLink::NsSmartObjects::CSmartObject obj1;
    int result = FormatterV1::fromString<FunctionID::eType, messageType::eType>(str1, obj1);

    ASSERT_FALSE(FormatterV1::kParsingError & result) << "Wrong error code";
    ASSERT_FALSE(FormatterV1::kMessageTypeNotFound & result) << "MessageType error code is set";
    ASSERT_FALSE(FormatterV1::kFunctionIdNotFound & result) << "FunctionID error code is set";
    ASSERT_FALSE(FormatterV1::kCorrelationIdNotFound & result) << "CorrelationID error code is set";
    ASSERT_FALSE(obj1[S_PARAMS].keyExists(S_CORRELATION_ID)) << "CorrelationID field exists in SmartObject";
    ASSERT_EQ(FunctionID::SetGlobalProperties, (int)obj1[S_PARAMS][S_FUNCTION_ID]);
    ASSERT_EQ(messageType::notification, (int)obj1[S_PARAMS][S_MESSAGE_TYPE]);

    std::string str2 = "\
    {\
        \"notification\": {\
            \"name\" : \"UnknownNotification\",\
            \"parameters\": {\
                \"appName\": \"some app name\"\
            }\
        }\
    }";

    NsSmartDeviceLink::NsSmartObjects::CSmartObject obj2;
    result = FormatterV1::fromString<FunctionID::eType, messageType::eType>(str2, obj2);

    ASSERT_FALSE(FormatterV1::kParsingError & result) << "Wrong error code";
    ASSERT_FALSE(FormatterV1::kMessageTypeNotFound & result) << "MessageType error code is set";
    ASSERT_TRUE(FormatterV1::kFunctionIdNotFound & result) << "FunctionID error code is not set";
    ASSERT_FALSE(FormatterV1::kCorrelationIdNotFound & result) << "CorrelationID error code is set";
    ASSERT_FALSE(obj2[S_PARAMS].keyExists(S_CORRELATION_ID)) << "CorrelationID field exists in SmartObject";
    ASSERT_EQ(FunctionID::INVALID_ENUM, (int)obj2[S_PARAMS][S_FUNCTION_ID]);
    ASSERT_EQ(messageType::notification, (int)obj2[S_PARAMS][S_MESSAGE_TYPE]);
  }

  TEST_F(CFormatterTestHelper, test_SDLRPCv1_EmptyMapArrayTest) {
    using namespace NsSmartDeviceLink::NsSmartObjects;

    CSmartObject srcObj, dstObj;
    std::string str;

    fillTestObject(srcObj);
    srcObj[S_PARAMS][S_FUNCTION_ID] = "UnregisterAppInterface"; // a hack, it shouldn't be a string in the SmartObject
    srcObj[S_PARAMS][S_MESSAGE_TYPE] = "request";               // the same thing
    srcObj[S_PARAMS][S_PROTOCOL_VERSION] = 1;
    srcObj[S_MSG_PARAMS]["EmptyArray"] = CSmartObject(SmartType_Array);
    srcObj[S_MSG_PARAMS]["EmptyMap"] = CSmartObject(SmartType_Map);
    srcObj[S_MSG_PARAMS]["AnotherEmptyArray"] = srcObj[S_MSG_PARAMS]["EmptyArray"];
    srcObj[S_MSG_PARAMS]["AnotherEmptyMap"]  =  srcObj[S_MSG_PARAMS]["EmptyMap"];

    // SmartObjects --> JSON
    FormatterV1::toString(srcObj, str);

    std::cout << str << std::endl;

    // JSON --> SmartObjects
    int result = FormatterV1::fromString<FunctionID::eType, messageType::eType>(str, dstObj);

    ASSERT_EQ(FormatterV1::kSuccess, result) << "Error parsing JSON string";

    srcObj[S_PARAMS][S_FUNCTION_ID] = FunctionID::UnregisterAppInterface;   // write the proper enum values
    srcObj[S_PARAMS][S_MESSAGE_TYPE] = messageType::request;

    compareObjects(srcObj, dstObj);
  }

  TEST_F(CFormatterTestHelper, test_SDLRPCv1_NotificationTest) {
    using namespace NsSmartDeviceLink::NsSmartObjects;

    CSmartObject src_obj, dst_obj;
    std::string str;

    src_obj[S_PARAMS][S_FUNCTION_ID] = "SetGlobalProperties"; // a hack, it shouldn't be a string in the SmartObject
    src_obj[S_PARAMS][S_MESSAGE_TYPE] = "notification";               // the same thing
    src_obj[S_PARAMS][S_PROTOCOL_VERSION] = 1;
    src_obj[S_PARAMS][S_PROTOCOL_TYPE] = 0;
    src_obj[S_MSG_PARAMS]["appId"] = "APP ID";
    src_obj[S_MSG_PARAMS]["appName"] = "APP NAME";
    src_obj[S_MSG_PARAMS]["item"] = "value";

    // SmartObjects --> JSON
    FormatterV1::toString(src_obj, str);

    ASSERT_TRUE(str.find("correlation") == std::string::npos) << "CorrelationId is present in JSON string";

    // JSON --> SmartObjects
    int result = FormatterV1::fromString<FunctionID::eType, messageType::eType>(str, dst_obj);

    ASSERT_EQ(FormatterV1::kSuccess, result) << "Error parsing JSON string";

    src_obj[S_PARAMS][S_FUNCTION_ID] = FunctionID::SetGlobalProperties;   // write the proper enum values
    src_obj[S_PARAMS][S_MESSAGE_TYPE] = messageType::notification;

    ASSERT_FALSE(dst_obj[S_PARAMS].keyExists(S_CORRELATION_ID)) << "CorrelationID is present in SmartObject";

    compareObjects(src_obj, dst_obj);
  }

  namespace so = NsSmartDeviceLink::NsSmartObjects;
  namespace mf = NsSmartDeviceLink::NsJSONHandler::Formatters::
      meta_formatter_error_code;
      
  TEST_F(CFormatterTestHelper, test_SDLRPCv1_MetaFormatToString_Empty) {
    std::string result;
    FormatterV1::tMetaFormatterErrorCode error_code;

    so::CSmartObject empty_object;
    so::CSmartSchema empty_schema;

    error_code = FormatterV1::MetaFormatToString(empty_object,
                                                 empty_schema,
                                                 result);

    std::string expected_result(
        "{\n"
        "   \"\" : {\n"
        "      \"name\" : \"\",\n"
        "      \"parameters\" : \"\"\n"
        "   }\n"
        "}\n");

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

    error_code = FormatterV1::MetaFormatToString(object,
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
        FunctionID::UnregisterAppInterface;
    function_object[S_PARAMS][S_MESSAGE_TYPE] = messageType::request;
    function_object[S_MSG_PARAMS]["Some data"] = "Some data";

    error_code = FormatterV1::MetaFormatToString(function_object,
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

    error_code = FormatterV1::MetaFormatToString(empty_object,
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

  TEST_F(CFormatterTestHelper, test_SDLRPCv1_MetaFormatToString_Valid) {
    std::string result;
    FormatterV1::tMetaFormatterErrorCode error_code;
    
    so::CSmartObject empty_object;

    std::set<FunctionID::eType> function_id_items;
    function_id_items.insert(FunctionID::RegisterAppInterface);
    function_id_items.insert(FunctionID::UnregisterAppInterface);
    function_id_items.insert(FunctionID::SetGlobalProperties);

    std::set<messageType::eType> message_type_items;
    message_type_items.insert(messageType::request);
    message_type_items.insert(messageType::response);
    message_type_items.insert(messageType::notification);

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
            so::TEnumSchemaItem<messageType::eType>::create(
                message_type_items),
            true);

    schema_members["mandatory_manual_default_enum"] =
        so::CObjectSchemaItem::SMember(
            so::TEnumSchemaItem<messageType::eType>::create(
                message_type_items,
                messageType::request),
            true);

    schema_members["non_mandatory_auto_default_enum"] =
        so::CObjectSchemaItem::SMember(
            so::TEnumSchemaItem<messageType::eType>::create(
                message_type_items),
            false);

    schema_members["non_mandatory_manual_default_enum"] =
        so::CObjectSchemaItem::SMember(
            so::TEnumSchemaItem<messageType::eType>::create(
                message_type_items,
                messageType::request),
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
            so::TEnumSchemaItem<FunctionID::eType>::create(function_id_items),
            true);
    function_params_members[S_MESSAGE_TYPE] =
        so::CObjectSchemaItem::SMember(
            so::TEnumSchemaItem<messageType::eType>::create(
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
        "   \"-1\" : {\n"
        "      \"correlationID\" : 0,\n"
        "      \"name\" : \"-1\",\n"
        "      \"parameters\" : {\n"
        "         \"mandatory_auto_default_bool\" : false,\n"
        "         \"mandatory_auto_default_enum\" : -1,\n"
        "         \"mandatory_auto_default_int\" : 0,\n"
        "         \"mandatory_auto_default_string\" : \"\",\n"
        "         \"mandatory_empty_array\" : [],\n"
        "         \"mandatory_empty_map\" : {},\n"
        "         \"mandatory_manual_default_bool\" : true,\n"
        "         \"mandatory_manual_default_enum\" : \"request\",\n"
        "         \"mandatory_manual_default_int\" : 10,\n"
        "         \"mandatory_manual_default_string\" : \"String\",\n"
        "         \"mandatory_struct\" : {\n"
        "            \"mandatory_int_field\" : 15,\n"
        "            \"mandatory_string_field\" : \"Mandatory text\"\n"
        "         },\n"
        "         \"mandatory_struct_nm\" : {}\n"        
        "      }\n"
        "   }\n"
        "}\n");
    
    error_code = FormatterV1::MetaFormatToString(empty_object,
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

    function_object[S_PARAMS][S_MESSAGE_TYPE] = messageType::response;
    function_object[S_PARAMS][S_FUNCTION_ID] =
        FunctionID::RegisterAppInterface;
    function_object[S_PARAMS][S_PROTOCOL_VERSION] = 13;

    std::string expected_result2(
        "{\n"
        "   \"response\" : {\n"
        "      \"correlationID\" : 0,\n"
        "      \"name\" : \"RegisterAppInterface\",\n"
        "      \"parameters\" : {\n"
        "         \"mandatory_auto_default_bool\" : false,\n"
        "         \"mandatory_auto_default_enum\" : -1,\n"
        "         \"mandatory_auto_default_int\" : 0,\n"
        "         \"mandatory_auto_default_string\" : \"\",\n"
        "         \"mandatory_empty_array\" : [],\n"
        "         \"mandatory_empty_map\" : {},\n"        
        "         \"mandatory_manual_default_bool\" : true,\n"
        "         \"mandatory_manual_default_enum\" : \"request\",\n"
        "         \"mandatory_manual_default_int\" : 10,\n"
        "         \"mandatory_manual_default_string\" : \"String\",\n"
        "         \"mandatory_struct\" : {\n"
        "            \"mandatory_int_field\" : 15,\n"
        "            \"mandatory_string_field\" : \"Mandatory text\"\n"
        "         },\n"
        "         \"mandatory_struct_nm\" : {}\n"
        "      }\n"
        "   }\n"
        "}\n");
    
    error_code = FormatterV1::MetaFormatToString(function_object,
                                                 function_schema,
                                                 result);

    ASSERT_EQ(expected_result2, result) <<
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

    error_code = FormatterV1::MetaFormatToString(function_object,
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

    std::string expected_result3(
        "{\n"
        "   \"response\" : {\n"
        "      \"correlationID\" : 0,\n"
        "      \"name\" : \"RegisterAppInterface\",\n"
        "      \"parameters\" : {\n"
        "         \"mandatory_auto_default_bool\" : false,\n"
        "         \"mandatory_auto_default_enum\" : -1,\n"
        "         \"mandatory_auto_default_int\" : 25,\n"
        "         \"mandatory_auto_default_string\" : \"\",\n"
        "         \"mandatory_empty_array\" : [ 0, 1, 2 ],\n"
        "         \"mandatory_empty_map\" : {},\n"
        "         \"mandatory_manual_default_bool\" : true,\n"
        "         \"mandatory_manual_default_enum\" : \"request\",\n"
        "         \"mandatory_manual_default_int\" : 10,\n"
        "         \"mandatory_manual_default_string\" : \"String\",\n"
        "         \"mandatory_struct\" : {\n"
        "            \"mandatory_int_field\" : 15,\n"
        "            \"mandatory_string_field\" : \"Mandatory text\"\n"
        "         },\n"
        "         \"mandatory_struct_nm\" : {},\n"
        "         \"non_mandatory_auto_default_int\" : 100,\n"
        "         \"non_mandatory_non_empty_array\" : [ 1, 2, 3 ],\n"
        "         \"non_mandatory_struct_nm\" : {\n"
        "            \"non_mandatory_int_field\" : 3\n"
        "         }\n"
        "      }\n"
        "   }\n"
        "}\n");

    error_code = FormatterV1::MetaFormatToString(function_object,
                                                 function_schema,
                                                 result);

    ASSERT_EQ(expected_result3, result) <<
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

namespace NsSmartDeviceLink { namespace NsSmartObjects {

    template <>
    const std::map<test::components::JSONHandler::Formatters::FunctionID::eType, std::string> &
    NsSmartDeviceLink::NsSmartObjects::TEnumSchemaItem<test::components::JSONHandler::Formatters::FunctionID::eType>::getEnumElementsStringRepresentation(void)
    {
        static bool isInitialized = false;
        static std::map<test::components::JSONHandler::Formatters::FunctionID::eType, std::string> enumStringRepresentationMap;

        if (false == isInitialized)
        {
            enumStringRepresentationMap.insert(std::make_pair(test::components::JSONHandler::Formatters::FunctionID::RegisterAppInterface, "RegisterAppInterface"));
            enumStringRepresentationMap.insert(std::make_pair(test::components::JSONHandler::Formatters::FunctionID::UnregisterAppInterface, "UnregisterAppInterface"));
            enumStringRepresentationMap.insert(std::make_pair(test::components::JSONHandler::Formatters::FunctionID::SetGlobalProperties, "SetGlobalProperties"));

            isInitialized = true;
        }

        return enumStringRepresentationMap;
    }

    template <>
    const std::map<test::components::JSONHandler::Formatters::messageType::eType, std::string> &
    NsSmartDeviceLink::NsSmartObjects::TEnumSchemaItem<test::components::JSONHandler::Formatters::messageType::eType>::getEnumElementsStringRepresentation(void)
    {
        static bool isInitialized = false;
        static std::map<test::components::JSONHandler::Formatters::messageType::eType, std::string> enumStringRepresentationMap;

        if (false == isInitialized)
        {
            enumStringRepresentationMap.insert(std::make_pair(test::components::JSONHandler::Formatters::messageType::request, "request"));
            enumStringRepresentationMap.insert(std::make_pair(test::components::JSONHandler::Formatters::messageType::response, "response"));
            enumStringRepresentationMap.insert(std::make_pair(test::components::JSONHandler::Formatters::messageType::notification, "notification"));

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
