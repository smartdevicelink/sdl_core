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

#ifndef TEST_COMPONENTS_JSON_HANDLER_INCLUDE_JSON_HANDLER_FORMATTERS_FORMATTER_JSON_ALRPCV1_TEST_H_
#define TEST_COMPONENTS_JSON_HANDLER_INCLUDE_JSON_HANDLER_FORMATTERS_FORMATTER_JSON_ALRPCV1_TEST_H_

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <iostream>

#include "JSONHandler/formatters/CFormatterJsonALRPCv1.hpp"
#include "json/json.h"
#include "JSONHandler/CSmartFactory.hpp"

#include "json_handler/formatters/formatter_test_helper.h"

namespace test { namespace components { namespace json_handler { namespace formatters {

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
    typedef NsSmartDeviceLink::NsJSONHandler::Formatters::CFormatterJsonALRPCv1 FormatterV1;

    TEST_F(CFormatterTestHelper, test_fromObjToALRPCv1AndBack)
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

    TEST_F(CFormatterTestHelper, test_fromJsonALRPCv1)
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

  TEST_F(CFormatterTestHelper, test_ALRPCv1_ErrorParsing)
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

  TEST_F(CFormatterTestHelper, test_ALRPCv1_MessageTypeError)
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

  TEST_F(CFormatterTestHelper, test_ALRPCv1_FunctionID_Error)
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

  TEST_F(CFormatterTestHelper, test_ALRPCv1_CorrelationID_Error)
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

  TEST_F(CFormatterTestHelper, test_ALRPCv1_CombinationError)
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

  TEST_F(CFormatterTestHelper, test_ALRPCv1_NotificationCorrelationId)
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
    ASSERT_EQ(-1, (int)obj1[S_PARAMS][S_CORRELATION_ID]) << "Wrong CorrelationID";
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
    ASSERT_EQ(-1, (int)obj2[S_PARAMS][S_CORRELATION_ID]) << "Wrong CorrelationID";
    ASSERT_EQ(FunctionID::INVALID_ENUM, (int)obj2[S_PARAMS][S_FUNCTION_ID]);
    ASSERT_EQ(messageType::notification, (int)obj2[S_PARAMS][S_MESSAGE_TYPE]);
  }

}}}}

namespace NsSmartDeviceLink { namespace NsSmartObjects {

    template <>
    const std::map<test::components::JSONHandler::formatters::FunctionID::eType, std::string> &
    NsSmartDeviceLink::NsSmartObjects::TEnumSchemaItem<test::components::JSONHandler::formatters::FunctionID::eType>::getEnumElementsStringRepresentation(void)
    {
        static bool isInitialized = false;
        static std::map<test::components::JSONHandler::formatters::FunctionID::eType, std::string> enumStringRepresentationMap;

        if (false == isInitialized)
        {
            enumStringRepresentationMap.insert(std::make_pair(test::components::JSONHandler::formatters::FunctionID::RegisterAppInterface, "RegisterAppInterface"));
            enumStringRepresentationMap.insert(std::make_pair(test::components::JSONHandler::formatters::FunctionID::UnregisterAppInterface, "UnregisterAppInterface"));
            enumStringRepresentationMap.insert(std::make_pair(test::components::JSONHandler::formatters::FunctionID::SetGlobalProperties, "SetGlobalProperties"));

            isInitialized = true;
        }

        return enumStringRepresentationMap;
    }

    template <>
    const std::map<test::components::JSONHandler::formatters::messageType::eType, std::string> &
    NsSmartDeviceLink::NsSmartObjects::TEnumSchemaItem<test::components::JSONHandler::formatters::messageType::eType>::getEnumElementsStringRepresentation(void)
    {
        static bool isInitialized = false;
        static std::map<test::components::JSONHandler::formatters::messageType::eType, std::string> enumStringRepresentationMap;

        if (false == isInitialized)
        {
            enumStringRepresentationMap.insert(std::make_pair(test::components::JSONHandler::formatters::messageType::request, "request"));
            enumStringRepresentationMap.insert(std::make_pair(test::components::JSONHandler::formatters::messageType::response, "response"));
            enumStringRepresentationMap.insert(std::make_pair(test::components::JSONHandler::formatters::messageType::notification, "notification"));

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
