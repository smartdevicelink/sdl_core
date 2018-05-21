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

#include "gtest/gtest.h"
#include "application_manager/mock_application.h"
#include "application_manager/mock_help_prompt_manager.h"
#include "application_manager/mock_message_helper.h"
#include "application_manager/mock_event_dispatcher.h"
#include "application_manager/mock_resumption_data.h"
#include "connection_handler/mock_connection_handler.h"
#include "protocol_handler/mock_session_observer.h"
#include "policy/mock_policy_settings.h"
#include "policy/mock_policy_manager.h"
#include "policy/usage_statistics/mock_statistics_manager.h"
#include "application_manager/application_impl.h"
#include "application_manager/commands/mobile/set_global_properties_request.h"
#include "application_manager/commands/mobile/add_command_request.h"
#include "application_manager/commands/commands_test.h"
#include "application_manager/commands/command_request_test.h"
#include "application_manager/hmi_state.h"
#include "utils/file_system.h"

namespace test {
namespace components {
namespace application_manager_test {

namespace {
const uint32_t kConnectionKey = 1u;
const uint32_t kCorrelationKey = 2u;
const uint32_t kAppId = 10u;
const uint32_t kCmdId = 1u;
const std::string kPolicyAppId = "fake_app_id";
const uint32_t kTimeout = 10000u;
const std::string kText = "one";
const uint32_t kPosition = 1u;
const std::string kFirstVrCommand = "first";
const int32_t kGrammarId = 12;
const std::string kDirectoryName = "./test_storage";
const uint8_t expected_tread_pool_size = 2u;
const uint8_t stop_streaming_timeout = 1u;
const std::vector<std::string> kTimeoutPrompt{"timeoutPrompt"};
}  // namespace

using namespace application_manager;
using namespace policy;
using namespace policy_handler_test;
using namespace test::components::commands_test;
using namespace mobile_apis;

using testing::Mock;
using testing::NiceMock;
using testing::Return;
using testing::ReturnRef;
using testing::_;
using am::HmiStatePtr;
using am::commands::SetGlobalPropertiesRequest;
using usage_statistics_test::MockStatisticsManager;

class HelperTestHelpPromptManager : public ApplicationImpl {
 public:
  HelperTestHelpPromptManager(
      uint32_t application_id,
      const std::string& policy_app_id,
      const std::string& mac_address,
      const connection_handler::DeviceHandle device_id,
      const custom_str::CustomString& app_name,
      utils::SharedPtr<usage_statistics::StatisticsManager> statistics_manager,
      ApplicationManager& application_manager,
      MockHelpPromptManager& mock_help_prompt_manager)
      : ApplicationImpl(application_id,
                        policy_app_id,
                        mac_address,
                        device_id,
                        app_name,
                        statistics_manager,
                        application_manager)
      , mock_help_prompt_manager_(mock_help_prompt_manager) {}
  HelpPromptManager& help_prompt_manager() OVERRIDE {
    return mock_help_prompt_manager_;
  }

 private:
  MockHelpPromptManager& mock_help_prompt_manager_;
};

class HelpPromptManagerTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
  HelpPromptManagerTest()
      : app_set_(test_app_, app_lock_)
      , mock_message_helper_(*MockMessageHelper::message_helper_mock()) {}
  void SetUp() OVERRIDE;
  void TearDown() OVERRIDE;
  MessageSharedPtr CreateMsgParams();
  void VRArraySetupHelper(MessageSharedPtr msg,
                          SmartObject& vr_help_title,
                          SmartObject& vr_help_array);
  void CreateBasicParamsVRRequest(MessageSharedPtr msg);
  void CreateApplication(MockHelpPromptManager& mock_help_prompt_manager);
  HmiStatePtr CreateTestHmiState();

 protected:
  std::auto_ptr<am::ApplicationManagerImpl> app_manager_impl_;
  utils::SharedPtr<HelperTestHelpPromptManager> app_impl_;
  NiceMock<MockApplicationManagerSettings> mock_application_manager_settings_;
  NiceMock<MockPolicySettings> policy_settings_;
  std::auto_ptr<PolicyHandler> policy_handler_;
  utils::SharedPtr<policy_manager_test::MockPolicyManager> mock_policy_manager_;
  connection_handler_test::MockConnectionHandler conn_handler_;
  NiceMock<event_engine_test::MockEventDispatcher> mock_event_dispatcher_;
  utils::SharedPtr<application_manager_test::MockApplication> mock_app_;
  protocol_handler_test::MockSessionObserver mock_session_observer_;
  ApplicationState app_state_;
  ApplicationSet test_app_;
  sync_primitives::Lock app_lock_;
  sync_primitives::Lock app_commands_lock_;
  DataAccessor<ApplicationSet> app_set_;
  application_manager::MockMessageHelper& mock_message_helper_;
  utils::SharedPtr<MockHelpPromptManager> mock_help_prompt_manager_;
  utils::SharedPtr<NiceMock<resumption_test::MockResumptionData> >
      mock_storage_;
  HmiState::StateID state_id_;
  mobile_api::HMILevel::eType test_lvl_;
  AudioStreamingState::eType audiostate_;
  SystemContext::eType syst_context_;
};

void HelpPromptManagerTest::SetUp() {
  Mock::VerifyAndClearExpectations(&mock_message_helper_);

  test_lvl_ = HMILevel::INVALID_ENUM;
  state_id_ = HmiState::STATE_ID_REGULAR;
  audiostate_ = AudioStreamingState::NOT_AUDIBLE;
  syst_context_ = SystemContext::SYSCTXT_MAIN;

  mock_storage_ =
      ::utils::MakeShared<NiceMock<resumption_test::MockResumptionData> >(
          app_mngr_);

  app_manager_impl_.reset(new am::ApplicationManagerImpl(
      mock_application_manager_settings_, policy_settings_));
  app_manager_impl_->resume_controller().set_resumption_storage(mock_storage_);
  app_manager_impl_->set_connection_handler(&conn_handler_);
  policy_handler_.reset(
      new PolicyHandler(policy_settings_, *app_manager_impl_.get()));
  mock_help_prompt_manager_ =
      utils::SharedPtr<MockHelpPromptManager>(new MockHelpPromptManager());

  HmiStatePtr state = utils::MakeShared<HmiState>(
      static_cast<utils::SharedPtr<Application> >(mock_app_),
      app_mngr_,
      HmiState::STATE_ID_REGULAR);

  app_state_.InitState(state);

  std::string path = file_system::CreateDirectory("storage");
  file_system::CreateFile(path + "/" + "certificate");
  mock_policy_manager_ =
      utils::MakeShared<policy_manager_test::MockPolicyManager>();
  ASSERT_TRUE(mock_policy_manager_.valid());

  mock_app_ = utils::MakeShared<application_manager_test::MockApplication>();
  app_manager_impl_->AddMockApplication(mock_app_);
}

void HelpPromptManagerTest::TearDown() {
  Mock::VerifyAndClearExpectations(&mock_message_helper_);
  ON_CALL(mock_event_dispatcher_, remove_observer(_, _));
}

MessageSharedPtr HelpPromptManagerTest::CreateMsgParams() {
  MessageSharedPtr msg = CreateMessage();
  (*msg)[am::strings::params][am::strings::connection_key] = kConnectionKey;
  (*msg)[am::strings::msg_params][am::strings::app_id] = kAppId;
  return msg;
}

void HelpPromptManagerTest::VRArraySetupHelper(MessageSharedPtr msg,
                                               SmartObject& vr_help_title,
                                               SmartObject& vr_help_array) {
  (*msg)[am::strings::msg_params][am::strings::vr_help_title] = vr_help_title;
  vr_help_array[0] = SmartObject(smart_objects::SmartType_Map);
  vr_help_array[0][am::strings::text] = kText;
  vr_help_array[0][am::strings::position] = kPosition;
  (*msg)[am::strings::msg_params][am::strings::vr_help] = vr_help_array;
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));
}

void HelpPromptManagerTest::CreateBasicParamsVRRequest(MessageSharedPtr msg) {
  SmartObject& msg_params = (*msg)[strings::msg_params];
  msg_params[strings::cmd_id] = kCmdId;
  msg_params[strings::vr_commands] =
      SmartObject(smart_objects::SmartType_Array);
  msg_params[strings::vr_commands][0] = kFirstVrCommand;
  msg_params[strings::type] = kPosition;
  msg_params[strings::grammar_id] = kGrammarId;
  msg_params[strings::info] = "VR info";
}

void HelpPromptManagerTest::CreateApplication(
    MockHelpPromptManager& mock_help_prompt_manager) {
  const std::string policy_app_id = "p_app_id";
  const std::string mac_address = "MA:CA:DD:RE:SS";
  const connection_handler::DeviceHandle device_id = 1;
  const custom_str::CustomString app_name("");

  EXPECT_CALL(app_mngr_, get_settings())
      .WillRepeatedly(ReturnRef(mock_application_manager_settings_));
  EXPECT_CALL(mock_application_manager_settings_, app_icons_folder())
      .WillRepeatedly(ReturnRef(kDirectoryName));
  EXPECT_CALL(mock_application_manager_settings_, app_storage_folder())
      .WillRepeatedly(ReturnRef(kDirectoryName));
  EXPECT_CALL(mock_application_manager_settings_, audio_data_stopped_timeout())
      .WillOnce(Return(0));
  EXPECT_CALL(mock_application_manager_settings_, video_data_stopped_timeout())
      .WillOnce(Return(0));
  app_impl_.reset(new HelperTestHelpPromptManager(
      kAppId,
      policy_app_id,
      mac_address,
      device_id,
      app_name,
      utils::MakeShared<MockStatisticsManager>(),
      app_mngr_,
      mock_help_prompt_manager));
  HmiStatePtr initial_state = CreateTestHmiState();
  app_impl_->SetInitialState(initial_state);
}

HmiStatePtr HelpPromptManagerTest::CreateTestHmiState() {
  HmiStatePtr testState = utils::MakeShared<HmiState>(
      static_cast<utils::SharedPtr<Application> >(app_impl_),
      app_mngr_,
      state_id_);
  testState->set_hmi_level(test_lvl_);
  testState->set_audio_streaming_state(audiostate_);
  testState->set_system_context(syst_context_);
  return testState;
}

TEST_F(HelpPromptManagerTest, AddCommand_OnVrCommandAdded) {
  CreateApplication(*mock_help_prompt_manager_.get());
  MessageSharedPtr msg = CreateMessage(smart_objects::SmartType_Map);
  CreateBasicParamsVRRequest(msg);
  (*msg)[strings::params][hmi_response::code] =
      hmi_apis::Common_Result::SUCCESS;
  (*msg)[strings::msg_params][strings::cmd_id] = kCmdId;

  EXPECT_CALL(*mock_help_prompt_manager_,
              OnVrCommandAdded(kCmdId, (*msg)[strings::msg_params]));

  app_impl_->AddCommand(kCmdId, (*msg)[strings::msg_params]);
}

TEST_F(HelpPromptManagerTest, RemoveCommand_OnVrCommandDeleted) {
  CreateApplication(*mock_help_prompt_manager_.get());
  MessageSharedPtr msg = CreateMessage(smart_objects::SmartType_Map);
  CreateBasicParamsVRRequest(msg);
  (*msg)[strings::params][hmi_response::code] =
      hmi_apis::Common_Result::SUCCESS;
  (*msg)[strings::msg_params][strings::cmd_id] = kCmdId;

  EXPECT_CALL(*mock_help_prompt_manager_,
              OnVrCommandAdded(kCmdId, (*msg)[strings::msg_params]));
  EXPECT_CALL(*mock_help_prompt_manager_, OnVrCommandDeleted(kCmdId));

  app_impl_->AddCommand(kCmdId, (*msg)[strings::msg_params]);
  app_impl_->RemoveCommand(kCmdId);
}

TEST_F(HelpPromptManagerTest,
       Request_OnSetGlobalPropertiesReceived_TTS_SUCCES) {
  MessageSharedPtr msg = CreateMsgParams();
  SmartObject help_prompt(smart_objects::SmartType_Array);
  help_prompt[0][am::strings::text] = "Help_Prompt_One";
  (*msg)[am::strings::msg_params][am::strings::help_prompt] = help_prompt;
  SmartObject timeout_prompt(smart_objects::SmartType_Array);
  timeout_prompt[0][am::strings::text] = "Timeout_Prompt_One";
  (*msg)[am::strings::msg_params][am::strings::timeout_prompt] = timeout_prompt;

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(mock_message_helper_, VerifyImageVrHelpItems(_, _, _)).Times(0);
  EXPECT_CALL(app_mngr_, RemoveAppFromTTSGlobalPropertiesList(kConnectionKey));
  EXPECT_CALL(*mock_app_, set_help_prompt(help_prompt));
  EXPECT_CALL(*mock_app_, help_prompt()).WillOnce(Return(&help_prompt));
  EXPECT_CALL(*mock_app_, set_timeout_prompt(timeout_prompt));
  EXPECT_CALL(*mock_app_, timeout_prompt()).WillOnce(Return(&timeout_prompt));
  EXPECT_CALL(*mock_app_, app_id()).WillOnce(Return(kAppId));
  EXPECT_CALL(
      mock_hmi_interfaces_,
      GetInterfaceFromFunction(hmi_apis::FunctionID::TTS_SetGlobalProperties))
      .WillOnce(Return(am::HmiInterfaces::HMI_INTERFACE_TTS));
  ON_CALL(mock_hmi_interfaces_,
          GetInterfaceState(am::HmiInterfaces::HMI_INTERFACE_TTS))
      .WillByDefault(Return(am::HmiInterfaces::STATE_NOT_AVAILABLE));

  EXPECT_CALL(*mock_app_, help_prompt_manager())
      .WillOnce(ReturnRef(*mock_help_prompt_manager_.get()));
  EXPECT_CALL(*mock_help_prompt_manager_,
              OnSetGlobalPropertiesReceived(_, false));

  SharedPtr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  command->Run();
}

TEST_F(HelpPromptManagerTest, Request_OnSetGlobalPropertiesReceived_UI_SUCCES) {
  MessageSharedPtr msg = CreateMsgParams();
  SmartObject vr_help_title("yes");
  SmartObject vr_help_array(smart_objects::SmartType_Array);
  VRArraySetupHelper(msg, vr_help_title, vr_help_array);
  (*msg)[am::strings::msg_params][am::strings::vr_help] = vr_help_array;
  SmartObject menu_title("Menu_Title");
  (*msg)[am::strings::msg_params][am::hmi_request::menu_title] = menu_title;
  SmartObject menu_icon(smart_objects::SmartType_Map);
  menu_icon[am::strings::value] = "1";
  (*msg)[am::strings::msg_params][am::hmi_request::menu_icon] = menu_icon;
  SmartObject keyboard_properties(smart_objects::SmartType_Map);
  (*msg)[am::strings::msg_params][am::hmi_request::keyboard_properties] =
      keyboard_properties;

  EXPECT_CALL(mock_message_helper_, VerifyImage(menu_icon, _, _))
      .WillOnce((Return(mobile_apis::Result::SUCCESS)));
  EXPECT_CALL(mock_message_helper_, VerifyImageVrHelpItems(vr_help_array, _, _))
      .WillOnce((Return(mobile_apis::Result::SUCCESS)));
  EXPECT_CALL(app_mngr_, RemoveAppFromTTSGlobalPropertiesList(kConnectionKey));
  EXPECT_CALL(*mock_app_, set_vr_help_title(vr_help_title));
  EXPECT_CALL(*mock_app_, set_vr_help(vr_help_array));
  EXPECT_CALL(*mock_app_, vr_help_title()).WillOnce(Return(&vr_help_title));
  EXPECT_CALL(*mock_app_, vr_help()).WillOnce(Return(&vr_help_array));
  EXPECT_CALL(*mock_app_, set_menu_title(menu_title));
  EXPECT_CALL(*mock_app_, set_menu_icon(menu_icon));
  EXPECT_CALL(*mock_app_, set_keyboard_props(keyboard_properties));
  EXPECT_CALL(*mock_app_, app_id()).WillOnce(Return(kAppId));
  EXPECT_CALL(
      mock_hmi_interfaces_,
      GetInterfaceFromFunction(hmi_apis::FunctionID::UI_SetGlobalProperties))
      .WillOnce(Return(am::HmiInterfaces::HMI_INTERFACE_UI));
  ON_CALL(mock_hmi_interfaces_,
          GetInterfaceState(am::HmiInterfaces::HMI_INTERFACE_UI))
      .WillByDefault(Return(am::HmiInterfaces::STATE_NOT_AVAILABLE));

  EXPECT_CALL(*mock_app_, help_prompt_manager())
      .WillOnce(ReturnRef(*mock_help_prompt_manager_.get()));
  EXPECT_CALL(*mock_help_prompt_manager_,
              OnSetGlobalPropertiesReceived(_, false));

  SharedPtr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  command->Run();
}

TEST_F(HelpPromptManagerTest, StartMechanismHandlingVR_ActivateApp) {
  policy_handler_->SetPolicyManager(mock_policy_manager_);

  const HmiStatePtr state = app_state_.GetState(HmiState::STATE_ID_CURRENT);

  EXPECT_CALL(*mock_app_, RegularHmiState()).WillOnce(Return(state));

  EXPECT_CALL(*mock_help_prompt_manager_, OnAppActivated(false));

  EXPECT_CALL(*mock_app_, help_prompt_manager())
      .WillOnce(ReturnRef(*mock_help_prompt_manager_.get()));

  EXPECT_CALL(app_mngr_, ActivateApplication(_)).WillRepeatedly(Return(true));

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillRepeatedly(Return(mock_app_));

  AppPermissions permissions(kPolicyAppId);
  permissions.appPermissionsConsentNeeded = true;

  EXPECT_CALL(*mock_app_, policy_app_id()).WillOnce(Return(kPolicyAppId));
  EXPECT_CALL(*mock_policy_manager_, GetAppPermissionsChanges(_))
      .WillOnce(Return(permissions));
  ON_CALL(*mock_policy_manager_, Increment(_, _)).WillByDefault(Return());
  EXPECT_CALL(*mock_policy_manager_, RemovePendingPermissionChanges(_));
  EXPECT_CALL(mock_message_helper_, SendSDLActivateAppResponse(_, _, _));
  ON_CALL(*mock_app_, app_id()).WillByDefault(Return(kAppId));

  policy_handler_->OnActivateApp(kAppId, kCorrelationKey);
}

TEST_F(HelpPromptManagerTest, StopMechanismHandlingVR_UnregisterApp) {
  std::string dummy_mac_address;
  EXPECT_CALL(*mock_app_, app_id()).WillRepeatedly(Return(kAppId));
  EXPECT_CALL(*mock_app_, device()).WillRepeatedly(Return(0));
  EXPECT_CALL(*mock_app_, mac_address())
      .WillRepeatedly(ReturnRef(dummy_mac_address));
  EXPECT_CALL(*mock_app_, policy_app_id()).WillRepeatedly(Return(""));
  EXPECT_CALL(*mock_app_, protocol_version())
      .WillRepeatedly(
          Return(protocol_handler::MajorProtocolVersion::PROTOCOL_VERSION_4));
  smart_objects::SmartObject sm_object;
  smart_objects::SmartObjectSPtr sptr =
      MakeShared<smart_objects::SmartObject>(sm_object);
  EXPECT_CALL(mock_message_helper_, CreateModuleInfoSO(_, _))
      .WillOnce(Return(sptr));
  EXPECT_CALL(mock_application_manager_settings_, default_timeout())
      .WillOnce(ReturnRef(kTimeout));

  EXPECT_CALL(*mock_app_, help_prompt_manager())
      .WillOnce(ReturnRef(*mock_help_prompt_manager_.get()));
  EXPECT_CALL(*mock_help_prompt_manager_, OnAppUnregistered());

  app_manager_impl_->UnregisterApplication(
      kAppId, mobile_apis::Result::SUCCESS, false, true);
}

}  // namespace application_manager_test
}  // namespace components
}  // namespace test
