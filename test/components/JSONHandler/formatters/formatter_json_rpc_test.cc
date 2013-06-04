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

#include "JSONHandler/formatters/formatter_json_rpc.h"

#include "json/json.h"

#include "CFormatterTestHelper.hpp"

namespace test {
namespace components {
namespace JSONHandler {
namespace Formatters {

// The code below is cut and paste from the generated code just to provide necessary environment for string <-> enum conversion.
namespace FunctionID {
/**
 * @brief Enumeration FunctionID.
 */
enum eType {
  /**
   * @brief INVALID_ENUM.
   */
  INVALID_ENUM = -1,

  /**
   * @brief RegisterAppInterface.
   */
  RegisterAppInterface,

  /**
   * @brief UnregisterAppInterface.
   */
  UnregisterAppInterface,

  /**
   * @brief SetGlobalProperties.
   */
  SetGlobalProperties,

// ...
};
}

namespace messageType {
/**
 * @brief Enumeration messageType.
 */
enum eType {
  /**
   * @brief INVALID_ENUM.
   */
  INVALID_ENUM = -1,

  /**
   * @brief request.
   */
  request,

  /**
   * @brief response.
   */
  response,

  /**
   * @brief notification.
   */
  notification
};
}
// end of cut and pasted code
using namespace NsSmartDeviceLink::NsJSONHandler::strings;
typedef NsSmartDeviceLink::NsJSONHandler::Formatters::FormatterJsonRpc JSONFormatter;

TEST_F(CFormatterTestHelper, test_JsonRPC2) {
  Json::Value value;  // just a quick workaround to avoid undefined reference to Json
  Json::Reader reader;    // the same thing

  std::string str;
  NsSmartDeviceLink::NsSmartObjects::CSmartObject srcObj;
  NsSmartDeviceLink::NsSmartObjects::CSmartObject dstObj;

  // TODO: Implement test on valid RPC2 test data

  srcObj[S_PARAMS][S_MESSAGE_TYPE] = messageType::request;
  srcObj[S_PARAMS][S_FUNCTION_ID] = FunctionID::RegisterAppInterface;
  srcObj[S_PARAMS][S_PROTOCOL_VERSION] = 2;
  srcObj[S_PARAMS][S_PROTOCOL_TYPE] = 1;
  srcObj[S_PARAMS][S_CORRELATION_ID] = 12;
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
  srcObj[S_MSG_PARAMS]["null"] =
    NsSmartDeviceLink::NsSmartObjects::CSmartObject();
  srcObj[S_MSG_PARAMS]["double"] = -0.1234;

  // SmartObjects --> JSON
  JSONFormatter::ToString(srcObj, str);

  std::cout << str << std::endl;

  // JSON --> SmartObjects
  JSONFormatter::FromString<FunctionID::eType, messageType::eType>(str, dstObj);

  // Compare SmartObjects
  ASSERT_EQ("APP NAME",
            static_cast<std::string>(dstObj["msg_params"]["appName"]));

  ASSERT_EQ(10,
            static_cast<int>(
                dstObj["msg_params"]["syncMsgVersion"]["minorVersion"]));

  ASSERT_EQ("TEXT",
            static_cast<std::string>(
                dstObj["msg_params"]["ttsName"][0]["type"]));

  ASSERT_TRUE(static_cast<bool>(dstObj["msg_params"]["isMediaApplication"]));

  compareObjects(srcObj, dstObj);
}

/**
 * @brief Check result of parsing of the specified string for specific error.
 *
 * @param error Error code to check.
 * @param str String to parse.
 *
 * @return true if parsing result contains specified error code.
 */
bool CheckErrorCode(int error, const std::string &str) {
  NsSmartDeviceLink::NsSmartObjects::CSmartObject out;

  return error == (error & JSONFormatter::FromString<FunctionID::eType,
                                                     messageType::eType>(
      str, out));
}

/**
 * @brief Invalid JSON RPC string.
 *
 * Used in several tests because it must generate multiple error codes.
 */
static const char *g_invalid_jsonrpc_string =
    "{"
    "  id: 1"
    "  method: \"Method1\""
    "  params: {";

TEST(FormatterJsonRpc, ParsingError) {
  ASSERT_TRUE(CheckErrorCode(JSONFormatter::kParsingError,
                             g_invalid_jsonrpc_string));


  ASSERT_FALSE(CheckErrorCode(JSONFormatter::kParsingError,
                              "{"
                              "  \"method\": \"Method1\""
                              "}"));
}

TEST(FormatterJsonRpc, InvalidFormat) {
  ASSERT_TRUE(CheckErrorCode(JSONFormatter::kInvalidFormat,
                             "{"
                             "  \"id\": 1,"
                             "  \"method\": \"Method1\""
                             "}"));

  ASSERT_TRUE(CheckErrorCode(JSONFormatter::kInvalidFormat,
                             "{"
                             "  \"jsonrpc\": 2,"
                             "  \"id\": 1,"
                             "  \"method\": \"Method1\""
                             "}"));

  ASSERT_TRUE(CheckErrorCode(JSONFormatter::kInvalidFormat,
                             "{"
                             "  \"jsonrpc\": \"2.0\","
                             "  \"id\": [],"
                             "  \"method\": \"Method1\""
                             "}"));

  ASSERT_TRUE(CheckErrorCode(JSONFormatter::kInvalidFormat,
                             "{"
                             "  \"jsonrpc\": \"2.0\","
                             "  \"id\": {},"
                             "  \"method\": \"Method1\""
                             "}"));

  ASSERT_TRUE(CheckErrorCode(JSONFormatter::kInvalidFormat,
                             "{"
                             "  \"jsonrpc\": \"2.0\","
                             "  \"id\": {}"
                             "}"));

  ASSERT_TRUE(CheckErrorCode(JSONFormatter::kInvalidFormat,
                             "{"
                             "  \"jsonrpc\": \"2.0\","
                             "  \"id\": {},"
                             "  \"method\": 1"
                             "}"));

  ASSERT_FALSE(CheckErrorCode(JSONFormatter::kInvalidFormat,
                              "{"
                              "  \"jsonrpc\": \"2.0\","
                              "  \"id\": Null,"
                              "  \"method\": \"Method1\""
                              "}"));

  ASSERT_FALSE(CheckErrorCode(JSONFormatter::kInvalidFormat,
                              "{"
                              "  \"jsonrpc\": \"2.0\","
                              "  \"id\": 1,"
                              "  \"method\": \"Method1\""
                              "}"));

  ASSERT_FALSE(CheckErrorCode(JSONFormatter::kInvalidFormat,
                              "{"
                              "  \"jsonrpc\": \"2.0\","
                              "  \"id\": \"stringId\","
                              "  \"method\": \"Method1\""
                              "}"));

  ASSERT_TRUE(CheckErrorCode(JSONFormatter::kInvalidFormat,
                             "{"
                             "  \"jsonrpc\": \"2.0\","
                             "  \"params\": 10"
                             "}"));

  ASSERT_FALSE(CheckErrorCode(JSONFormatter::kInvalidFormat,
                              "{"
                              "  \"jsonrpc\": \"2.0\","
                              "  \"params\": {}"
                              "}"));

  ASSERT_TRUE(CheckErrorCode(JSONFormatter::kInvalidFormat,
                             "{"
                             "  \"jsonrpc\": \"2.0\","
                             "  \"id\": 1,"
                             "  \"params\": 10"
                             "}"));

  ASSERT_FALSE(CheckErrorCode(JSONFormatter::kInvalidFormat,
                              "{"
                              "  \"jsonrpc\": \"2.0\","
                              "  \"id\": 1,"
                              "  \"params\": {}"
                              "}"));

  ASSERT_TRUE(CheckErrorCode(JSONFormatter::kInvalidFormat,
                             "{"
                             "  \"jsonrpc\": \"2.0\","
                             "  \"id\": 1,"
                             "  \"result\": 10"
                             "}"));

  ASSERT_FALSE(CheckErrorCode(JSONFormatter::kInvalidFormat,
                              "{"
                              "  \"jsonrpc\": \"2.0\","
                              "  \"id\": 1,"
                              "  \"result\": {}"
                              "}"));

  ASSERT_TRUE(CheckErrorCode(JSONFormatter::kInvalidFormat,
                             "{"
                             "  \"jsonrpc\": \"2.0\","
                             "  \"id\": 1,"
                             "  \"error\": 10"
                             "}"));

  ASSERT_FALSE(CheckErrorCode(JSONFormatter::kInvalidFormat,
                              "{"
                              "  \"jsonrpc\": \"2.0\","
                              "  \"id\": 1,"
                              "  \"error\": {}"
                              "}"));
}

TEST(FormatterJsonRpc, MethodNotSpecified) {
  ASSERT_TRUE(CheckErrorCode(JSONFormatter::kMethodNotSpecified,
                             g_invalid_jsonrpc_string));

  ASSERT_TRUE(CheckErrorCode(JSONFormatter::kMethodNotSpecified,
                             "{"
                             "  \"jsonrpc\": \"2.0\""
                             "}"));

  ASSERT_FALSE(CheckErrorCode(JSONFormatter::kMethodNotSpecified,
                              "{"
                              "  \"jsonrpc\": \"2.0\","
                              "  \"method\": \"Method1\""
                              "}"));

  ASSERT_TRUE(CheckErrorCode(JSONFormatter::kMethodNotSpecified,
                             "{"
                             "  \"jsonrpc\": \"2.0\","
                             "  \"id\": 1,"
                             "  \"params\":"
                             "  {"
                             "    \"p\": 0"
                             "  }"
                             "}"));

  ASSERT_FALSE(CheckErrorCode(JSONFormatter::kMethodNotSpecified,
                              "{"
                              "  \"jsonrpc\": \"2.0\","
                              "  \"id\": 1,"
                              "  \"method\": \"Method1\","
                              "  \"params\":"
                              "  {"
                              "    \"p\": 0"
                              "  }"
                              "}"));

  ASSERT_TRUE(CheckErrorCode(JSONFormatter::kMethodNotSpecified,
                             "{"
                             "  \"jsonrpc\": \"2.0\","
                             "  \"id\": 1,"
                             "  \"result\":"
                             "  {"
                             "    \"p\": 0"
                             "  }"
                             "}"));

  ASSERT_FALSE(CheckErrorCode(JSONFormatter::kMethodNotSpecified,
                              "{"
                              "  \"jsonrpc\": \"2.0\","
                              "  \"id\": 1,"
                              "  \"result\":"
                              "  {"
                              "    \"method\": \"Method1\","
                              "    \"p\": 0"
                              "  }"
                              "}"));

  ASSERT_TRUE(CheckErrorCode(JSONFormatter::kMethodNotSpecified,
                             "{"
                             "  \"jsonrpc\": \"2.0\","
                             "  \"id\": 1,"
                             "  \"error\":"
                             "  {"
                             "    \"p\": 0"
                             "  }"
                             "}"));

  ASSERT_FALSE(CheckErrorCode(JSONFormatter::kMethodNotSpecified,
                              "{"
                              "  \"jsonrpc\": \"2.0\","
                              "  \"id\": 1,"
                              "  \"error\":"
                              "  {"
                              "    \"data\":"
                              "    {"
                              "      \"method\": \"Method1\""
                              "    },"
                              "    \"p\": 0"
                              "  }"
                              "}"));
}

TEST(FormatterJsonRpc, UnknownMethod) {
  ASSERT_TRUE(CheckErrorCode(JSONFormatter::kUnknownMethod,
                             g_invalid_jsonrpc_string));

  ASSERT_TRUE(CheckErrorCode(JSONFormatter::kUnknownMethod,
                             "{"
                             "  \"jsonrpc\": \"2.0\""
                             "}"));

  ASSERT_TRUE(CheckErrorCode(JSONFormatter::kUnknownMethod,
                             "{"
                             "  \"jsonrpc\": \"2.0\","
                             "  \"method\": \"Method1\""
                             "}"));

  ASSERT_FALSE(CheckErrorCode(JSONFormatter::kUnknownMethod,
                              "{"
                              "  \"jsonrpc\": \"2.0\","
                              "  \"method\": \"RegisterAppInterface\""
                              "}"));

  ASSERT_TRUE(CheckErrorCode(JSONFormatter::kUnknownMethod,
                             "{"
                             "  \"jsonrpc\": \"2.0\","
                             "  \"id\": 1,"
                             "  \"params\":"
                             "  {"
                             "    \"p\": 0"
                             "  }"
                             "}"));

  ASSERT_TRUE(CheckErrorCode(JSONFormatter::kUnknownMethod,
                             "{"
                             "  \"jsonrpc\": \"2.0\","
                             "  \"id\": 1,"
                             "  \"method\": \"Method1\","
                             "  \"params\":"
                             "  {"
                             "    \"p\": 0"
                             "  }"
                             "}"));

  ASSERT_FALSE(CheckErrorCode(JSONFormatter::kUnknownMethod,
                              "{"
                              "  \"jsonrpc\": \"2.0\","
                              "  \"id\": 1,"
                              "  \"method\": \"UnregisterAppInterface\","
                              "  \"params\":"
                              "  {"
                              "    \"p\": 0"
                              "  }"
                              "}"));
  ASSERT_TRUE(CheckErrorCode(JSONFormatter::kUnknownMethod,
                             "{"
                             "  \"jsonrpc\": \"2.0\","
                             "  \"id\": 1,"
                             "  \"result\":"
                             "  {"
                             "    \"p\": 0"
                             "  }"
                             "}"));

  ASSERT_TRUE(CheckErrorCode(JSONFormatter::kUnknownMethod,
                             "{"
                             "  \"jsonrpc\": \"2.0\","
                             "  \"id\": 1,"
                             "  \"result\":"
                             "  {"
                             "    \"method\": \"Method1\","
                             "    \"p\": 0"
                             "  }"
                             "}"));

  ASSERT_FALSE(CheckErrorCode(JSONFormatter::kUnknownMethod,
                              "{"
                              "  \"jsonrpc\": \"2.0\","
                              "  \"id\": 1,"
                              "  \"result\":"
                              "  {"
                              "    \"method\": \"SetGlobalProperties\","
                              "    \"p\": 0"
                              "  }"
                              "}"));

  ASSERT_TRUE(CheckErrorCode(JSONFormatter::kUnknownMethod,
                             "{"
                             "  \"jsonrpc\": \"2.0\","
                             "  \"id\": 1,"
                             "  \"error\":"
                             "  {"
                             "    \"p\": 0"
                             "  }"
                             "}"));

  ASSERT_TRUE(CheckErrorCode(JSONFormatter::kUnknownMethod,
                             "{"
                             "  \"jsonrpc\": \"2.0\","
                             "  \"id\": 1,"
                             "  \"error\":"
                             "  {"
                             "    \"data\":"
                             "    {"
                             "      \"method\": \"Method1\""
                             "    },"
                             "    \"p\": 0"
                             "  }"
                             "}"));

  ASSERT_FALSE(CheckErrorCode(JSONFormatter::kUnknownMethod,
                              "{"
                              "  \"jsonrpc\": \"2.0\","
                              "  \"id\": 1,"
                              "  \"error\":"
                              "  {"
                              "    \"data\":"
                              "    {"
                              "      \"method\": \"RegisterAppInterface\""
                              "    },"
                              "    \"p\": 0"
                              "  }"
                              "}"));
}

TEST(FormatterJsonRpc, UnknownMessageType) {
  ASSERT_TRUE(CheckErrorCode(JSONFormatter::kUnknownMessageType,
                             g_invalid_jsonrpc_string));

  ASSERT_FALSE(CheckErrorCode(JSONFormatter::kUnknownMessageType, "{}"));

  ASSERT_TRUE(CheckErrorCode(JSONFormatter::kUnknownMessageType,
                             "{"
                             "  \"jsonrpc\": \"2.0\","
                             "  \"id\": 1"
                             "}"));

  ASSERT_FALSE(CheckErrorCode(JSONFormatter::kUnknownMessageType,
                              "{"
                              "  \"jsonrpc\": \"2.0\""
                              "}"));

  ASSERT_FALSE(CheckErrorCode(JSONFormatter::kUnknownMessageType,
                              "{"
                              "  \"jsonrpc\": \"2.0\","
                              "  \"params\": {}"
                              "}"));

  ASSERT_FALSE(CheckErrorCode(JSONFormatter::kUnknownMessageType,
                              "{"
                              "  \"jsonrpc\": \"2.0\","
                              "  \"id\": 1,"
                              "  \"params\": {}"
                              "}"));

  ASSERT_FALSE(CheckErrorCode(JSONFormatter::kUnknownMessageType,
                              "{"
                              "  \"jsonrpc\": \"2.0\","
                              "  \"id\": 1,"
                              "  \"result\": {}"
                              "}"));

  ASSERT_FALSE(CheckErrorCode(JSONFormatter::kUnknownMessageType,
                              "{"
                              "  \"jsonrpc\": \"2.0\","
                              "  \"id\": 1,"
                              "  \"error\": {}"
                              "}"));
}

TEST(FormatterJsonRpc, InvalidId) {
  ASSERT_FALSE(CheckErrorCode(JSONFormatter::kInvalidId,
                             g_invalid_jsonrpc_string));

  ASSERT_FALSE(CheckErrorCode(JSONFormatter::kInvalidId, "{}"));

  ASSERT_FALSE(CheckErrorCode(JSONFormatter::kInvalidId,
                              "{"
                              "  \"jsonrpc\": \"2.0\","
                              "  \"id\": 1"
                              "}"));

  ASSERT_FALSE(CheckErrorCode(JSONFormatter::kInvalidId,
                              "{"
                              "  \"jsonrpc\": \"2.0\","
                              "  \"id\": 1.5"
                              "}"));

  ASSERT_FALSE(CheckErrorCode(JSONFormatter::kInvalidId,
                              "{"
                              "  \"jsonrpc\": \"2.0\","
                              "  \"id\": \"StringId\""
                              "}"));

  ASSERT_FALSE(CheckErrorCode(JSONFormatter::kInvalidId,
                              "{"
                              "  \"jsonrpc\": \"2.0\","
                              "  \"id\": Null"
                              "}"));

  ASSERT_TRUE(CheckErrorCode(JSONFormatter::kInvalidId,
                             "{"
                             "  \"jsonrpc\": \"2.0\","
                             "  \"id\": []"
                             "}"));

  ASSERT_TRUE(CheckErrorCode(JSONFormatter::kInvalidId,
                             "{"
                             "  \"jsonrpc\": \"2.0\","
                             "  \"id\": {}"
                             "}"));
}

TEST(FormatterJsonRpc, ResponseCodeNotAvailable) {
  ASSERT_FALSE(CheckErrorCode(JSONFormatter::kResponseCodeNotAvailable,
                              g_invalid_jsonrpc_string));

  ASSERT_FALSE(CheckErrorCode(JSONFormatter::kResponseCodeNotAvailable, "{}"));

  ASSERT_FALSE(CheckErrorCode(JSONFormatter::kResponseCodeNotAvailable,
                              "{"
                              "  \"jsonrpc\": \"2.0\""
                              "}"));

  ASSERT_FALSE(CheckErrorCode(JSONFormatter::kResponseCodeNotAvailable,
                              "{"
                              "  \"jsonrpc\": \"2.0\","
                              "  \"id\": 1"
                              "}"));

  ASSERT_FALSE(CheckErrorCode(JSONFormatter::kResponseCodeNotAvailable,
                              "{"
                              "  \"jsonrpc\": \"2.0\","
                              "  \"id\": 1,"
                              "  \"params\": {}"
                              "}"));

  ASSERT_TRUE(CheckErrorCode(JSONFormatter::kResponseCodeNotAvailable,
                             "{"
                             "  \"jsonrpc\": \"2.0\","
                             "  \"id\": 1,"
                             "  \"result\": {}"
                             "}"));

  ASSERT_TRUE(CheckErrorCode(JSONFormatter::kResponseCodeNotAvailable,
                             "{"
                             "  \"jsonrpc\": \"2.0\","
                             "  \"id\": 1,"
                             "  \"result\":"
                             "  {"
                             "    \"code\": \"c\""
                             "  }"
                             "}"));

  ASSERT_FALSE(CheckErrorCode(JSONFormatter::kResponseCodeNotAvailable,
                              "{"
                              "  \"jsonrpc\": \"2.0\","
                              "  \"id\": 1,"
                              "  \"result\":"
                              "  {"
                              "    \"code\": 10"
                              "  }"
                              "}"));

  ASSERT_TRUE(CheckErrorCode(JSONFormatter::kResponseCodeNotAvailable,
                             "{"
                             "  \"jsonrpc\": \"2.0\","
                             "  \"id\": 1,"
                             "  \"error\":"
                             "  {"
                             "    \"code\": \"c\""
                             "  }"
                             "}"));

  ASSERT_FALSE(CheckErrorCode(JSONFormatter::kResponseCodeNotAvailable,
                              "{"
                              "  \"jsonrpc\": \"2.0\","
                              "  \"id\": 1,"
                              "  \"error\":"
                              "  {"
                              "    \"code\": 10"
                              "  }"
                              "}"));
}

}  //namespace Formatters
}  //namespace JSONHandler
}  //namespace components
}  //namespace test


namespace func_id_ns = test::components::JSONHandler::Formatters::FunctionID;
namespace msg_type_ns = test::components::JSONHandler::Formatters::messageType;

namespace NsSmartDeviceLink {
namespace NsSmartObjects {

template<>
const std::map<func_id_ns::eType, std::string> &
NsSmartDeviceLink::NsSmartObjects::TEnumSchemaItem<func_id_ns::eType>::
getEnumElementsStringRepresentation(void) {
  static bool isInitialized = false;
  static std::map<func_id_ns::eType, std::string> enumStringRepresentationMap;

  if (false == isInitialized) {
    enumStringRepresentationMap.insert(
      std::make_pair(func_id_ns::RegisterAppInterface,
                     "RegisterAppInterface"));
    enumStringRepresentationMap.insert(
      std::make_pair(func_id_ns::UnregisterAppInterface,
                     "UnregisterAppInterface"));
    enumStringRepresentationMap.insert(
      std::make_pair(func_id_ns::SetGlobalProperties,
                     "SetGlobalProperties"));

    isInitialized = true;
  }

  return enumStringRepresentationMap;
}

template<>
const std::map<msg_type_ns::eType, std::string> &
NsSmartDeviceLink::NsSmartObjects::TEnumSchemaItem<msg_type_ns::eType>::
getEnumElementsStringRepresentation(void) {
  static bool isInitialized = false;
  static std::map<msg_type_ns::eType, std::string> enumStringRepresentationMap;

  if (false == isInitialized) {
    enumStringRepresentationMap.insert(
      std::make_pair(msg_type_ns::request, "request"));
    enumStringRepresentationMap.insert(
      std::make_pair(msg_type_ns::response, "response"));
    enumStringRepresentationMap.insert(
      std::make_pair(msg_type_ns::notification, "notification"));

    isInitialized = true;
  }

  return enumStringRepresentationMap;
}
}  //namespace NsSmartObjects
}  //namespace NsSmartDeviceLink

int main(int argc, char **argv)
{
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
