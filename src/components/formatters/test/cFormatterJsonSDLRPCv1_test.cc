/*
 * Copyright (c) 2015, Ford Motor Company
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of the Ford Motor Company nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "gtest/gtest.h"

#include "formatters/CFormatterJsonSDLRPCv1.h"
#include "create_smartSchema.h"

namespace test {
namespace components {
namespace formatters {

TEST(CFormatterJsonSDLRPCv1Test, EmptySmartObjectToString) {
  SmartObject srcObj;

  EXPECT_EQ(Errors::eType::OK, srcObj.validate());

  std::string jsonString;
  bool result = CFormatterJsonSDLRPCv1::toString(srcObj, jsonString);

  EXPECT_TRUE(result);

  std::string expectOutputJsonString =
      "{\n \
  \"\" : {\n\
      \"name\" : \"\",\n\
      \"parameters\" : \"\"\n\
   }\n\
}\n";

  EXPECT_EQ(expectOutputJsonString, jsonString);
}

TEST(CFormatterJsonSDLRPCv1Test, SmObjWithRequestWithoutMsgNotValid_ToString) {
  SmartObject srcObj;
  CSmartSchema schema = initObjectSchema();
  srcObj.setSchema(schema);

  srcObj[S_PARAMS][S_MESSAGE_TYPE] = MessageTypeTest::request;
  srcObj[S_PARAMS][S_FUNCTION_ID] = FunctionIDTest::RegisterAppInterface;
  srcObj[S_PARAMS][S_CORRELATION_ID] = 13;
  srcObj[S_PARAMS][S_PROTOCOL_TYPE] = 0;
  srcObj[S_PARAMS][S_PROTOCOL_VERSION] = 1;

  EXPECT_EQ(Errors::eType::MISSING_MANDATORY_PARAMETER, srcObj.validate());

  std::string jsonString;
  bool result = CFormatterJsonSDLRPCv1::toString(srcObj, jsonString);
  EXPECT_TRUE(result);

  std::string expectOutputJsonString =
      "{\n \
  \"request\" : {\n\
      \"correlationID\" : 13,\n\
      \"name\" : \"RegisterAppInterface\",\n\
      \"parameters\" : \"\"\n\
   }\n\
}\n";

  EXPECT_EQ(expectOutputJsonString, jsonString);
}

TEST(CFormatterJsonSDLRPCv1Test, SmObjWithRequestWithEmptyMsgWithTestSchemaToString) {
  SmartObject srcObj;
  CSmartSchema schema = initObjectSchema();
  srcObj.setSchema(schema);

  srcObj[S_PARAMS][S_MESSAGE_TYPE] = MessageTypeTest::request;
  srcObj[S_PARAMS][S_FUNCTION_ID] = FunctionIDTest::RegisterAppInterface;
  srcObj[S_PARAMS][S_CORRELATION_ID] = 13;
  srcObj[S_PARAMS][S_PROTOCOL_TYPE] = 0;
  srcObj[S_PARAMS][S_PROTOCOL_VERSION] = 1;
  srcObj[S_MSG_PARAMS][""] = "";

  EXPECT_EQ(Errors::eType::OK, srcObj.validate());

  std::string jsonString;

  bool result = CFormatterJsonSDLRPCv1::toString(srcObj, jsonString);

  EXPECT_TRUE(result);

  std::string expectOutputJsonString =
      "{\n \
  \"request\" : {\n\
      \"correlationID\" : 13,\n\
      \"name\" : \"RegisterAppInterface\",\n\
      \"parameters\" : {}\n\
   }\n\
}\n";

  EXPECT_EQ(expectOutputJsonString, jsonString);
}

TEST(CFormatterJsonSDLRPCv1Test, SmObjWithRequestWithNonemptyMsgWithTestSchemaToString) {
  SmartObject srcObj;
  CSmartSchema schema = initObjectSchema();
  srcObj.setSchema(schema);

  srcObj[S_PARAMS][S_MESSAGE_TYPE] = MessageTypeTest::request;
  srcObj[S_PARAMS][S_FUNCTION_ID] = FunctionIDTest::RegisterAppInterface;
  srcObj[S_PARAMS][S_CORRELATION_ID] = 13;
  srcObj[S_PARAMS][S_PROTOCOL_TYPE] = 0;
  srcObj[S_PARAMS][S_PROTOCOL_VERSION] = 1;
  srcObj[S_MSG_PARAMS]["info"] = "value";

  std::string jsonString;

  bool result = CFormatterJsonSDLRPCv1::toString(srcObj, jsonString);

  EXPECT_TRUE(result);

  std::string expectOutputJsonString =
      "{\n \
  \"request\" : {\n\
      \"correlationID\" : 13,\n\
      \"name\" : \"RegisterAppInterface\",\n\
      \"parameters\" : {\n\
         \"info\" : \"value\"\n\
      }\n\
   }\n\
}\n";

  EXPECT_EQ(expectOutputJsonString, jsonString);
}

TEST(CFormatterJsonSDLRPCv1Test, SmObjWithRequestWithNonemptyMsgToString) {
  SmartObject srcObj;

  srcObj[S_PARAMS][S_MESSAGE_TYPE] = MessageTypeTest::request;
  srcObj[S_PARAMS][S_FUNCTION_ID] = 5;
  srcObj[S_PARAMS][S_CORRELATION_ID] = 13;
  srcObj[S_PARAMS][S_PROTOCOL_TYPE] = 0;
  srcObj[S_PARAMS][S_PROTOCOL_VERSION] = 1;
  srcObj[S_MSG_PARAMS]["vrSynonyms"][0] = "Synonym 1";

  std::string jsonString;

  bool result = CFormatterJsonSDLRPCv1::toString(srcObj, jsonString);

  EXPECT_TRUE(result);

  std::string expectOutputJsonString =
      "{\n \
  \"0\" : {\n\
      \"correlationID\" : 13,\n\
      \"name\" : \"5\",\n\
      \"parameters\" : {\n\
         \"vrSynonyms\" : [ \"Synonym 1\" ]\n\
      }\n\
   }\n\
}\n";
  EXPECT_EQ(expectOutputJsonString, jsonString);
}

TEST(CFormatterJsonSDLRPCv1Test, SmObjWithResponseWithoutSchemaToString) {
  SmartObject srcObj;

  srcObj[S_PARAMS][S_MESSAGE_TYPE] = MessageTypeTest::response;
  srcObj[S_PARAMS][S_FUNCTION_ID] = 5;
  srcObj[S_PARAMS][S_CORRELATION_ID] = 13;
  srcObj[S_PARAMS][S_PROTOCOL_TYPE] = 0;
  srcObj[S_PARAMS][S_PROTOCOL_VERSION] = 1;
  srcObj[S_MSG_PARAMS]["success"] = true;
  srcObj[S_MSG_PARAMS]["resultCode"] = 0;

  std::string jsonString;

  bool result = CFormatterJsonSDLRPCv1::toString(srcObj, jsonString);

  EXPECT_TRUE(result);

  std::string expectOutputJsonString =
      "{\n \
  \"1\" : {\n\
      \"correlationID\" : 13,\n\
      \"name\" : \"5\",\n\
      \"parameters\" : {\n\
         \"resultCode\" : 0,\n\
         \"success\" : true\n\
      }\n\
   }\n\
}\n";
  EXPECT_EQ(expectOutputJsonString, jsonString);
}

TEST(CFormatterJsonSDLRPCv1Test, SmObjWithNotificationToString) {
  SmartObject srcObj;
  CSmartSchema schema = initObjectSchema();
  srcObj.setSchema(schema);

  srcObj[S_PARAMS][S_MESSAGE_TYPE] = MessageTypeTest::notification;
  srcObj[S_PARAMS][S_FUNCTION_ID] = FunctionIDTest::SetGlobalProperties;
  srcObj[S_PARAMS][S_CORRELATION_ID] = 13;
  srcObj[S_PARAMS][S_PROTOCOL_TYPE] = 0;
  srcObj[S_PARAMS][S_PROTOCOL_VERSION] = 1;
  srcObj[S_MSG_PARAMS][""] = "";
  std::string jsonString;

  bool result = CFormatterJsonSDLRPCv1::toString(srcObj, jsonString);

  EXPECT_TRUE(result);

  std::string expectOutputJsonString =
      "{\n \
  \"notification\" : {\n\
      \"correlationID\" : 13,\n\
      \"name\" : \"SetGlobalProperties\",\n\
      \"parameters\" : {}\n\
   }\n\
}\n";

  EXPECT_EQ(expectOutputJsonString, jsonString);
}

TEST(CFormatterJsonSDLRPCv1Test, SmObjWithResponseToString) {
  SmartObject srcObj;

  CSmartSchema schema = initObjectSchema();
  srcObj.setSchema(schema);

  srcObj[S_PARAMS][S_MESSAGE_TYPE] = MessageTypeTest::response;
  srcObj[S_PARAMS][S_FUNCTION_ID] = FunctionIDTest::RegisterAppInterface;
  srcObj[S_PARAMS][S_CORRELATION_ID] = 13;
  srcObj[S_PARAMS][S_PROTOCOL_TYPE] = 0;
  srcObj[S_PARAMS][S_PROTOCOL_VERSION] = 1;

  srcObj[S_MSG_PARAMS]["success"] = true;
  srcObj[S_MSG_PARAMS]["resultCode"] = TestType::SUCCESS;

  std::string jsonString;

  bool result = CFormatterJsonSDLRPCv1::toString(srcObj, jsonString);

  EXPECT_TRUE(result);

  std::string expectOutputJsonString =
      "{\n \
  \"response\" : {\n\
      \"correlationID\" : 13,\n\
      \"name\" : \"RegisterAppInterface\",\n\
      \"parameters\" : {\n\
         \"resultCode\" : \"SUCCESS\",\n\
         \"success\" : true\n\
      }\n\
   }\n\
}\n";

  EXPECT_EQ(expectOutputJsonString, jsonString);
}

TEST(CFormatterJsonSDLRPCv1Test, SmObjWithResponseWithoutSchemaWithoutParamsToString) {
  SmartObject srcObj;
  srcObj[S_PARAMS][S_MESSAGE_TYPE] = MessageTypeTest::response;
  std::string jsonString;

  bool result = CFormatterJsonSDLRPCv1::toString(srcObj, jsonString);

  EXPECT_TRUE(result);

  std::string expectOutputJsonString =
      "{\n \
  \"1\" : {\n\
      \"name\" : \"\",\n\
      \"parameters\" : \"\"\n\
   }\n\
}\n";

  EXPECT_EQ(expectOutputJsonString, jsonString);
}

TEST(CFormatterJsonSDLRPCv1Test, StringRequestToSmObj) {
  std::string inputJsonString =
      "\
          {\
              \"request\": {\
                  \"correlationID\": 5,\
                  \"name\" : \"RegisterAppInterface\",\n\
                  \"parameters\": {\
                      \"syncMsgVersion\" : {\
                          \"majorVersion\" : 2,\
                          \"minorVersion\" : 10\
                      },\
                      \"appName\": \"some app name\",\
                      \"ttsName\": [{\
                          \"text\": \"ABC\",\
                          \"type\": \"TEXT\"\
                      }],\
                     \"vrSynonyms\": [\"Synonym 1\", \"Synonym 2\"]\
                  }\
              }\
          }";

  SmartObject obj;

  CSmartSchema schema = initObjectSchema();
  obj.setSchema(schema);

  bool result = CFormatterJsonSDLRPCv1::fromString<FunctionIDTest::eType,
      MessageTypeTest::eType>(inputJsonString, obj);

  EXPECT_EQ(CFormatterJsonSDLRPCv1::kSuccess, result);
  EXPECT_EQ(Errors::eType::OK, obj.validate());
  EXPECT_EQ(obj[S_PARAMS][S_MESSAGE_TYPE], MessageTypeTest::request);
  EXPECT_EQ(obj[S_PARAMS][S_FUNCTION_ID], FunctionIDTest::RegisterAppInterface);
  EXPECT_EQ(obj[S_PARAMS][S_CORRELATION_ID], 5);
  EXPECT_EQ(obj[S_PARAMS][S_PROTOCOL_TYPE], 0);
  EXPECT_EQ(obj[S_PARAMS][S_PROTOCOL_VERSION], 1);
  EXPECT_EQ(obj[S_MSG_PARAMS]["appName"], "some app name");

  EXPECT_EQ(obj[S_MSG_PARAMS]["syncMsgVersion"]["majorVersion"], 2);
  EXPECT_EQ(obj[S_MSG_PARAMS]["syncMsgVersion"]["minorVersion"], 10);
  EXPECT_EQ(obj[S_MSG_PARAMS]["ttsName"][0]["text"], "ABC");
  EXPECT_EQ(obj[S_MSG_PARAMS]["ttsName"][0]["type"], "TEXT");
  EXPECT_EQ(obj[S_MSG_PARAMS]["vrSynonyms"][0], "Synonym 1");
  EXPECT_EQ(obj[S_MSG_PARAMS]["vrSynonyms"][1], "Synonym 2");
}

TEST(CFormatterJsonSDLRPCv1Test, StringRequestWithoutNameToSmartObject) {
  std::string inputJsonString =
      "\
          {\
              \"request\": {\
                  \"correlationID\": 5,\
                  \"parameters\": {\
                      \"syncMsgVersion\" : {\
                          \"majorVersion\" : 2,\
                          \"minorVersion\" : 10\
                      },\
                      \"appName\": \"some app name\",\
                      \"ttsName\": [{\
                          \"text\": \"ABC\",\
                          \"type\": \"TEXT\"\
                      }],\
                     \"vrSynonyms\": [\"Synonym 1\", \"Synonym 2\"]\
                  }\
              }\
          }";

  SmartObject obj;

  bool result = CFormatterJsonSDLRPCv1::fromString<FunctionIDTest::eType,
      MessageTypeTest::eType>(inputJsonString, obj);

  EXPECT_EQ(CFormatterJsonSDLRPCv1::kParsingError, result);

  EXPECT_EQ(obj[S_PARAMS][S_MESSAGE_TYPE], MessageTypeTest::request);
  EXPECT_EQ(obj[S_PARAMS][S_FUNCTION_ID], "-1");
  EXPECT_EQ(obj[S_PARAMS][S_CORRELATION_ID], 5);
  EXPECT_EQ(obj[S_MSG_PARAMS]["appName"], "some app name");

  EXPECT_EQ(obj[S_MSG_PARAMS]["syncMsgVersion"]["majorVersion"], 2);
  EXPECT_EQ(obj[S_MSG_PARAMS]["syncMsgVersion"]["minorVersion"], 10);
  EXPECT_EQ(obj[S_MSG_PARAMS]["ttsName"][0]["text"], "ABC");
  EXPECT_EQ(obj[S_MSG_PARAMS]["ttsName"][0]["type"], "TEXT");
  EXPECT_EQ(obj[S_MSG_PARAMS]["vrSynonyms"][0], "Synonym 1");
  EXPECT_EQ(obj[S_MSG_PARAMS]["vrSynonyms"][1], "Synonym 2");
}

TEST(CFormatterJsonSDLRPCv1Test, StringRequestWithIncorrectCorIDToSmartObject) {
  std::string inputJsonString =
      "\
          {\
              \"request\": {\
                  \"correlationID\": \"5\",\
                  \"parameters\": {\
                      \"syncMsgVersion\" : {\
                          \"majorVersion\" : 2,\
                          \"minorVersion\" : 10\
                      },\
                      \"appName\": \"some app name\",\
                      \"ttsName\": [{\
                          \"text\": \"ABC\",\
                          \"type\": \"TEXT\"\
                      }],\
                     \"vrSynonyms\": [\"Synonym 1\", \"Synonym 2\"]\
                  }\
              }\
          }";

  SmartObject obj;

  bool result = CFormatterJsonSDLRPCv1::fromString<FunctionIDTest::eType,
      MessageTypeTest::eType>(inputJsonString, obj);
  EXPECT_EQ(CFormatterJsonSDLRPCv1::kParsingError, result);

  EXPECT_EQ(obj[S_PARAMS][S_MESSAGE_TYPE], MessageTypeTest::request);
  EXPECT_EQ(obj[S_PARAMS][S_FUNCTION_ID], "-1");
  EXPECT_EQ(obj[S_MSG_PARAMS]["appName"], "some app name");
  EXPECT_EQ(obj[S_MSG_PARAMS]["syncMsgVersion"]["majorVersion"], 2);
  EXPECT_EQ(obj[S_MSG_PARAMS]["ttsName"][0]["text"], "ABC");
  EXPECT_EQ(obj[S_MSG_PARAMS]["vrSynonyms"][0], "Synonym 1");
}

TEST(CFormatterJsonSDLRPCv1Test, StringResponceToSmartObject) {
  std::string inputJsonString =
      "{\n \
  \"response\" : {\n\
      \"correlationID\" : 13,\n\
      \"name\" : \"RegisterAppInterface\",\n\
      \"parameters\" : {\n\
         \"resultCode\" : \"SUCCESS\",\n\
         \"success\" : true\n\
      }\n\
   }\n\
}\n";

  SmartObject obj;

  CSmartSchema schema = initObjectSchema();
  obj.setSchema(schema);

  bool result = CFormatterJsonSDLRPCv1::fromString<FunctionIDTest::eType,
      MessageTypeTest::eType>(inputJsonString, obj);
  EXPECT_EQ(CFormatterJsonSDLRPCv1::kSuccess, result);
  EXPECT_EQ(obj[S_PARAMS][S_MESSAGE_TYPE], MessageTypeTest::response);
  EXPECT_EQ(obj[S_PARAMS][S_FUNCTION_ID], 0);
  EXPECT_EQ(obj[S_PARAMS][S_CORRELATION_ID], 13);
  EXPECT_EQ(obj[S_PARAMS][S_PROTOCOL_TYPE], 0);
  EXPECT_EQ(obj[S_PARAMS][S_PROTOCOL_VERSION], 1);
  EXPECT_EQ(obj[S_MSG_PARAMS]["resultCode"], "SUCCESS");
  EXPECT_EQ(obj[S_MSG_PARAMS]["success"], true);
}

TEST(CFormatterJsonSDLRPCv1Test, StringNotificationToSmartObject) {
  std::string inputJsonString =
      "{\n \
  \"notification\" : {\n\
      \"correlationID\" : 13,\n\
      \"name\" : \"SetGlobalProperties\",\n\
      \"parameters\" : {}\n\
   }\n\
}\n";

  SmartObject obj;

  CSmartSchema schema = initObjectSchema();
  obj.setSchema(schema);

  bool result = CFormatterJsonSDLRPCv1::fromString<FunctionIDTest::eType,
      MessageTypeTest::eType>(inputJsonString, obj);
  EXPECT_EQ(CFormatterJsonSDLRPCv1::kSuccess, result);
  EXPECT_EQ(Errors::eType::OK, obj.validate());
  EXPECT_EQ(obj[S_PARAMS][S_MESSAGE_TYPE], MessageTypeTest::notification);
  EXPECT_EQ(obj[S_PARAMS][S_FUNCTION_ID], FunctionIDTest::SetGlobalProperties);
  EXPECT_EQ(obj[S_PARAMS][S_CORRELATION_ID], 13);
  EXPECT_EQ(obj[S_PARAMS][S_PROTOCOL_TYPE], 0);
  EXPECT_EQ(obj[S_PARAMS][S_PROTOCOL_VERSION], 1);
}

TEST(CFormatterJsonSDLRPCv1Test, MetaFormatToString) {
  SmartObject srcObj;

  srcObj[S_PARAMS][S_MESSAGE_TYPE] = MessageTypeTest::request;
  srcObj[S_PARAMS][S_FUNCTION_ID] = FunctionIDTest::RegisterAppInterface;
  srcObj[S_PARAMS][S_CORRELATION_ID] = 13;
  srcObj[S_PARAMS][S_PROTOCOL_TYPE] = 0;
  srcObj[S_PARAMS][S_PROTOCOL_VERSION] = 1;
  srcObj[S_MSG_PARAMS]["info"] = "value";

  std::string jsonString;

  CSmartSchema schema = initObjectSchema();
  srcObj.setSchema(schema);

  meta_formatter_error_code::tMetaFormatterErrorCode result =
      CFormatterJsonSDLRPCv1::MetaFormatToString(srcObj, schema, jsonString);
  EXPECT_EQ(meta_formatter_error_code::kErrorOk, result);
}

} // namespace formatters
} // namespace components
} // namespace test
