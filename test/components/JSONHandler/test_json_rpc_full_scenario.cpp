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

#include <string>

#include "gtest/gtest.h"

#include "JSONHandler/formatters/formatter_json_rpc.h"
#include "test/components/JSONHandler/test_json_rpc.h"


namespace test {
namespace components {
namespace JSONHandler {
namespace test_json_rpc_full_scenario {

namespace so = NsSmartDeviceLink::NsSmartObjects;
namespace gen = gen::test::components::json_rpc;
namespace fm = NsSmartDeviceLink::NsJSONHandler::Formatters;
namespace jhs = NsSmartDeviceLink::NsJSONHandler::strings;

TEST(test_general, test_json_rpc_full) {
  std::string input_json =
  "{\n"
  "   \"id\" : 1,\n"
  "   \"jsonrpc\" : \"2.0\",\n"
  "   \"method\" : \"interface1.Function1\",\n"
  "   \"params\" : {\n"
  "      \"param1\" : \"String Value\",\n"
  "      \"param2\" : 13,\n"
  "      \"param3\" : {\n"
  "         \"member1\" : 1,\n"
  "         \"member2\" : true,\n"
  "         \"member3\" : 13.130,\n"
  "         \"member4\" : [ 30, 40, 50 ]\n"
  "      }\n"
  "   }\n"
  "}\n";

  so::CSmartObject object;
  ASSERT_TRUE(fm::FormatterJsonRpc::kSuccess ==
              (fm::FormatterJsonRpc::FromString<gen::FunctionID::eType,
                                                gen::messageType::eType>(
      input_json, object)));

  ASSERT_EQ(gen::FunctionID::interface1_Function1,
            (int)object[jhs::S_PARAMS][jhs::S_FUNCTION_ID]);
  ASSERT_EQ(gen::messageType::request,
            (int)object[jhs::S_PARAMS][jhs::S_MESSAGE_TYPE]);

  gen::test_json_rpc factory;
  ASSERT_TRUE(factory.attachSchema(object));
  ASSERT_EQ(so::Errors::OK, object.isValid());

  std::string output_json;
  ASSERT_TRUE(fm::FormatterJsonRpc::ToString(object, output_json));

  ASSERT_EQ(input_json, output_json);
}

TEST(test_JSONRPC_general, test_AttachSchema) {
  so::CSmartObject object(so::SmartType_Map);
  gen::test_json_rpc factory;

  ASSERT_FALSE(factory.AttachSchema(gen::StructIdentifiers::INVALID_ENUM,
                                    object));

  ASSERT_TRUE(factory.AttachSchema(
      gen::StructIdentifiers::interface1_struct1,
      object));

  object["member1"] = 1;
  object["member2"] = true;
  object["member3"] = 13.1313;
  object["member4"][0] = 12;

  ASSERT_EQ(so::Errors::OK, object.isValid());

  object["member3"] = 29.0;

  ASSERT_EQ(so::Errors::OUT_OF_RANGE, object.isValid());

  object["member3"] = 13.0;
  object["xxx"] = 1234;

  ASSERT_EQ(so::Errors::UNEXPECTED_PARAMETER, object.isValid());
}

TEST(test_JSONRPC_general, test_SmartObjectCreation) {
  gen::test_json_rpc factory;

  so::CSmartObject object = factory.CreateSmartObject(
      gen::StructIdentifiers::INVALID_ENUM);

  ASSERT_EQ(so::SmartType_Null, object.getType());

  object = factory.CreateSmartObject(gen::FunctionID::INVALID_ENUM,
                                     gen::messageType::INVALID_ENUM);

  ASSERT_EQ(so::SmartType_Null, object.getType());

  object = factory.CreateSmartObject(gen::FunctionID::interface1_Function1,
                                     gen::messageType::INVALID_ENUM);

  ASSERT_EQ(so::SmartType_Null, object.getType());

  object = factory.CreateSmartObject(gen::FunctionID::INVALID_ENUM,
                                     gen::messageType::response);

  ASSERT_EQ(so::SmartType_Null, object.getType());

  object = factory.CreateSmartObject(
      gen::StructIdentifiers::interface1_struct2);

  ASSERT_EQ(so::SmartType_Map, object.getType());

  object["m1"] = "xxx";
  object["m2"][0] = "yyy";
  object["m3"] = gen::interface1_enum1::element1;
  object["m4"][0]["member1"] = 1;
  object["m4"][0]["member2"] = true;
  object["m4"][0]["member3"] = 13.1313;
  object["m4"][0]["member4"][0] = 12;

  ASSERT_EQ(so::Errors::OK, object.isValid());

  object["zzz"] = "yyy";

  ASSERT_EQ(so::Errors::UNEXPECTED_PARAMETER, object.isValid());

  object = factory.CreateSmartObject(gen::FunctionID::interface2_Function1,
                                     gen::messageType::notification);

  object[jhs::S_PARAMS][jhs::S_FUNCTION_ID] =
      gen::FunctionID::interface2_Function1;
  object[jhs::S_PARAMS][jhs::S_MESSAGE_TYPE] =
      gen::messageType::notification;
  object[jhs::S_PARAMS][jhs::S_PROTOCOL_VERSION] = 2;
  object[jhs::S_PARAMS][jhs::S_PROTOCOL_TYPE] = 111;
  object[jhs::S_MSG_PARAMS]["param"] = gen::interface2_enum2::element2;
  object[jhs::S_MSG_PARAMS]["i1"]["m1"] = "xxx";
  object[jhs::S_MSG_PARAMS]["i1"]["m2"][0] = "yyy";
  object[jhs::S_MSG_PARAMS]["i1"]["m3"] = gen::interface1_enum1::element1;
  object[jhs::S_MSG_PARAMS]["i1"]["m4"][0]["member1"] = 1;
  object[jhs::S_MSG_PARAMS]["i1"]["m4"][0]["member2"] = true;
  object[jhs::S_MSG_PARAMS]["i1"]["m4"][0]["member3"] = 13.1313;
  object[jhs::S_MSG_PARAMS]["i1"]["m4"][0]["member4"][0] = 12;

  ASSERT_EQ(so::Errors::OK, object.isValid());

  object[jhs::S_MSG_PARAMS]["Noise"] = "Bzzzzzz!!!";

  ASSERT_EQ(so::Errors::UNEXPECTED_PARAMETER, object.isValid());
}

TEST(test_JSONRPC_general, test_GetSmartSchema) {
  gen::test_json_rpc factory;
  so::CSmartSchema schema;
  
  ASSERT_FALSE(factory.GetSchema(gen::StructIdentifiers::INVALID_ENUM,
                                 schema));

  ASSERT_FALSE(factory.GetSchema(gen::FunctionID::INVALID_ENUM,
                                 gen::messageType::INVALID_ENUM,
                                 schema));

  ASSERT_FALSE(factory.GetSchema(gen::FunctionID::interface1_Function1,
                                 gen:: messageType::INVALID_ENUM,
                                 schema));

  ASSERT_FALSE(factory.GetSchema(gen::FunctionID::INVALID_ENUM,
                                 gen::messageType::response,
                                 schema));

  ASSERT_TRUE(factory.GetSchema(gen::StructIdentifiers::interface1_struct1,
                                schema));

  so::CSmartObject object(so::SmartType_Map);
  object.setSchema(schema);

  object["member1"] = 1;
  object["member2"] = true;
  object["member3"] = 13.1313;
  object["member4"][0] = 12;
  
  ASSERT_EQ(so::Errors::OK, object.isValid());

  object["member3"] = 1000.0;

  ASSERT_EQ(so::Errors::OUT_OF_RANGE, object.isValid());

  object["member3"] = 13.1313;
  object["zzzz"] = 200;

  ASSERT_EQ(so::Errors::UNEXPECTED_PARAMETER,
            object.isValid());

  ASSERT_TRUE(factory.GetSchema(gen::FunctionID::interface1_Function1,
                                gen::messageType::request,
                                schema));

  object = so::CSmartObject(so::SmartType_Map);
  object.setSchema(schema);

  object[jhs::S_PARAMS][jhs::S_FUNCTION_ID] =
      gen::FunctionID::interface1_Function1;
  object[jhs::S_PARAMS][jhs::S_MESSAGE_TYPE] = gen::messageType::request;
  object[jhs::S_PARAMS][jhs::S_CORRELATION_ID] = 22;
  object[jhs::S_PARAMS][jhs::S_PROTOCOL_VERSION] = 1;
  object[jhs::S_PARAMS][jhs::S_PROTOCOL_TYPE] = 1;
  object[jhs::S_MSG_PARAMS]["param2"] = 10;

  ASSERT_EQ(so::Errors::OK, object.isValid());

  object[jhs::S_PARAMS]["blah-blah"] = "YouShallNotPass!";

  ASSERT_EQ(so::Errors::UNEXPECTED_PARAMETER,
            object.isValid());
}

} // namespace test_json_rpc_full_scenario
} // namespace JSONHandler
} // namespace components
} // namespace test

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}



