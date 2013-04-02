#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <iostream>

#include "JSONHandler/formatters/CFormatterJsonALRPCv2.hpp"

#include "json/json.h"

#include "CFormatterTestHelper.hpp"


namespace test { namespace components { namespace JSONHandler { namespace formatters {

    using namespace NsAppLink::NsJSONHandler::strings;

    TEST_F(CFormatterTestHelper, test_JsonALRPCv2)
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

}}}}

int main(int argc, char **argv)
{
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}

