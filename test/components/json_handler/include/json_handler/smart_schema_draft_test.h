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

#ifndef TEST_COMPONENTS_JSON_HANDLER_INCLUDE_JSON_HANDLER_SMART_SCHEMA_DRAFT_TEST_H_
#define TEST_COMPONENTS_JSON_HANDLER_INCLUDE_JSON_HANDLER_SMART_SCHEMA_DRAFT_TEST_H_

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <string>
#include <iostream>
#include "JSONHandler/formatters/CFormatterJsonSDLRPCv2.hpp"
#include "test/components/json_handler/test_JSONHandler_v4_protocol_v2_0_revP_schema.h"

using namespace Gen::test::components::JSONHandler2;
using namespace NsSmartDeviceLink::NsSmartObjects;
using namespace NsSmartDeviceLink::NsJSONHandler::strings;
using namespace NsSmartDeviceLink::NsJSONHandler::Formatters;

namespace test { namespace components { namespace json_handler { namespace smart_schema_draft_test {

    TEST(test_general, test_SmartSchemaDraftTest)
    {
        bool bResult = false;

        // STEP 1. Getting object from string
        std::string inputJsonString = "{\
            \"appID\" : \"APP ID\",\
            \"appName\" : \"MY AMAZING APP NAME\",\
            \"appType\" : [ \"SYSTEM\", \"COMMUNICATION\" ],\
            \"isMediaApplication\" : true,\
            \"languageDesired\" : \"DE-EU\",\
            \"hmiDisplayLanguageDesired\" : \"RU-RU\",\
            \"ngnMediaScreenAppName\" : \"SCREEN NAME\",\
            \"syncMsgVersion\" : {\
                \"majorVersion\" : 2,\
                \"minorVersion\" : 10\
            },\
            \"ttsName\" : [\
            {\
                \"text\" : \"ABC\",\
                \"type\" : \"PRE_RECORDED\"\
            }\
            ],\
            \"vrSynonyms\" : [ \"Synonym1\", \"Synonym2\" ]\
        }\
        ";

        CSmartObject obj;
        bResult = CFormatterJsonSDLRPCv2::fromString<FunctionID::eType, messageType::eType>(inputJsonString, obj, FunctionID::RegisterAppInterfaceID, messageType::request, 1);
        ASSERT_TRUE(bResult);

        ASSERT_EQ(FunctionID::RegisterAppInterfaceID, (int)obj[S_PARAMS][S_FUNCTION_ID]);
        ASSERT_EQ(messageType::request, (int)obj[S_PARAMS][S_MESSAGE_TYPE]);

        //std::cout<<"TTS:"<<static_cast<std::string>(obj["msg_params"]["ttsName"][0]["type"])<<std::endl;
        // While fromString is not fully complete yet
        //obj["params"]["msg_type"] = messageType::request;
        //obj["params"]["function_id"] = FunctionID::RegisterAppInterfaceID;

        //obj["msg_params"]["appId"] = "APP ID";
        //obj["msg_params"]["appName"] = "APP NAME";
        //obj["msg_params"]["appType"][0] = "SYSTEM";
        //obj["msg_params"]["appType"][1] = "COMMUNICATION";
        //obj["msg_params"]["hmiDisplayLanguageDesired"] = "RU-RU";
        //obj["msg_params"]["isMediaApplication"] = true;
        //obj["msg_params"]["languageDesired"] = "EN-US";
        //obj["msg_params"]["ngnMediaScreenAppName"] = "SCREEN NAME";
        //obj["msg_params"]["syncMsgVersion"]["majorVersion"] = 2;
        //obj["msg_params"]["syncMsgVersion"]["minorVersion"] = 10;
        //obj["msg_params"]["ttsName"][0]["text"] = "ABC";
        //obj["msg_params"]["ttsName"][0]["type"] = "TEXT";
        //obj["msg_params"]["vrSynonyms"][0] = "Synonym1";
        //obj["msg_params"]["vrSynonyms"][1] = "Synonym2";

        // STEP 2. Attaching schema to object
        test_JSONHandler_v4_protocol_v2_0_revP factory;

        bResult = factory.attachSchema(obj);
        ASSERT_TRUE(bResult);

        // STEP 3. Validating object
        ASSERT_EQ(NsSmartDeviceLink::NsSmartObjects::Errors::OK, obj.isValid());

        // STEP 4. Working with object
        obj[S_PARAMS][S_MESSAGE_TYPE] = messageType::notification;
        obj[S_MSG_PARAMS]["appName"] = "NEW APP NAME";

        // STEP 5. Validating object
        ASSERT_EQ(NsSmartDeviceLink::NsSmartObjects::Errors::OK, obj.isValid());

        // STEP 6. Converting object to json string

        std::string outputJsonString;
        bResult = CFormatterJsonSDLRPCv2::toString(obj, outputJsonString);
        ASSERT_TRUE(bResult);

        std::string expectedOutputJsonString = "{\
            \"appID\" : \"APP ID\",\
            \"appName\" : \"NEW APP NAME\",\
            \"appType\" : [ 9, 1 ],\
            \"isMediaApplication\" : true,\
            \"languageDesired\" : \"DE-EU\",\
            \"hmiDisplayLanguageDesired\" : \"RU-RU\",\
            \"ngnMediaScreenAppName\" : \"SCREEN NAME\",\
            \"syncMsgVersion\" : {\
            \"majorVersion\" : 2,\
            \"minorVersion\" : 10\
        },\
        \"ttsName\" : [\
        {\
        \"text\" : \"ABC\",\
        \"type\" : \"PRE_RECORDED\"\
        }\
        ],\
        \"vrSynonyms\" : [ \"Synonym1\", \"Synonym2\" ]\
        }\
        ";

        //ASSERT_EQ(inputJsonString, outputJsonString);

        //std::cout<<outputJsonString<<std::endl;
    }

    TEST(test_general, test_AttachSchema) {
      CSmartObject object(SmartType_Map);
      test_JSONHandler_v4_protocol_v2_0_revP factory;

      ASSERT_FALSE(factory.AttachSchema(StructIdentifiers::INVALID_ENUM,
                                       object));
      
      ASSERT_TRUE(factory.AttachSchema(StructIdentifiers::TextField,
                                       object));

      // Valid TextField object
      object["name"] = TextFieldName::mainField1;
      object["characterSet"] = CharacterSet::TYPE2SET;
      object["width"] = 100;
      object["rows"] = 2;

      ASSERT_EQ(NsSmartDeviceLink::NsSmartObjects::Errors::OK,
                object.isValid());

      // Invalid value range for TextField
      object["rows"] = 20;

      ASSERT_EQ(NsSmartDeviceLink::NsSmartObjects::Errors::OUT_OF_RANGE,
                object.isValid());

      object["rows"] = 2;
      // Add unexpected field
      object["xxx"] = 1234;

      ASSERT_EQ(
          NsSmartDeviceLink::NsSmartObjects::Errors::UNEXPECTED_PARAMETER,
          object.isValid());
    }
    
    TEST(test_general, test_SmartObjectCreation) {
      test_JSONHandler_v4_protocol_v2_0_revP factory;

      CSmartObject object = factory.CreateSmartObject(
          StructIdentifiers::INVALID_ENUM);

      ASSERT_EQ(SmartType_Null, object.getType());

      object = factory.CreateSmartObject(
          FunctionID::INVALID_ENUM,
          messageType::INVALID_ENUM);

      ASSERT_EQ(SmartType_Null, object.getType());

      object = factory.CreateSmartObject(
          FunctionID::RegisterAppInterfaceID,
          messageType::INVALID_ENUM);

      ASSERT_EQ(SmartType_Null, object.getType());
      
      object = factory.CreateSmartObject(
          FunctionID::INVALID_ENUM,
          messageType::response);

      ASSERT_EQ(SmartType_Null, object.getType());

      object = factory.CreateSmartObject(StructIdentifiers::Image);

      ASSERT_EQ(SmartType_Map, object.getType());

      object["value"] = "xxx";
      object["imageType"] = ImageType::STATIC;

      ASSERT_EQ(NsSmartDeviceLink::NsSmartObjects::Errors::OK,
                object.isValid());

      object["zzz"] = "yyy";

      ASSERT_EQ(
          NsSmartDeviceLink::NsSmartObjects::Errors::UNEXPECTED_PARAMETER,
          object.isValid());

      object = factory.CreateSmartObject(FunctionID::AddSubMenuID,
                                         messageType::request);

      object[S_PARAMS][S_FUNCTION_ID] = FunctionID::AddSubMenuID;
      object[S_PARAMS][S_MESSAGE_TYPE] = messageType::request;
      object[S_PARAMS][S_CORRELATION_ID] = 0;
      object[S_PARAMS][S_PROTOCOL_VERSION] = 2;
      object[S_PARAMS][S_PROTOCOL_TYPE] = 111;
      object[S_MSG_PARAMS]["menuID"] = 10;
      object[S_MSG_PARAMS]["position"] = 20;
      object[S_MSG_PARAMS]["menuName"] = "MenuItem";
      
      ASSERT_EQ(NsSmartDeviceLink::NsSmartObjects::Errors::OK,
                object.isValid());

      object[S_MSG_PARAMS]["Noise"] = "Bzzzzzz!!!";      
      
      ASSERT_EQ(
          NsSmartDeviceLink::NsSmartObjects::Errors::UNEXPECTED_PARAMETER,
          object.isValid());
    }

    TEST(test_general, test_GetSmartSchema) {
      test_JSONHandler_v4_protocol_v2_0_revP factory;

      CSmartSchema schema;
      ASSERT_FALSE(factory.GetSchema(StructIdentifiers::INVALID_ENUM,
                                    schema));

      ASSERT_FALSE(factory.GetSchema(FunctionID::INVALID_ENUM,
                                    messageType::INVALID_ENUM,
                                    schema));

      ASSERT_FALSE(factory.GetSchema(FunctionID::RegisterAppInterfaceID,
                                    messageType::INVALID_ENUM,
                                    schema));

      ASSERT_FALSE(factory.GetSchema(FunctionID::INVALID_ENUM,
                                    messageType::response,
                                    schema));

      ASSERT_TRUE(factory.GetSchema(StructIdentifiers::SyncMsgVersion,
                                    schema));
      
      CSmartObject object(SmartType_Map);      
      object.setSchema(schema);

      object["majorVersion"] = 1;
      object["minorVersion"] = 2;

      ASSERT_EQ(NsSmartDeviceLink::NsSmartObjects::Errors::OK,
                object.isValid());

      object["majorVersion"] = 1000;

      ASSERT_EQ(NsSmartDeviceLink::NsSmartObjects::Errors::OUT_OF_RANGE,
                object.isValid());

      object["majorVersion"] = 1;
      object["zzzz"] = 200;

      ASSERT_EQ(
          NsSmartDeviceLink::NsSmartObjects::Errors::UNEXPECTED_PARAMETER,
          object.isValid());

      ASSERT_TRUE(factory.GetSchema(FunctionID::UnregisterAppInterfaceID,
                                    messageType::request,
                                    schema));

      object = CSmartObject(SmartType_Map);
      object.setSchema(schema);

      object[S_PARAMS][S_FUNCTION_ID] = FunctionID::UnregisterAppInterfaceID;
      object[S_PARAMS][S_MESSAGE_TYPE] = messageType::request;
      object[S_PARAMS][S_CORRELATION_ID] = 22;
      object[S_PARAMS][S_PROTOCOL_VERSION] = 1;
      object[S_PARAMS][S_PROTOCOL_TYPE] = 1;
      object[S_MSG_PARAMS] = CSmartObject(SmartType_Map);

      ASSERT_EQ(NsSmartDeviceLink::NsSmartObjects::Errors::OK,
                object.isValid());

      object[S_PARAMS]["blah-blah"] = "YouShallNotPass!";

      ASSERT_EQ(
          NsSmartDeviceLink::NsSmartObjects::Errors::UNEXPECTED_PARAMETER,
          object.isValid());
    }
}}}}

int main(int argc, char **argv) {
    //PropertyConfigurator::doConfigure(LOG4CPLUS_TEXT("log4cplus.properties"));
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
