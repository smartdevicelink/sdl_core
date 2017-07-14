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
#include <string>

#include "gtest/gtest.h"
#include "utils/shared_ptr.h"
#include "smart_objects/smart_object.h"
#include "application_manager/smart_object_keys.h"
#include "application_manager/commands/commands_test.h"
#include "application_manager/commands/command.h"
#include "application_manager/commands/command_response_impl.h"
#include "application_manager/mock_application.h"

namespace test {
namespace components {
namespace commands_test {
namespace command_response_impl {

namespace strings = ::application_manager::strings;
namespace hmi_response = ::application_manager::hmi_response;
using ::utils::SharedPtr;
using ::application_manager::commands::MessageSharedPtr;
using ::application_manager::commands::CommandResponseImpl;

class CommandResponseImplTest
    : public CommandsTest<CommandsTestMocks::kIsNice> {};

TEST_F(CommandResponseImplTest, BasicMethodsOverloads_SUCCESS) {
  SharedPtr<CommandResponseImpl> command = CreateCommand<CommandResponseImpl>();
  // Current implementation always return `true`
  EXPECT_TRUE(command->Init());
  EXPECT_TRUE(command->CleanUp());
  EXPECT_NO_THROW(command->Run());
}

TEST_F(CommandResponseImplTest, SendResponse_MessageWithResultCode_SUCCESS) {
  MessageSharedPtr msg;
  SharedPtr<CommandResponseImpl> command =
      CreateCommand<CommandResponseImpl>(msg);
  // Do not have a weight in this case
  const bool kSuccess = true;
  const mobile_apis::Result::eType kResultCode =
      mobile_apis::Result::eType::INVALID_ENUM;
  const bool kFinalResponse = true;

  // If `msg_params->result_code` exist in message,
  // then send message to mobile.
  (*msg)[strings::msg_params][strings::result_code] = kResultCode;

  EXPECT_CALL(app_mngr_, SendMessageToMobile(msg, kFinalResponse));

  command->SendResponse(kSuccess, kResultCode, kFinalResponse);
}

TEST_F(CommandResponseImplTest,
       SendResponse_EmptyMessageValidResultCode_SUCCESS) {
  MessageSharedPtr msg;
  SharedPtr<CommandResponseImpl> command =
      CreateCommand<CommandResponseImpl>(msg);

  const bool kSuccess = true;
  const mobile_apis::Result::eType kResultCode =
      mobile_apis::Result::eType::SUCCESS;
  const bool kFinalResponse = true;

  EXPECT_CALL(app_mngr_, SendMessageToMobile(msg, kFinalResponse));

  // If `msg_params->result_code` does not exist in message
  // and arg `result_code` not equals `INVALID_ENUM`,
  // then set it to `msg_params->result_code` and send message to mobile.
  command->SendResponse(kSuccess, kResultCode, kFinalResponse);

  EXPECT_EQ(kResultCode,
            (*msg)[strings::msg_params][strings::result_code].asInt());
}

TEST_F(CommandResponseImplTest,
       SendResponse_EmptyMessageInvalidResultCode_SUCCESS) {
  MessageSharedPtr msg;
  SharedPtr<CommandResponseImpl> command =
      CreateCommand<CommandResponseImpl>(msg);

  const bool kSuccess = true;
  const mobile_apis::Result::eType kResultCode =
      mobile_apis::Result::eType::INVALID_ENUM;
  const bool kFinalResponse = true;

  // If `msg_params->result_code` does not exist in message
  // and arg `result_code` equals `INVALID_ENUM`,
  // then if `params->hmi_response::code` exist in message,
  // then set it to `msg_params->result_code` and send message to mobile.
  (*msg)[strings::params][hmi_response::code] = mobile_apis::Result::SUCCESS;

  EXPECT_CALL(app_mngr_, SendMessageToMobile(msg, kFinalResponse));

  command->SendResponse(kSuccess, kResultCode, kFinalResponse);

  EXPECT_EQ((*msg)[strings::params][hmi_response::code].asInt(),
            (*msg)[strings::msg_params][strings::result_code].asInt());
}

TEST_F(CommandResponseImplTest,
       SendResponse_EmptyMessageInvalidResultCodeNoHmiResponse_SUCCESS) {
  MessageSharedPtr msg;
  SharedPtr<CommandResponseImpl> command =
      CreateCommand<CommandResponseImpl>(msg);

  const mobile_apis::Result::eType kResultCode =
      mobile_apis::Result::eType::INVALID_ENUM;
  const bool kFinalResponse = true;

  // If `msg_params->result_code` does not exist in message
  // and arg `result_code` equals `INVALID_ENUM`,
  // then if `params->hmi_response::code` does not exist in message,
  // then if `kSuccess` equals `true`,
  // then `msg_params->result_code` will be `SUCCESS`
  const bool kSuccess = true;

  EXPECT_CALL(app_mngr_, SendMessageToMobile(msg, kFinalResponse));

  command->SendResponse(kSuccess, kResultCode, kFinalResponse);

  EXPECT_EQ(mobile_apis::Result::SUCCESS,
            (*msg)[strings::msg_params][strings::result_code].asInt());
}

TEST_F(CommandResponseImplTest,
       SendResponse_EmptyMessageInvalidResultCodeNoHmiResponse_INVALID_ENUM) {
  MessageSharedPtr msg;
  SharedPtr<CommandResponseImpl> command =
      CreateCommand<CommandResponseImpl>(msg);

  const mobile_apis::Result::eType kResultCode =
      mobile_apis::Result::eType::INVALID_ENUM;
  const bool kFinalResponse = true;

  // If `msg_params->result_code` does not exist in message
  // and arg `result_code` equals `INVALID_ENUM`,
  // then if `params->hmi_response::code` does not exist in message,
  // then if `kSuccess` equals `false`,
  // then `msg_params->result_code` will be `INVALID_ENUM`
  const bool kSuccess = false;

  EXPECT_CALL(app_mngr_, SendMessageToMobile(msg, kFinalResponse));

  command->SendResponse(kSuccess, kResultCode, kFinalResponse);

  EXPECT_EQ(mobile_apis::Result::INVALID_ENUM,
            (*msg)[strings::msg_params][strings::result_code].asInt());
}

}  // namespace command_response_impl
}  // namespace commands_test
}  // namespace components
}  // namespace test
