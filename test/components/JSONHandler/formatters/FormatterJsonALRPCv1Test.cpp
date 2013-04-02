#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <iostream>

#include "JSONHandler/formatters/CFormatterJsonALRPCv1.hpp"
#include "json/json.h"
#include "JSONHandler/CSmartFactory.hpp"

#include "CFormatterTestHelper.hpp"

namespace test { namespace components { namespace JSONHandler { namespace formatters {

    using namespace NsAppLink::NsJSONHandler::strings;

    TEST_F(CFormatterTestHelper, test_JsonALRPCv1)
    {
        Json::Value value;  // just a quick workaround to avoid undefined reference to Json
        Json::Reader reader;    // the same thing

        std::string str;
        NsAppLink::NsSmartObjects::CSmartObject srcObj;
        NsAppLink::NsSmartObjects::CSmartObject dstObj;

        fillTestObject(srcObj);
        srcObj[S_PARAMS][S_PROTOCOL_VERSION] = 1;

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
