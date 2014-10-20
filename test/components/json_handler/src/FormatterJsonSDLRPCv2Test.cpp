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

#include "formatters/CFormatterJsonSDLRPCv2.hpp"
#include "formatters/CSmartFactory.hpp"

#include "smart_objects/array_schema_item.h"
#include "smart_objects/bool_schema_item.h"
#include "smart_objects/object_schema_item.h"
#include "smart_objects/string_schema_item.h"
#include "smart_objects/enum_schema_item.h"
#include "smart_objects/number_schema_item.h"
#include "smart_objects/schema_item_parameter.h"

#include "json_handler/formatter_test_helper.h"


namespace test { namespace components { namespace json_handler { namespace formatters {

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
        NsSmartDeviceLink::NsSmartObjects::SmartObject srcObj;
        NsSmartDeviceLink::NsSmartObjects::SmartObject dstObj;

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

        NsSmartDeviceLink::NsSmartObjects::SmartObject obj;
        bool result;

        result = NsSmartDeviceLink::NsJSONHandler::Formatters::CFormatterJsonSDLRPCv2::
                fromString<std::string, std::string>(str, obj, "some name", "request", 12);

        ASSERT_TRUE(result) << "Error parsing JSON string";

        ASSERT_EQ("some name", obj[S_PARAMS][S_FUNCTION_ID].asString());
        ASSERT_EQ(12, obj[S_PARAMS][S_CORRELATION_ID].asInt());
        ASSERT_EQ(2, obj[S_PARAMS][S_PROTOCOL_VERSION].asInt());
        ASSERT_EQ(10, obj[S_MSG_PARAMS]["syncMsgVersion"]["minorVersion"].asInt());
        ASSERT_EQ("TEXT", obj[S_MSG_PARAMS]["ttsName"][0]["type"].asString());
        ASSERT_EQ("Synonym 2", obj[S_MSG_PARAMS]["vrSynonyms"][1].asString());
    }

    TEST_F(CFormatterTestHelper, test_SDLRPCv2_EmptyMapArrayTest) {
      using namespace NsSmartDeviceLink::NsSmartObjects;

      SmartObject srcObj, dstObj;
      std::string str;

      fillTestObject(srcObj);
      srcObj[S_PARAMS][S_PROTOCOL_VERSION] = 2;
      srcObj[S_PARAMS][S_MESSAGE_TYPE] = 0;
      srcObj[S_PARAMS][S_CORRELATION_ID] = 0;
      srcObj[S_PARAMS][S_FUNCTION_ID] = 0;
      srcObj[S_MSG_PARAMS]["EmptyArray"] = SmartObject(SmartType_Array);
      srcObj[S_MSG_PARAMS]["EmptyMap"] = SmartObject(SmartType_Map);
      srcObj[S_MSG_PARAMS]["AnotherEmptyArray"] = srcObj[S_MSG_PARAMS]["EmptyArray"];
      srcObj[S_MSG_PARAMS]["AnotherEmptyMap"]  =  srcObj[S_MSG_PARAMS]["EmptyMap"];

      // SmartObjects --> JSON
      FormatterV2::toString(srcObj, str);

      // JSON --> SmartObjects
      FormatterV2::fromString<int, int>(str, dstObj, 0, 0, 0);

      compareObjects(srcObj, dstObj);
    }

    TEST_F(CFormatterTestHelper, test_CorrelationIdTest) {

        std::string str = "\
        {\
            \"appID\": \"APP ID\",\
            \"appName\": \"APP NAME\",\
            \"appType\": [\"SYSTEM\", \"COMMUNICATION\"],\
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

        NsSmartDeviceLink::NsSmartObjects::SmartObject obj1, obj2;
        bool result;

        result = NsSmartDeviceLink::NsJSONHandler::Formatters::CFormatterJsonSDLRPCv2::
                fromString<std::string, std::string>(str, obj1, "some name", "notification");

        ASSERT_TRUE(result) << "Error parsing JSON string";

        ASSERT_FALSE(obj1[S_PARAMS].keyExists(S_CORRELATION_ID)) << "CorrelationID exists for notification";

        result = NsSmartDeviceLink::NsJSONHandler::Formatters::CFormatterJsonSDLRPCv2::
                fromString<std::string, std::string>(str, obj2, "some name", "request", 10);

        ASSERT_TRUE(result) << "Error parsing JSON string";

        ASSERT_TRUE(obj2[S_PARAMS].keyExists(S_CORRELATION_ID)) << "CorrelationID doesn't exist for request";
    }
}}}}

namespace NsSmartDeviceLink {
namespace NsSmartObjects {

template<>
const EnumConversionHelper<test::components::json_handler::formatters::function_id::Type>::EnumToCStringMap
EnumConversionHelper<test::components::json_handler::formatters::function_id::Type>::enum_to_cstring_map_ =
  EnumConversionHelper<test::components::json_handler::formatters::function_id::Type>::InitEnumToCStringMap();

template<>
const EnumConversionHelper<test::components::json_handler::formatters::function_id::Type>::CStringToEnumMap
EnumConversionHelper<test::components::json_handler::formatters::function_id::Type>::cstring_to_enum_map_ =
  EnumConversionHelper<test::components::json_handler::formatters::function_id::Type>::InitCStringToEnumMap();

template<>
const char* const
EnumConversionHelper<test::components::json_handler::formatters::function_id::Type>::cstring_values_[] = {
    "RegisterAppInterface",
    "UnregisterAppInterface",
    "SetGlobalProperties"
};

template<>
const test::components::json_handler::formatters::function_id::Type
EnumConversionHelper<test::components::json_handler::formatters::function_id::Type>::enum_values_[] = {
    test::components::json_handler::formatters::function_id::kRegisterAppInterface,
    test::components::json_handler::formatters::function_id::kUnregisterAppInterface,
    test::components::json_handler::formatters::function_id::kSetGlobalProperties
};

template<>
const EnumConversionHelper<test::components::json_handler::formatters::message_type::Type>::EnumToCStringMap
EnumConversionHelper<test::components::json_handler::formatters::message_type::Type>::enum_to_cstring_map_ =
  EnumConversionHelper<test::components::json_handler::formatters::message_type::Type>::InitEnumToCStringMap();

template<>
const EnumConversionHelper<test::components::json_handler::formatters::message_type::Type>::CStringToEnumMap
EnumConversionHelper<test::components::json_handler::formatters::message_type::Type>::cstring_to_enum_map_ =
  EnumConversionHelper<test::components::json_handler::formatters::message_type::Type>::InitCStringToEnumMap();

template<>
const char* const
EnumConversionHelper<test::components::json_handler::formatters::message_type::Type>::cstring_values_[] = {
    "request",
    "response",
    "notification"
};

template<>
const test::components::json_handler::formatters::message_type::Type
EnumConversionHelper<test::components::json_handler::formatters::message_type::Type>::enum_values_[] = {
    test::components::json_handler::formatters::message_type::kRequest,
    test::components::json_handler::formatters::message_type::kResponse,
    test::components::json_handler::formatters::message_type::kNotification
};

}
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}

