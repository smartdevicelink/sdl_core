#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <iostream>

#include "JSONHandler/formatters/CFormatterJsonALRPCv2.hpp"

#include "json/json.h"

#include "CFormatterTestHelper.hpp"


namespace test { namespace components { namespace JSONHandler { namespace formatters {

    using namespace NsAppLink::NsJSONHandler::strings;

    TEST_F(CFormatterTestHelper, test_fromObjToALRPCv2AndBack)
    {
        Json::Value value;  // just a quick workaround to avoid undefined reference to Json

        std::string str;
        NsAppLink::NsSmartObjects::CSmartObject srcObj;
        NsAppLink::NsSmartObjects::CSmartObject dstObj;

        fillTestObject(srcObj);
        srcObj[S_PARAMS][S_PROTOCOL_VERSION] = 2;

        // SmartObjects --> JSON
        NsAppLink::NsJSONHandler::Formatters::CFormatterJsonALRPCv2::toString(srcObj, str);

        //std::cout << str << std::endl;

        // JSON --> SmartObjects
        NsAppLink::NsJSONHandler::Formatters::CFormatterJsonALRPCv2::
            fromString<std::string, std::string>(str, dstObj, "some function", "request", 12);


        compareObjects(srcObj, dstObj);
    }

    TEST_F(CFormatterTestHelper, test_fromJsonALRPCv2)
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

        NsAppLink::NsSmartObjects::CSmartObject obj;
        bool result;

        result = NsAppLink::NsJSONHandler::Formatters::CFormatterJsonALRPCv2::
                fromString<std::string, std::string>(str, obj, "some name", "request", 12);

        ASSERT_TRUE(result) << "Error parsing JSON string";

        ASSERT_EQ("some name", (std::string)obj[S_PARAMS][S_FUNCTION_ID]);
        ASSERT_EQ(12, (int)obj[S_PARAMS][S_CORRELATION_ID]);
        ASSERT_EQ(2, (int)obj[S_PARAMS][S_PROTOCOL_VERSION]);
        ASSERT_EQ(10, (int)obj[S_MSG_PARAMS]["syncMsgVersion"]["minorVersion"]);
        ASSERT_EQ("TEXT", (std::string)obj[S_MSG_PARAMS]["ttsName"][0]["type"]);
        ASSERT_EQ("Synonym 2", (std::string)obj[S_MSG_PARAMS]["vrSynonyms"][1]);
    }

}}}}

int main(int argc, char **argv)
{
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}

