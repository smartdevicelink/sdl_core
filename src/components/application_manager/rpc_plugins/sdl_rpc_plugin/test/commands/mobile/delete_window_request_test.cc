/*
 * Copyright (c) 2019, Ford Motor Company
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
#include <memory>
#include <set>
#include <string>

#include "mobile/delete_window_request.h"

#include "application_manager/commands/command_request_test.h"
#include "application_manager/event_engine/event.h"
#include "application_manager/hmi_state.h"
#include "application_manager/mock_application.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/mock_hmi_interface.h"
#include "application_manager/mock_message_helper.h"
#include "application_manager/mock_state_controller.h"
#include "gtest/gtest.h"
#include "interfaces/MOBILE_API.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {
namespace delete_window_request {

namespace am = application_manager;
using am::MockMessageHelper;
using am::commands::CommandImpl;
using am::commands::MessageSharedPtr;
using sdl_rpc_plugin::commands::DeleteWindowRequest;
using namespace mobile_apis::PredefinedWindows;
using am::commands::Command;
using application_manager::WindowID;
using test::components::application_manager_test::MockStateController;
using test::components::policy_test::MockPolicyHandlerInterface;
using ::testing::_;
using ::testing::Return;
using ::testing::ReturnRef;

namespace {
const uint32_t kConnectionKey = 2u;
const int32_t kFunctionID = mobile_apis::FunctionID::DeleteWindowID;
const WindowID kTestWindowId = 12;
}  // namespace

MATCHER_P2(CheckMessageToMobile, result_code, success, "") {
  const bool is_success =
      (*arg)[am::strings::msg_params][am::strings::success].asBool() == success;

  const bool is_result_code_correct =
      (*arg)[am::strings::msg_params][am::strings::result_code].asInt() ==
      static_cast<int32_t>(result_code);
  return is_success && is_result_code_correct;
}

class DeleteWindowRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
  DeleteWindowRequestTest() {
    mock_app_ = CreateMockApp();
    ON_CALL(app_mngr_, application(kConnectionKey))
        .WillByDefault(Return(mock_app_));
  }

 protected:
  MessageSharedPtr CreateMsgParams() {
    auto msg = CreateMessage();
    (*msg)[am::strings::params][am::strings::connection_key] = kConnectionKey;
    (*msg)[am::strings::params][am::strings::function_id] = kFunctionID;
    return msg;
  }

  MockAppPtr mock_app_;
  NiceMock<MockStateController> mock_state_controller;
};

TEST_F(DeleteWindowRequestTest, WindowID_ExpectDefaultWindowID) {
  auto msg = CreateMsgParams();
  const auto command = CreateCommand<DeleteWindowRequest>(msg);
  EXPECT_EQ(mobile_apis::PredefinedWindows::DEFAULT_WINDOW,
            command->window_id());
}

TEST_F(DeleteWindowRequestTest,
       Run_AppDoesNotExist_ExpectAppNotRegisteredResponseToMobile) {
  const auto result_code = mobile_apis::Result::APPLICATION_NOT_REGISTERED;
  EXPECT_CALL(mock_rpc_service_,
              ManageMobileCommand(CheckMessageToMobile(result_code, false),
                                  Command::CommandSource::SOURCE_SDL))
      .WillOnce(Return(true));
  EXPECT_CALL(app_mngr_, application(kConnectionKey)).WillOnce(Return(nullptr));

  auto msg = CreateMsgParams();
  auto command = CreateCommand<DeleteWindowRequest>(msg);
  EXPECT_TRUE(command->Init());
  command->Run();
}

TEST_F(DeleteWindowRequestTest,
       Run_WindowIDForMainApplicationWindow_ExpectInvalidIDResponseToMobile) {
  const auto result_code = mobile_apis::Result::INVALID_ID;
  EXPECT_CALL(mock_rpc_service_,
              ManageMobileCommand(CheckMessageToMobile(result_code, false),
                                  Command::CommandSource::SOURCE_SDL))
      .WillOnce(Return(true));

  auto msg = CreateMsgParams();
  (*msg)[am::strings::msg_params][am::strings::window_id] =
      mobile_apis::PredefinedWindows::DEFAULT_WINDOW;

  auto command = CreateCommand<DeleteWindowRequest>(msg);
  EXPECT_TRUE(command->Init());
  command->Run();
}

TEST_F(DeleteWindowRequestTest,
       Run_WindowIDNotExist_ExpectInvalidIDResponseToMobile) {
  const auto result_code = mobile_apis::Result::INVALID_ID;
  EXPECT_CALL(mock_rpc_service_,
              ManageMobileCommand(CheckMessageToMobile(result_code, false),
                                  Command::CommandSource::SOURCE_SDL))
      .WillOnce(Return(true));
  ON_CALL(*mock_app_, GetWindowIds()).WillByDefault(Return(am::WindowIds()));

  auto msg = CreateMsgParams();
  (*msg)[am::strings::msg_params][am::strings::window_id] = kTestWindowId;

  auto command = CreateCommand<DeleteWindowRequest>(msg);
  EXPECT_TRUE(command->Init());
  command->Run();
}

TEST_F(DeleteWindowRequestTest,
       Run_AllParametersCorrect_ExpectUIDeleteWindowIsSentToHMI) {
  EXPECT_CALL(
      mock_rpc_service_,
      ManageHMICommand(HMIResultCodeIs(hmi_apis::FunctionID::UI_DeleteWindow),
                       Command::CommandSource::SOURCE_SDL_TO_HMI))
      .WillOnce(Return(true));

  ON_CALL(*mock_app_, GetWindowIds())
      .WillByDefault(Return(am::WindowIds(1, kTestWindowId)));
  EXPECT_CALL(*mock_app_, WindowIdExists(kTestWindowId)).WillOnce(Return(true));
  auto msg = CreateMsgParams();
  (*msg)[am::strings::msg_params][am::strings::window_id] = kTestWindowId;

  auto command = CreateCommand<DeleteWindowRequest>(msg);
  EXPECT_TRUE(command->Init());
  command->Run();
}

TEST_F(DeleteWindowRequestTest,
       Run_AllParametersCorrectDeleteWindowForPrimaryWidget_SendMessageToHMI) {
  EXPECT_CALL(
      mock_rpc_service_,
      ManageHMICommand(HMIResultCodeIs(hmi_apis::FunctionID::UI_DeleteWindow),
                       Command::CommandSource::SOURCE_SDL_TO_HMI))
      .WillOnce(Return(true));
  const auto primary_widget_window_id =
      mobile_apis::PredefinedWindows::PRIMARY_WIDGET;
  ON_CALL(*mock_app_, GetWindowIds())
      .WillByDefault(Return(am::WindowIds(1, primary_widget_window_id)));
  EXPECT_CALL(*mock_app_, WindowIdExists(primary_widget_window_id))
      .WillOnce(Return(true));
  auto msg = CreateMsgParams();
  (*msg)[am::strings::msg_params][am::strings::window_id] =
      primary_widget_window_id;

  auto command = CreateCommand<DeleteWindowRequest>(msg);
  EXPECT_TRUE(command->Init());
  command->Run();
}

TEST_F(DeleteWindowRequestTest,
       OnEvent_InvalidFunctionId_MessageNotSendToMobile) {
  EXPECT_CALL(mock_rpc_service_, ManageMobileCommand(_, _)).Times(0);
  auto msg = CreateMsgParams();
  auto command = CreateCommand<DeleteWindowRequest>(msg);
  EXPECT_TRUE(command->Init());
  Event event(hmi_apis::FunctionID::INVALID_ENUM);
  command->on_event(event);
}

TEST_F(DeleteWindowRequestTest,
       OnEvent_AppDoesNotExist_ExpectAppNotRegisteredResponseToMobile) {
  const auto result_code = mobile_apis::Result::APPLICATION_NOT_REGISTERED;
  EXPECT_CALL(mock_rpc_service_,
              ManageMobileCommand(CheckMessageToMobile(result_code, false),
                                  Command::CommandSource::SOURCE_SDL))
      .WillOnce(Return(true));
  EXPECT_CALL(app_mngr_, application(kConnectionKey)).WillOnce(Return(nullptr));

  auto msg = CreateMsgParams();
  auto command = CreateCommand<DeleteWindowRequest>(msg);
  EXPECT_TRUE(command->Init());
  Event event(hmi_apis::FunctionID::UI_DeleteWindow);
  command->on_event(event);
}

TEST_F(DeleteWindowRequestTest,
       OnEvent_RequestFailedOnHMISide_SendGenericErrorResponseToMobile) {
  const auto result_code = mobile_apis::Result::GENERIC_ERROR;
  EXPECT_CALL(mock_rpc_service_,
              ManageMobileCommand(CheckMessageToMobile(result_code, false),
                                  Command::CommandSource::SOURCE_SDL))
      .WillOnce(Return(true));

  auto msg = CreateMsgParams();
  (*msg)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::GENERIC_ERROR;

  Event event(hmi_apis::FunctionID::UI_DeleteWindow);
  event.set_smart_object(*msg);

  auto command = CreateCommand<DeleteWindowRequest>(msg);
  EXPECT_TRUE(command->Init());
  command->on_event(event);
}

TEST_F(DeleteWindowRequestTest,
       OnEvent_AllParametersCorrect_SendSuccessResponseToMobile) {
  const auto result_code = mobile_apis::Result::SUCCESS;
  EXPECT_CALL(mock_rpc_service_,
              ManageMobileCommand(CheckMessageToMobile(result_code, true),
                                  Command::CommandSource::SOURCE_SDL))
      .WillOnce(Return(true));
  EXPECT_CALL(*mock_app_, UpdateHash()).Times(1);
  EXPECT_CALL(*mock_app_, RemoveHMIState(kTestWindowId, _)).Times(1);

  auto msg = CreateMsgParams();
  (*msg)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::SUCCESS;
  (*msg)[am::strings::msg_params][am::strings::window_id] = kTestWindowId;

  Event event(hmi_apis::FunctionID::UI_DeleteWindow);
  event.set_smart_object(*msg);

  auto command = CreateCommand<DeleteWindowRequest>(msg);
  EXPECT_TRUE(command->Init());
  command->on_event(event);
}

}  // namespace delete_window_request
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
