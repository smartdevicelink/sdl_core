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

TEST_F(GeneratedInterfaceTests, OnAudioPassThru) {
  notification::OnAudioPassThru oapt;
  ASSERT_TRUE(oapt.is_initialized());
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

}  // namespace test
