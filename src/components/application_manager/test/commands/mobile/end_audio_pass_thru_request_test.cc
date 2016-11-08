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
#include <vector>

#include "application_manager/commands/mobile/end_audio_pass_thru_request.h"

#include "gtest/gtest.h"
#include "application_manager/smart_object_keys.h"
#include "application_manager/test/include/application_manager/commands/commands_test.h"
#include "application_manager/test/include/application_manager/commands/command_request_test.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/event_engine/event.h"
#include "application_manager/mock_message_helper.h"
#include "application_manager/mock_hmi_interface.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {
namespace end_audio_pass_thru_request {

namespace am = ::application_manager;
using ::testing::_;
using ::testing::Mock;
using ::testing::Return;
using ::testing::ReturnRef;
using am::commands::MessageSharedPtr;
using am::commands::EndAudioPassThruRequest;
using am::event_engine::Event;
using am::MockHmiInterfaces;
using am::MockMessageHelper;

typedef SharedPtr<EndAudioPassThruRequest> EndAudioPassThruRequestPtr;

class EndAudioPassThruRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
  EndAudioPassThruRequestTest()
      : mock_message_helper_(*MockMessageHelper::message_helper_mock()) {}
  MockMessageHelper& mock_message_helper_;
};

TEST_F(EndAudioPassThruRequestTest, OnEvent_UI_UNSUPPORTED_RESOUCRE) {
  const uint32_t kConnectionKey = 2u;

  MessageSharedPtr command_msg(CreateMessage(smart_objects::SmartType_Map));
  (*command_msg)[am::strings::params][am::strings::connection_key] =
      kConnectionKey;

  EndAudioPassThruRequestPtr command(
      CreateCommand<EndAudioPassThruRequest>(command_msg));

  MessageSharedPtr event_msg(CreateMessage(smart_objects::SmartType_Map));
  (*event_msg)[am::strings::msg_params] = 0;
  (*event_msg)[am::strings::params][am::hmi_response::code] =
      mobile_apis::Result::UNSUPPORTED_RESOURCE;

  Event event(hmi_apis::FunctionID::UI_EndAudioPassThru);
  event.set_smart_object(*event_msg);

  MockHmiInterfaces hmi_interfaces;
  ON_CALL(app_mngr_, hmi_interfaces()).WillByDefault(ReturnRef(hmi_interfaces));
  ON_CALL(hmi_interfaces,
          GetInterfaceState(am::HmiInterfaces::HMI_INTERFACE_UI))
      .WillByDefault(Return(am::HmiInterfaces::STATE_AVAILABLE));

  EXPECT_CALL(mock_message_helper_,
              HMIToMobileResult(hmi_apis::Common_Result::UNSUPPORTED_RESOURCE))
      .WillOnce(Return(mobile_apis::Result::UNSUPPORTED_RESOURCE));

  EXPECT_CALL(app_mngr_, EndAudioPassThrough()).WillOnce(Return(false));

  MessageSharedPtr ui_command_result;
  EXPECT_CALL(
      app_mngr_,
      ManageMobileCommand(_, am::commands::Command::CommandOrigin::ORIGIN_SDL))
      .WillOnce(DoAll(SaveArg<0>(&ui_command_result), Return(true)));

  command->on_event(event);

  EXPECT_EQ((*ui_command_result)[am::strings::msg_params][am::strings::success]
                .asBool(),
            true);
  EXPECT_EQ(
      (*ui_command_result)[am::strings::msg_params][am::strings::result_code]
          .asInt(),
      static_cast<int32_t>(hmi_apis::Common_Result::UNSUPPORTED_RESOURCE));
  if ((*ui_command_result)[am::strings::msg_params].keyExists(
          am::strings::info)) {
    EXPECT_FALSE(
        (*ui_command_result)[am::strings::msg_params][am::strings::info]
            .asString()
            .empty());
  }
  Mock::VerifyAndClearExpectations(&mock_message_helper_);
}

}  // namespace end_audio_pass_thru_request
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
