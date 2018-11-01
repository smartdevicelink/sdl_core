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

#include "application_manager/application_impl.h"
#include "application_manager/commands/command_request_test.h"
#include "application_manager/commands/commands_test.h"
#include "application_manager/hmi_state.h"
#include "application_manager/mock_application.h"
#include "application_manager/mock_event_dispatcher.h"
#include "application_manager/mock_help_prompt_manager.h"
#include "application_manager/mock_message_helper.h"
#include "application_manager/mock_resumption_data.h"
#include "connection_handler/mock_connection_handler.h"
#include "gtest/gtest.h"
#include "policy/usage_statistics/mock_statistics_manager.h"
#include "protocol_handler/mock_session_observer.h"
#include "sdl_rpc_plugin/commands/mobile/set_global_properties_request.h"
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
using namespace test::components::commands_test;
using namespace mobile_apis;

using am::HmiStatePtr;
using sdl_rpc_plugin::commands::SetGlobalPropertiesRequest;
using testing::_;
using testing::Mock;
using testing::NiceMock;
using testing::Return;
using testing::ReturnRef;
using usage_statistics_test::MockStatisticsManager;

class ApplicationImplTest : public ApplicationImpl {
 public:
  ApplicationImplTest(
      uint32_t application_id,
      const std::string& policy_app_id,
      const std::string& mac_address,
      const connection_handler::DeviceHandle device_id,
      const custom_str::CustomString& app_name,
      std::shared_ptr<usage_statistics::StatisticsManager> statistics_manager,
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

class HelpPromptManagerTest : public ::testing::Test {
 public:
  HelpPromptManagerTest()
      : mock_message_helper_(*MockMessageHelper::message_helper_mock()) {}
  void SetUp() OVERRIDE;
  void TearDown() OVERRIDE;
  MessageSharedPtr CreateMsgParams();
  void VRArraySetupHelper(MessageSharedPtr msg,
                          SmartObject& vr_help_title,
                          SmartObject& vr_help_array);
  void CreateBasicParamsVRRequest(MessageSharedPtr msg);
  void CreateApplication(MockHelpPromptManager& mock_help_prompt_manager);
  HmiStatePtr CreateTestHmiState();

  template <class Command>
  std::shared_ptr<Command> CreateCommand(MessageSharedPtr& msg) {
    return std::make_shared<Command>(msg,
                                     app_mngr_,
                                     mock_rpc_service_,
                                     mock_hmi_capabilities_,
                                     mock_policy_handler_);
  }

 protected:
  typedef CommandsTest<CommandsTestMocks::kIsNice>::MockAppManager
      MockAppManager;
  MockAppManager app_mngr_;
  std::shared_ptr<ApplicationImplTest> app_impl_;
  MockHmiInterfaces mock_hmi_interfaces_;
  NiceMock<event_engine_test::MockEventDispatcher> mock_event_dispatcher_;
  NiceMock<MockApplicationManagerSettings> app_mngr_settings_;
  std::shared_ptr<application_manager_test::MockApplication> mock_app_;
  sync_primitives::Lock app_lock_;
  MockRPCService mock_rpc_service_;
  application_manager_test::MockHMICapabilities mock_hmi_capabilities_;
  policy_test::MockPolicyHandlerInterface mock_policy_handler_;
  MockMessageHelper& mock_message_helper_;
  std::shared_ptr<MockHelpPromptManager> mock_help_prompt_manager_;
};

void HelpPromptManagerTest::SetUp() {
  ON_CALL(app_mngr_, hmi_interfaces())
      .WillByDefault(ReturnRef(mock_hmi_interfaces_));
  ON_CALL(mock_hmi_interfaces_, GetInterfaceFromFunction(_))
      .WillByDefault(Return(am::HmiInterfaces::HMI_INTERFACE_SDL));
  ON_CALL(mock_hmi_interfaces_, GetInterfaceState(_))
      .WillByDefault(Return(am::HmiInterfaces::STATE_AVAILABLE));

  ON_CALL(app_mngr_, get_settings())
      .WillByDefault(ReturnRef(app_mngr_settings_));
  ON_CALL(app_mngr_, event_dispatcher())
      .WillByDefault(ReturnRef(mock_event_dispatcher_));
  ON_CALL(app_mngr_settings_, default_timeout())
      .WillByDefault(ReturnRef(kTimeout));
  ON_CALL(app_mngr_settings_, app_icons_folder())
      .WillByDefault(ReturnRef(kDirectoryName));
  ON_CALL(app_mngr_settings_, app_storage_folder())
      .WillByDefault(ReturnRef(kDirectoryName));
  ON_CALL(app_mngr_settings_, audio_data_stopped_timeout())
      .WillByDefault(Return(0));
  ON_CALL(app_mngr_settings_, video_data_stopped_timeout())
      .WillByDefault(Return(0));

  mock_help_prompt_manager_ =
      std::shared_ptr<MockHelpPromptManager>(new MockHelpPromptManager());

  HmiStatePtr state = std::make_shared<HmiState>(
      static_cast<std::shared_ptr<Application> >(mock_app_),
      app_mngr_,
      HmiState::STATE_ID_REGULAR);

  std::string path("storage");
  if (file_system::CreateDirectory(path))
    file_system::CreateFile(path + "/" + "certificate");

  mock_app_ = std::make_shared<application_manager_test::MockApplication>();
}

void HelpPromptManagerTest::TearDown() {
  Mock::VerifyAndClearExpectations(&mock_message_helper_);
}

MessageSharedPtr HelpPromptManagerTest::CreateMsgParams() {
  MessageSharedPtr msg =
      std::make_shared<SmartObject>(smart_objects::SmartType_Map);
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

HmiStatePtr HelpPromptManagerTest::CreateTestHmiState() {
  HmiStatePtr testState = std::make_shared<HmiState>(
      static_cast<std::shared_ptr<Application> >(app_impl_),
      app_mngr_,
      HmiState::STATE_ID_REGULAR);
  testState->set_hmi_level(HMILevel::INVALID_ENUM);
  testState->set_audio_streaming_state(AudioStreamingState::NOT_AUDIBLE);
  testState->set_system_context(SystemContext::SYSCTXT_MAIN);
  return testState;
}

void HelpPromptManagerTest::CreateApplication(
    MockHelpPromptManager& mock_help_prompt_manager) {
  const std::string policy_app_id = "p_app_id";
  const std::string mac_address = "MA:CA:DD:RE:SS";
  const connection_handler::DeviceHandle device_id = 1;
  const custom_str::CustomString app_name("");

  app_impl_.reset(
      new ApplicationImplTest(kAppId,
                              policy_app_id,
                              mac_address,
                              device_id,
                              app_name,
                              std::make_shared<MockStatisticsManager>(),
                              app_mngr_,
                              mock_help_prompt_manager));
  HmiStatePtr initial_state = CreateTestHmiState();
  app_impl_->SetInitialState(initial_state);
}

TEST_F(HelpPromptManagerTest, AddCommand_OnVrCommandAdded) {
  CreateApplication(*mock_help_prompt_manager_.get());
  MessageSharedPtr msg =
      std::make_shared<SmartObject>(smart_objects::SmartType_Map);
  CreateBasicParamsVRRequest(msg);
  (*msg)[strings::params][hmi_response::code] =
      hmi_apis::Common_Result::SUCCESS;
  (*msg)[strings::msg_params][strings::cmd_id] = kCmdId;

  app_impl_->AddCommand(kCmdId, (*msg)[strings::msg_params]);
}

TEST_F(HelpPromptManagerTest, RemoveCommand_OnVrCommandDeleted) {
  CreateApplication(*mock_help_prompt_manager_.get());
  MessageSharedPtr msg =
      std::make_shared<SmartObject>(smart_objects::SmartType_Map);
  CreateBasicParamsVRRequest(msg);
  (*msg)[strings::params][hmi_response::code] =
      hmi_apis::Common_Result::SUCCESS;
  (*msg)[strings::msg_params][strings::cmd_id] = kCmdId;

  app_impl_->AddCommand(kCmdId, (*msg)[strings::msg_params]);
  app_impl_->RemoveCommand(kCmdId);
}

TEST_F(HelpPromptManagerTest,
       Request_OnSetGlobalPropertiesReceived_TTS_SUCCESS) {
  MessageSharedPtr msg = CreateMsgParams();
  SmartObject help_prompt(smart_objects::SmartType_Array);
  help_prompt[0][am::strings::text] = "Help_Prompt_One";
  (*msg)[am::strings::msg_params][am::strings::help_prompt] = help_prompt;
  SmartObject timeout_prompt(smart_objects::SmartType_Array);
  timeout_prompt[0][am::strings::text] = "Timeout_Prompt_One";
  (*msg)[am::strings::msg_params][am::strings::timeout_prompt] = timeout_prompt;

  EXPECT_CALL(mock_message_helper_, VerifyImageVrHelpItems(_, _, _)).Times(0);
  EXPECT_CALL(mock_message_helper_, VerifyTtsFiles(help_prompt, _, _))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));
  EXPECT_CALL(mock_message_helper_, VerifyTtsFiles(timeout_prompt, _, _))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(app_mngr_, RemoveAppFromTTSGlobalPropertiesList(kConnectionKey));

  EXPECT_CALL(*mock_app_, app_id()).WillOnce(Return(kAppId));
  EXPECT_CALL(*mock_app_, set_help_prompt(help_prompt));
  EXPECT_CALL(*mock_app_, help_prompt()).WillOnce(Return(&help_prompt));
  EXPECT_CALL(*mock_app_, set_timeout_prompt(timeout_prompt));
  EXPECT_CALL(*mock_app_, timeout_prompt()).WillOnce(Return(&timeout_prompt));
  EXPECT_CALL(
      mock_hmi_interfaces_,
      GetInterfaceFromFunction(hmi_apis::FunctionID::TTS_SetGlobalProperties))
      .WillOnce(Return(am::HmiInterfaces::HMI_INTERFACE_TTS));

  EXPECT_CALL(*mock_app_, help_prompt_manager())
      .WillOnce(ReturnRef(*mock_help_prompt_manager_.get()));
  EXPECT_CALL(*mock_help_prompt_manager_,
              OnSetGlobalPropertiesReceived(_, false));

  std::shared_ptr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  command->Run();
}

TEST_F(HelpPromptManagerTest,
       Request_OnSetGlobalPropertiesReceived_UI_SUCCESS) {
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
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillRepeatedly(Return(mock_app_));

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

  EXPECT_CALL(*mock_app_, help_prompt_manager())
      .WillOnce(ReturnRef(*mock_help_prompt_manager_.get()));
  EXPECT_CALL(*mock_help_prompt_manager_,
              OnSetGlobalPropertiesReceived(_, false));

  std::shared_ptr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  command->Run();
}

}  // namespace application_manager_test
}  // namespace components
}  // namespace test
