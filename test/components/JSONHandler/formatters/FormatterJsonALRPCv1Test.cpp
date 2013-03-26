#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <iostream>

#include "JSONHandler/formatters/CFormatterJsonALRPCv1.hpp"
#include "json/json.h"
#include "JSONHandler/CSmartFactory.hpp"

namespace test { namespace components { namespace JSONHandler { namespace formatters {

    using namespace NsAppLink::NsJSONHandler::strings;

    TEST(test_SimpleTwoWaysTest, test_JsonALRPCv1)
    {
        Json::Value value;  // just a quick workaround to avoid undefined reference to Json
        Json::Reader reader;    // the same thing

        std::string str;
        NsAppLink::NsSmartObjects::CSmartObject srcObj;
        NsAppLink::NsSmartObjects::CSmartObject dstObj;

        srcObj[S_PARAMS][S_MESSAGE_TYPE] = "request";
        srcObj[S_PARAMS][S_FUNCTION_ID] = "some function";
        srcObj[S_PARAMS][S_CORRELATION_ID] = 12;
        srcObj[S_PARAMS][S_PROTOCOL_TYPE] = 0;
        srcObj[S_PARAMS][S_PROTOCOL_VERSION] = 1;
        srcObj[S_MSG_PARAMS]["appId"] = "APP ID";
        srcObj[S_MSG_PARAMS]["appName"] = "APP NAME";
        srcObj[S_MSG_PARAMS]["appType"][0] = "SYSTEM";
        srcObj[S_MSG_PARAMS]["appType"][1] = "COMMUNICATION";
        srcObj[S_MSG_PARAMS]["hmiDisplayLanguageDesired"] = "RU-RU";
        srcObj[S_MSG_PARAMS]["isMediaApplication"] = true;
        srcObj[S_MSG_PARAMS]["languageDesired"] = "EN-US";
        srcObj[S_MSG_PARAMS]["ngnMediaScreenAppName"] = "SCREEN NAME";
        srcObj[S_MSG_PARAMS]["syncMsgVersion"]["majorVersion"] = 2;
        srcObj[S_MSG_PARAMS]["syncMsgVersion"]["minorVersion"] = 10;
        srcObj[S_MSG_PARAMS]["ttsName"][0]["text"] = "ABC";
        srcObj[S_MSG_PARAMS]["ttsName"][0]["type"] = "TEXT";
        srcObj[S_MSG_PARAMS]["vrSynonyms"][0] = "Synonym1";
        srcObj[S_MSG_PARAMS]["vrSynonyms"][1] = "Synonym2";
        srcObj[S_MSG_PARAMS]["null"] = NsAppLink::NsSmartObjects::CSmartObject();
        srcObj[S_MSG_PARAMS]["double"] = -0.1234;

        // SmartObjects --> JSON
        NsAppLink::NsJSONHandler::Formatters::CFormatterJsonALRPCv1::toString(srcObj, str);

        //std::cout << str << std::endl;

        // JSON --> SmartObjects
        NsAppLink::NsJSONHandler::Formatters::CFormatterJsonALRPCv1::fromString(str, dstObj);

        // Compare SmartObjects
        ASSERT_EQ("APP NAME",  static_cast<std::string>(dstObj[S_MSG_PARAMS]["appName"]));
        ASSERT_EQ(10, static_cast<int>(dstObj[S_MSG_PARAMS]["syncMsgVersion"]["minorVersion"]));
        ASSERT_EQ("TEXT", static_cast<std::string>(dstObj[S_MSG_PARAMS]["ttsName"][0]["type"]));
        ASSERT_TRUE(static_cast<bool>(dstObj[S_MSG_PARAMS]["isMediaApplication"]));

        ASSERT_EQ("request", static_cast<std::string>(dstObj[S_PARAMS][S_MESSAGE_TYPE]));
        ASSERT_EQ("some function", static_cast<std::string>(dstObj[S_PARAMS][S_FUNCTION_ID]));
        ASSERT_EQ(12, static_cast<int>(dstObj[S_PARAMS][S_CORRELATION_ID]));
        ASSERT_EQ(0, static_cast<int>(dstObj[S_PARAMS][S_PROTOCOL_TYPE]));
        ASSERT_EQ(1, static_cast<int>(dstObj[S_PARAMS][S_PROTOCOL_VERSION]));

        ASSERT_TRUE(srcObj == dstObj);      // High level comparison
    }

}}}}

int main(int argc, char **argv)
{
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
