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
