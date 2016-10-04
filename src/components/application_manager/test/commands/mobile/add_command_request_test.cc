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
#include <set>

#include "gtest/gtest.h"
#include "utils/shared_ptr.h"
#include "utils/helpers.h"
#include "utils/make_shared.h"
#include "smart_objects/smart_object.h"
#include "interfaces/MOBILE_API.h"
#include "utils/custom_string.h"
#include "application_manager/commands/command_request_test.h"
#include "application_manager/commands/mobile/add_command_request.h"
#include "application_manager/smart_object_keys.h"
#include "application_manager/commands/commands_test.h"
#include "application_manager/commands/command_impl.h"
#include "policy/usage_statistics/mock_statistics_manager.h"
#include "application_manager/mock_application.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/mock_message_helper.h"
#include "application_manager/event_engine/event.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {
namespace add_command_request {

namespace am = application_manager;
using am::commands::CommandImpl;
using am::ApplicationManager;
using am::commands::MessageSharedPtr;
using am::ApplicationSharedPtr;
using am::MockMessageHelper;
using ::testing::_;
using ::utils::SharedPtr;
using ::testing::Return;
using ::testing::Mock;
using am::commands::AddCommandRequest;
using ::test::components::application_manager_test::MockApplication;
using usage_statistics_test::MockStatisticsManager;
using am::event_engine::EventObserver;

namespace custom_str = utils::custom_string;
namespace strings = ::application_manager::strings;
namespace hmi_response = ::application_manager::hmi_response;

namespace {
const hmi_apis::FunctionID::eType kInvalidFunctionId =
    hmi_apis::FunctionID::INVALID_ENUM;
const int32_t kCommandId = 1;
const uint32_t kAppId = 1u;
const uint32_t kCmdId = 1u;
const uint32_t kConnectionKey = 2u;
}  // namespace

class AddCommandRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 protected:
  AddCommandRequestTest() {
    mock_app_ = CreateMockApp();
  }

  ~AddCommandRequestTest() {
    // Fix DataAccessor release and WinQt crash
    Mock::VerifyAndClearExpectations(mock_app_.get());
  }

  MessageSharedPtr CreateFullParamsUISO() {
    MessageSharedPtr msg = CreateMessage(smart_objects::SmartType_Map);
    (*msg)[strings::params][strings::connection_key] = kConnectionKey;
    smart_objects::SmartObject menu_params =
        smart_objects::SmartObject(smart_objects::SmartType_Map);
    menu_params[strings::position] = 10;
    menu_params[strings::menu_name] = "LG";

    smart_objects::SmartObject msg_params =
        smart_objects::SmartObject(smart_objects::SmartType_Map);
    msg_params[strings::cmd_id] = kCmdId;
    msg_params[strings::menu_params] = menu_params;
    msg_params[strings::app_id] = kAppId;
    msg_params[strings::cmd_icon] = 1;
    msg_params[strings::cmd_icon][strings::value] = "10";
    (*msg)[strings::msg_params] = msg_params;

    return msg;
  }

  MessageSharedPtr CreateFullParamsVRSO() {
    MessageSharedPtr msg = CreateMessage(smart_objects::SmartType_Map);
    (*msg)[strings::params][strings::connection_key] = kConnectionKey;
    smart_objects::SmartObject msg_params =
        smart_objects::SmartObject(smart_objects::SmartType_Map);
    msg_params[strings::cmd_id] = kCmdId;
    msg_params[strings::vr_commands] =
        smart_objects::SmartObject(smart_objects::SmartType_Array);
    msg_params[strings::vr_commands][0] = "lamer";
    msg_params[strings::type] = 34;
    msg_params[strings::grammar_id] = 12;
    msg_params[strings::app_id] = kAppId;
    (*msg)[strings::msg_params] = msg_params;

    return msg;
  }

  sync_primitives::Lock lock_;
  MockAppPtr mock_app_;
};

TEST_F(AddCommandRequestTest, GetRunMethods_SUCCESS) {
  MessageSharedPtr msg = CreateFullParamsUISO();
  SmartObject image = (*msg)[strings::msg_params][strings::cmd_icon];

  utils::SharedPtr<AddCommandRequest> req =
      CreateCommand<AddCommandRequest>(msg);

  ON_CALL(mock_app_manager_, application(kConnectionKey))
      .WillByDefault(Return(mock_app_));

  MockMessageHelper* mock_message_helper =
      MockMessageHelper::message_helper_mock();
  EXPECT_CALL(*mock_message_helper, VerifyImage(image, _, _))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));

  smart_objects::SmartObject* ptr = NULL;
  EXPECT_CALL(*mock_app_, FindCommand(kCmdId)).WillOnce(Return(ptr));
  EXPECT_EQ(NULL, ptr);
  EXPECT_CALL(*mock_app_, AddCommand(kCmdId, (*msg)[strings::msg_params]));

  am::CommandsMap commands_map;
  EXPECT_CALL(*mock_app_, commands_map())
      .WillRepeatedly(Return(
          DataAccessor<application_manager::CommandsMap>(commands_map, lock_)));

  MessageSharedPtr ui_command_result;
  {
    EXPECT_CALL(mock_app_manager_, ManageHMICommand(_))
        .WillOnce(DoAll(SaveArg<0>(&ui_command_result), Return(true)));
  }

  req->Run();

  ASSERT_TRUE(ui_command_result);
}

TEST_F(AddCommandRequestTest, OnEvent_UI_SUCCESS) {
  MessageSharedPtr msg_ui = CreateFullParamsUISO();
  SmartObject image = (*msg_ui)[strings::msg_params][strings::cmd_icon];

  utils::SharedPtr<AddCommandRequest> req_ui =
      CreateCommand<AddCommandRequest>(msg_ui);

  (*msg_ui)[strings::params][hmi_response::code] =
      hmi_apis::Common_Result::SUCCESS;

  ON_CALL(mock_app_manager_, application(kConnectionKey))
      .WillByDefault(Return(mock_app_));
  ON_CALL(*mock_app_, app_id()).WillByDefault(Return(1));

  MockMessageHelper* mock_message_helper =
      MockMessageHelper::message_helper_mock();
  EXPECT_CALL(*mock_message_helper, VerifyImage(image, _, _))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));

  smart_objects::SmartObject* ptr = NULL;
  ON_CALL(*mock_app_, FindCommand(kCmdId)).WillByDefault(Return(ptr));
  EXPECT_EQ(NULL, ptr);

  EXPECT_CALL(*mock_app_, AddCommand(kCmdId, (*msg_ui)[strings::msg_params]));

  am::CommandsMap commands_map;
  EXPECT_CALL(*mock_app_, commands_map())
      .WillRepeatedly(
          Return(DataAccessor<am::CommandsMap>(commands_map, lock_)));

  am::event_engine::Event event(hmi_apis::FunctionID::UI_AddCommand);
  event.set_smart_object(*msg_ui);

  MessageSharedPtr ui_command_result;
  {
    EXPECT_CALL(mock_app_manager_, ManageHMICommand(_))
        .WillOnce(DoAll(SaveArg<0>(&ui_command_result), Return(true)));
  }

  req_ui->Run();

  ASSERT_TRUE(ui_command_result);

  ON_CALL(mock_app_manager_, application(kConnectionKey))
      .WillByDefault(Return(mock_app_));
  ON_CALL(*mock_app_, app_id()).WillByDefault(Return(1));

  EXPECT_CALL(*mock_message_helper,
              HMIToMobileResult(hmi_apis::Common_Result::SUCCESS))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));

  EXPECT_CALL(*mock_app_, UpdateHash());

  req_ui->on_event(event);
}

TEST_F(AddCommandRequestTest, OnEvent_VR_SUCCESS) {
  MessageSharedPtr msg_vr = CreateFullParamsVRSO();

  utils::SharedPtr<AddCommandRequest> req_vr =
      CreateCommand<AddCommandRequest>(msg_vr);

  ON_CALL(mock_app_manager_, application(kConnectionKey))
      .WillByDefault(Return(mock_app_));
  ON_CALL(*mock_app_, app_id()).WillByDefault(Return(1));

  MessageSharedPtr msg = CreateMessage(smart_objects::SmartType_Map);
  (*msg)[strings::params][hmi_response::code] =
      hmi_apis::Common_Result::SUCCESS;
  (*msg)[strings::msg_params][strings::cmd_id] = kCommandId;

  am::event_engine::Event event(hmi_apis::FunctionID::VR_AddCommand);
  event.set_smart_object(*msg);

  smart_objects::SmartObject* ptr = NULL;
  ON_CALL(*mock_app_, FindCommand(kCmdId)).WillByDefault(Return(ptr));
  EXPECT_EQ(NULL, ptr);

  MockMessageHelper* mock_message_helper =
      MockMessageHelper::message_helper_mock();
  EXPECT_CALL(*mock_message_helper,
              HMIToMobileResult(hmi_apis::Common_Result::SUCCESS))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));

  EXPECT_CALL(*mock_app_, AddCommand(kCmdId, (*msg_vr)[strings::msg_params]));

  am::CommandsMap commands_map;
  EXPECT_CALL(*mock_app_, commands_map())
      .WillRepeatedly(
          Return(DataAccessor<am::CommandsMap>(commands_map, lock_)));

  EXPECT_CALL(*mock_app_, UpdateHash());

  MessageSharedPtr vr_command_result;
  {
    EXPECT_CALL(mock_app_manager_, ManageHMICommand(_))
        .WillOnce(DoAll(SaveArg<0>(&vr_command_result), Return(true)));
  }

  req_vr->Run();

  ASSERT_TRUE(vr_command_result);

  req_vr->on_event(event);
}

TEST_F(AddCommandRequestTest, OnEvent_BothSend_SUCCESS) {
  MessageSharedPtr msg = CreateMessage(smart_objects::SmartType_Map);
  smart_objects::SmartObject msg_params =
      smart_objects::SmartObject(smart_objects::SmartType_Map);
  (*msg)[strings::params][strings::connection_key] = kConnectionKey;

  msg_params[strings::cmd_id] = kCommandId;
  msg_params[strings::vr_commands] =
      smart_objects::SmartObject(smart_objects::SmartType_Array);
  msg_params[strings::vr_commands][0] = "lamer";

  smart_objects::SmartObject menu_params =
      smart_objects::SmartObject(smart_objects::SmartType_Map);
  menu_params[strings::position] = 10;
  menu_params[strings::menu_name] = "LG";

  msg_params[strings::menu_params] = menu_params;
  msg_params[strings::type] = 34;
  msg_params[strings::grammar_id] = 12;
  msg_params[strings::app_id] = 20;

  utils::SharedPtr<AddCommandRequest> req_ui =
      CreateCommand<AddCommandRequest>(msg);

  MessageSharedPtr msg1 = CreateMessage(smart_objects::SmartType_Map);
  (*msg1)[strings::params][hmi_response::code] =
      hmi_apis::Common_Result::SUCCESS;
  (*msg1)[strings::msg_params][strings::cmd_id] = kCommandId;
  am::event_engine::Event event_ui(hmi_apis::FunctionID::UI_AddCommand);
  event_ui.set_smart_object(*msg1);

  am::event_engine::Event event_vr(hmi_apis::FunctionID::VR_AddCommand);
  event_vr.set_smart_object(*msg1);

  ON_CALL(mock_app_manager_, application(kConnectionKey))
      .WillByDefault(Return(mock_app_));
  ON_CALL(*mock_app_, app_id()).WillByDefault(Return(1));

  EXPECT_CALL(*mock_app_, RemoveCommand(kCommandId)).Times(0);

  req_ui->Run();
  req_ui->on_event(event_ui);
  req_ui->on_event(event_vr);
}

TEST_F(AddCommandRequestTest, OnEvent_UnknownEvent_UNSUCCESS) {
  Event event(hmi_apis::FunctionID::INVALID_ENUM);
  MessageSharedPtr msg = CreateMessage(smart_objects::SmartType_Map);
  utils::SharedPtr<AddCommandRequest> request =
      CreateCommand<AddCommandRequest>(msg);

  ON_CALL(mock_app_manager_, application(_)).WillByDefault(Return(mock_app_));

  EXPECT_CALL(mock_app_manager_, ManageMobileCommand(_, _)).Times(0);

  request->on_event(event);
}

}  // namespace add_command_request
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace tests
