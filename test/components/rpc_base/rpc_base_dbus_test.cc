/* Copyright (c) 2014, Ford Motor Company
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
#include "gtest/gtest-spi.h"

#include "rpc_base/dbus_message.h"

namespace test {

TEST(DbusMessageConstructionTest, DbusMessageConstruction) {
  DBusMessage* rawmsg = dbus_message_new(DBUS_MESSAGE_TYPE_METHOD_CALL);
  dbus::MessageRef msgref(rawmsg);
}

class DbusTest: public testing::Test {
public:
  dbus::MessageRef msgref;
  DbusTest():
    msgref(dbus_message_new(DBUS_MESSAGE_TYPE_METHOD_CALL)) {
  }
};

TEST_F(DbusTest, DbusWriterConstructionTest) {
 dbus::MessageWriter writer(msgref);
}

TEST_F(DbusTest, DbusEmptyMessageReaderTest) {
  dbus::MessageReader reader(msgref);
  ASSERT_TRUE(reader.has_failed());
}

TEST_F(DbusTest, DbusMessageWriterBoolWriteRead) {
  dbus::MessageWriter writer(msgref);
  writer.WriteBool(true);
  dbus::MessageReader reader(msgref);
  bool redback_value = reader.ReadBool();
  ASSERT_FALSE(reader.has_failed());
  ASSERT_TRUE(redback_value);
}

TEST_F(DbusTest, DbusMessageWriterInt32WriteRead) {
  dbus::MessageWriter writer(msgref);
  writer.WriteInt32(42);
  dbus::MessageReader reader(msgref);
  int32_t readback_value = reader.ReadInt32();
  ASSERT_FALSE(reader.has_failed());
  ASSERT_EQ(readback_value, 42);
}

TEST_F(DbusTest, DbusMessageWriterStringWriteRead) {
  dbus::MessageWriter writer(msgref);
  writer.WriteString("Hello DBus!");
  dbus::MessageReader reader(msgref);
  std::string readback_value = reader.ReadString();
  ASSERT_FALSE(reader.has_failed());
  ASSERT_EQ(readback_value, "Hello DBus!");
}

TEST_F(DbusTest, DbusMultipleParamsReadWrite) {
  {
    dbus::MessageWriter writer(msgref);
    writer.WriteString("Hello DBus!");
    writer.WriteInt16(42);
    writer.WriteDouble(3.14);
  }
  {
    dbus::MessageReader reader(msgref);
    std::string readback_string = reader.ReadString();
    ASSERT_FALSE(reader.has_failed());
    ASSERT_EQ(readback_string, "Hello DBus!");
    int16_t readback_int = reader.ReadInt16();
    ASSERT_FALSE(reader.has_failed());
    ASSERT_EQ(readback_int, 42);
    double readback_double = reader.ReadDouble();
    ASSERT_FALSE(reader.has_failed());
    ASSERT_DOUBLE_EQ(readback_double, 3.14);
    ASSERT_FALSE(reader.HasNext());
  }
}

TEST_F(DbusTest, DbusArrayTest) {
  {
    dbus::MessageWriter writer(msgref);
    dbus::MessageWriter array_writer(&writer, dbus::kArray,
                                     DBUS_TYPE_INT16_AS_STRING);
    array_writer.WriteInt16(3);
    array_writer.WriteInt16(4);
    array_writer.WriteInt16(5);
  }
  {
    dbus::MessageReader reader(msgref);
    dbus::MessageReader array_reader = reader.GetArrayReader();
    int16_t readback_val = array_reader.ReadInt16();
    ASSERT_FALSE(reader.has_failed());
    ASSERT_EQ(readback_val, 3);
    readback_val = array_reader.ReadInt16();
    ASSERT_FALSE(reader.has_failed());
    ASSERT_EQ(readback_val, 4);
    readback_val = array_reader.ReadInt16();
    ASSERT_FALSE(reader.has_failed());
    ASSERT_EQ(readback_val, 5);
    ASSERT_FALSE(array_reader.HasNext());
  }
}

class DbusFailuresTest: public testing::Test {
public:
  dbus::MessageRef int_msg;
  DbusFailuresTest()
    : int_msg(dbus_message_new(DBUS_MESSAGE_TYPE_METHOD_CALL)) {
    dbus::MessageWriter writer(int_msg);
    writer.WriteInt64(42);
  }
};

TEST_F(DbusFailuresTest, DbusInconsistentTypeReadFailureTest) {
  dbus::MessageReader reader(int_msg);
  std::string str = reader.ReadString();
  ASSERT_EQ(str, std::string(""));
  ASSERT_TRUE(reader.has_failed());
}

TEST_F(DbusFailuresTest, DbusNonExistentArrayReadTest) {
  dbus::MessageReader reader(int_msg);
  ASSERT_FALSE(reader.has_failed());
  dbus::MessageReader array_reader = reader.GetArrayReader();
  ASSERT_TRUE(array_reader.has_failed());
  ASSERT_TRUE(reader.has_failed());
  int64_t val = array_reader.ReadInt64();
  ASSERT_TRUE(array_reader.has_failed());
  ASSERT_EQ(val, 0);
}

}  // namespace test
