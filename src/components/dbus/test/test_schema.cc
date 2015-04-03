/*
 * \file test_schema.cc
 * \brief
 *
 * Copyright (c) 2013, Ford Motor Company
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

#include <gtest/gtest.h>
#include "dbus/schema.h"

using dbus::DBusSchema;
using dbus::MessageId;
using dbus::MessageName;
using dbus::MessageType;
using dbus::ListArgs;
using ford_message_descriptions::ParameterDescription;

namespace test {
namespace components {
namespace dbus {

class DBusSchemaTest : public ::testing::Test {
 protected:
  static void SetUpTestCase() {
    schema_ = new DBusSchema(ford_message_descriptions::message_descriptions);
  }

  static void TearDownTestCase() {
    delete schema_;
  }

  static const DBusSchema* schema_;
  static const int test_negative_value = -3;
};

const DBusSchema* DBusSchemaTest::schema_ = 0;

TEST_F(DBusSchemaTest, GetName) {
  const MessageId kId = hmi_apis::FunctionID::Buttons_GetCapabilities;
  const MessageName kExpName("Buttons", "GetCapabilities");
  MessageName name = schema_->getMessageName(kId);
  EXPECT_EQ(kExpName, name);

  const MessageId kIdWrong = static_cast<const MessageId>(test_negative_value);
  const MessageName kExpNameWrong("", "");
  name = schema_->getMessageName(kIdWrong);
  EXPECT_EQ(kExpNameWrong, name);
}

TEST_F(DBusSchemaTest, GetId) {
  const MessageName kName("Buttons", "GetCapabilities");
  const MessageId kExpId = hmi_apis::FunctionID::Buttons_GetCapabilities;
  MessageId id = schema_->getMessageId(kName);
  EXPECT_EQ(kExpId, id);

  const MessageName kNameWrong("TestInterface", "TestMessage");
  const MessageId kExpIdWrong = hmi_apis::FunctionID::INVALID_ENUM;
  id = schema_->getMessageId(kNameWrong);
  EXPECT_EQ(kExpIdWrong, id);
}

TEST_F(DBusSchemaTest, GetListArg) {
  const MessageName kName("Buttons", "GetCapabilities");
  const MessageType kType = hmi_apis::messageType::response;
  const MessageId kId = hmi_apis::FunctionID::Buttons_GetCapabilities;
  ListArgs argsName = schema_->getListArgs(kName, kType);
  const ParameterDescription** params =
      ford_message_descriptions::message_descriptions[1]->parameters;
  EXPECT_EQ(params[0], argsName[0]);
  EXPECT_EQ(params[1], argsName[1]);

  ListArgs argsId = schema_->getListArgs(kId, kType);
  EXPECT_EQ(params[0], argsId[0]);
  EXPECT_EQ(params[1], argsId[1]);

  const MessageId kIdWrong = static_cast<const MessageId>(test_negative_value);
  const MessageName kNameWrong("TestInterface", "TestMessage");
  const MessageType kTypeWrong = static_cast<const MessageType>(test_negative_value);
  const ListArgs kExpListWrong;
  argsName = schema_->getListArgs(kNameWrong, kTypeWrong);
  EXPECT_EQ(kExpListWrong, argsName);

  argsId = schema_->getListArgs(kIdWrong, kTypeWrong);
  EXPECT_EQ(kExpListWrong, argsId);
}

}  // namespace dbus
}  // namespace components
}  // namespace test


