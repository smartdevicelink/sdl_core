#include "gmock/gmock.h"

#include <iostream>
#include <test_rpc_interface/interface.h>
#include <test_rpc_interface/functions.h>
#include "json/reader.h"
#include "json/writer.h"
#include "rpc_base/dbus_message.h"

namespace test {
using namespace rpc::test_rpc_interface;

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
  std::string serialized = writer.write(oapt.ToJsonValue());
  ASSERT_EQ("{}\n", serialized);
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
  ts.optionalResArrMap["World"].push_back(R_INVALID_DATA);
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
  request::Request* req = request::NewFromJson(json_value, kAddSubMenuID);
  request::AddSubMenu& add_sub_menu_ref =
      static_cast<request::AddSubMenu&>(*req);
  EXPECT_CALL(mock, HandleAddSubMenu(testing::Ref(add_sub_menu_ref)))
      .Times(1);
  req->HandleWith(&mock);
}

TEST_F(GeneratedInterfaceTests, TestFailedDbusDeserialization) {
  dbus::MessageRef msg(dbus_message_new(DBUS_MESSAGE_TYPE_METHOD_CALL));
  {
    dbus::MessageWriter writer(msg);
  }
  {
    dbus::MessageReader reader(msg);
    request::DiagnosticMessage dm(&reader);
    ASSERT_TRUE(reader.has_failed());
    ASSERT_TRUE(dm.is_initialized());
    ASSERT_FALSE(dm.is_valid());
  }
}

TEST_F(GeneratedInterfaceTests, TestDbusDeserialization) {
  dbus::MessageRef msg(dbus_message_new(DBUS_MESSAGE_TYPE_METHOD_CALL));
  {
    dbus::MessageWriter writer(msg);
    writer.PutUint16(22);
    writer.PutUint16(42);
    dbus::MessageWriter array_writer(&writer, dbus::kArray, DBUS_TYPE_BYTE_AS_STRING);
    array_writer.PutByte(11);
  }
  {
    dbus::MessageReader reader(msg);
    request::DiagnosticMessage dm(&reader);
    ASSERT_FALSE(reader.has_failed());
    ASSERT_FALSE(reader.HasNext());
    ASSERT_TRUE(dm.is_initialized());
    ASSERT_TRUE(dm.is_valid());
    ASSERT_EQ(dm.targetID, 22);
    ASSERT_EQ(dm.messageLength, 42);
    ASSERT_EQ(dm.messageData.size(), 1);
  }
}

TEST_F(GeneratedInterfaceTests, TestDbusSerializationDeserialization) {
  dbus::MessageRef msg(dbus_message_new(DBUS_MESSAGE_TYPE_METHOD_CALL));
  {
    dbus::MessageWriter writer(msg);
    request::DiagnosticMessage dm;
    dm.targetID = 70;
    dm.messageLength = 1;
    dm.messageData.push_back(42);
    dm.ToDbusWriter(&writer);
  }
  {
    dbus::MessageReader reader(msg);
    request::DiagnosticMessage dm(&reader);
    ASSERT_FALSE(reader.has_failed());
    ASSERT_FALSE(reader.HasNext());
    ASSERT_TRUE(dm.is_initialized());
    ASSERT_TRUE(dm.is_valid());
    ASSERT_EQ(dm.targetID, 70);
    ASSERT_EQ(dm.messageLength, 1);
    ASSERT_EQ(dm.messageData.size(), 1);
    ASSERT_EQ(dm.messageData[0], 42);
  }
}

TEST_F(GeneratedInterfaceTests, TestDbusAddSubMenuSerializatioDeserialization) {
  dbus::MessageRef msg(dbus_message_new(DBUS_MESSAGE_TYPE_METHOD_CALL));
  {
    dbus::MessageWriter writer(msg);
    notification::DummyNotification dn;
    TdStruct tds;
    tds.resArrMap["Hello"].push_back(R_SUCCESS);
    tds.optionalResArrMap["World"].push_back(R_INVALID_DATA);
    dn.tds.push_back(tds);
    ASSERT_TRUE(dn.is_valid());
    dn.ToDbusWriter(&writer);
  }
  {
    dbus::MessageReader reader(msg);
    notification::DummyNotification dn(&reader);
    ASSERT_TRUE(dn.is_initialized());
    ASSERT_TRUE(dn.is_valid());
    ASSERT_FALSE(reader.has_failed());
    ASSERT_FALSE(reader.HasNext());
    ASSERT_EQ(dn.tds[0].resArrMap["Hello"][0], R_SUCCESS);
    ASSERT_EQ(dn.tds[0].optionalResArrMap["World"][0], R_INVALID_DATA);
  }

}

}  // namespace test
