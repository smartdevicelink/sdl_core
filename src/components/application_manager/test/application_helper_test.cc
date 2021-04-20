/*
 * Copyright (c) 2017, Ford Motor Company
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

#include <gmock/gmock.h>
#include <string>
#include <vector>

#include "application_manager/mock_application_manager_settings.h"
#include "application_manager/mock_message_helper.h"
#include "policy/mock_policy_settings.h"
#include "policy/usage_statistics/mock_statistics_manager.h"

#include "application_manager/application.h"
#include "application_manager/application_impl.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/helpers/application_helper.h"
#include "application_manager/smart_object_keys.h"
#include "application_manager/usage_statistics.h"
#include "connection_handler/device.h"
#include "interfaces/MOBILE_API.h"
#include "smart_objects/smart_object.h"
#include "utils/custom_string.h"
#include "utils/macro.h"

#include "media_manager/mock_media_manager.h"
#include "resumption/last_state_wrapper_impl.h"
#include "test/resumption/mock_last_state.h"

namespace {
const uint8_t expected_tread_pool_size = 2u;
const uint8_t stop_streaming_timeout = 1u;
const std::string kDirectoryName = "./test_storage";
const std::vector<std::string> kTimeoutPrompt{"timeoutPrompt"};
}  // namespace

namespace test {
namespace components {
namespace application_manager_test {

using resumption_test::MockLastState;
using test::components::media_manager_test::MockMediaManager;
using testing::_;
using ::testing::Mock;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::ReturnRef;

using namespace application_manager;
using namespace policy_handler_test;

class ApplicationHelperTest : public testing::Test {
 public:
  ApplicationHelperTest()
      : mock_message_helper_(
            application_manager::MockMessageHelper::message_helper_mock())
      , app_manager_impl_(mock_application_manager_settings_,
                          mock_policy_settings_) {}

  void SetUp() OVERRIDE {
    ON_CALL(mock_application_manager_settings_, thread_pool_size())
        .WillByDefault(Return(expected_tread_pool_size));
    ON_CALL(mock_application_manager_settings_, app_icons_folder())
        .WillByDefault(ReturnRef(kDirectoryName));
    ON_CALL(mock_application_manager_settings_, app_storage_folder())
        .WillByDefault(ReturnRef(kDirectoryName));
    ON_CALL(mock_application_manager_settings_, launch_hmi())
        .WillByDefault(Return(true));
    ON_CALL(mock_application_manager_settings_, stop_streaming_timeout())
        .WillByDefault(Return(stop_streaming_timeout));
    ON_CALL(mock_application_manager_settings_, time_out_promt())
        .WillByDefault(ReturnRef(kTimeoutPrompt));

    CreateApplication();
    app_manager_impl_.AddMockApplication(app_impl_);

    Mock::VerifyAndClearExpectations(&mock_message_helper_);
  }

  void TearDown() OVERRIDE {
    Mock::VerifyAndClearExpectations(&mock_message_helper_);
  }

  void CreateApplication() {
    const uint32_t application_id = 1;
    const std::string policy_app_id = "p_app_id";
    const std::string mac_address = "MA:CA:DD:RE:SS";
    const connection_handler::DeviceHandle device_id = 1;
    const custom_str::CustomString app_name("");

    const std::string path_to_plagin = "";
    EXPECT_CALL(mock_application_manager_settings_, plugins_folder())
        .WillOnce(ReturnRef(path_to_plagin));
    mock_last_state_ = std::make_shared<MockLastState>();
    last_state_wrapper_ =
        std::make_shared<resumption::LastStateWrapperImpl>(mock_last_state_);
    app_manager_impl_.Init(last_state_wrapper_, &mock_media_manager_);

    app_impl_ = std::make_shared<ApplicationImpl>(
        application_id,
        policy_app_id,
        mac_address,
        device_id,
        app_name,
        std::make_shared<usage_statistics_test::MockStatisticsManager>(),
        app_manager_impl_);
  }

  MockMessageHelper* mock_message_helper_;
  NiceMock<MockApplicationManagerSettings> mock_application_manager_settings_;
  NiceMock<MockPolicySettings> mock_policy_settings_;

  ApplicationManagerImpl app_manager_impl_;
  MockMediaManager mock_media_manager_;
  std::shared_ptr<MockLastState> mock_last_state_;
  std::shared_ptr<resumption::LastStateWrapperImpl> last_state_wrapper_;
  ApplicationSharedPtr app_impl_;
};

TEST_F(ApplicationHelperTest, RecallApplicationData_ExpectAppDataReset) {
  const uint32_t cmd_id = 1;
  const uint32_t menu_id = 2;
  const uint32_t choice_set_id = 3;
  const mobile_apis::ButtonName::eType button = mobile_apis::ButtonName::AC;

  smart_objects::SmartObject cmd(smart_objects::SmartType_Map);
  cmd[strings::msg_params][strings::cmd_id] = cmd_id;
  cmd[strings::msg_params][strings::vr_commands][0] = "vrCmd";
  cmd[strings::msg_params][strings::menu_id] = menu_id;
  cmd[strings::menu_params] =
      smart_objects::SmartObject(smart_objects::SmartType_Map);
  cmd[strings::msg_params][strings::interaction_choice_set_id] = choice_set_id;

  app_impl_->AddCommand(cmd_id, cmd[strings::msg_params]);
  app_impl_->AddSubMenu(menu_id, cmd[strings::menu_params]);
  app_impl_->AddChoiceSet(choice_set_id, cmd[strings::msg_params]);

  EXPECT_TRUE(app_impl_->SubscribeToButton(button));

  const std::string some_string = "some_string";
  smart_objects::SmartObject dummy_data =
      smart_objects::SmartObject(smart_objects::SmartType_String);
  dummy_data = some_string;
  app_impl_->set_help_prompt(dummy_data);
  app_impl_->set_timeout_prompt(dummy_data);
  app_impl_->set_vr_help(dummy_data);
  app_impl_->set_vr_help_title(dummy_data);
  app_impl_->set_keyboard_props(dummy_data);
  app_impl_->set_menu_title(dummy_data);
  app_impl_->set_menu_icon(dummy_data);

  const bool persistent = false;
  const bool downloaded = true;
  const std::string filename = "filename";
  AppFile file(filename, persistent, downloaded, mobile_apis::FileType::BINARY);

  app_impl_->AddFile(file);

  const auto command1 = app_impl_->FindCommand(cmd_id);
  EXPECT_TRUE(smart_objects::SmartType_Null != command1.getType());
  const auto sub_menu1 = app_impl_->FindSubMenu(menu_id);
  EXPECT_TRUE(smart_objects::SmartType_Null != sub_menu1.getType());
  const auto choice_set1 = app_impl_->FindChoiceSet(choice_set_id);
  EXPECT_TRUE(smart_objects::SmartType_Null != choice_set1.getType());
  EXPECT_TRUE(app_impl_->IsSubscribedToButton(button));
  auto help_prompt = app_impl_->help_prompt();
  EXPECT_TRUE(help_prompt->asString() == some_string);
  auto timeout_prompt = app_impl_->timeout_prompt();
  EXPECT_TRUE(timeout_prompt->asString() == some_string);
  auto vr_help = app_impl_->vr_help();
  EXPECT_TRUE(vr_help->asString() == some_string);
  auto vr_help_title = app_impl_->vr_help_title();
  EXPECT_TRUE(vr_help_title->asString() == some_string);
  auto kb_properties = app_impl_->keyboard_props();
  EXPECT_TRUE(kb_properties->asString() == some_string);
  auto menu_title = app_impl_->menu_title();
  EXPECT_TRUE(menu_title->asString() == some_string);
  auto menu_icon = app_impl_->menu_icon();
  EXPECT_TRUE(menu_icon->asString() == some_string);
  auto file_ptr = app_impl_->GetFile(filename);
  EXPECT_TRUE(NULL != file_ptr);
  EXPECT_TRUE(file_ptr->file_name == filename);

  EXPECT_CALL(*mock_message_helper_, CreateDeleteUICommandRequest(_, _, _))
      .WillOnce(Return(std::make_shared<smart_objects::SmartObject>()));

  EXPECT_CALL(*mock_message_helper_, CreateDeleteVRCommandRequest(_, _, _))
      .WillOnce(Return(std::make_shared<smart_objects::SmartObject>()));

  // Act
  application_manager::DeleteApplicationData(app_impl_, app_manager_impl_);

  const auto command2 = app_impl_->FindCommand(cmd_id);
  EXPECT_TRUE(smart_objects::SmartType_Null == command2.getType());
  const auto sub_menu2 = app_impl_->FindSubMenu(menu_id);
  EXPECT_TRUE(smart_objects::SmartType_Null == sub_menu2.getType());
  const auto choice_set2 = app_impl_->FindChoiceSet(choice_set_id);
  EXPECT_TRUE(smart_objects::SmartType_Null == choice_set2.getType());
  EXPECT_FALSE(app_impl_->IsSubscribedToButton(button));
  help_prompt = app_impl_->help_prompt();
  EXPECT_FALSE(help_prompt->asString() == some_string);
  timeout_prompt = app_impl_->timeout_prompt();
  EXPECT_FALSE(timeout_prompt->asString() == some_string);
  vr_help = app_impl_->vr_help();
  EXPECT_TRUE(vr_help == NULL);
  vr_help_title = app_impl_->vr_help_title();
  EXPECT_TRUE(vr_help_title == NULL);
  kb_properties = app_impl_->keyboard_props();
  EXPECT_FALSE(kb_properties->asString() == some_string);
  menu_title = app_impl_->menu_title();
  EXPECT_FALSE(menu_title->asString() == some_string);
  menu_icon = app_impl_->menu_icon();
  EXPECT_FALSE(menu_icon->asString() == some_string);
  file_ptr = app_impl_->GetFile(filename);
  EXPECT_TRUE(NULL == file_ptr);
}

TEST_F(ApplicationHelperTest, RecallApplicationData_ExpectHMICleanupRequests) {
  const uint32_t cmd_id = 1;
  const uint32_t menu_id = 2;
  const uint32_t choice_set_id = 3;
  smart_objects::SmartObject cmd;
  cmd[strings::msg_params][strings::cmd_id] = cmd_id;
  cmd[strings::msg_params][strings::vr_commands][0] = "vrCmd";
  cmd[strings::msg_params][strings::menu_id] = menu_id;
  cmd[strings::msg_params][strings::interaction_choice_set_id] = choice_set_id;

  app_impl_->AddCommand(cmd_id, cmd[strings::msg_params]);
  app_impl_->AddSubMenu(menu_id, cmd[strings::menu_params]);
  app_impl_->AddChoiceSet(choice_set_id, cmd[strings::msg_params]);
  app_impl_->SubscribeToButton(mobile_apis::ButtonName::AC);

  EXPECT_CALL(*mock_message_helper_, CreateDeleteUICommandRequest(_, _, _))
      .WillOnce(Return(std::make_shared<smart_objects::SmartObject>()));

  EXPECT_CALL(*mock_message_helper_, CreateDeleteVRCommandRequest(_, _, _))
      .WillOnce(Return(std::make_shared<smart_objects::SmartObject>()));

  EXPECT_CALL(*mock_message_helper_, SendDeleteSubmenuRequest(_, _, _));

  EXPECT_CALL(*mock_message_helper_, SendDeleteChoiceSetRequest(_, _, _));

  EXPECT_CALL(*mock_message_helper_, SendResetPropertiesRequest(_, _));

  EXPECT_CALL(*mock_message_helper_,
              SendUnsubscribeButtonNotification(_, _, _));

  // Act
  application_manager::DeleteApplicationData(app_impl_, app_manager_impl_);
}

}  // namespace application_manager_test
}  // namespace components
}  // namespace test
