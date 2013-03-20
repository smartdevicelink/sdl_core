#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <iostream>

#include "JSONHandler/formatters/CFormatterJsonALRPCv1.hpp"

#include "json/json.h"



namespace test { namespace components { namespace JSONHandler { namespace formatters {

    TEST(test_SimpleTwoWaysTest, test_JsonALRPCv1)
    {
        Json::Value value;  // just a quick workaround to avoid undefined reference to Json

        std::string str;
        NsAppLink::NsSmartObjects::CSmartObject srcObj;
        NsAppLink::NsSmartObjects::CSmartObject dstObj;

        srcObj["params"]["MessageType"] = 0;
        srcObj["params"]["FunctionId"] = 0;
        srcObj["msg_params"]["appId"] = "APP ID";
        srcObj["msg_params"]["appName"] = "APP NAME";
        srcObj["msg_params"]["appType"][0] = "SYSTEM";
        srcObj["msg_params"]["appType"][1] = "COMMUNICATION";
        srcObj["msg_params"]["hmiDisplayLanguageDesired"] = "RU-RU";
        srcObj["msg_params"]["isMediaApplication"] = true;
        srcObj["msg_params"]["languageDesired"] = "EN-US";
        srcObj["msg_params"]["ngnMediaScreenAppName"] = "SCREEN NAME";
        srcObj["msg_params"]["syncMsgVersion"]["majorVersion"] = 2;
        srcObj["msg_params"]["syncMsgVersion"]["minorVersion"] = 10;
        srcObj["msg_params"]["ttsName"][0]["text"] = "ABC";
        srcObj["msg_params"]["ttsName"][0]["type"] = "TEXT";
        srcObj["msg_params"]["vrSynonyms"][0] = "Synonym1";
        srcObj["msg_params"]["vrSynonyms"][1] = "Synonym2";
        srcObj["msg_params"]["null"] = NsAppLink::NsSmartObjects::CSmartObject();
        srcObj["msg_params"]["double"] = -0.1234;

        // SmartObjects --> JSON
        NsAppLink::NsJSONHandler::Formatters::CFormatterJsonALRPCv1::toString(srcObj, str);

        std::cout << str << std::endl;

        // JSON --> SmartObjects
        NsAppLink::NsJSONHandler::Formatters::CFormatterJsonALRPCv1::fromString(str, dstObj);

        // Compare SmartObjects
        ASSERT_EQ("APP NAME",  static_cast<std::string>(dstObj["msg_params"]["appName"]));
        ASSERT_EQ(10, static_cast<int>(dstObj["msg_params"]["syncMsgVersion"]["minorVersion"]));
        ASSERT_EQ("TEXT", static_cast<std::string>(dstObj["msg_params"]["ttsName"][0]["type"]));
        ASSERT_TRUE(static_cast<bool>(dstObj["msg_params"]["isMediaApplication"]));

        ASSERT_TRUE(srcObj == dstObj);      // High level comparison
    }

}}}}

int main(int argc, char **argv)
{
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
