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

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <iostream>

#include "JSONHandler/formatters/CFormatterJsonSDLRPCv2.hpp"

#include "json/json.h"

#include "CFormatterTestHelper.hpp"


namespace test { namespace components { namespace JSONHandler { namespace formatters {

    using namespace NsSmartDeviceLink::NsJSONHandler::strings;

    typedef NsSmartDeviceLink::NsJSONHandler::Formatters::CFormatterJsonSDLRPCv2 FormatterV2;

    TEST_F(CFormatterTestHelper, test_fromObjToSDLRPCv2AndBack)
    {
        Json::Value value;  // just a quick workaround to avoid undefined reference to Json

        std::string str;
        NsSmartDeviceLink::NsSmartObjects::CSmartObject srcObj;
        NsSmartDeviceLink::NsSmartObjects::CSmartObject dstObj;

        fillTestObject(srcObj);
        srcObj[S_PARAMS][S_PROTOCOL_VERSION] = 2;

        // SmartObjects --> JSON
        NsSmartDeviceLink::NsJSONHandler::Formatters::CFormatterJsonSDLRPCv2::toString(srcObj, str);

        //std::cout << str << std::endl;

        // JSON --> SmartObjects
        NsSmartDeviceLink::NsJSONHandler::Formatters::CFormatterJsonSDLRPCv2::
            fromString<std::string, std::string>(str, dstObj, "some function", "request", 12);


        compareObjects(srcObj, dstObj);
    }

    TEST_F(CFormatterTestHelper, test_fromJsonSDLRPCv2)
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

        NsSmartDeviceLink::NsSmartObjects::CSmartObject obj;
        bool result;

        result = NsSmartDeviceLink::NsJSONHandler::Formatters::CFormatterJsonSDLRPCv2::
                fromString<std::string, std::string>(str, obj, "some name", "request", 12);

        ASSERT_TRUE(result) << "Error parsing JSON string";

        ASSERT_EQ("some name", (std::string)obj[S_PARAMS][S_FUNCTION_ID]);
        ASSERT_EQ(12, (int)obj[S_PARAMS][S_CORRELATION_ID]);
        ASSERT_EQ(2, (int)obj[S_PARAMS][S_PROTOCOL_VERSION]);
        ASSERT_EQ(10, (int)obj[S_MSG_PARAMS]["syncMsgVersion"]["minorVersion"]);
        ASSERT_EQ("TEXT", (std::string)obj[S_MSG_PARAMS]["ttsName"][0]["type"]);
        ASSERT_EQ("Synonym 2", (std::string)obj[S_MSG_PARAMS]["vrSynonyms"][1]);
    }

    TEST_F(CFormatterTestHelper, test_SDLRPCv2_EmptyMapArrayTest) {
      using namespace NsSmartDeviceLink::NsSmartObjects;

      CSmartObject srcObj, dstObj;
      std::string str;

      fillTestObject(srcObj);
      srcObj[S_PARAMS][S_PROTOCOL_VERSION] = 2;
      srcObj[S_PARAMS][S_MESSAGE_TYPE] = 0;
      srcObj[S_PARAMS][S_CORRELATION_ID] = 0;
      srcObj[S_PARAMS][S_FUNCTION_ID] = 0;
      srcObj[S_MSG_PARAMS]["EmptyArray"] = CSmartObject(SmartType_Array);
      srcObj[S_MSG_PARAMS]["EmptyMap"] = CSmartObject(SmartType_Map);
      srcObj[S_MSG_PARAMS]["AnotherEmptyArray"] = srcObj[S_MSG_PARAMS]["EmptyArray"];
      srcObj[S_MSG_PARAMS]["AnotherEmptyMap"]  =  srcObj[S_MSG_PARAMS]["EmptyMap"];

      // SmartObjects --> JSON
      FormatterV2::toString(srcObj, str);

      std::cout << str << std::endl;

      // JSON --> SmartObjects
      FormatterV2::fromString<int, int>(str, dstObj, 0, 0, 0);

      compareObjects(srcObj, dstObj);
    }

}}}}

int main(int argc, char **argv)
{
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}

