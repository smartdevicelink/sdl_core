#include "gmock/gmock.h"

#include <test_rpc_interface/interface.h>
#include <test_rpc_interface/functions.h>

#include "dbus/dbus_message.h"

namespace test {
using namespace rpc::test_rpc_interface;

TEST(GeneratedInterfaceDbusTests, TestFailedDbusDeserialization) {
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

TEST(GeneratedInterfaceDbusTests, TestDbusDeserialization) {
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
    ASSERT_EQ(dm.messageData.size(), 1u);
  }
}

TEST(GeneratedInterfaceDbusTests, TestDbusSerializationDeserialization) {
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
    ASSERT_EQ(dm.messageData.size(), 1u);
    ASSERT_EQ(dm.messageData[0], 42u);
  }
}

TEST(GeneratedInterfaceDbusTests, TestDbusAddSubMenuSerializatioDeserialization) {
  dbus::MessageRef msg(dbus_message_new(DBUS_MESSAGE_TYPE_METHOD_CALL));
  {
    dbus::MessageWriter writer(msg);
    notification::DummyNotification dn;
    TdStruct tds;
    tds.resArrMap["Hello"].push_back(R_SUCCESS);
    (*tds.optionalResArrMap)["World"].push_back(R_INVALID_DATA);
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
    ASSERT_EQ((*dn.tds[0].optionalResArrMap)["World"][0], R_INVALID_DATA);
  }
}

}  // namespace test
