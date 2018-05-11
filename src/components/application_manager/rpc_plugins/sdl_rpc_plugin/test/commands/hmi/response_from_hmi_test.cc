/*
 * Copyright (c) 2016, Ford Motor Company
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

#include <stdint.h>

#include "gtest/gtest.h"
#include "utils/shared_ptr.h"
#include "smart_objects/smart_object.h"
#include "application_manager/smart_object_keys.h"
#include "application_manager/test/include/application_manager/commands/commands_test.h"
#include "application_manager/commands/command.h"
#include "application_manager/test/include/application_manager/mock_event_dispatcher.h"
#include "application_manager/commands/response_from_hmi.h"
namespace test {
namespace components {
namespace commands_test {
namespace hmi_commands_test {
namespace response_from_hmi {

using ::testing::_;
using ::testing::Return;
using ::testing::SaveArg;
using ::testing::DoAll;

using ::utils::SharedPtr;
using ::test::components::event_engine_test::MockEventDispatcher;

namespace am = ::application_manager;
using application_manager::commands::ResponseFromHMI;

typedef SharedPtr<ResponseFromHMI> ResponseFromHMIPtr;

class ResponseFromHMITest : public CommandsTest<CommandsTestMocks::kIsNice> {};

TEST_F(ResponseFromHMITest, BasicMethodsOverloads_SUCCESS) {
  ResponseFromHMIPtr command(CreateCommand<ResponseFromHMI>());

  // Current implementation always return `true`
  EXPECT_TRUE(command->Init());
  EXPECT_NO_THROW(command->Run());
  EXPECT_TRUE(command->CleanUp());
}

TEST_F(ResponseFromHMITest, SendResponseToMobile_SUCCESS) {
  ResponseFromHMIPtr command(CreateCommand<ResponseFromHMI>());

  MessageSharedPtr msg(CreateMessage(smart_objects::SmartType_Map));
  EXPECT_CALL(mock_rpc_service_, ManageMobileCommand(msg, _));

  command->SendResponseToMobile(msg, app_mngr_);

  const application_manager::MessageType received_message_tipe =
      static_cast<application_manager::MessageType>(
          (*msg)[am::strings::params][am::strings::message_type].asInt());

  EXPECT_EQ(application_manager::MessageType::kResponse, received_message_tipe);
}

TEST_F(ResponseFromHMITest, CreateHMIRequest_SUCCESS) {
  ResponseFromHMIPtr command(CreateCommand<ResponseFromHMI>());

  MessageSharedPtr result_msg;
  EXPECT_CALL(mock_rpc_service_, ManageHMICommand(_))
      .WillOnce(DoAll(SaveArg<0>(&result_msg), Return(true)));

  const hmi_apis::FunctionID::eType posted_function_id =
      hmi_apis::FunctionID::INVALID_ENUM;
  MessageSharedPtr dummy_msg_params = CreateMessage();
  command->CreateHMIRequest(posted_function_id, *dummy_msg_params);

  ASSERT_TRUE(result_msg);

  const application_manager::MessageType received_message_tipe =
      static_cast<application_manager::MessageType>(
          (*result_msg)[am::strings::params][am::strings::message_type]
              .asInt());

  EXPECT_EQ(am::MessageType::kRequest, received_message_tipe);

  const hmi_apis::FunctionID::eType received_function_id =
      static_cast<hmi_apis::FunctionID::eType>(
          (*result_msg)[am::strings::params][am::strings::function_id].asInt());

  EXPECT_EQ(posted_function_id, received_function_id);
}

TEST_F(ResponseFromHMITest, CreateHMIRequest_CantManageCommand_Covering) {
  ResponseFromHMIPtr command(CreateCommand<ResponseFromHMI>());

  MessageSharedPtr result_msg;
  ON_CALL(app_mngr_, GetNextHMICorrelationID()).WillByDefault(Return(1u));
  EXPECT_CALL(mock_rpc_service_, ManageHMICommand(_))
      .WillOnce(DoAll(SaveArg<0>(&result_msg), Return(false)));

  const hmi_apis::FunctionID::eType posted_function_id =
      hmi_apis::FunctionID::INVALID_ENUM;
  MessageSharedPtr dummy_msg_params = CreateMessage();
  command->CreateHMIRequest(posted_function_id, *dummy_msg_params);
}

}  // namespace response_from_hmi
}  // namespace hmi_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
