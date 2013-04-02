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


        compareObjects(srcObj, dstObj);
    }

    TEST_F(CFormatterTestHelper, test_fromJsonALRPCv1)
    {
        std::string str = "\
        {\
            \"request\": {\
                \"name\" : \"some name\",\
                \"correlationID\": 11,\
                \"parameters\": {\
                    \"syncMsgVersion\": \"version\",\
                    \"appName\": \"some app name\"\
                }\
            }\
        }";

        NsAppLink::NsSmartObjects::CSmartObject obj;
        bool result;

        result = NsAppLink::NsJSONHandler::Formatters::CFormatterJsonALRPCv1::fromString(str, obj);

        ASSERT_TRUE(result) << "Error parsing JSON string";

        ASSERT_EQ("some name", (std::string)obj[S_PARAMS][S_FUNCTION_ID]);
        ASSERT_EQ(11, (int)obj[S_PARAMS][S_CORRELATION_ID]);
        ASSERT_EQ("version", (std::string)obj[S_MSG_PARAMS]["syncMsgVersion"]);
        ASSERT_EQ("some app name", (std::string)obj[S_MSG_PARAMS]["appName"]);
    }

}}}}

int main(int argc, char **argv)
{
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
