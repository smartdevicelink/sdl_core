#include "gtest/gtest.h"

#include <iostream>
#include <test_rpc_interface/functions.h>
#include "json/reader.h"
#include "json/writer.h"

namespace test {
using namespace rpc::test_rpc_interface;

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
  notification::OnAppInterfaceUnregistered oaiu(JsonValue(org_json));
  ASSERT_TRUE(oaiu.is_initialized());
  ASSERT_TRUE(oaiu.is_valid());

  std::string serialized = writer.write(oaiu.ToJsonValue());
  ASSERT_EQ(org_json, serialized);
}

TEST_F(GeneratedInterfaceTests, FunctionWithoutParams) {
  notification::OnAudioPassThru oapt;
  ASSERT_FALSE(oapt.is_initialized());
  ASSERT_TRUE(oapt.is_valid());
}

TEST_F(GeneratedInterfaceTests, DefValueTest) {
  const char* org_json = "{\"menuID\":2,\"menuName\":\"Hello\"}";
  const char* awaited_json = "{\"menuID\":2,\"menuName\":\"Hello\",\"position\":1000}\n";
  request::AddSubMenu aasm(JsonValue(org_json));
  ASSERT_TRUE(aasm.is_initialized());
  ASSERT_TRUE(aasm.is_valid());
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
  ASSERT_TRUE(choice.is_valid());

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
  ASSERT_TRUE(ts.is_valid());
  std::string serialized = writer.write(ts.ToJsonValue());
  ASSERT_EQ(expected_json, serialized);
}

TEST_F(GeneratedInterfaceTests, OverflowedDiagnosticMessageTest) {
  const char* input_json =
      "{\"messageData\":[300, 20],\"messageLength\":2,\"targetID\":5}";
  request::DiagnosticMessage dm(JsonValue(input_json));
  ASSERT_TRUE(dm.is_initialized());
  ASSERT_FALSE(dm.is_valid());
}

TEST_F(GeneratedInterfaceTests, OverflowedDiagnosticMessageTest64) {
  const char* input_json =
      "{\"messageData\":[10, 123456789123],\"messageLength\":2,\"targetID\":5}";
  request::DiagnosticMessage dm(JsonValue(input_json));
  ASSERT_TRUE(dm.is_initialized());
  ASSERT_FALSE(dm.is_valid());
}

}  // namespace test
