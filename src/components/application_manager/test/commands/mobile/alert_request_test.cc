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
#include "commands/commands_test.h"
#include "commands/command_request_test.h"
#include "application_manager/application.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/mock_application.h"
#include "application_manager/mock_message_helper.h"
#include "application_manager/event_engine/event.h"
#include "mobile/alert_request.h"
#include "interfaces/MOBILE_API.h"
#include "application_manager/policies/policy_handler_interface.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {

using ::testing::_;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::AtLeast;
namespace am = ::application_manager;
using am::commands::AlertRequest;
using am::commands::MessageSharedPtr;
using am::event_engine::Event;
using am::MockMessageHelper;

typedef SharedPtr<AlertRequest> CommandPtr;

class AlertRequestTest : public CommandRequestTest<CommandsTestMocks::kIsNice> {
};

class CallOnTimeOut {
 public:
  CallOnTimeOut(CommandRequestImpl& command) : command_(command) {}

  void operator()() {
    command_.onTimeOut();
  }

  CommandRequestImpl& command_;
};

TEST_F(AlertRequestTest, OnTimeOut_UNSUCCESS) {
  MessageSharedPtr msg(CreateMessage(smart_objects::SmartType_Map));
  (*msg)[am::strings::msg_params][am::strings::soft_buttons] = 0;
  CommandPtr command(CreateCommand<AlertRequest>(msg));

  command->onTimeOut();
  EXPECT_CALL(app_mngr_, ManageMobileCommand(_, _)).Times(0);
}

TEST_F(AlertRequestTest, OnTimeOut_SUCCESS) {
  CommandPtr command(CreateCommand<AlertRequest>());

  MessageSharedPtr sended_result_msg(
      CreateMessage(smart_objects::SmartType_Null));
  EXPECT_CALL(
      *MockMessageHelper::message_helper_mock(),
      CreateNegativeResponse(_, _, _, mobile_apis::Result::GENERIC_ERROR))
      .WillOnce(Return(sended_result_msg));

  MessageSharedPtr received_result_msg(
      CatchMobileCommandResult(CallOnTimeOut(*command)));
  EXPECT_EQ(sended_result_msg, received_result_msg);
}

TEST_F(AlertRequestTest, Run_ApplicationIsNotRegistered_UNSUCCESS) {
  CommandPtr command(CreateCommand<AlertRequest>());

  ON_CALL(app_mngr_, application(_))
      .WillByDefault(Return(ApplicationSharedPtr()));

  MessageSharedPtr result_msg(CatchMobileCommandResult(CallRun(*command)));
  EXPECT_EQ(mobile_apis::Result::APPLICATION_NOT_REGISTERED,
            static_cast<mobile_apis::Result::eType>(
                (*result_msg)[am::strings::msg_params][am::strings::result_code]
                    .asInt()));
}

TEST_F(AlertRequestTest, Run_AlertFrequencyIsTooHigh_UNSUCCESS) {
  CommandPtr command(CreateCommand<AlertRequest>());

  MockAppPtr app(CreateMockApp());
  ON_CALL(app_mngr_, application(_)).WillByDefault(Return(app));

  ON_CALL(*app, hmi_level())
      .WillByDefault(Return(mobile_apis::HMILevel::HMI_BACKGROUND));
  ON_CALL(*app, AreCommandLimitsExceeded(_, _)).WillByDefault(Return(true));

  MessageSharedPtr result_msg(CatchMobileCommandResult(CallRun(*command)));
  EXPECT_EQ(mobile_apis::Result::REJECTED,
            static_cast<mobile_apis::Result::eType>(
                (*result_msg)[am::strings::msg_params][am::strings::result_code]
                    .asInt()));
}

TEST_F(AlertRequestTest, Run_FailToProcessSoftButtons_UNSUCCESS) {
  const mobile_apis::Result::eType kResultCode =
      mobile_apis::Result::INVALID_ENUM;

  CommandPtr command(CreateCommand<AlertRequest>());

  MockAppPtr app(CreateMockApp());
  ON_CALL(app_mngr_, application(_)).WillByDefault(Return(app));
  ON_CALL(*app, hmi_level())
      .WillByDefault(Return(mobile_apis::HMILevel::HMI_BACKGROUND));
  ON_CALL(*app, AreCommandLimitsExceeded(_, _)).WillByDefault(Return(false));

  ON_CALL(app_mngr_, GetPolicyHandler())
      .WillByDefault(
          ReturnRef(*static_cast<policy::PolicyHandlerInterface*>(NULL)));
  EXPECT_CALL(*MockMessageHelper::message_helper_mock(),
              ProcessSoftButtons(_, _, _, _)).WillOnce(Return(kResultCode));

  MessageSharedPtr result_msg(CatchMobileCommandResult(CallRun(*command)));
  EXPECT_EQ(kResultCode,
            static_cast<mobile_apis::Result::eType>(
                (*result_msg)[am::strings::msg_params][am::strings::result_code]
                    .asInt()));
}

TEST_F(AlertRequestTest, Run_MandatoryParametersAreMissed_UNSUCCESS) {
  CommandPtr command(CreateCommand<AlertRequest>());

  MockAppPtr app(CreateMockApp());
  ON_CALL(app_mngr_, application(_)).WillByDefault(Return(app));
  ON_CALL(*app, hmi_level())
      .WillByDefault(Return(mobile_apis::HMILevel::HMI_BACKGROUND));
  ON_CALL(*app, AreCommandLimitsExceeded(_, _)).WillByDefault(Return(false));

  ON_CALL(app_mngr_, GetPolicyHandler())
      .WillByDefault(
          ReturnRef(*static_cast<policy::PolicyHandlerInterface*>(NULL)));
  EXPECT_CALL(*MockMessageHelper::message_helper_mock(),
              ProcessSoftButtons(_, _, _, _))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));

  MessageSharedPtr result_msg(CatchMobileCommandResult(CallRun(*command)));
  EXPECT_EQ(mobile_apis::Result::INVALID_DATA,
            static_cast<mobile_apis::Result::eType>(
                (*result_msg)[am::strings::msg_params][am::strings::result_code]
                    .asInt()));
}

TEST_F(AlertRequestTest, Run_MandatoryParametersAreInvalid_UNSUCCESS) {
  MessageSharedPtr msg(CreateMessage(smart_objects::SmartType_Map));
  (*msg)[am::strings::msg_params][am::strings::alert_text1] = 0;
  (*msg)[am::strings::msg_params][am::strings::alert_text2] =
      "invalid\t\nParam";

  CommandPtr command(CreateCommand<AlertRequest>(msg));

  MockAppPtr app(CreateMockApp());
  ON_CALL(app_mngr_, application(_)).WillByDefault(Return(app));
  ON_CALL(*app, hmi_level())
      .WillByDefault(Return(mobile_apis::HMILevel::HMI_BACKGROUND));
  ON_CALL(*app, AreCommandLimitsExceeded(_, _)).WillByDefault(Return(false));

  MessageSharedPtr result_msg(CatchMobileCommandResult(CallRun(*command)));
  EXPECT_EQ(mobile_apis::Result::INVALID_DATA,
            static_cast<mobile_apis::Result::eType>(
                (*result_msg)[am::strings::msg_params][am::strings::result_code]
                    .asInt()));
}

TEST_F(AlertRequestTest, Run_SUCCESS) {
  MessageSharedPtr msg(CreateMessage(smart_objects::SmartType_Map));
  (*msg)[am::strings::msg_params][am::strings::alert_text1] = "alert_text1";
  (*msg)[am::strings::msg_params][am::strings::alert_text2] = "alert_text2";
  (*msg)[am::strings::msg_params][am::strings::tts_chunks][0]
        [am::strings::text] = "alert_text2";

  CommandPtr command(CreateCommand<AlertRequest>(msg));

  MockAppPtr app(CreateMockApp());
  ON_CALL(app_mngr_, application(_)).WillByDefault(Return(app));
  ON_CALL(*app, hmi_level())
      .WillByDefault(Return(mobile_apis::HMILevel::HMI_BACKGROUND));
  ON_CALL(*app, AreCommandLimitsExceeded(_, _)).WillByDefault(Return(false));

  ON_CALL(app_mngr_, GetPolicyHandler())
      .WillByDefault(
          ReturnRef(*static_cast<policy::PolicyHandlerInterface*>(NULL)));
  EXPECT_CALL(*MockMessageHelper::message_helper_mock(),
              ProcessSoftButtons(_, _, _, _))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));

  EXPECT_CALL(this->app_mngr_, ManageHMICommand(_))
      .Times(AtLeast(1))
      .WillRepeatedly(Return(true));

  command->Run();
}

}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
