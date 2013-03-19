#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <string>
#include <iostream>
#include "JSONHandler/formatters/CFormatterJsonALRPCv2.hpp"
#include "test/components/JSONHandler/test_v4_protocol_v2_0_revP.hpp"

using namespace Gen::test::components::JSONHandler;
using namespace NsAppLink::NsSmartObjects;
using namespace NsAppLink::NsJSONHandler::Formatters;

namespace test { namespace components { namespace JSONHandler { namespace SmartSchemaDraftTest {

    TEST(test_general, test_SmartSchemaDraftTest)
    {
        bool bResult = false;

        // STEP 1. Getting object from string
        std::string inputJsonString = "";

        CSmartObject obj;
        bResult = CFormatterJsonALRPCv2::fromString<FunctionID, messageType>(inputJsonString, obj, FunctionID::RegisterAppInterfaceID, messageType::request);
        ASSERT_TRUE(bResult);

        // While fromString is not fully complete yet
        obj["params"]["msg_type"] = messageType::request;
        obj["params"]["function_id"] = FunctionID::RegisterAppInterfaceID;

        obj["msg_params"]["appId"] = "APP ID";
        obj["msg_params"]["appName"] = "APP NAME";
        obj["msg_params"]["appType"][0] = "SYSTEM";
        obj["msg_params"]["appType"][1] = "COMMUNICATION";
        obj["msg_params"]["hmiDisplayLanguageDesired"] = "RU-RU";
        obj["msg_params"]["isMediaApplication"] = true;
        obj["msg_params"]["languageDesired"] = "EN-US";
        obj["msg_params"]["ngnMediaScreenAppName"] = "SCREEN NAME";
        obj["msg_params"]["syncMsgVersion"]["majorVersion"] = 2;
        obj["msg_params"]["syncMsgVersion"]["minorVersion"] = 10;
        obj["msg_params"]["ttsName"][0]["text"] = "ABC";
        obj["msg_params"]["ttsName"][0]["type"] = "TEXT";
        obj["msg_params"]["vrSynonyms"][0] = "Synonym1";
        obj["msg_params"]["vrSynonyms"][1] = "Synonym2";

        // STEP 2. Attaching schema to object
        test_v4_protocol_v2_0_revP factory;

        bResult = factory.attachSchema(obj);
        ASSERT_TRUE(bResult);

        // STEP 3. Validating object
        ASSERT_TRUE(obj.isValid());

        // STEP 4. Working with object
        obj["params"]["msg_type"] = messageType::notification;
        obj["msg_params"]["appName"] = "MY AMAZING APP NAME";

        // STEP 5. Validating object
        ASSERT_TRUE(obj.isValid());

        // STEP 6. Converting object to json string

        std::string outputJsonString;
        bResult = CFormatterJsonALRPCv2::toString(obj, outputJsonString);
        std::cout<<outputJsonString<<std::endl;
    }
}}}}

int main(int argc, char **argv) {
    //PropertyConfigurator::doConfigure(LOG4CPLUS_TEXT("log4cplus.properties"));
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}