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

#include "gmock/gmock.h"

#include <test_rpc_interface/interface.h>
#include <test_rpc_interface/functions.h>

#include "json/reader.h"
#include "json/writer.h"
#include "rpc_base/gtest_support.h"

namespace test {
using namespace rpc::test_rpc_interface;
using Json::Value;

class TestRequestHandlerMock: public request::Handler {
public:
  MOCK_METHOD1(HandleAddSubMenu, void(const request::AddSubMenu& params));
  MOCK_METHOD1(HandleDiagnosticMessage, void(const request::DiagnosticMessage& params));
  ~TestRequestHandlerMock() {}
};

class GeneratedInterfaceTests: public ::testing::Test {
 public:
  Json::Value JsonValue(const char* json) {
    Json::Value value;
    reader.parse(json, value);
    return value;
  }
  Json::Reader reader;
  Json::FastWriter writer;
};

TEST_F(GeneratedInterfaceTests, ScrollableMessageTest) {
  const char* org_json = "{\"reason\":\"MASTER_RESET\"}\n";
  Value json_value = JsonValue(org_json);
  notification::OnAppInterfaceUnregistered oaiu(&json_value);
  ASSERT_TRUE(oaiu.is_initialized());
  ASSERT_RPCTYPE_VALID(oaiu);

  std::string serialized = writer.write(oaiu.ToJsonValue());
  ASSERT_EQ(org_json, serialized);
}

TEST_F(GeneratedInterfaceTests, FunctionWithoutParams) {
  notification::OnAudioPassThru oapt;
  ASSERT_FALSE(oapt.is_initialized());
  ASSERT_FALSE(oapt.is_valid());
  oapt.mark_initialized();
  ASSERT_TRUE(oapt.is_initialized());
  ASSERT_RPCTYPE_VALID(oapt);
  std::string serialized = writer.write(oapt.ToJsonValue());
  ASSERT_EQ("{}\n", serialized);
}

TEST_F(GeneratedInterfaceTests, DefValueTest) {
  const char* org_json = "{\"menuID\":2,\"menuName\":\"Hello\"}";
  const char* awaited_json = "{\"menuID\":2,\"menuName\":\"Hello\",\"position\":1000}\n";
  Value json_value = JsonValue(org_json);
  request::AddSubMenu aasm(&json_value);
  ASSERT_TRUE(aasm.is_initialized());
  ASSERT_RPCTYPE_VALID(aasm);
  ASSERT_EQ(aasm.position, 1000);

  std::string serialized = writer.write(aasm.ToJsonValue());
  ASSERT_EQ(awaited_json, serialized);
}

TEST_F(GeneratedInterfaceTests, MapTest) {
  const char* expected_json =
  "{\"choiceID\":1,\"menuName\":\"Menu name\",\"vrCommands\":{\"one\":\"First value\",\"two\":\"Second value\"}}\n";

  Choice choice;
  ASSERT_FALSE(choice.is_initialized());
  ASSERT_FALSE(choice.is_valid());
  std::map<std::string, std::string> init_map;
  init_map.insert(std::make_pair("one", "First value"));
  init_map.insert(std::make_pair("two", "Second value"));
  choice = Choice(1, "Menu name", init_map);
  ASSERT_TRUE(choice.is_initialized());
  ASSERT_RPCTYPE_VALID(choice);

  std::string serialized = writer.write(choice.ToJsonValue());
  ASSERT_EQ(expected_json, serialized);
}

TEST_F(GeneratedInterfaceTests, TypedefTest) {
  const char* expected_json =
  "{\"optionalResArrMap\":{\"World\":[\"INVALID_DATA\"]},\"resArrMap\":{\"Hello\":[\"SUCCESS\"]}}\n";

  TdStruct ts;
  ts.resArrMap["Hello"].push_back(R_SUCCESS);
  (*ts.optionalResArrMap)["World"].push_back(R_INVALID_DATA);
  ASSERT_TRUE(ts.is_initialized());
  ASSERT_RPCTYPE_VALID(ts);
  std::string serialized = writer.write(ts.ToJsonValue());
  ASSERT_EQ(expected_json, serialized);
}

//TODO(VVeremjova) APPLINK-12831 Fix constructors' work in case invalid values
TEST_F(GeneratedInterfaceTests, DISABLED_OverflowedDiagnosticMessageTest) {
  const char* input_json =
      "{\"messageData\":[300, 20],\"messageLength\":2,\"targetID\":5}";
  Value json_value = JsonValue(input_json);
  request::DiagnosticMessage dm(&json_value);
  ASSERT_TRUE(dm.is_initialized());
  ASSERT_FALSE(dm.is_valid());
}

//TODO(VVeremjova) APPLINK-12831 Fix constructors' work in case invalid values
TEST_F(GeneratedInterfaceTests, DISABLED_OverflowedDiagnosticMessageTest64) {
  const char* input_json =
      "{\"messageData\":[10, 123456789123],\"messageLength\":2,\"targetID\":5}";
  Value json_value = JsonValue(input_json);
  request::DiagnosticMessage dm(&json_value);
  ASSERT_TRUE(dm.is_initialized());
  ASSERT_FALSE(dm.is_valid());
}

TEST_F(GeneratedInterfaceTests, TestHandlerCalled) {
  testing::StrictMock<TestRequestHandlerMock> mock;
  request::AddSubMenu add_submenu;
  EXPECT_CALL(mock, HandleAddSubMenu(testing::Ref(add_submenu)))
      .Times(1);
  add_submenu.HandleWith(&mock);
}

TEST_F(GeneratedInterfaceTests, TestFactory) {
  testing::StrictMock<TestRequestHandlerMock> mock;
  Json::Value json_value;
  request::Request* req = request::NewFromJson(&json_value, kAddSubMenuID);
  request::AddSubMenu& add_sub_menu_ref =
      static_cast<request::AddSubMenu&>(*req);
  EXPECT_CALL(mock, HandleAddSubMenu(testing::Ref(add_sub_menu_ref)))
      .Times(1);
  req->HandleWith(&mock);
}

TEST_F(GeneratedInterfaceTests, TestNullableStructMember) {
  TestStructWithNullableParam with_nullable;
  ASSERT_FALSE(with_nullable.is_initialized());
  ASSERT_FALSE(with_nullable.is_valid());
  ASSERT_FALSE(with_nullable.nullableInt.is_valid());
  ASSERT_FALSE(with_nullable.nullableInt.is_null());
  with_nullable.nullableInt.set_to_null();
  ASSERT_RPCTYPE_VALID(with_nullable);
  ASSERT_TRUE(with_nullable.is_initialized());
  ASSERT_TRUE(with_nullable.nullableInt.is_null());
  ASSERT_RPCTYPE_VALID(with_nullable.nullableInt);
  ASSERT_TRUE(with_nullable.nullableInt.is_initialized());
}

TEST_F(GeneratedInterfaceTests, TestNullableStructMemberNullInitializationFromJson) {
  const char* input_json =
      "{\"nullableInt\":null}\n";
  Value json_value = JsonValue(input_json);
  TestStructWithNullableParam with_nullable(&json_value);
  ASSERT_TRUE(with_nullable.is_initialized());
  ASSERT_RPCTYPE_VALID(with_nullable);
  ASSERT_TRUE(with_nullable.nullableInt.is_null());
  std::string result = writer.write(with_nullable.ToJsonValue());
  ASSERT_EQ(input_json, result);
}

TEST_F(GeneratedInterfaceTests, TestNullableStructMemberInitializationFromJson) {
  const char* input_json =
      "{\"nullableInt\":3}\n";
  Value json_value = JsonValue(input_json);
  TestStructWithNullableParam with_nullable(&json_value);
  ASSERT_TRUE(with_nullable.is_initialized());
  ASSERT_RPCTYPE_VALID(with_nullable);
  ASSERT_FALSE(with_nullable.nullableInt.is_null());
  ASSERT_EQ(3, with_nullable.nullableInt);
  std::string result = writer.write(with_nullable.ToJsonValue());
  ASSERT_EQ(input_json, result);
}

TEST_F(GeneratedInterfaceTests, TestNullableEnumInitialization) {
  TestStructWithNullableStructParam strct_with_nullable;
  strct_with_nullable.nullableEnum = IT_DYNAMIC;
  strct_with_nullable.nonNullableEnum = IT_STATIC;
  ASSERT_TRUE(strct_with_nullable.is_initialized());
  ASSERT_RPCTYPE_VALID(strct_with_nullable);
  std::string result = writer.write(strct_with_nullable.ToJsonValue());
  const char* awaited_json1 = "{\"nonNullableEnum\":\"STATIC\",\"nullableEnum\":\"DYNAMIC\"}\n";
  ASSERT_EQ(awaited_json1, result);

  strct_with_nullable.nullableEnum.set_to_null();
  ASSERT_TRUE(strct_with_nullable.is_initialized());
  ASSERT_RPCTYPE_VALID(strct_with_nullable);
  result = writer.write(strct_with_nullable.ToJsonValue());
  const char* awaited_json2 = "{\"nonNullableEnum\":\"STATIC\",\"nullableEnum\":null}\n";
  ASSERT_EQ(awaited_json2, result);
}

TEST_F(GeneratedInterfaceTests, TestStructWithNullableTypedef) {
  StructWithNullableTypedef swntd;
  ASSERT_FALSE(swntd.is_initialized());
  ASSERT_FALSE(swntd.is_valid());
  swntd.nullableTdResult = R_SUCCESS;
  ASSERT_TRUE(swntd.is_initialized());
  ASSERT_RPCTYPE_VALID(swntd);
  ASSERT_EQ(R_SUCCESS, swntd.nullableTdResult);

  swntd.nullableTdResult.set_to_null();
  const char* awaited_json = "{\"nullableTdResult\":null}\n";
  std::string result = writer.write(swntd.ToJsonValue());
  ASSERT_EQ(awaited_json, result);
}

TEST_F(GeneratedInterfaceTests, TestNullingStructWithNullableMapOfNullableInts) {
  StructWithNullableMapOfNullableInts nmoni;
  ASSERT_FALSE(nmoni.is_initialized());
  ASSERT_FALSE(nmoni.is_valid());
  ASSERT_FALSE(nmoni.nullableMap.is_null());
  nmoni.nullableMap.set_to_null();
  ASSERT_TRUE(nmoni.is_initialized());
  ASSERT_RPCTYPE_VALID(nmoni);
  ASSERT_TRUE(nmoni.nullableMap.is_null());
  const char* awaited_json = "{\"nullableMap\":null}\n";
  std::string result = writer.write(nmoni.ToJsonValue());
  ASSERT_EQ(awaited_json, result);
}

TEST_F(GeneratedInterfaceTests, TestNullingValueInStructWithNullableMapOfNullableInts) {
  StructWithNullableMapOfNullableInts nmoni;
  ASSERT_FALSE(nmoni.is_initialized());
  ASSERT_FALSE(nmoni.is_valid());
  ASSERT_FALSE(nmoni.nullableMap.is_null());
  nmoni.nullableMap["Hello"].set_to_null();

  ASSERT_TRUE(nmoni.is_initialized());
  ASSERT_RPCTYPE_VALID(nmoni);
  ASSERT_FALSE(nmoni.nullableMap.is_null());
  ASSERT_TRUE(nmoni.nullableMap["Hello"].is_null());
  const char* awaited_json = "{\"nullableMap\":{\"Hello\":null}}\n";
  std::string result = writer.write(nmoni.ToJsonValue());
  ASSERT_EQ(awaited_json, result);
}

TEST_F(GeneratedInterfaceTests, EmptyStructTests) {
  EmptyStruct e;
  ASSERT_TRUE(e.struct_empty());
  ASSERT_FALSE(e.is_valid());
  ASSERT_FALSE(e.is_initialized());
  e.mark_initialized();
  ASSERT_TRUE(e.struct_empty());
  ASSERT_RPCTYPE_VALID(e);
  ASSERT_TRUE(e.is_initialized());
}

TEST_F(GeneratedInterfaceTests, StructWithOptionalEmptyStructFieldTest) {
  StructWithOptionalEmptyStructField oe;
  ASSERT_FALSE(oe.is_valid());
  ASSERT_FALSE(oe.is_initialized());
  oe.emptyOne->mark_initialized();
  ASSERT_RPCTYPE_VALID(oe);
  ASSERT_TRUE(oe.is_initialized());
  ASSERT_FALSE(oe.struct_empty());
  ASSERT_TRUE(oe.emptyOne->struct_empty());
}

TEST_F(GeneratedInterfaceTests, StructWithMandatoryEmptyStructFieldTest) {
  StructWithMandatoryEmptyStructField me;
  ASSERT_FALSE(me.is_valid());
  ASSERT_FALSE(me.is_initialized());
}

TEST_F(GeneratedInterfaceTests, EmptyStructJsonTests) {
  EmptyStruct e;
  ASSERT_FALSE(e.is_valid());
  ASSERT_FALSE(e.is_initialized());
  e.mark_initialized();
  ASSERT_RPCTYPE_VALID(e);
  ASSERT_TRUE(e.is_initialized());
  const char* expected_json = "{}\n";
  ASSERT_EQ(expected_json, writer.write(e.ToJsonValue()));
}

TEST_F(GeneratedInterfaceTests, StructWithOptionalEmptyStructFieldJsonTest) {
  StructWithOptionalEmptyStructField oe;
  ASSERT_FALSE(oe.is_valid());
  ASSERT_FALSE(oe.is_initialized());
  oe.mark_initialized();
  ASSERT_RPCTYPE_VALID(oe);
  ASSERT_TRUE(oe.is_initialized());
  const char* expected_json = "{}\n";
  ASSERT_EQ(expected_json, writer.write(oe.ToJsonValue()));
}

TEST_F(GeneratedInterfaceTests, StructWithMandatoryEmptyStructFieldJsonTest) {
  StructWithMandatoryEmptyStructField me;
  ASSERT_FALSE(me.is_valid());
  ASSERT_FALSE(me.is_initialized());
  me.emptyOne.mark_initialized();
  ASSERT_RPCTYPE_VALID(me);
  ASSERT_TRUE(me.is_initialized());
  const char* expected_json = "{\"emptyOne\":{}}\n";
  ASSERT_EQ(expected_json, writer.write(me.ToJsonValue()));
}

TEST_F(GeneratedInterfaceTests, StructWithOneOptionalIntFieldTest) {
  StructWithOneOptionalIntField soo;
  ASSERT_FALSE(soo.is_valid());
  ASSERT_FALSE(soo.is_initialized());
  soo.mark_initialized();
  ASSERT_TRUE(soo.is_initialized());
  ASSERT_RPCTYPE_VALID(soo);
}

TEST_F(GeneratedInterfaceTests, StructWithOneOptionalInitializedIntFieldTest) {
  StructWithOneOptionalIntField soo;
  *soo.optionalInt = 13;
  ASSERT_RPCTYPE_VALID(soo);
  ASSERT_TRUE(soo.is_initialized());
}

TEST_F(GeneratedInterfaceTests, StructWithOneOptionalIntFieldJsonTest) {
  StructWithOneOptionalIntField soo;
  ASSERT_FALSE(soo.is_valid());
  ASSERT_FALSE(soo.is_initialized());
  soo.mark_initialized();
  const char* expected_json = "{}\n";
  ASSERT_EQ(expected_json, writer.write(soo.ToJsonValue()));
}

TEST_F(GeneratedInterfaceTests, StructWithOneInitializedOptionalIntFieldJsonTest) {
  const char* input_json = "{\"optionalInt\":11}\n";
  Json::Value json_value = JsonValue(input_json);
  StructWithOneOptionalIntField soo(&json_value);
  ASSERT_RPCTYPE_VALID(soo);
  ASSERT_TRUE(soo.is_initialized());
  ASSERT_EQ(11, *soo.optionalInt);
  ASSERT_EQ(input_json, writer.write(soo.ToJsonValue()));
}

TEST_F(GeneratedInterfaceTests, StructWithFieldOfStructThatMightBeEmptyTest) {
  StructWithFieldOfStructThatMightBeEmpty sfme;
  ASSERT_FALSE(sfme.is_valid());
  ASSERT_FALSE(sfme.is_initialized());
  ASSERT_TRUE(sfme.struct_empty());
  *sfme.fieldThatMightBeEmpty.optionalInt = 5;
  ASSERT_FALSE(sfme.struct_empty());
  ASSERT_RPCTYPE_VALID(sfme);
  ASSERT_TRUE(sfme.is_initialized());
}

TEST_F(GeneratedInterfaceTests, StructWithFieldOfStructThatMightBeEmptyJsonNoValueTest) {
  StructWithFieldOfStructThatMightBeEmpty sfme;
  ASSERT_TRUE(sfme.struct_empty());
  ASSERT_FALSE(sfme.is_valid());
  ASSERT_FALSE(sfme.is_initialized());
  sfme.fieldThatMightBeEmpty.mark_initialized();
  ASSERT_FALSE(sfme.struct_empty());
  ASSERT_RPCTYPE_VALID(sfme);
  ASSERT_TRUE(sfme.is_initialized());
  const char* expcected_json = "{\"fieldThatMightBeEmpty\":{}}\n";
  ASSERT_EQ(expcected_json, writer.write(sfme.ToJsonValue()));
}

TEST_F(GeneratedInterfaceTests, StructWithFieldOfStructThatMightBeEmptyJsonHasValueTest) {
  const char* input_json = "{\"fieldThatMightBeEmpty\":{\"optionalInt\":12}}\n";
  Json::Value json_value = JsonValue(input_json);
  StructWithFieldOfStructThatMightBeEmpty sfme(&json_value);
  ASSERT_RPCTYPE_VALID(sfme);
  ASSERT_TRUE(sfme.is_initialized());
  ASSERT_EQ(12, *sfme.fieldThatMightBeEmpty.optionalInt);
  ASSERT_EQ(input_json, writer.write(sfme.ToJsonValue()));
}

TEST_F(GeneratedInterfaceTests, StructWithFieldOfOptionalMapTest) {
  StructWithNullableOptionalMap snom;
  ASSERT_TRUE(snom.struct_empty());
  ASSERT_FALSE(snom.is_valid());
  ASSERT_FALSE(snom.is_initialized());
  (*snom.nullableOptionalIntMap)["a"] = 5;
  ASSERT_FALSE(snom.struct_empty());
  ASSERT_RPCTYPE_VALID(snom);
  ASSERT_TRUE(snom.is_initialized());
}

TEST_F(GeneratedInterfaceTests, StructWithFieldOfOptionalMapToJsonTest) {
  StructWithNullableOptionalMap snom;
  ASSERT_TRUE(snom.struct_empty());
  ASSERT_FALSE(snom.is_valid());
  ASSERT_FALSE(snom.is_initialized());
  snom.mark_initialized();
  ASSERT_TRUE(snom.struct_empty());
  ASSERT_RPCTYPE_VALID(snom);
  ASSERT_TRUE(snom.is_initialized());
  const char* expected_json = "{}\n";
  ASSERT_EQ(expected_json, writer.write(snom.ToJsonValue()));
}

TEST_F(GeneratedInterfaceTests, StructWithFieldOfOptionalMapNulledToJsonTest) {
  StructWithNullableOptionalMap snom;
  ASSERT_TRUE(snom.struct_empty());
  ASSERT_FALSE(snom.is_valid());
  ASSERT_FALSE(snom.is_initialized());
  snom.nullableOptionalIntMap->set_to_null();
  ASSERT_FALSE(snom.struct_empty());
  ASSERT_RPCTYPE_VALID(snom);
  ASSERT_TRUE(snom.is_initialized());
  const char* expected_json = "{\"nullableOptionalIntMap\":null}\n";
  ASSERT_EQ(expected_json, writer.write(snom.ToJsonValue()));
}

TEST_F(GeneratedInterfaceTests, StructWithFieldOfOptionalMapNulledInJsonTest) {
  const char* input_json = "{\"nullableOptionalIntMap\":null}\n";
  Json::Value json_value = JsonValue(input_json);
  StructWithNullableOptionalMap snom(&json_value);
  ASSERT_RPCTYPE_VALID(snom);
  ASSERT_TRUE(snom.is_initialized());
  ASSERT_TRUE(snom.nullableOptionalIntMap->is_null());
  ASSERT_EQ(input_json, writer.write(snom.ToJsonValue()));
}

TEST_F(GeneratedInterfaceTests, StructWithFieldOfOptionalMapInitializedInJsonTest) {
  const char* input_json = "{\"nullableOptionalIntMap\":{\"Hello\":2}}\n";
  Json::Value json_value = JsonValue(input_json);
  StructWithNullableOptionalMap snom(&json_value);
  ASSERT_RPCTYPE_VALID(snom);
  ASSERT_TRUE(snom.is_initialized());
  ASSERT_FALSE(snom.nullableOptionalIntMap->is_null());
  ASSERT_EQ(input_json, writer.write(snom.ToJsonValue()));
}

TEST_F(GeneratedInterfaceTests, StructWithOptionalArrayTest) {
  const char* expected_json = "{}\n";
  StructWithOptionalIntArray soia;
  ASSERT_TRUE(soia.struct_empty());
  ASSERT_FALSE(soia.is_valid());
  ASSERT_FALSE(soia.is_initialized());
  soia.mark_initialized();
  ASSERT_EQ(expected_json, writer.write(soia.ToJsonValue()));
  (*soia.optionalIntArray).push_back(2);
  ASSERT_RPCTYPE_VALID(soia);
  ASSERT_TRUE(soia.is_initialized());
  const char* expected_json2 = "{\"optionalIntArray\":[2]}\n";
  ASSERT_EQ(expected_json2, writer.write(soia.ToJsonValue()));
}

TEST_F(GeneratedInterfaceTests, StructWithMandatoryArrayTest) {
  const char* expected_json = "{\"mandatoryIntArray\":[]}\n";
  StructWithMandatoryIntArray smia;
  ASSERT_FALSE(smia.is_valid());
  ASSERT_FALSE(smia.is_initialized());
  smia.mandatoryIntArray.mark_initialized();
  ASSERT_RPCTYPE_VALID(smia);
  ASSERT_TRUE(smia.is_initialized());
  ASSERT_EQ(expected_json, writer.write(smia.ToJsonValue()));

  smia.mandatoryIntArray.push_back(3);
  ASSERT_RPCTYPE_VALID(smia);
  ASSERT_TRUE(smia.is_initialized());
  const char* expected_json2 = "{\"mandatoryIntArray\":[3]}\n";
  ASSERT_EQ(expected_json2, writer.write(smia.ToJsonValue()));
}

TEST_F(GeneratedInterfaceTests, StructWithOptionalMapTest) {
  const char* expected_json = "{}\n";
  StructWithOptionalIntMap soim;
  ASSERT_TRUE(soim.struct_empty());
  ASSERT_FALSE(soim.is_valid());
  ASSERT_FALSE(soim.is_initialized());
  soim.mark_initialized();
  ASSERT_TRUE(soim.struct_empty());
  ASSERT_RPCTYPE_VALID(soim);
  ASSERT_TRUE(soim.is_initialized());
  ASSERT_EQ(expected_json, writer.write(soim.ToJsonValue()));
  (*soim.optionalIntMap)["Yay"] = 2;
  ASSERT_RPCTYPE_VALID(soim);
  ASSERT_TRUE(soim.is_initialized());
  const char* expected_json2 = "{\"optionalIntMap\":{\"Yay\":2}}\n";
  ASSERT_EQ(expected_json2, writer.write(soim.ToJsonValue()));
}

TEST_F(GeneratedInterfaceTests, StructWithMandatoryMapTest) {
  const char* expected_json = "{\"mandatoryIntMap\":{}}\n";
  StructWithMandatoryIntMap smim;
  ASSERT_FALSE(smim.is_valid());
  ASSERT_FALSE(smim.is_initialized());
  smim.mandatoryIntMap.mark_initialized();
  ASSERT_RPCTYPE_VALID(smim);
  ASSERT_TRUE(smim.is_initialized());
  ASSERT_EQ(expected_json, writer.write(smim.ToJsonValue()));
  smim.mandatoryIntMap["Yay"] = 2;
  ASSERT_RPCTYPE_VALID(smim);
  ASSERT_TRUE(smim.is_initialized());
  const char* expected_json2 = "{\"mandatoryIntMap\":{\"Yay\":2}}\n";
  ASSERT_EQ(expected_json2, writer.write(smim.ToJsonValue()));
}

TEST_F(GeneratedInterfaceTests, StructWithMandatoryMapInitFromWrongJsonTest) {
  const char* empty_json = "{}\n";
  Json::Value json_value = JsonValue(empty_json);

  StructWithMandatoryIntMap smim(&json_value);

  ASSERT_TRUE(smim.struct_empty());
  ASSERT_FALSE(smim.is_valid());
  ASSERT_TRUE(smim.is_initialized());

  smim.mandatoryIntMap["Yay"] = 2;
  ASSERT_FALSE(smim.struct_empty());
  ASSERT_RPCTYPE_VALID(smim);
  ASSERT_TRUE(smim.is_initialized());
  const char* expected_json = "{\"mandatoryIntMap\":{\"Yay\":2}}\n";
  ASSERT_EQ(expected_json, writer.write(smim.ToJsonValue()));
}

TEST_F(GeneratedInterfaceTests, ReportIncorrectlyInitializedMap1) {
  StructWithMandatoryIntMap smim;
  smim.mark_initialized();
  ASSERT_FALSE(smim.is_valid());
  rpc::ValidationReport report("smim");
  smim.ReportErrors(&report);
  ASSERT_EQ("smim.mandatoryIntMap: object is not initialized\n", PrettyFormat(report));
}

TEST_F(GeneratedInterfaceTests, ReportIncorrectlyInitializedMap2) {
  Choice c;
  ASSERT_FALSE(c.is_valid());
  rpc::ValidationReport report("c");
  c.ReportErrors(&report);
  ASSERT_EQ("c: object is not initialized\n"
            "c.choiceID: value is not initialized\n"
            "c.menuName: value is not initialized\n"
            "c.vrCommands: object is not initialized\n", PrettyFormat(report));
}

TEST_F(GeneratedInterfaceTests, TestFrankenstructCreation) {
  FrankenstructOfEmptyStringWithMandatoryInt fbmi;
  ASSERT_FALSE(fbmi.is_valid());
  ASSERT_FALSE(fbmi.is_initialized());
  ASSERT_TRUE(fbmi.empty());
  ASSERT_TRUE(fbmi.struct_empty());
  fbmi.mandatoryInt = 5;
  ASSERT_FALSE(fbmi.struct_empty());
  ASSERT_TRUE(fbmi.empty());
  ASSERT_TRUE(fbmi.is_initialized());

  // Ok known bug: frankenstructs must be explicitly marked initialized
  // or has values in map
  ASSERT_FALSE(fbmi.is_valid());
  fbmi.mark_initialized();
  ASSERT_RPCTYPE_VALID(fbmi);

  ASSERT_TRUE(fbmi.is_initialized());

  rpc::ValidationReport report("fbmi");
  fbmi.ReportErrors(&report);
  ASSERT_EQ("", PrettyFormat(report));
}

TEST_F(GeneratedInterfaceTests, FrankenstructToJson) {
  const char* expected_json = "{\"hello\":\"str\",\"mandatoryInt\":2}\n";
  FrankenstructOfEmptyStringWithMandatoryInt fbmi;
  fbmi.mandatoryInt = 2;
  fbmi["hello"] = "str";
  ASSERT_TRUE(fbmi.is_initialized());
  ASSERT_RPCTYPE_VALID(fbmi);
  ASSERT_FALSE(fbmi.empty());
  ASSERT_FALSE(fbmi.struct_empty());
  ASSERT_EQ(expected_json,
            writer.write(fbmi.ToJsonValue()));
}

TEST_F(GeneratedInterfaceTests, FrankenstructFromJson) {
  const char* input_json = "{\"hello\":\"str\",\"mandatoryInt\":2}\n";
  Json::Value json_value = JsonValue(input_json);
  FrankenstructOfEmptyStringWithMandatoryInt fbmi(&json_value);
  ASSERT_TRUE(fbmi.is_initialized());
  ASSERT_RPCTYPE_VALID(fbmi);
  ASSERT_FALSE(fbmi.empty());
  ASSERT_FALSE(fbmi.struct_empty());
  ASSERT_EQ(1u, fbmi.size());
  ASSERT_EQ(2, fbmi.mandatoryInt);
  ASSERT_EQ("str", std::string(fbmi["hello"]));
}

TEST_F(GeneratedInterfaceTests, FrankenstructFromInvalidJson) {
  const char* input_json = "{\"hello\":true,\"mandatoryInt\":2}\n";
  Json::Value json_value = JsonValue(input_json);
  FrankenstructOfEmptyStringWithMandatoryInt fbmi(&json_value);
  ASSERT_TRUE(fbmi.is_initialized());
  ASSERT_FALSE(fbmi.is_valid());
  ASSERT_FALSE(fbmi.empty());
  ASSERT_FALSE(fbmi.struct_empty());
  ASSERT_EQ(1u, fbmi.size());
  ASSERT_EQ(2, fbmi.mandatoryInt);
  rpc::ValidationReport report("fbmi");
  fbmi.ReportErrors(&report);
  ASSERT_EQ("fbmi[\"hello\"]: value initialized incorrectly\n", PrettyFormat(report));
}

}  // namespace test
