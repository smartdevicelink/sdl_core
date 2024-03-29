/*
 * Copyright (c) 2018, Ford Motor Company
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

#include "application_manager/application.h"
#include "application_manager/commands/command_request_test.h"
#include "application_manager/commands/commands_test.h"
#include "application_manager/mock_application.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/mock_hmi_capabilities.h"
#include "application_manager/smart_object_keys.h"
#include "interfaces/MOBILE_API.h"
#include "mobile/subscribe_button_request.h"
#include "smart_objects/smart_object.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {
namespace subscribe_button_request {

using ::testing::_;
using ::testing::DoAll;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::SaveArg;
namespace am = ::application_manager;
using am::commands::MessageSharedPtr;
using sdl_rpc_plugin::commands::SubscribeButtonRequest;

typedef std::shared_ptr<SubscribeButtonRequest> CommandPtr;

class SubscribeButtonRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
  typedef TypeIf<kMocksAreNice,
                 NiceMock<application_manager_test::MockHMICapabilities>,
                 application_manager_test::MockHMICapabilities>::Result
      MockHMICapabilities;
};

typedef SubscribeButtonRequestTest::MockHMICapabilities MockHMICapabilities;
const utils::SemanticVersion mock_semantic_version(5, 0, 0);
const utils::SemanticVersion mock_base_rpc_version(4, 5, 1);

TEST_F(SubscribeButtonRequestTest, Run_AppNotRegistered_UNSUCCESS) {
  CommandPtr command(CreateCommand<SubscribeButtonRequest>());

  ON_CALL(app_mngr_, application(_))
      .WillByDefault(Return(std::shared_ptr<am::Application>()));

  MessageSharedPtr result_msg(CatchMobileCommandResult(CallRun(*command)));
  EXPECT_EQ(mobile_apis::Result::APPLICATION_NOT_REGISTERED,
            static_cast<mobile_apis::Result::eType>(
                (*result_msg)[am::strings::msg_params][am::strings::result_code]
                    .asInt()));
}

TEST_F(SubscribeButtonRequestTest, Run_SubscriptionNotAllowed_UNSUCCESS) {
  MessageSharedPtr msg(CreateMessage());
  (*msg)[am::strings::msg_params][am::strings::button_name] =
      mobile_apis::ButtonName::SEEKLEFT;
  CommandPtr command(CreateCommand<SubscribeButtonRequest>(msg));

  MockAppPtr app(CreateMockApp());
  ON_CALL(app_mngr_, application(_)).WillByDefault(Return(app));
  ON_CALL(*app, is_media_application()).WillByDefault(Return(false));

  MessageSharedPtr result_msg(CatchMobileCommandResult(CallRun(*command)));
  EXPECT_EQ(mobile_apis::Result::REJECTED,
            static_cast<mobile_apis::Result::eType>(
                (*result_msg)[am::strings::msg_params][am::strings::result_code]
                    .asInt()));
}

TEST_F(SubscribeButtonRequestTest, Run_NavSubscriptionNotAllowed_UNSUCCESS) {
  MessageSharedPtr msg(CreateMessage());
  (*msg)[am::strings::msg_params][am::strings::button_name] =
      mobile_apis::ButtonName::NAV_CENTER_LOCATION;
  CommandPtr command(CreateCommand<SubscribeButtonRequest>(msg));

  MockAppPtr app(CreateMockApp());
  ON_CALL(app_mngr_, application(_)).WillByDefault(Return(app));
  ON_CALL(*app, is_navi()).WillByDefault(Return(false));

  MessageSharedPtr result_msg(CatchMobileCommandResult(CallRun(*command)));
  EXPECT_EQ(mobile_apis::Result::REJECTED,
            static_cast<mobile_apis::Result::eType>(
                (*result_msg)[am::strings::msg_params][am::strings::result_code]
                    .asInt()));
}

TEST_F(SubscribeButtonRequestTest, Run_UiIsNotSupported_UNSUCCESS) {
  CommandPtr command(CreateCommand<SubscribeButtonRequest>());

  MockAppPtr app(CreateMockApp());
  ON_CALL(app_mngr_, application(_)).WillByDefault(Return(app));

  ON_CALL(*app, msg_version()).WillByDefault(ReturnRef(mock_semantic_version));
  ON_CALL(*app, is_media_application()).WillByDefault(Return(false));

  ON_CALL(mock_hmi_capabilities_, is_ui_cooperating())
      .WillByDefault(Return(false));

  MessageSharedPtr result_msg(CatchMobileCommandResult(CallRun(*command)));
  EXPECT_EQ(mobile_apis::Result::UNSUPPORTED_RESOURCE,
            static_cast<mobile_apis::Result::eType>(
                (*result_msg)[am::strings::msg_params][am::strings::result_code]
                    .asInt()));
}

TEST_F(SubscribeButtonRequestTest, Run_IsSubscribedToButton_UNSUCCESS) {
  const mobile_apis::ButtonName::eType kButtonName =
      mobile_apis::ButtonName::SEEKLEFT;

  MessageSharedPtr msg(CreateMessage());
  (*msg)[am::strings::msg_params][am::strings::button_name] = kButtonName;
  CommandPtr command(CreateCommand<SubscribeButtonRequest>(msg));

  MockAppPtr app(CreateMockApp());
  ON_CALL(app_mngr_, application(_)).WillByDefault(Return(app));
  ON_CALL(*app, msg_version()).WillByDefault(ReturnRef(mock_semantic_version));
  ON_CALL(*app, is_media_application()).WillByDefault(Return(true));

  ON_CALL(mock_hmi_capabilities_, is_ui_cooperating())
      .WillByDefault(Return(true));

  MessageSharedPtr button_caps_ptr(CreateMessage(smart_objects::SmartType_Map));
  (*button_caps_ptr)[0][am::hmi_response::button_name] = kButtonName;

  ON_CALL(mock_hmi_capabilities_, button_capabilities())
      .WillByDefault(Return(button_caps_ptr));

  ON_CALL(*app, IsSubscribedToButton(_)).WillByDefault(Return(true));

  MessageSharedPtr result_msg(CatchMobileCommandResult(CallRun(*command)));
  EXPECT_EQ(mobile_apis::Result::IGNORED,
            static_cast<mobile_apis::Result::eType>(
                (*result_msg)[am::strings::msg_params][am::strings::result_code]
                    .asInt()));
}

TEST_F(SubscribeButtonRequestTest, Run_SUCCESS) {
  const mobile_apis::ButtonName::eType kButtonName =
      mobile_apis::ButtonName::SEEKLEFT;

  MessageSharedPtr msg(CreateMessage());
  (*msg)[am::strings::msg_params][am::strings::button_name] = kButtonName;
  CommandPtr command(CreateCommand<SubscribeButtonRequest>(msg));

  MockAppPtr app(CreateMockApp());
  ON_CALL(app_mngr_, application(_)).WillByDefault(Return(app));
  ON_CALL(*app, msg_version()).WillByDefault(ReturnRef(mock_semantic_version));
  ON_CALL(*app, is_media_application()).WillByDefault(Return(true));

  ON_CALL(mock_hmi_capabilities_, is_ui_cooperating())
      .WillByDefault(Return(true));

  MessageSharedPtr button_caps_ptr(CreateMessage(smart_objects::SmartType_Map));
  (*button_caps_ptr)[0][am::hmi_response::button_name] = kButtonName;

  ON_CALL(mock_hmi_capabilities_, button_capabilities())
      .WillByDefault(Return(button_caps_ptr));

  ON_CALL(*app, IsSubscribedToButton(_)).WillByDefault(Return(false));

  MessageSharedPtr hmi_result_msg;

  EXPECT_CALL(mock_rpc_service_, ManageHMICommand(_, _))
      .WillOnce(DoAll(SaveArg<0>(&hmi_result_msg), Return(true)));

  ASSERT_TRUE(command->Init());
  command->Run();

  EXPECT_EQ(hmi_apis::FunctionID::Buttons_SubscribeButton,
            static_cast<hmi_apis::FunctionID::eType>(
                (*hmi_result_msg)[am::strings::params][am::strings::function_id]
                    .asInt()));

  EXPECT_EQ(
      hmi_apis::Common_ButtonName::eType::SEEKLEFT,
      static_cast<hmi_apis::Common_ButtonName::eType>(
          (*hmi_result_msg)[am::strings::msg_params][am::strings::button_name]
              .asInt()));
}

TEST_F(SubscribeButtonRequestTest, Run_NAV_SUCCESS) {
  const mobile_apis::ButtonName::eType kButtonName =
      mobile_apis::ButtonName::NAV_CENTER_LOCATION;

  MessageSharedPtr msg(CreateMessage());
  (*msg)[am::strings::msg_params][am::strings::button_name] = kButtonName;
  CommandPtr command(CreateCommand<SubscribeButtonRequest>(msg));

  MockAppPtr app(CreateMockApp());
  ON_CALL(app_mngr_, application(_)).WillByDefault(Return(app));
  ON_CALL(*app, msg_version()).WillByDefault(ReturnRef(mock_semantic_version));
  ON_CALL(*app, is_navi()).WillByDefault(Return(true));

  ON_CALL(mock_hmi_capabilities_, is_ui_cooperating())
      .WillByDefault(Return(true));

  MessageSharedPtr button_caps_ptr(CreateMessage(smart_objects::SmartType_Map));
  (*button_caps_ptr)[0][am::hmi_response::button_name] = kButtonName;

  ON_CALL(mock_hmi_capabilities_, button_capabilities())
      .WillByDefault(Return(button_caps_ptr));

  ON_CALL(*app, IsSubscribedToButton(_)).WillByDefault(Return(false));

  MessageSharedPtr hmi_result_msg;

  EXPECT_CALL(mock_rpc_service_, ManageHMICommand(_, _))
      .WillOnce(DoAll(SaveArg<0>(&hmi_result_msg), Return(true)));

  ASSERT_TRUE(command->Init());
  command->Run();

  EXPECT_EQ(hmi_apis::FunctionID::Buttons_SubscribeButton,
            static_cast<hmi_apis::FunctionID::eType>(
                (*hmi_result_msg)[am::strings::params][am::strings::function_id]
                    .asInt()));

  EXPECT_EQ(
      hmi_apis::Common_ButtonName::eType::NAV_CENTER_LOCATION,
      static_cast<hmi_apis::Common_ButtonName::eType>(
          (*hmi_result_msg)[am::strings::msg_params][am::strings::button_name]
              .asInt()));
}

TEST_F(SubscribeButtonRequestTest,
       Run_App_Version_4_5_and_OK_btn_Expect_PlayPause_btn_sent_to_HMI) {
  const mobile_apis::ButtonName::eType button_ok = mobile_apis::ButtonName::OK;

  const mobile_apis::ButtonName::eType button_play_pause =
      mobile_apis::ButtonName::PLAY_PAUSE;

  MessageSharedPtr msg(CreateMessage(smart_objects::SmartType_Map));
  (*msg)[am::strings::msg_params][am::strings::button_name] = button_ok;
  CommandPtr command(CreateCommand<SubscribeButtonRequest>(msg));

  MockAppPtr app(CreateMockApp());
  ON_CALL(app_mngr_, application(_)).WillByDefault(Return(app));
  ON_CALL(*app, msg_version()).WillByDefault(ReturnRef(mock_base_rpc_version));
  ON_CALL(*app, is_media_application()).WillByDefault(Return(true));

  ON_CALL(mock_hmi_capabilities_, is_ui_cooperating())
      .WillByDefault(Return(true));

  MessageSharedPtr button_caps_ptr(CreateMessage(smart_objects::SmartType_Map));
  (*button_caps_ptr)[0][am::hmi_response::button_name] = button_ok;
  (*button_caps_ptr)[1][am::hmi_response::button_name] = button_play_pause;

  ON_CALL(mock_hmi_capabilities_, button_capabilities())
      .WillByDefault(Return(button_caps_ptr));

  ON_CALL(*app, IsSubscribedToButton(button_play_pause))
      .WillByDefault(Return(false));

  MessageSharedPtr hmi_result_msg;

  EXPECT_CALL(mock_rpc_service_, ManageHMICommand(_, _))
      .WillOnce(DoAll(SaveArg<0>(&hmi_result_msg), Return(true)));

  ASSERT_TRUE(command->Init());
  command->Run();

  EXPECT_EQ(hmi_apis::FunctionID::Buttons_SubscribeButton,
            static_cast<hmi_apis::FunctionID::eType>(
                (*hmi_result_msg)[am::strings::params][am::strings::function_id]
                    .asInt()));

  EXPECT_EQ(
      hmi_apis::Common_ButtonName::PLAY_PAUSE,
      static_cast<hmi_apis::Common_ButtonName::eType>(
          (*hmi_result_msg)[am::strings::msg_params][am::strings::button_name]
              .asInt()));
}

}  // namespace subscribe_button_request
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
