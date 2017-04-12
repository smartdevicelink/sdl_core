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
#include "application_manager/commands/command_request_test.h"
#include "application_manager/application.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/mock_application.h"
#include "application_manager/mock_message_helper.h"
#include "event_engine/event.h"
#include "application_manager/commands/mobile/alert_maneuver_request.h"
#include "interfaces/MOBILE_API.h"
#include "application_manager/policies/policy_handler_interface.h"
#include "application_manager/policies/mock_policy_handler_interface.h"
#include "application_manager/mock_hmi_interface.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {
namespace alert_maneuver_request {

using ::testing::_;
using ::testing::Return;
using ::testing::ReturnRef;
namespace am = ::application_manager;
using am::commands::AlertManeuverRequest;
using am::commands::MessageSharedPtr;
using am::event_engine::Event;
using am::MockHmiInterfaces;
using am::MockMessageHelper;

typedef SharedPtr<AlertManeuverRequest> CommandPtr;

class AlertManeuverRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
  void CheckExpectations(const hmi_apis::Common_Result::eType hmi_response,
                         const mobile_apis::Result::eType mobile_response,
                         const am::HmiInterfaces::InterfaceState state,
                         const bool success) {
    MessageSharedPtr response = CreateMessage(smart_objects::SmartType_Map);
    (*response)[am::strings::params][am::hmi_response::code] = hmi_response;
    (*response)[am::strings::msg_params][am::strings::info] = "test";

    am::event_engine::Event event(hmi_apis::FunctionID::TTS_Speak);
    event.set_smart_object(*response);

    utils::SharedPtr<AlertManeuverRequest> command =
        CreateCommand<AlertManeuverRequest>(response);

    MockAppPtr mock_app(CreateMockApp());
    ON_CALL(app_mngr_, application(_)).WillByDefault(Return(mock_app));

    MockMessageHelper* mock_message_helper =
        MockMessageHelper::message_helper_mock();
    EXPECT_CALL(*mock_message_helper, HMIToMobileResult(_))
        .WillOnce(Return(mobile_apis::Result::UNSUPPORTED_RESOURCE));

    MockHmiInterfaces hmi_interfaces;
    EXPECT_CALL(app_mngr_, hmi_interfaces())
        .WillRepeatedly(ReturnRef(hmi_interfaces));
    EXPECT_CALL(hmi_interfaces, GetInterfaceState(_))
        .WillRepeatedly(Return(state));

    MessageSharedPtr response_to_mobile;
    EXPECT_CALL(app_mngr_,
                ManageMobileCommand(
                    _, am::commands::Command::CommandOrigin::ORIGIN_SDL))
        .WillOnce(DoAll(SaveArg<0>(&response_to_mobile), Return(true)));
    command->on_event(event);

    EXPECT_EQ(
        (*response_to_mobile)[am::strings::msg_params][am::strings::success]
            .asBool(),
        success);
    EXPECT_EQ(
        (*response_to_mobile)[am::strings::msg_params][am::strings::result_code]
            .asInt(),
        static_cast<int32_t>(mobile_response));
  }

 protected:
  NiceMock<policy_test::MockPolicyHandlerInterface> policy_interface_;
};

TEST_F(AlertManeuverRequestTest, Run_RequiredFieldsDoesNotExist_UNSUCCESS) {
  CommandPtr command(CreateCommand<AlertManeuverRequest>());
  EXPECT_CALL(app_mngr_, application(_)).Times(0);
  MessageSharedPtr result_msg(CatchMobileCommandResult(CallRun(*command)));
  EXPECT_EQ(mobile_apis::Result::INVALID_DATA,
            static_cast<mobile_apis::Result::eType>(
                (*result_msg)[am::strings::msg_params][am::strings::result_code]
                    .asInt()));
}

TEST_F(AlertManeuverRequestTest, Run_ApplicationIsNotRegistered_UNSUCCESS) {
  MessageSharedPtr msg = CreateMessage(smart_objects::SmartType_Map);
  (*msg)[am::strings::msg_params][am::strings::soft_buttons] = 0;
  (*msg)[am::strings::msg_params][am::strings::tts_chunks] = 0;

  CommandPtr command(CreateCommand<AlertManeuverRequest>(msg));

  ON_CALL(app_mngr_, application(_))
      .WillByDefault(Return(ApplicationSharedPtr()));

  MessageSharedPtr result_msg(CatchMobileCommandResult(CallRun(*command)));
  EXPECT_EQ(mobile_apis::Result::APPLICATION_NOT_REGISTERED,
            static_cast<mobile_apis::Result::eType>(
                (*result_msg)[am::strings::msg_params][am::strings::result_code]
                    .asInt()));
}

TEST_F(AlertManeuverRequestTest, Run_ProcessingResult_UNSUCCESS) {
  MessageSharedPtr msg = CreateMessage(smart_objects::SmartType_Map);
  (*msg)[am::strings::msg_params][am::strings::soft_buttons] = 0;

  CommandPtr command(CreateCommand<AlertManeuverRequest>(msg));

  MockAppPtr app(CreateMockApp());
  ON_CALL(app_mngr_, application(_)).WillByDefault(Return(app));

  ON_CALL(app_mngr_, GetPolicyHandler())
      .WillByDefault(ReturnRef(policy_interface_));

  const mobile_apis::Result::eType kProcessingResult =
      mobile_apis::Result::ABORTED;

  EXPECT_CALL(*(am::MockMessageHelper::message_helper_mock()),
              ProcessSoftButtons(_, _, _, _))
      .WillOnce(Return(kProcessingResult));

  MessageSharedPtr result_msg(CatchMobileCommandResult(CallRun(*command)));
  EXPECT_EQ(kProcessingResult,
            static_cast<mobile_apis::Result::eType>(
                (*result_msg)[am::strings::msg_params][am::strings::result_code]
                    .asInt()));
}

TEST_F(AlertManeuverRequestTest, Run_IsWhiteSpaceExist_UNSUCCESS) {
  MessageSharedPtr msg = CreateMessage(smart_objects::SmartType_Map);
  (*msg)[am::strings::msg_params][am::strings::soft_buttons] = 0;
  (*msg)[am::strings::msg_params][am::strings::tts_chunks] =
      SmartObject(smart_objects::SmartType_Array);

  SmartObject tts_chunk(smart_objects::SmartType_Map);
  tts_chunk[am::strings::text] = "wrong chunk syntax\t\n";

  (*msg)[am::strings::msg_params][am::strings::tts_chunks].asArray()->push_back(
      tts_chunk);

  CommandPtr command(CreateCommand<AlertManeuverRequest>(msg));

  MockAppPtr app(CreateMockApp());
  ON_CALL(app_mngr_, application(_)).WillByDefault(Return(app));

  MessageSharedPtr result_msg(CatchMobileCommandResult(CallRun(*command)));
  EXPECT_EQ(mobile_apis::Result::INVALID_DATA,
            static_cast<mobile_apis::Result::eType>(
                (*result_msg)[am::strings::msg_params][am::strings::result_code]
                    .asInt()));
}

TEST_F(AlertManeuverRequestTest, Run_ProcessingResult_SUCCESS) {
  MessageSharedPtr msg = CreateMessage(smart_objects::SmartType_Map);
  (*msg)[am::strings::msg_params][am::strings::soft_buttons] = 0;

  CommandPtr command(CreateCommand<AlertManeuverRequest>(msg));

  MockAppPtr app(CreateMockApp());
  ON_CALL(app_mngr_, application(_)).WillByDefault(Return(app));

  ON_CALL(app_mngr_, GetPolicyHandler())
      .WillByDefault(ReturnRef(policy_interface_));

  EXPECT_CALL(*(am::MockMessageHelper::message_helper_mock()),
              ProcessSoftButtons(_, _, _, _))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));

  MockHmiInterfaces hmi_interfaces;
  EXPECT_CALL(app_mngr_, hmi_interfaces())
      .WillRepeatedly(ReturnRef(hmi_interfaces));
  EXPECT_CALL(hmi_interfaces, GetInterfaceFromFunction(_))
      .WillRepeatedly(
          Return(am::HmiInterfaces::InterfaceID::HMI_INTERFACE_TTS));
  EXPECT_CALL(hmi_interfaces, GetInterfaceState(_))
      .WillRepeatedly(Return(am::HmiInterfaces::STATE_AVAILABLE));

  EXPECT_CALL(*(am::MockMessageHelper::message_helper_mock()),
              SubscribeApplicationToSoftButton(_, _, _));

  MessageSharedPtr result_msg(CatchHMICommandResult(CallRun(*command)));
  EXPECT_EQ(hmi_apis::FunctionID::Navigation_AlertManeuver,
            static_cast<hmi_apis::FunctionID::eType>(
                (*result_msg)[am::strings::params][am::strings::function_id]
                    .asInt()));
}

TEST_F(AlertManeuverRequestTest, OnEvent_ReceivedUnknownEvent_UNSUCCESS) {
  CommandPtr command(CreateCommand<AlertManeuverRequest>());
  Event event(hmi_apis::FunctionID::INVALID_ENUM);

  MessageSharedPtr result_msg(
      CatchMobileCommandResult(CallOnEvent(*command, event)));
  EXPECT_EQ(mobile_apis::Result::INVALID_ENUM,
            static_cast<mobile_apis::Result::eType>(
                (*result_msg)[am::strings::msg_params][am::strings::result_code]
                    .asInt()));
}

TEST_F(AlertManeuverRequestTest, OnEvent_UNSUPPORTED_RESOURCE_Case1) {
  CheckExpectations(hmi_apis::Common_Result::SUCCESS,
                    mobile_apis::Result::UNSUPPORTED_RESOURCE,
                    am::HmiInterfaces::STATE_AVAILABLE,
                    true);
}

TEST_F(AlertManeuverRequestTest, OnEvent_UNSUPPORTED_RESOURCE_Case2) {
  CheckExpectations(hmi_apis::Common_Result::SUCCESS,
                    mobile_apis::Result::UNSUPPORTED_RESOURCE,
                    am::HmiInterfaces::STATE_NOT_AVAILABLE,
                    true);
}

TEST_F(AlertManeuverRequestTest, OnEvent_UNSUPPORTED_RESOURCE_Case3) {
  CheckExpectations(hmi_apis::Common_Result::SUCCESS,
                    mobile_apis::Result::UNSUPPORTED_RESOURCE,
                    am::HmiInterfaces::STATE_NOT_RESPONSE,
                    true);
}

TEST_F(AlertManeuverRequestTest, OnEvent_UNSUPPORTED_RESOURCE_Case4) {
  CheckExpectations(hmi_apis::Common_Result::GENERIC_ERROR,
                    mobile_apis::Result::UNSUPPORTED_RESOURCE,
                    am::HmiInterfaces::STATE_NOT_RESPONSE,
                    false);
}

}  // namespace alert_maneuver_request
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
