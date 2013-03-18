#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <iostream>

#include "JSONHandler/formatters/CFormatterJsonALRPCv2.hpp"

#include "json/json.h"



namespace test { namespace components { namespace JSONHandler { namespace formatters {

    TEST(test_SimpleObjToString, test_JsonALRPCv2)
    {
        Json::Value value;  // just a quick workaround to avoid undefined reference to Json

        std::string str;
        NsAppLink::NsSmartObjects::CSmartObject obj;

        obj["params"]["MessageType"] = 0;
        obj["params"]["CorrelationId"] = 0;
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
        obj["msg_params"]["null"] = NsAppLink::NsSmartObjects::CSmartObject();
        obj["msg_params"]["double"] = -0.1234;

        NsAppLink::NsJSONHandler::Formatters::CFormatterJsonALRPCv2::toString(obj, str);

        std::cout << str;
    }

}}}}

int main(int argc, char **argv)
{
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}

