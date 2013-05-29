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

#include "json_handler/formatters/formatter_test_helper.h"

#include "JSONHandler/CSmartFactory.hpp"

using namespace test::components::json_handler::formatters;
using namespace NsSmartDeviceLink::NsSmartObjects;
using namespace NsSmartDeviceLink::NsJSONHandler::strings;

void CFormatterTestHelper::fillTestObject(CSmartObject& obj)
{
    obj[S_PARAMS][S_MESSAGE_TYPE] = "request";
    obj[S_PARAMS][S_FUNCTION_ID] = "some function";
    obj[S_PARAMS][S_CORRELATION_ID] = 12;
    obj[S_PARAMS][S_PROTOCOL_TYPE] = 0;
    obj[S_MSG_PARAMS]["appId"] = "APP ID";
    obj[S_MSG_PARAMS]["appName"] = "APP NAME";
    obj[S_MSG_PARAMS]["appType"][0] = "SYSTEM";
    obj[S_MSG_PARAMS]["appType"][1] = "COMMUNICATION";
    obj[S_MSG_PARAMS]["hmiDisplayLanguageDesired"] = "RU-RU";
    obj[S_MSG_PARAMS]["isMediaApplication"] = true;
    obj[S_MSG_PARAMS]["languageDesired"] = "EN-US";
    obj[S_MSG_PARAMS]["ngnMediaScreenAppName"] = "SCREEN NAME";
    obj[S_MSG_PARAMS]["syncMsgVersion"]["majorVersion"] = 2;
    obj[S_MSG_PARAMS]["syncMsgVersion"]["minorVersion"] = 10;
    obj[S_MSG_PARAMS]["ttsName"][0]["text"] = "ABC";
    obj[S_MSG_PARAMS]["ttsName"][0]["type"] = "TEXT";
    obj[S_MSG_PARAMS]["vrSynonyms"][0] = "Synonym1";
    obj[S_MSG_PARAMS]["vrSynonyms"][1] = "Synonym2";
    obj[S_MSG_PARAMS]["null"] = NsSmartDeviceLink::NsSmartObjects::CSmartObject();
    obj[S_MSG_PARAMS]["double"] = -0.1234;
}


void CFormatterTestHelper::compareObjects(
    const NsSmartDeviceLink::NsSmartObjects::CSmartObject& first,
    const NsSmartDeviceLink::NsSmartObjects::CSmartObject& second) {
  if (SmartType_Array == first.getType()) {
    ASSERT_EQ(SmartType_Array, second.getType());

    for (int i = 0; i < first.length(); i++) {
      compareObjects(first.getElement(i), second.getElement(i));
    }
  } else if (SmartType_Map == first.getType()) {
    ASSERT_EQ(SmartType_Map, second.getType());

    std::set<std::string> keys = first.enumerate();

    for (std::set<std::string>::const_iterator key = keys.begin();
        key != keys.end(); key++) {
      compareObjects(first.getElement(*key), second.getElement(*key));
    }
  } else if (SmartType_Boolean == first.getType()) {
    ASSERT_EQ((bool) first, (bool) second);
  } else if (SmartType_Integer == first.getType()) {
    ASSERT_EQ((int) first, (int) second);
  } else if (SmartType_Double == first.getType()) {
    ASSERT_EQ((double) first, (double) second);
  } else if (SmartType_String == first.getType()) {
    ASSERT_EQ((std::string) first, (std::string) second);
  } else if (SmartType_Null == first.getType()) {
    ASSERT_EQ(SmartType_Null, second.getType());
  } else {
    FAIL() << "Unknown SmartObject type: " << first.getType();
  }
}
