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

#include "mobile/create_window_request.h"

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
namespace create_window_request {

namespace am = application_manager;
using am::MockMessageHelper;
using am::commands::CommandImpl;
using am::commands::MessageSharedPtr;
using sdl_rpc_plugin::commands::CreateWindowRequest;
using namespace mobile_apis::PredefinedWindows;
using am::commands::Command;
using application_manager::WindowID;
using test::components::application_manager_test::MockStateController;
using test::components::policy_test::MockPolicyHandlerInterface;
using ::testing::_;
using ::testing::Return;
using ::testing::ReturnRef;

namespace {
const uint32_t kAppId = 1u;
const uint32_t kConnectionKey = 2u;
const int32_t kFunctionID = mobile_apis::FunctionID::CreateWindowID;
const WindowID kTestWindowId = 12;
const WindowID kDuplicateWindowID = 13;
const char* const kWindowName = "WindowName";
const utils::custom_string::CustomString kAppName("TestApp");
}  // namespace

MATCHER_P2(CheckMessageToMobile, result_code, success, "") {
  const bool is_success =
      (*arg)[am::strings::msg_params][am::strings::success].asBool() == success;

  const bool is_result_code_correct =
      (*arg)[am::strings::msg_params][am::strings::result_code].asInt() ==
      static_cast<int32_t>(result_code);
  return is_success && is_result_code_correct;
}

class CreateWindowRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
  CreateWindowRequestTest() {
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

  void SetupHelperApplyWindowInitialState(const MockAppPtr& app,
                                          const MessageSharedPtr& msg) {
    am::HmiStatePtr state(new am::HmiState(app, app_mngr_));
    const auto window_type = static_cast<mobile_apis::WindowType::eType>(
        (*msg)[am::strings::msg_params][am::strings::window_type].asInt());
    const auto window_id = static_cast<mobile_apis::WindowType::eType>(
        (*msg)[am::strings::msg_params][am::strings::window_id].asInt());
    const auto window_name =
        (*msg)[am::strings::msg_params][am::strings::window_name].asString();

    state->set_window_type(window_type);
    state->set_hmi_level(mobile_apis::HMILevel::INVALID_ENUM);
    state->set_audio_streaming_state(
        mobile_apis::AudioStreamingState::INVALID_ENUM);
    state->set_video_streaming_state(
        mobile_apis::VideoStreamingState::INVALID_ENUM);
    state->set_system_context(mobile_apis::SystemContext::INVALID_ENUM);
    ApplicationSharedPtr AppPtr = app;
    ON_CALL(app_mngr_,
            CreateRegularState(AppPtr,
                               window_type,
                               mobile_apis::HMILevel::INVALID_ENUM,
                               mobile_apis::AudioStreamingState::INVALID_ENUM,
                               mobile_apis::VideoStreamingState::INVALID_ENUM,
                               mobile_apis::SystemContext::INVALID_ENUM))
        .WillByDefault(Return(state));
    ON_CALL(*mock_app_, SetInitialState(kTestWindowId, window_name, state))
        .WillByDefault(Return());
    ON_CALL(app_mngr_, state_controller())
        .WillByDefault(ReturnRef(mock_state_controller));
    ON_CALL(
        mock_state_controller,
        OnAppWindowAdded(
            AppPtr, window_id, window_type, mobile_apis::HMILevel::HMI_NONE))
        .WillByDefault(Return());
  }

  void SetUp() OVERRIDE {
    using namespace application_manager;
    display_capabilities_ = std::make_shared<smart_objects::SmartObject>(
        smart_objects::SmartType_Array);

    smart_objects::SmartObject window_type_supported(
        smart_objects::SmartType_Array);

    const uint32_t maximum_widgets_amount = 4;
    smart_objects::SmartObject maximum_widgets(smart_objects::SmartType_Map);
    maximum_widgets[strings::window_type] = mobile_apis::WindowType::WIDGET;
    maximum_widgets[strings::maximum_number_of_windows] =
        maximum_widgets_amount;

    window_type_supported[window_type_supported.length()] = maximum_widgets;

    (*display_capabilities_)[0][strings::window_type_supported] =
        window_type_supported;

    ON_CALL(*mock_app_, display_capabilities())
        .WillByDefault(Return(display_capabilities_));

    window_params_map_lock_ptr_ = std::make_shared<sync_primitives::Lock>();

    DataAccessor<am::WindowParamsMap> window_params_map(
        test_window_params_map_, window_params_map_lock_ptr_);
    ON_CALL(*mock_app_, window_optional_params_map())
        .WillByDefault(Return(window_params_map));
  }

  MockAppPtr mock_app_;
  NiceMock<MockStateController> mock_state_controller;
  std::shared_ptr<sync_primitives::Lock> window_params_map_lock_ptr_;
  application_manager::WindowParamsMap test_window_params_map_;
  smart_objects::SmartObjectSPtr display_capabilities_;
};

TEST_F(CreateWindowRequestTest, WindowID_ExpectDefaultWindowID) {
  auto msg = CreateMsgParams();
  const auto command = CreateCommand<CreateWindowRequest>(msg);
  EXPECT_EQ(mobile_apis::PredefinedWindows::DEFAULT_WINDOW,
            command->window_id());
}

TEST_F(CreateWindowRequestTest,
       Run_AppDoesNotExist_ExpectAppNotRegisteredResponseToMobile) {
  const auto result_code = mobile_apis::Result::APPLICATION_NOT_REGISTERED;
  EXPECT_CALL(mock_rpc_service_,
              ManageMobileCommand(CheckMessageToMobile(result_code, false),
                                  Command::CommandSource::SOURCE_SDL))
      .WillOnce(Return(true));
  EXPECT_CALL(app_mngr_, application(kConnectionKey)).WillOnce(Return(nullptr));

  auto msg = CreateMsgParams();
  auto command = CreateCommand<CreateWindowRequest>(msg);
  EXPECT_TRUE(command->Init());
  command->Run();
}

TEST_F(CreateWindowRequestTest,
       Run_WindowIDAlreadyExist_ExpectInvalidIDResponseToMobile) {
  const auto result_code = mobile_apis::Result::INVALID_ID;
  EXPECT_CALL(mock_rpc_service_,
              ManageMobileCommand(CheckMessageToMobile(result_code, false),
                                  Command::CommandSource::SOURCE_SDL))
      .WillOnce(Return(true));
  ON_CALL(*mock_app_, WindowIdExists(kTestWindowId))
      .WillByDefault(Return(true));

  auto msg = CreateMsgParams();
  (*msg)[am::strings::msg_params][am::strings::window_id] = kTestWindowId;

  auto command = CreateCommand<CreateWindowRequest>(msg);
  EXPECT_TRUE(command->Init());
  command->Run();
}

TEST_F(CreateWindowRequestTest,
       Run_CreateWindowForMAINWindowType_ExpectInvalidDataResponseToMobile) {
  const auto result_code = mobile_apis::Result::INVALID_DATA;
  EXPECT_CALL(mock_rpc_service_,
              ManageMobileCommand(CheckMessageToMobile(result_code, false),
                                  Command::CommandSource::SOURCE_SDL))
      .WillOnce(Return(true));
  ON_CALL(*mock_app_, WindowIdExists(kTestWindowId))
      .WillByDefault(Return(false));

  auto msg = CreateMsgParams();
  (*msg)[am::strings::msg_params][am::strings::window_id] = kTestWindowId;
  (*msg)[am::strings::msg_params][am::strings::window_type] =
      mobile_apis::WindowType::eType::MAIN;

  auto command = CreateCommand<CreateWindowRequest>(msg);
  EXPECT_TRUE(command->Init());
  command->Run();
}

TEST_F(
    CreateWindowRequestTest,
    Run_DuplicateUpdatesFromNotExistingWindowId_ExpectInvalidDataResponseToMobile) {
  const auto result_code = mobile_apis::Result::INVALID_DATA;
  EXPECT_CALL(mock_rpc_service_,
              ManageMobileCommand(CheckMessageToMobile(result_code, false),
                                  Command::CommandSource::SOURCE_SDL))
      .WillOnce(Return(true));
  ON_CALL(*mock_app_, WindowIdExists(kTestWindowId))
      .WillByDefault(Return(false));
  ON_CALL(*mock_app_, WindowIdExists(kDuplicateWindowID))
      .WillByDefault(Return(false));

  auto msg = CreateMsgParams();
  (*msg)[am::strings::msg_params][am::strings::window_id] = kTestWindowId;
  (*msg)[am::strings::msg_params]
        [am::strings::duplicate_updates_from_window_id] = kDuplicateWindowID;

  auto command = CreateCommand<CreateWindowRequest>(msg);
  EXPECT_TRUE(command->Init());
  command->Run();
}

TEST_F(
    CreateWindowRequestTest,
    Run_DuplicateUpdatesFromExistingWindowId_ExpectUICreateWindowIsSentToHMI) {
  EXPECT_CALL(
      mock_rpc_service_,
      ManageHMICommand(HMIResultCodeIs(hmi_apis::FunctionID::UI_CreateWindow),
                       Command::CommandSource::SOURCE_SDL_TO_HMI))
      .WillOnce(Return(true));
  ON_CALL(*mock_app_, WindowIdExists(kTestWindowId))
      .WillByDefault(Return(false));
  ON_CALL(*mock_app_, WindowIdExists(kDuplicateWindowID))
      .WillByDefault(Return(true));
  ON_CALL(*mock_app_, name()).WillByDefault(ReturnRef(kAppName));
  ON_CALL(*mock_app_, GetWindowNames())
      .WillByDefault(Return(std::vector<std::string>()));

  auto msg = CreateMsgParams();
  (*msg)[am::strings::msg_params][am::strings::window_id] = kTestWindowId;
  (*msg)[am::strings::msg_params]
        [am::strings::duplicate_updates_from_window_id] = kDuplicateWindowID;
  (*msg)[am::strings::msg_params][am::strings::window_type] =
      mobile_apis::WindowType::WIDGET;

  auto command = CreateCommand<CreateWindowRequest>(msg);
  EXPECT_TRUE(command->Init());
  command->Run();
}

TEST_F(CreateWindowRequestTest,
       Run_WindowNameAppNameAreEqual_ExpectDuplicateNameResponseToMobile) {
  const auto result_code = mobile_apis::Result::DUPLICATE_NAME;
  EXPECT_CALL(mock_rpc_service_,
              ManageMobileCommand(CheckMessageToMobile(result_code, false),
                                  Command::CommandSource::SOURCE_SDL))
      .WillOnce(Return(true));
  ON_CALL(*mock_app_, WindowIdExists(kTestWindowId))
      .WillByDefault(Return(false));
  ON_CALL(*mock_app_, name()).WillByDefault(ReturnRef(kAppName));

  auto msg = CreateMsgParams();
  (*msg)[am::strings::msg_params][am::strings::window_id] = kTestWindowId;
  (*msg)[am::strings::msg_params][am::strings::window_name] = kAppName;

  auto command = CreateCommand<CreateWindowRequest>(msg);
  EXPECT_TRUE(command->Init());
  command->Run();
}

TEST_F(CreateWindowRequestTest,
       Run_AllParametersCorrect_ExpectUICreateWindowIsSentToHMI) {
  EXPECT_CALL(
      mock_rpc_service_,
      ManageHMICommand(HMIResultCodeIs(hmi_apis::FunctionID::UI_CreateWindow),
                       Command::CommandSource::SOURCE_SDL_TO_HMI))
      .WillOnce(Return(true));
  ON_CALL(*mock_app_, WindowIdExists(kTestWindowId))
      .WillByDefault(Return(false));
  ON_CALL(*mock_app_, WindowIdExists(kDuplicateWindowID))
      .WillByDefault(Return(true));
  ON_CALL(*mock_app_, name()).WillByDefault(ReturnRef(kAppName));

  const std::vector<std::string> windowNames = {
      "WindowName_1", "WindowName_2", "WindowName_3"};
  ON_CALL(*mock_app_, GetWindowNames()).WillByDefault(Return(windowNames));

  auto msg = CreateMsgParams();
  (*msg)[am::strings::msg_params][am::strings::window_id] = kTestWindowId;
  (*msg)[am::strings::msg_params][am::strings::window_name] = kWindowName;
  (*msg)[am::strings::msg_params]
        [am::strings::duplicate_updates_from_window_id] = kDuplicateWindowID;
  (*msg)[am::strings::msg_params][am::strings::associated_service_type] =
      "MEDIA";
  (*msg)[am::strings::msg_params][am::strings::window_type] =
      mobile_apis::WindowType::WIDGET;

  auto command = CreateCommand<CreateWindowRequest>(msg);
  EXPECT_TRUE(command->Init());
  command->Run();
}

TEST_F(
    CreateWindowRequestTest,
    CheckWindowName_AllParametersCorrectCreateWindowForPrimaryWidget_SendMessageToHMI) {
  EXPECT_CALL(
      mock_rpc_service_,
      ManageHMICommand(HMIResultCodeIs(hmi_apis::FunctionID::UI_CreateWindow),
                       Command::CommandSource::SOURCE_SDL_TO_HMI))
      .WillOnce(Return(true));
  ON_CALL(*mock_app_, WindowIdExists(kTestWindowId))
      .WillByDefault(Return(false));
  ON_CALL(*mock_app_, WindowIdExists(kDuplicateWindowID))
      .WillByDefault(Return(true));
  ON_CALL(*mock_app_, name()).WillByDefault(ReturnRef(kAppName));

  auto msg = CreateMsgParams();
  (*msg)[am::strings::msg_params][am::strings::window_id] =
      mobile_apis::PredefinedWindows::PRIMARY_WIDGET;
  (*msg)[am::strings::msg_params][am::strings::window_name] = kWindowName;
  (*msg)[am::strings::msg_params]
        [am::strings::duplicate_updates_from_window_id] = kDuplicateWindowID;
  (*msg)[am::strings::msg_params][am::strings::associated_service_type] =
      "MEDIA";
  (*msg)[am::strings::msg_params][am::strings::window_type] =
      mobile_apis::WindowType::WIDGET;

  auto command = CreateCommand<CreateWindowRequest>(msg);
  EXPECT_TRUE(command->Init());
  command->Run();
}

TEST_F(CreateWindowRequestTest,
       OnEvent_InvalidFunctionId_MessageNotSendToMobile) {
  EXPECT_CALL(mock_rpc_service_, ManageMobileCommand(_, _)).Times(0);
  auto msg = CreateMsgParams();
  auto command = CreateCommand<CreateWindowRequest>(msg);
  EXPECT_TRUE(command->Init());
  Event event(hmi_apis::FunctionID::INVALID_ENUM);
  command->on_event(event);
}

TEST_F(CreateWindowRequestTest,
       OnEvent_AppDoesNotExist_ExpectAppNotRegisteredResponseToMobile) {
  const auto result_code = mobile_apis::Result::APPLICATION_NOT_REGISTERED;
  EXPECT_CALL(mock_rpc_service_,
              ManageMobileCommand(CheckMessageToMobile(result_code, false),
                                  Command::CommandSource::SOURCE_SDL))
      .WillOnce(Return(true));
  EXPECT_CALL(app_mngr_, application(kConnectionKey)).WillOnce(Return(nullptr));

  auto msg = CreateMsgParams();
  auto command = CreateCommand<CreateWindowRequest>(msg);
  EXPECT_TRUE(command->Init());
  Event event(hmi_apis::FunctionID::UI_CreateWindow);
  command->on_event(event);
}

TEST_F(CreateWindowRequestTest,
       OnEvent_RequestFailedOnHMISide_SendGenericErrorResponseToMobile) {
  const auto result_code = mobile_apis::Result::GENERIC_ERROR;
  EXPECT_CALL(mock_rpc_service_,
              ManageMobileCommand(CheckMessageToMobile(result_code, false),
                                  Command::CommandSource::SOURCE_SDL))
      .WillOnce(Return(true));

  auto msg = CreateMsgParams();
  (*msg)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::GENERIC_ERROR;
  (*msg)[am::strings::msg_params][am::strings::window_name] = kWindowName;
  (*msg)[am::strings::msg_params][am::strings::window_id] = kTestWindowId;
  (*msg)[am::strings::msg_params][am::strings::window_type] =
      mobile_apis::WindowType::eType::WIDGET;

  SetupHelperApplyWindowInitialState(mock_app_, msg);

  Event event(hmi_apis::FunctionID::UI_CreateWindow);
  event.set_smart_object(*msg);

  auto command = CreateCommand<CreateWindowRequest>(msg);
  EXPECT_TRUE(command->Init());
  command->on_event(event);
}

TEST_F(CreateWindowRequestTest,
       OnEvent_AllParametersCorrect_SendSuccessResponseToMobile) {
  const auto result_code = mobile_apis::Result::SUCCESS;
  EXPECT_CALL(mock_rpc_service_,
              ManageMobileCommand(CheckMessageToMobile(result_code, true),
                                  Command::CommandSource::SOURCE_SDL))
      .WillOnce(Return(true));
  EXPECT_CALL(*mock_app_, UpdateHash()).Times(1);
  auto msg = CreateMsgParams();
  (*msg)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::SUCCESS;
  (*msg)[am::strings::msg_params][am::strings::window_name] = kWindowName;
  (*msg)[am::strings::msg_params][am::strings::window_id] = kTestWindowId;
  (*msg)[am::strings::msg_params][am::strings::window_type] =
      mobile_apis::WindowType::eType::WIDGET;

  SetupHelperApplyWindowInitialState(mock_app_, msg);

  Event event(hmi_apis::FunctionID::UI_CreateWindow);
  event.set_smart_object(*msg);

  auto command = CreateCommand<CreateWindowRequest>(msg);
  EXPECT_TRUE(command->Init());
  command->on_event(event);
}

}  // namespace create_window_request
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
