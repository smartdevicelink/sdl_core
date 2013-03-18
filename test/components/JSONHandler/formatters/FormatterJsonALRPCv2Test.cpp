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

        obj["appId"] = "APP ID";
        obj["appName"] = "APP NAME";
        obj["appType"][0] = "SYSTEM";
        obj["appType"][1] = "COMMUNICATION";
        obj["hmiDisplayLanguageDesired"] = "RU-RU";
        obj["isMediaApplication"] = true;
        obj["languageDesired"] = "EN-US";
        obj["ngnMediaScreenAppName"] = "SCREEN NAME";
        obj["syncMsgVersion"]["majorVersion"] = 2;
        obj["syncMsgVersion"]["minorVersion"] = 10;
        obj["ttsName"][0]["text"] = "ABC";
        obj["ttsName"][0]["type"] = "TEXT";
        obj["vrSynonyms"][0] = "Synonym1";
        obj["vrSynonyms"][1] = "Synonym2";

        NsAppLink::NsJSONHandler::CFormatterJsonALRPCv2::toString(obj, str);

        std::cout << str;
    }

}}}}

int main(int argc, char **argv)
{
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}

