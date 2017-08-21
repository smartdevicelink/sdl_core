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

#include "application_manager/commands/mobile/unsubscribe_vehicle_data_response.h"
#include <stdint.h>
#include <string>
#include "gtest/gtest.h"
#include "utils/shared_ptr.h"
#include "application_manager/commands/commands_test.h"
#include "application_manager/mock_application_manager.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {
namespace unsubscribe_vehicle_data_response {

namespace commands = ::application_manager::commands;

using ::testing::_;
using ::testing::NotNull;
using ::testing::Types;

using commands::MessageSharedPtr;
namespace am = ::application_manager;

class UnsubscribeVehicleResponseTest
    : public CommandsTest<CommandsTestMocks::kIsNice> {};

MATCHER_P(ResultCodeIs, result_code, "") {
  return result_code ==
         static_cast<mobile_apis::Result::eType>(
             (*arg)[am::strings::msg_params][am::strings::result_code].asInt());
}

TEST_F(UnsubscribeVehicleResponseTest,
       Run_SendFalseResponseToMobile_SendInvalidEnum) {
  MessageSharedPtr command_msg =
      ::utils::MakeShared<SmartObject>(smart_objects::SmartType_Map);
  (*command_msg)[am::strings::msg_params][am::strings::success] = false;

  ::utils::SharedPtr<commands::UnsubscribeVehicleDataResponse> command =
      CreateCommand<commands::UnsubscribeVehicleDataResponse>(command_msg);
  EXPECT_CALL(
      app_mngr_,
      SendMessageToMobile(ResultCodeIs(mobile_apis::Result::INVALID_ENUM), _));
  command->Run();
}

TEST_F(UnsubscribeVehicleResponseTest,
       Run_SendSuccessfulResponseToMobile_SUCCESS) {
  MessageSharedPtr command_msg =
      ::utils::MakeShared<SmartObject>(smart_objects::SmartType_Map);
  (*command_msg)[am::strings::msg_params][am::strings::success] = true;
  ::utils::SharedPtr<commands::UnsubscribeVehicleDataResponse> command =
      CreateCommand<commands::UnsubscribeVehicleDataResponse>(command_msg);
  EXPECT_CALL(
      app_mngr_,
      SendMessageToMobile(ResultCodeIs(mobile_apis::Result::SUCCESS), _));
  command->Run();
}

TEST_F(UnsubscribeVehicleResponseTest,
       Run_SendResponseToMobile_SendCodeToMobile) {
  MessageSharedPtr command_msg =
      ::utils::MakeShared<SmartObject>(smart_objects::SmartType_Map);
  (*command_msg)[am::strings::msg_params][am::strings::success] = true;

  mobile_apis::Result::eType result_type = mobile_apis::Result::WARNINGS;
  (*command_msg)[am::strings::msg_params][am::strings::result_code] =
      result_type;
  ::utils::SharedPtr<commands::UnsubscribeVehicleDataResponse> command =
      CreateCommand<commands::UnsubscribeVehicleDataResponse>(command_msg);
  EXPECT_CALL(app_mngr_, SendMessageToMobile(ResultCodeIs(result_type), _));
  command->Run();
}

}  // namespace unsubscribe_vehicle_data_response
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
