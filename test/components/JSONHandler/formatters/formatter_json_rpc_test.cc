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
#include "test/components/JSONHandler/test_json_rpc.h"

#include "json/json.h"

#include "CFormatterTestHelper.hpp"

namespace test {
namespace components {
namespace JSONHandler {
namespace Formatters {

using namespace NsSmartDeviceLink::NsJSONHandler::strings;
using namespace gen::test::components::json_rpc;
typedef NsSmartDeviceLink::NsJSONHandler::Formatters::FormatterJsonRpc JSONFormatter;
typedef NsSmartDeviceLink::NsJSONHandler::Formatters::CFormatterJsonBase FBase;

/**
 * @brief Convert SmartObject to JSON string.
 *
 * @param obj Source SmartObject to convert.
 * @param[out] result_string Output JSON string.
 */
void AnyObjectToJsonString(
  const NsSmartDeviceLink::NsSmartObjects::CSmartObject& obj,
    std::string& result_string) {

  Json::Value params(Json::objectValue);
  NsSmartDeviceLink::NsSmartObjects::CSmartObject formattedObj(obj);
  FBase::objToJsonValue(formattedObj, params);
  result_string = params.toStyledString();
}

TEST_F(CFormatterTestHelper, ToString) {

  std::string str;
  NsSmartDeviceLink::NsSmartObjects::CSmartObject srcObj;
  NsSmartDeviceLink::NsSmartObjects::CSmartObject dstObj;

  NsSmartDeviceLink::NsSmartObjects::CSmartSchema schema;
  test_json_rpc factory;
  factory.GetSchema(FunctionID::interface1_Function1,
                                messageType::request,
                                schema);

  srcObj.setSchema(schema);

  srcObj[S_PARAMS][S_FUNCTION_ID] = FunctionID::interface1_Function1;
  srcObj[S_PARAMS][S_MESSAGE_TYPE] = messageType::request;
  srcObj[S_PARAMS][S_PROTOCOL_VERSION] = 2;
  srcObj[S_PARAMS][S_PROTOCOL_TYPE] = 1;
  srcObj[S_PARAMS][S_CORRELATION_ID] = 123;

  srcObj[S_MSG_PARAMS]["param1"] = "Param1_string";
  srcObj[S_MSG_PARAMS]["param2"] = 123456789;

  srcObj[S_MSG_PARAMS]["param3"]["member1"] = 1;
  srcObj[S_MSG_PARAMS]["param3"]["member2"] = true;
  srcObj[S_MSG_PARAMS]["param3"]["member3"] = 13.3;
  srcObj[S_MSG_PARAMS]["param3"]["member4"][0] = 13;
  srcObj[S_MSG_PARAMS]["param3"]["member4"][1] = 14;

  ASSERT_EQ(NsSmartDeviceLink::NsSmartObjects::Errors::OK, srcObj.isValid());

  // SmartObjects --> JSON
  ASSERT_TRUE(JSONFormatter::ToString(srcObj, str));

  // JSON --> SmartObjects
  int result;
  result = JSONFormatter::FromString<FunctionID::eType, messageType::eType>(str, dstObj);
  ASSERT_TRUE(JSONFormatter::kSuccess == result);

  // Compare SmartObjects
  compareObjects(srcObj, dstObj);

  NsSmartDeviceLink::NsSmartObjects::CSmartObject srcObj2;
  NsSmartDeviceLink::NsSmartObjects::CSmartObject dstObj2;
  factory.GetSchema(FunctionID::interface2_Function1,
                                  messageType::notification,
                                  schema);

  srcObj2.setSchema(schema);

  srcObj2[S_PARAMS][S_FUNCTION_ID] = FunctionID::interface2_Function1;
  srcObj2[S_PARAMS][S_MESSAGE_TYPE] = messageType::notification;
  srcObj2[S_PARAMS][S_PROTOCOL_VERSION] = 2;
  srcObj2[S_PARAMS][S_PROTOCOL_TYPE] = 1;

  srcObj2[S_MSG_PARAMS]["param"] = interface2_enum2::element2;
  srcObj2[S_MSG_PARAMS]["i1"]["m1"] = "if1_struct2";
  srcObj2[S_MSG_PARAMS]["i1"]["m2"][0] = "arr_str";
  srcObj2[S_MSG_PARAMS]["i1"]["m3"] = interface1_enum1::element1;
  srcObj2[S_MSG_PARAMS]["i1"]["m4"][0]["member1"] = 1313;
  srcObj2[S_MSG_PARAMS]["i1"]["m4"][0]["member2"] = true;
  srcObj2[S_MSG_PARAMS]["i1"]["m4"][0]["member3"] = 0.01;
  srcObj2[S_MSG_PARAMS]["i1"]["m4"][0]["member4"][0] = 99;

  ASSERT_EQ(NsSmartDeviceLink::NsSmartObjects::Errors::OK, srcObj2.isValid());

  // SmartObjects --> JSON
  ASSERT_TRUE(JSONFormatter::ToString(srcObj2, str));

  // JSON --> SmartObjects
  result =  JSONFormatter::FromString<FunctionID::eType, messageType::eType>(str, dstObj2);
  ASSERT_TRUE(JSONFormatter::kSuccess == result);

  factory.attachSchema(dstObj2);

  // Compare SmartObjects
  compareObjects(srcObj2, dstObj2);
}

TEST_F(CFormatterTestHelper, FromString) {

  std::string srcStr = "\
        {\
            \"id\": 99,\
            \"jsonrpc\": \"2.0\",\
            \"result\": {\
                \"code\": 0,\
                \"method\": \"interface1.Function1\",\
                \"p1\": 10,\
                \"p2\": 12,\
                \"p3\": true\
            }\
        }";

  NsSmartDeviceLink::NsSmartObjects::CSmartObject dstObj;

  int result =
      JSONFormatter::FromString<FunctionID::eType, messageType::eType>(
          srcStr, dstObj);

  ASSERT_TRUE(JSONFormatter::kSuccess == result);

  NsSmartDeviceLink::NsSmartObjects::CSmartSchema schema;
  test_json_rpc factory;
  factory.GetSchema(FunctionID::interface1_Function1,
                                messageType::response,
                                schema);
  dstObj.setSchema(schema);

  ASSERT_EQ(NsSmartDeviceLink::NsSmartObjects::Errors::OK, dstObj.isValid());

  ASSERT_EQ(99, (int)dstObj[S_PARAMS][S_CORRELATION_ID]);
  ASSERT_EQ(2, (int)dstObj[S_PARAMS][S_PROTOCOL_VERSION]);
  ASSERT_EQ(messageType::response, (int)dstObj[S_PARAMS][S_MESSAGE_TYPE]);
  ASSERT_EQ(0, (int)dstObj[S_PARAMS][kCode]);
  ASSERT_EQ(FunctionID::interface1_Function1, (int)dstObj[S_PARAMS][S_FUNCTION_ID]);
  ASSERT_EQ(interface1_enum1::element1, (int)dstObj[S_MSG_PARAMS]["p1"]);
  ASSERT_EQ(interface1_enum1::element3, (int)dstObj[S_MSG_PARAMS]["p2"]);
  ASSERT_EQ(true, (bool)dstObj[S_MSG_PARAMS]["p3"]);
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
                              "  \"method\": \"interface1.Function1\""
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
                              "  \"method\": \"interface1.Function2\","
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
                              "    \"method\": \"interface2.Function1\","
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
                              "      \"method\": \"interface1.Function1\""
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

TEST(FormatterJsonRpc, ErrorResponseMessageNotAvailable) {
  ASSERT_FALSE(CheckErrorCode(JSONFormatter::kErrorResponseMessageNotAvailable,
                              g_invalid_jsonrpc_string));

  ASSERT_FALSE(CheckErrorCode(JSONFormatter::kErrorResponseMessageNotAvailable,
                              "{}"));

  ASSERT_FALSE(CheckErrorCode(JSONFormatter::kErrorResponseMessageNotAvailable,
                              "{"
                              "  \"jsonrpc\": \"2.0\""
                              "}"));

  ASSERT_FALSE(CheckErrorCode(JSONFormatter::kErrorResponseMessageNotAvailable,
                              "{"
                              "  \"jsonrpc\": \"2.0\","
                              "  \"id\": 1"
                              "}"));

  ASSERT_FALSE(CheckErrorCode(JSONFormatter::kErrorResponseMessageNotAvailable,
                              "{"
                              "  \"jsonrpc\": \"2.0\","
                              "  \"id\": 1,"
                              "  \"params\": {}"
                              "}"));

  ASSERT_FALSE(CheckErrorCode(JSONFormatter::kErrorResponseMessageNotAvailable,
                              "{"
                              "  \"jsonrpc\": \"2.0\","
                              "  \"id\": 1,"
                              "  \"result\": {}"
                              "}"));

  ASSERT_FALSE(CheckErrorCode(JSONFormatter::kErrorResponseMessageNotAvailable,
                              "{"
                              "  \"jsonrpc\": \"2.0\","
                              "  \"id\": 1,"
                              "  \"result\":"
                              "  {"
                              "    \"message\": \"c\""
                              "  }"
                              "}"));

  ASSERT_FALSE(CheckErrorCode(JSONFormatter::kErrorResponseMessageNotAvailable,
                              "{"
                              "  \"jsonrpc\": \"2.0\","
                              "  \"id\": 1,"
                              "  \"result\":"
                              "  {"
                              "    \"message\": 1"
                              "  }"
                              "}"));

  ASSERT_TRUE(CheckErrorCode(JSONFormatter::kErrorResponseMessageNotAvailable,
                             "{"
                             "  \"jsonrpc\": \"2.0\","
                             "  \"id\": 1,"
                             "  \"error\": 10"
                             "}"));

  ASSERT_TRUE(CheckErrorCode(JSONFormatter::kErrorResponseMessageNotAvailable,
                             "{"
                             "  \"jsonrpc\": \"2.0\","
                             "  \"id\": 1,"
                             "  \"error\":"
                             "  {"
                             "    \"code\": 1"
                             "  }"
                             "}"));

  ASSERT_TRUE(CheckErrorCode(JSONFormatter::kErrorResponseMessageNotAvailable,
                             "{"
                             "  \"jsonrpc\": \"2.0\","
                             "  \"id\": 1,"
                             "  \"error\":"
                             "  {"
                             "    \"message\": 1"
                             "  }"
                             "}"));

  ASSERT_FALSE(CheckErrorCode(JSONFormatter::kErrorResponseMessageNotAvailable,
                              "{"
                              "  \"jsonrpc\": \"2.0\","
                              "  \"id\": 1,"
                              "  \"error\":"
                              "  {"
                              "    \"message\": \"Error message\""
                              "  }"
                              "}"));
}

TEST(FormatterJsonRpc, MessageType) {
  NsSmartDeviceLink::NsSmartObjects::CSmartObject out;

  ASSERT_TRUE((JSONFormatter::kSuccess ==
      JSONFormatter::FromString<FunctionID::eType, messageType::eType>(
          "{"
          "  \"jsonrpc\": \"2.0\","
          "  \"method\": \"interface2.Function1\","
          "  \"params\": {}"
          "}",
          out)));

  ASSERT_TRUE(messageType::notification ==
      out[S_PARAMS][S_MESSAGE_TYPE].asInt());

  ASSERT_TRUE((JSONFormatter::kSuccess ==
      JSONFormatter::FromString<FunctionID::eType, messageType::eType>(
          "{"
          "  \"jsonrpc\": \"2.0\","
          "  \"method\": \"interface2.Function1\","
          "  \"id\": 1,"
          "  \"params\": {}"
          "}",
          out)));

  ASSERT_TRUE(messageType::request == out[S_PARAMS][S_MESSAGE_TYPE].asInt());

  ASSERT_TRUE((JSONFormatter::kSuccess ==
      JSONFormatter::FromString<FunctionID::eType, messageType::eType>(
          "{"
          "  \"jsonrpc\": \"2.0\","
          "  \"id\": 1,"
          "  \"result\": {"
          "    \"method\": \"interface2.Function1\","
          "    \"code\": 1"
          "  }"
          "}",
          out)));

  ASSERT_TRUE(messageType::response == out[S_PARAMS][S_MESSAGE_TYPE].asInt());

  ASSERT_TRUE((JSONFormatter::kSuccess ==
      JSONFormatter::FromString<FunctionID::eType, messageType::eType>(
          "{"
          "  \"jsonrpc\": \"2.0\","
          "  \"id\": 1,"
          "  \"error\": {"
          "    \"data\": {"
          "      \"method\": \"interface2.Function1\""
          "    },"
          "    \"code\": 1,"
          "    \"message\": \"Fail\""
          "  }"
          "}",
          out)));

  ASSERT_TRUE(messageType::error_response ==
      out[S_PARAMS][S_MESSAGE_TYPE].asInt());
}

}  //namespace Formatters
}  //namespace JSONHandler
}  //namespace components
}  //namespace test


int main(int argc, char **argv)
{
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
