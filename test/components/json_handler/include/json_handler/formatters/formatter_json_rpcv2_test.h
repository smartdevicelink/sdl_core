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

#ifndef TEST_COMPONENTS_JSON_HANDLER_INCLUDE_JSON_HANDLER_FORMATTERS_FORMATTER_JSON_RPCV2_TEST_H_
#define TEST_COMPONENTS_JSON_HANDLER_INCLUDE_JSON_HANDLER_FORMATTERS_FORMATTER_JSON_RPCV2_TEST_H_

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <iostream>

#include "JSONHandler/formatters/CFormatterJsonRPC2.hpp"

#include "json/json.h"



namespace test { namespace components { namespace JSONHandler { namespace formatters {

    TEST(test_SimpleTwoWaysTest, test_JsonRPC2)
    {
        Json::Value value;  // just a quick workaround to avoid undefined reference to Json
        Json::Reader reader;    // the same thing

        std::string str;
        NsSmartDeviceLink::NsSmartObjects::CSmartObject srcObj;
        NsSmartDeviceLink::NsSmartObjects::CSmartObject dstObj;

        // TODO: Implement test on valid RPC2 test data

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
        srcObj["msg_params"]["null"] = NsSmartDeviceLink::NsSmartObjects::CSmartObject();
        srcObj["msg_params"]["double"] = -0.1234;

        // SmartObjects --> JSON
        NsSmartDeviceLink::NsJSONHandler::Formatters::CFormatterJsonRPC2::toString(srcObj, str);

        std::cout << str << std::endl;

        // JSON --> SmartObjects
        NsSmartDeviceLink::NsJSONHandler::Formatters::CFormatterJsonRPC2::fromString(str, dstObj);

        // Compare SmartObjects
        ASSERT_EQ("APP NAME",  static_cast<std::string>(dstObj["msg_params"]["appName"]));
        ASSERT_EQ(10, static_cast<int>(dstObj["msg_params"]["syncMsgVersion"]["minorVersion"]));
        ASSERT_EQ("TEXT", static_cast<std::string>(dstObj["msg_params"]["ttsName"][0]["type"]));
        ASSERT_TRUE(static_cast<bool>(dstObj["msg_params"]["isMediaApplication"]));

        ASSERT_TRUE(srcObj == dstObj);      // High level comparison
    }

}}}}

#endif  // TEST_COMPONENTS_JSON_HANDLER_INCLUDE_JSON_HANDLER_FORMATTERS_FORMATTER_JSON_RPCV2_TEST_H_
