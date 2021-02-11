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
#include <vector>

#include "application_manager/application.h"
#include "application_manager/commands/command_request_test.h"
#include "application_manager/commands/commands_test.h"
#include "application_manager/mock_application.h"
#include "application_manager/mock_application_helper.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/mock_hmi_capabilities.h"
#include "application_manager/mock_hmi_interface.h"
#include "application_manager/mock_resume_ctrl.h"
#include "application_manager/mock_rpc_plugin_manager.h"
#include "application_manager/policies/mock_policy_handler_interface.h"
#include "application_manager/resumption/resumption_data_processor.h"
#include "application_manager/smart_object_keys.h"
#include "connection_handler/mock_connection_handler.h"
#include "gtest/gtest.h"
#include "interfaces/MOBILE_API.h"
#include "mobile/register_app_interface_request.h"
#include "protocol_handler/mock_session_observer.h"
#include "smart_objects/smart_object.h"
#include "utils/custom_string.h"
#include "utils/data_accessor.h"
#include "utils/lock.h"
#include "utils/macro.h"
#include "utils/semantic_version.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {
namespace register_app_interface_request {

using ::testing::_;
using ::testing::DoAll;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::SaveArg;
using ::testing::SetArgPointee;

namespace am = ::application_manager;

using am::commands::MessageSharedPtr;
using am::plugin_manager::MockRPCPluginManager;
using sdl_rpc_plugin::commands::RegisterAppInterfaceRequest;

namespace {
const uint32_t kConnectionKey = 1u;
const uint32_t kConnectionKey2 = 2u;
const uint32_t kDefaultTimeout = 0u;
const connection_handler::DeviceHandle kDeviceHandle = 3u;
const hmi_apis::Common_Language::eType kHmiLanguage =
    hmi_apis::Common_Language::EN_US;
const mobile_apis::Language::eType kMobileLanguage =
    mobile_apis::Language::EN_US;
const mobile_apis::HybridAppPreference::eType kHybridAppPreference =
    mobile_apis::HybridAppPreference::INVALID_ENUM;
const std::string kMacAddress1 = "test_mac_address1";
const std::string kMacAddress2 = "test_mac_address2";
const std::string kAppId1 = "test_app1_id";
const std::string kAppId2 = "test_app2_id";
const std::string kFullAppId = "test_app_id_long";
const std::string kDummyString = "test_string";
const std::vector<uint32_t> kDummyDiagModes;
const utils::SemanticVersion mock_semantic_version(1, 0, 0);
const connection_handler::DeviceHandle kHandle = 1;
}  // namespace

class RegisterAppInterfaceRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
  RegisterAppInterfaceRequestTest()
      : msg_(CreateMessage())
      , command_(CreateCommand<RegisterAppInterfaceRequest>(msg_))
      , app_name_("test_app_name_")
      , app2_name_("test_app2_name_")
      , lock_ptr_(std::make_shared<sync_primitives::Lock>())
      , pending_lock_ptr_(std::make_shared<sync_primitives::Lock>())
      , mock_application_helper_(
            application_manager_test::MockApplicationHelper::
                application_helper_mock())
      , mock_rpc_plugin_manager_(
            std::make_shared<NiceMock<MockRPCPluginManager> >())
      , message_helper_mock_(
            *application_manager::MockMessageHelper::message_helper_mock())
      , notify_upd_manager_(std::make_shared<utils::CallNothing>()) {
    InitGetters();
    InitLanguage();
  }

  void SetUp() OVERRIDE {
    testing::Mock::VerifyAndClearExpectations(&mock_application_helper_);
    testing::Mock::VerifyAndClearExpectations(&message_helper_mock_);

    ON_CALL(app_mngr_, GetPluginManager())
        .WillByDefault(ReturnRef(*mock_rpc_plugin_manager_));

    ON_CALL(app_mngr_, GetPolicyHandler())
        .WillByDefault(ReturnRef(mock_policy_handler_));

    ON_CALL(app_mngr_, GetRPCService())
        .WillByDefault(ReturnRef(mock_rpc_service_));
  }

  void TearDown() OVERRIDE {
    testing::Mock::VerifyAndClearExpectations(&mock_application_helper_);
    testing::Mock::VerifyAndClearExpectations(&message_helper_mock_);
  }

  void InitBasicMessage() {
    (*msg_)[am::strings::params][am::strings::connection_key] = kConnectionKey;
    (*msg_)[am::strings::msg_params][am::strings::app_id] = kAppId1;
    (*msg_)[am::strings::msg_params][am::strings::full_app_id] = kFullAppId;
    (*msg_)[am::strings::msg_params][am::strings::app_name] = app_name_;
    (*msg_)[am::strings::msg_params][am::strings::language_desired] =
        kHmiLanguage;
    (*msg_)[am::strings::msg_params]
           [am::strings::hmi_display_language_desired] = kHmiLanguage;
    (*msg_)[am::strings::msg_params][am::strings::sync_msg_version]
           [am::strings::major_version] = 4;
    (*msg_)[am::strings::msg_params][am::strings::sync_msg_version]
           [am::strings::minor_version] = 0;
    (*msg_)[am::strings::msg_params][am::strings::sync_msg_version]
           [am::strings::patch_version] = 0;
  }

  MockAppPtr CreateBasicMockedApp() {
    MockAppPtr mock_app = CreateMockApp();
    ON_CALL(*mock_app, name()).WillByDefault(ReturnRef(app_name_));
    ON_CALL(*mock_app, mac_address()).WillByDefault(ReturnRef(kMacAddress1));
    ON_CALL(*mock_app, app_icon_path()).WillByDefault(ReturnRef(kDummyString));
    ON_CALL(*mock_app, language()).WillByDefault(ReturnRef(kMobileLanguage));
    ON_CALL(*mock_app, ui_language()).WillByDefault(ReturnRef(kMobileLanguage));
    ON_CALL(*mock_app, is_cloud_app()).WillByDefault(Return(false));
    ON_CALL(*mock_app, hybrid_app_preference())
        .WillByDefault(ReturnRef(kHybridAppPreference));
    ON_CALL(*mock_app, policy_app_id()).WillByDefault(Return(kAppId1));
    ON_CALL(*mock_app, msg_version())
        .WillByDefault(ReturnRef(mock_semantic_version));
    ON_CALL(*mock_app, protocol_version())
        .WillByDefault(
            Return(protocol_handler::MajorProtocolVersion::PROTOCOL_VERSION_5));

    capabilities_builder_ptr_.reset(
        new application_manager::DisplayCapabilitiesBuilder(*mock_app));
    ON_CALL(*mock_app, display_capabilities_builder())
        .WillByDefault(ReturnRef(*capabilities_builder_ptr_));

    return mock_app;
  }

  void InitLanguage(
      hmi_apis::Common_Language::eType ui_language = kHmiLanguage,
      hmi_apis::Common_Language::eType vr_language = kHmiLanguage,
      hmi_apis::Common_Language::eType tts_language = kHmiLanguage) {
    ON_CALL(mock_hmi_capabilities_, active_vr_language())
        .WillByDefault(Return(vr_language));
    ON_CALL(mock_hmi_capabilities_, active_ui_language())
        .WillByDefault(Return(ui_language));
    ON_CALL(mock_hmi_capabilities_, active_tts_language())
        .WillByDefault(Return(tts_language));
  }

  void InitGetters() {
    ON_CALL(app_mngr_, GetCorrectMobileIDFromMessage(msg_))
        .WillByDefault(Return(kAppId1));
    ON_CALL(app_mngr_, WaitForHmiIsReady()).WillByDefault(Return(true));
    ON_CALL(app_mngr_, resume_controller())
        .WillByDefault(ReturnRef(mock_resume_crt_));
    ON_CALL(app_mngr_, connection_handler())
        .WillByDefault(ReturnRef(mock_connection_handler_));
    ON_CALL(mock_connection_handler_, get_session_observer())
        .WillByDefault(ReturnRef(mock_session_observer_));
    ON_CALL(app_mngr_settings_, sdl_version())
        .WillByDefault(ReturnRef(kDummyString));
    ON_CALL(mock_hmi_capabilities_, ccpu_version())
        .WillByDefault(ReturnRef(kDummyString));
    ON_CALL(mock_hmi_capabilities_, speech_capabilities())
        .WillByDefault(Return(smart_objects::SmartObjectSPtr()));
    ON_CALL(mock_hmi_capabilities_, prerecorded_speech())
        .WillByDefault(Return(smart_objects::SmartObjectSPtr()));
    ON_CALL(mock_hmi_capabilities_, vr_capabilities())
        .WillByDefault(Return(smart_objects::SmartObjectSPtr()));
    ON_CALL(mock_hmi_capabilities_, display_capabilities())
        .WillByDefault(Return(smart_objects::SmartObjectSPtr()));
    ON_CALL(mock_hmi_capabilities_, seat_location_capability())
        .WillByDefault(Return(smart_objects::SmartObjectSPtr()));
    ON_CALL(mock_hmi_capabilities_, audio_pass_thru_capabilities())
        .WillByDefault(Return(smart_objects::SmartObjectSPtr()));
    ON_CALL(mock_hmi_capabilities_, vehicle_type())
        .WillByDefault(Return(smart_objects::SmartObjectSPtr()));
    ON_CALL(mock_hmi_capabilities_, button_capabilities())
        .WillByDefault(Return(smart_objects::SmartObjectSPtr()));
    ON_CALL(mock_hmi_capabilities_, soft_button_capabilities())
        .WillByDefault(Return(smart_objects::SmartObjectSPtr()));
    ON_CALL(mock_hmi_capabilities_, preset_bank_capabilities())
        .WillByDefault(Return(smart_objects::SmartObjectSPtr()));
    ON_CALL(mock_hmi_capabilities_, hmi_zone_capabilities())
        .WillByDefault(Return(smart_objects::SmartObjectSPtr()));
    ON_CALL(mock_hmi_capabilities_, pcm_stream_capabilities())
        .WillByDefault(Return(smart_objects::SmartObjectSPtr()));
    ON_CALL(mock_hmi_capabilities_, seat_location_capability())
        .WillByDefault(Return(smart_objects::SmartObjectSPtr()));
    ON_CALL(app_mngr_settings_, supported_diag_modes())
        .WillByDefault(ReturnRef(kDummyDiagModes));
    ON_CALL(mock_policy_handler_, GetAppRequestTypes(_, _))
        .WillByDefault(Return(std::vector<std::string>()));
    ON_CALL(mock_policy_handler_, GetAppRequestTypeState(_))
        .WillByDefault(Return(policy::RequestType::State::EMPTY));
    ON_CALL(mock_policy_handler_, GetAppRequestSubTypes(_))
        .WillByDefault(Return(std::vector<std::string>()));
    ON_CALL(mock_policy_handler_, GetAppRequestSubTypeState(_))
        .WillByDefault(Return(policy::RequestSubType::State::EMPTY));
    ON_CALL(mock_policy_handler_, GetUserConsentForDevice(_))
        .WillByDefault(Return(policy::DeviceConsent::kDeviceAllowed));
    ON_CALL(app_mngr_, GetDeviceTransportType(_))
        .WillByDefault(Return(hmi_apis::Common_TransportType::WIFI));
    ON_CALL(app_mngr_, IsAppInReconnectMode(_, _)).WillByDefault(Return(false));
    ON_CALL(app_mngr_, application_by_policy_id(_))
        .WillByDefault(Return(ApplicationSharedPtr()));
    ON_CALL(app_mngr_, pending_application_by_policy_id(_))
        .WillByDefault(Return(ApplicationSharedPtr()));
    ON_CALL(mock_hmi_interfaces_, GetInterfaceState(_))
        .WillByDefault(Return(am::HmiInterfaces::STATE_NOT_AVAILABLE));
    ON_CALL(
        mock_hmi_interfaces_,
        GetInterfaceFromFunction(hmi_apis::FunctionID::VR_ChangeRegistration))
        .WillByDefault(Return(am::HmiInterfaces::HMI_INTERFACE_VR));
    ON_CALL(
        mock_hmi_interfaces_,
        GetInterfaceFromFunction(hmi_apis::FunctionID::TTS_ChangeRegistration))
        .WillByDefault(Return(am::HmiInterfaces::HMI_INTERFACE_TTS));
    ON_CALL(
        mock_hmi_interfaces_,
        GetInterfaceFromFunction(hmi_apis::FunctionID::UI_ChangeRegistration))
        .WillByDefault(Return(am::HmiInterfaces::HMI_INTERFACE_UI));
  }

  void SetCommonExpectionsOnSwitchedApplication(
      MockAppPtr mock_app, mobile_apis::Result::eType response_result_code) {
    EXPECT_CALL(mock_policy_handler_, AddApplication(_, _, _)).Times(0);

    EXPECT_CALL(
        mock_rpc_service_,
        ManageMobileCommand(MobileResultCodeIs(response_result_code), _));

    EXPECT_CALL(
        mock_rpc_service_,
        ManageHMICommand(
            HMIResultCodeIs(
                hmi_apis::FunctionID::BasicCommunication_OnAppRegistered),
            _))
        .Times(0);

    EXPECT_CALL(
        mock_rpc_service_,
        ManageHMICommand(
            HMIResultCodeIs(hmi_apis::FunctionID::Buttons_OnButtonSubscription),
            _))
        .Times(0);

    EXPECT_CALL(
        mock_rpc_service_,
        ManageHMICommand(
            HMIResultCodeIs(hmi_apis::FunctionID::UI_ChangeRegistration), _))
        .Times(0);

    EXPECT_CALL(
        mock_rpc_service_,
        ManageHMICommand(
            HMIResultCodeIs(hmi_apis::FunctionID::TTS_ChangeRegistration), _))
        .Times(0);

    EXPECT_CALL(
        mock_rpc_service_,
        ManageHMICommand(
            HMIResultCodeIs(hmi_apis::FunctionID::VR_ChangeRegistration), _))
        .Times(0);

    auto notification = std::make_shared<smart_objects::SmartObject>();
    ON_CALL(message_helper_mock_, CreateHMIStatusNotification(_, _))
        .WillByDefault(Return(notification));
    EXPECT_CALL(mock_rpc_service_, ManageMobileCommand(notification, _));

    EXPECT_CALL(app_mngr_,
                OnApplicationSwitched(
                    std::static_pointer_cast<application_manager::Application>(
                        mock_app)));
  }

  void SetCommonPreconditionsToRegisterApplication() {
    ON_CALL(app_mngr_, IsApplicationForbidden(_, _))
        .WillByDefault(Return(false));
    ON_CALL(mock_connection_handler_,
            GetDataOnSessionKey(kConnectionKey, _, _, _))
        .WillByDefault(DoAll(SetArgPointee<3>(kDeviceHandle), Return(0)));
    ON_CALL(mock_session_observer_,
            GetDataOnDeviceID(kDeviceHandle, _, _, _, _))
        .WillByDefault(DoAll(SetArgPointee<3>(kMacAddress1), Return(0)));
    ON_CALL(app_mngr_, reregister_application_by_policy_id(kAppId1))
        .WillByDefault(Return(ApplicationSharedPtr()));
    ON_CALL(app_mngr_, application(kMacAddress1, kAppId1))
        .WillByDefault(Return(ApplicationSharedPtr()));
    ON_CALL(app_mngr_, applications())
        .WillByDefault(
            Return(DataAccessor<am::ApplicationSet>(app_set_, lock_ptr_)));
    ON_CALL(app_mngr_, pending_applications())
        .WillByDefault(Return(DataAccessor<am::AppsWaitRegistrationSet>(
            pending_app_set_, pending_lock_ptr_)));
    ON_CALL(mock_policy_handler_, PolicyEnabled()).WillByDefault(Return(true));
    ON_CALL(mock_policy_handler_, GetInitialAppData(kAppId1, _, _))
        .WillByDefault(Return(true));
    ON_CALL(mock_rpc_service_,
            ManageHMICommand(
                HMIResultCodeIs(
                    hmi_apis::FunctionID::BasicCommunication_OnAppRegistered),
                _))
        .WillByDefault(Return(true));
    ON_CALL(
        mock_rpc_service_,
        ManageHMICommand(
            HMIResultCodeIs(hmi_apis::FunctionID::Buttons_OnButtonSubscription),
            _))
        .WillByDefault(Return(true));
  }

  void SetCommonPreconditionsToCheckWithPolicyData(MockAppPtr mock_app) {
    ON_CALL(mock_policy_handler_, AddApplication(kMacAddress1, kAppId1, _))
        .WillByDefault(Return(notify_upd_manager_));
    ON_CALL(app_mngr_, application(kConnectionKey))
        .WillByDefault(Return(mock_app));
    ON_CALL(app_mngr_, RegisterApplication(msg_))
        .WillByDefault(Return(mock_app));
  }

  MessageSharedPtr msg_;
  std::shared_ptr<RegisterAppInterfaceRequest> command_;

  const utils::custom_string::CustomString app_name_;
  const utils::custom_string::CustomString app2_name_;
  std::shared_ptr<sync_primitives::Lock> lock_ptr_;
  am::ApplicationSet app_set_;
  std::shared_ptr<sync_primitives::Lock> pending_lock_ptr_;
  am::AppsWaitRegistrationSet pending_app_set_;

  typedef IsNiceMock<policy_test::MockPolicyHandlerInterface,
                     kMocksAreNice>::Result MockPolicyHandlerInterface;

  typedef IsNiceMock<resumption_test::MockResumeCtrl, kMocksAreNice>::Result
      MockResumeCtrl;

  typedef IsNiceMock<connection_handler_test::MockConnectionHandler,
                     kMocksAreNice>::Result MockConnectionHandler;

  typedef IsNiceMock<protocol_handler_test::MockSessionObserver,
                     kMocksAreNice>::Result MockSessionObserver;

  typedef IsNiceMock<application_manager_test::MockHMICapabilities,
                     kMocksAreNice>::Result MockHMICapabilities;

  MockResumeCtrl mock_resume_crt_;
  MockConnectionHandler mock_connection_handler_;
  MockSessionObserver mock_session_observer_;
  application_manager_test::MockApplicationHelper& mock_application_helper_;
  std::shared_ptr<am::plugin_manager::MockRPCPluginManager>
      mock_rpc_plugin_manager_;
  application_manager::MockMessageHelper& message_helper_mock_;
  policy::StatusNotifier notify_upd_manager_;
  std::unique_ptr<application_manager::DisplayCapabilitiesBuilder>
      capabilities_builder_ptr_;
};

TEST_F(RegisterAppInterfaceRequestTest, Init_SUCCESS) {
  EXPECT_TRUE(command_->Init());
}

TEST_F(RegisterAppInterfaceRequestTest, DefaultTimeout_CheckIfZero_SUCCESS) {
  command_->Init();
  EXPECT_EQ(command_->default_timeout(), kDefaultTimeout);
}

TEST_F(RegisterAppInterfaceRequestTest, Run_MinimalData_SUCCESS) {
  InitBasicMessage();
  (*msg_)[am::strings::msg_params][am::strings::hash_id] = kAppId1;
  EXPECT_CALL(app_mngr_, WaitForHmiIsReady()).WillOnce(Return(true));
  EXPECT_CALL(app_mngr_, IsApplicationForbidden(_, _)).WillOnce(Return(false));

  ON_CALL(mock_connection_handler_,
          GetDataOnSessionKey(kConnectionKey, _, _, _))
      .WillByDefault(DoAll(SetArgPointee<3>(kHandle), Return(0)));
  ON_CALL(mock_session_observer_, GetDataOnDeviceID(kHandle, _, _, _, _))
      .WillByDefault(DoAll(SetArgPointee<3>(kMacAddress1), Return(0)));

  MockAppPtr mock_app = CreateBasicMockedApp();
  EXPECT_CALL(app_mngr_, reregister_application_by_policy_id(kAppId1))
      .WillOnce(Return(ApplicationSharedPtr()));
  EXPECT_CALL(app_mngr_, application(kMacAddress1, kAppId1))
      .WillRepeatedly(Return(ApplicationSharedPtr()));

  ON_CALL(app_mngr_, applications())
      .WillByDefault(
          Return(DataAccessor<am::ApplicationSet>(app_set_, lock_ptr_)));
  ON_CALL(app_mngr_, pending_applications())
      .WillByDefault(Return(DataAccessor<am::AppsWaitRegistrationSet>(
          pending_app_set_, pending_lock_ptr_)));

  ON_CALL(app_mngr_, application(kConnectionKey))
      .WillByDefault(Return(mock_app));

  ON_CALL(mock_policy_handler_, PolicyEnabled()).WillByDefault(Return(true));
  ON_CALL(mock_policy_handler_, GetInitialAppData(kAppId1, _, _))
      .WillByDefault(Return(true));
  ON_CALL(mock_policy_handler_, AddApplication(kMacAddress1, kAppId1, _))
      .WillByDefault(Return(notify_upd_manager_));

  EXPECT_CALL(app_mngr_, RegisterApplication(msg_)).WillOnce(Return(mock_app));

  EXPECT_CALL(mock_rpc_service_,
              ManageHMICommand(
                  HMIResultCodeIs(
                      hmi_apis::FunctionID::BasicCommunication_OnAppRegistered),
                  _))
      .WillOnce(Return(true));
  EXPECT_CALL(
      mock_rpc_service_,
      ManageHMICommand(
          HMIResultCodeIs(hmi_apis::FunctionID::Buttons_OnButtonSubscription),
          _))
      .WillOnce(Return(true));

  application_manager::DisplayCapabilitiesBuilder builder(*mock_app);
  ON_CALL(*mock_app, display_capabilities_builder())
      .WillByDefault(ReturnRef(builder));

  EXPECT_CALL(mock_rpc_service_,
              ManageMobileCommand(_, am::commands::Command::SOURCE_SDL))
      .Times(2);
  EXPECT_CALL(app_mngr_, SendDriverDistractionState(_));
  ASSERT_TRUE(command_->Init());
  command_->Run();
}

MATCHER_P(CheckHMIInterfacesRealtedData, expected_data, "") {
  const bool is_result_id_correct =
      mobile_apis::Result::SUCCESS ==
      static_cast<mobile_apis::Result::eType>(
          (*arg)[am::strings::msg_params][am::strings::result_code].asInt());

  const bool are_ui_related_data_exist =
      (*arg)[am::strings::msg_params].keyExists(
          am::hmi_response::display_capabilities);

  const bool are_vi_related_data_exist =
      (*arg)[am::strings::msg_params][am::hmi_response::vehicle_type] ==
      (*expected_data)[am::hmi_response::vehicle_type];

  const bool are_vr_related_data_exist =
      (*arg)[am::strings::msg_params][am::strings::vr_capabilities] ==
      (*expected_data)[am::strings::vr_capabilities];

  return is_result_id_correct && are_ui_related_data_exist &&
         are_vi_related_data_exist && are_vr_related_data_exist;
}

TEST_F(RegisterAppInterfaceRequestTest,
       Run_HmiInterfacesStateAvailable_SUCCESS) {
  InitBasicMessage();

  ON_CALL(app_mngr_, WaitForHmiIsReady()).WillByDefault(Return(true));
  EXPECT_CALL(app_mngr_, IsApplicationForbidden(_, _)).WillOnce(Return(false));

  ON_CALL(mock_connection_handler_,
          GetDataOnSessionKey(kConnectionKey, _, _, _))
      .WillByDefault(DoAll(SetArgPointee<3>(kHandle), Return(0)));
  ON_CALL(mock_session_observer_, GetDataOnDeviceID(_, _, _, _, _))
      .WillByDefault(DoAll(SetArgPointee<3>(kMacAddress1), Return(0)));

  MockAppPtr mock_app = CreateBasicMockedApp();
  EXPECT_CALL(app_mngr_, reregister_application_by_policy_id(kAppId1))
      .WillOnce(Return(ApplicationSharedPtr()));
  EXPECT_CALL(app_mngr_, application(kMacAddress1, kAppId1))
      .WillRepeatedly(Return(ApplicationSharedPtr()));

  ON_CALL(app_mngr_, application(kConnectionKey))
      .WillByDefault(Return(mock_app));

  MessageSharedPtr expected_message =
      CreateMessage(smart_objects::SmartType_Map);

  (*expected_message)[am::hmi_response::vehicle_type] = "test_vehicle_type";
  (*expected_message)[am::strings::vr_capabilities] = "test_vr_capabilities";
  (*expected_message)[am::hmi_response::display_capabilities] = 0;
  SmartObject& display_capabilities =
      (*expected_message)[am::hmi_response::display_capabilities];
  display_capabilities[am::hmi_response::display_type] = "test_display_type";
  display_capabilities[am::hmi_response::display_name] = "GENERIC_DISPLAY";
  display_capabilities[am::hmi_response::text_fields] = "test_text_fields";
  display_capabilities[am::hmi_response::image_fields] = "test_image_fields";
  display_capabilities[am::hmi_response::media_clock_formats] =
      "test_media_clock_ptr_formats";
  display_capabilities[am::hmi_response::num_custom_presets_available] =
      "test_num_custom_presets_available";
  display_capabilities[am::hmi_response::graphic_supported] =
      "test_graphic_supported";
  display_capabilities[am::hmi_response::templates_available] =
      "test_templates_available";
  display_capabilities[am::hmi_response::screen_params] = "test_screen_params";

  auto vehicle_type_ptr = std::make_shared<smart_objects::SmartObject>(
      (*expected_message)[am::hmi_response::vehicle_type]);
  ON_CALL(mock_hmi_capabilities_, vehicle_type())
      .WillByDefault(Return(vehicle_type_ptr));

  auto vr_capabilities_ptr = std::make_shared<smart_objects::SmartObject>(
      (*expected_message)[am::strings::vr_capabilities]);
  ON_CALL(mock_hmi_capabilities_, vr_capabilities())
      .WillByDefault(Return(vr_capabilities_ptr));

  auto display_capabilities_ptr = std::make_shared<smart_objects::SmartObject>(
      (*expected_message)[am::hmi_response::display_capabilities]);
  ON_CALL(mock_hmi_capabilities_, display_capabilities())
      .WillByDefault(Return(display_capabilities_ptr));

  ON_CALL(app_mngr_, applications())
      .WillByDefault(
          Return(DataAccessor<am::ApplicationSet>(app_set_, lock_ptr_)));
  ON_CALL(app_mngr_, pending_applications())
      .WillByDefault(Return(DataAccessor<am::AppsWaitRegistrationSet>(
          pending_app_set_, pending_lock_ptr_)));
  ON_CALL(mock_policy_handler_, PolicyEnabled()).WillByDefault(Return(true));
  ON_CALL(mock_policy_handler_, GetInitialAppData(kAppId1, _, _))
      .WillByDefault(Return(true));
  ON_CALL(mock_policy_handler_, AddApplication(kMacAddress1, kAppId1, _))
      .WillByDefault(Return(notify_upd_manager_));

  EXPECT_CALL(app_mngr_, RegisterApplication(msg_)).WillOnce(Return(mock_app));

  EXPECT_CALL(mock_hmi_interfaces_, GetInterfaceState(_))
      .WillRepeatedly(Return(am::HmiInterfaces::STATE_AVAILABLE));

  EXPECT_CALL(mock_rpc_service_,
              ManageHMICommand(
                  HMIResultCodeIs(
                      hmi_apis::FunctionID::BasicCommunication_OnAppRegistered),
                  _))
      .WillOnce(Return(true));
  EXPECT_CALL(
      mock_rpc_service_,
      ManageHMICommand(
          HMIResultCodeIs(hmi_apis::FunctionID::Buttons_OnButtonSubscription),
          _))
      .WillOnce(Return(true));
  EXPECT_CALL(
      mock_rpc_service_,
      ManageHMICommand(
          HMIResultCodeIs(hmi_apis::FunctionID::VR_ChangeRegistration), _))
      .WillOnce(Return(true));
  EXPECT_CALL(
      mock_rpc_service_,
      ManageHMICommand(
          HMIResultCodeIs(hmi_apis::FunctionID::TTS_ChangeRegistration), _))
      .WillOnce(Return(true));
  EXPECT_CALL(
      mock_rpc_service_,
      ManageHMICommand(
          HMIResultCodeIs(hmi_apis::FunctionID::UI_ChangeRegistration), _))
      .WillOnce(Return(true));

  application_manager::DisplayCapabilitiesBuilder builder(*mock_app);
  ON_CALL(*mock_app, display_capabilities_builder())
      .WillByDefault(ReturnRef(builder));

  EXPECT_CALL(mock_rpc_service_,
              ManageMobileCommand(_, am::commands::Command::SOURCE_SDL))
      .Times(2);
  EXPECT_CALL(app_mngr_, SendDriverDistractionState(_));
  ASSERT_TRUE(command_->Init());
  command_->Run();
}

TEST_F(RegisterAppInterfaceRequestTest,
       SwitchApplication_CorrectHash_ExpectNoCleanupSuccess) {
  InitBasicMessage();

  const std::string request_hash_id = "abc123";
  (*msg_)[am::strings::msg_params][am::strings::hash_id] = request_hash_id;

  MockAppPtr mock_app = CreateBasicMockedApp();
  app_set_.insert(mock_app);
  EXPECT_CALL(app_mngr_, reregister_application_by_policy_id(kAppId1))
      .WillOnce(Return(mock_app));

  connection_handler::DeviceHandle device_id = 1;
  ON_CALL(mock_connection_handler_,
          GetDataOnSessionKey(kConnectionKey, _, _, _))
      .WillByDefault(DoAll(SetArgPointee<3>(device_id), Return(0)));

  ON_CALL(mock_session_observer_, GetDataOnDeviceID(device_id, _, _, _, _))
      .WillByDefault(DoAll(SetArgPointee<3>(kMacAddress1), Return(0)));

  EXPECT_CALL(*mock_app, device()).WillOnce(Return(kDeviceHandle));

  EXPECT_CALL(app_mngr_, ProcessReconnection(_, kConnectionKey));

  EXPECT_CALL(app_mngr_, RegisterApplication(msg_)).Times(0);

  EXPECT_CALL(
      mock_resume_crt_,
      CheckApplicationHash(
          std::static_pointer_cast<application_manager::Application>(mock_app),
          request_hash_id))
      .WillOnce(Return(true));

  EXPECT_CALL(mock_resume_crt_, RemoveApplicationFromSaved(_)).Times(0);

  EXPECT_CALL(mock_application_helper_, RecallApplicationData(_, _)).Times(0);

  ON_CALL(app_mngr_, application(kConnectionKey))
      .WillByDefault(Return(mock_app));

  SetCommonExpectionsOnSwitchedApplication(mock_app,
                                           mobile_apis::Result::SUCCESS);

  ASSERT_TRUE(command_->Init());
  command_->Run();
}

TEST_F(RegisterAppInterfaceRequestTest,
       SwitchApplication_WrongHash_ExpectCleanupResumeFailed) {
  InitBasicMessage();

  const std::string request_hash_id = "abc123";
  (*msg_)[am::strings::msg_params][am::strings::hash_id] = request_hash_id;

  connection_handler::DeviceHandle device_id = 1;
  ON_CALL(mock_connection_handler_,
          GetDataOnSessionKey(kConnectionKey, _, _, _))
      .WillByDefault(DoAll(SetArgPointee<3>(device_id), Return(0)));

  ON_CALL(mock_session_observer_, GetDataOnDeviceID(device_id, _, _, _, _))
      .WillByDefault(DoAll(SetArgPointee<3>(kMacAddress1), Return(0)));

  MockAppPtr mock_app = CreateBasicMockedApp();
  EXPECT_CALL(app_mngr_, reregister_application_by_policy_id(kAppId1))
      .WillOnce(Return(mock_app));
  EXPECT_CALL(*mock_app, device()).WillOnce(Return(kDeviceHandle));

  EXPECT_CALL(app_mngr_, ProcessReconnection(_, kConnectionKey));

  EXPECT_CALL(
      mock_resume_crt_,
      CheckApplicationHash(
          std::static_pointer_cast<application_manager::Application>(mock_app),
          request_hash_id))
      .WillOnce(Return(false));

  EXPECT_CALL(
      mock_application_helper_,
      RecallApplicationData(
          std::static_pointer_cast<application_manager::Application>(mock_app),
          _));

  EXPECT_CALL(app_mngr_, RegisterApplication(msg_)).Times(0);

  ON_CALL(app_mngr_, application(kConnectionKey))
      .WillByDefault(Return(mock_app));

  SetCommonExpectionsOnSwitchedApplication(mock_app,
                                           mobile_apis::Result::RESUME_FAILED);
  ASSERT_TRUE(command_->Init());
  command_->Run();
}

TEST_F(RegisterAppInterfaceRequestTest,
       SwitchApplication_NoHash_ExpectCleanupResumeFailed) {
  InitBasicMessage();

  connection_handler::DeviceHandle device_id = 1;
  ON_CALL(mock_connection_handler_,
          GetDataOnSessionKey(kConnectionKey, _, _, _))
      .WillByDefault(DoAll(SetArgPointee<3>(device_id), Return(0)));

  ON_CALL(mock_session_observer_, GetDataOnDeviceID(device_id, _, _, _, _))
      .WillByDefault(DoAll(SetArgPointee<3>(kMacAddress1), Return(0)));

  MockAppPtr mock_app = CreateBasicMockedApp();
  EXPECT_CALL(app_mngr_, reregister_application_by_policy_id(kAppId1))
      .WillOnce(Return(mock_app));
  EXPECT_CALL(*mock_app, device()).WillOnce(Return(kDeviceHandle));

  EXPECT_CALL(app_mngr_, ProcessReconnection(_, kConnectionKey));

  EXPECT_CALL(
      mock_application_helper_,
      RecallApplicationData(
          std::static_pointer_cast<application_manager::Application>(mock_app),
          _));

  EXPECT_CALL(app_mngr_, RegisterApplication(msg_)).Times(0);

  ON_CALL(app_mngr_, application(kConnectionKey))
      .WillByDefault(Return(mock_app));

  SetCommonExpectionsOnSwitchedApplication(mock_app,
                                           mobile_apis::Result::RESUME_FAILED);

  ASSERT_TRUE(command_->Init());
  command_->Run();
}

TEST_F(RegisterAppInterfaceRequestTest,
       RegisterApp_SameAppId_SameDeviceFailed) {
  using namespace am;

  InitBasicMessage();

  MockAppPtr mock_app1 = CreateBasicMockedApp();

  app_set_.insert(mock_app1);
  ON_CALL(app_mngr_, applications())
      .WillByDefault(
          Return(DataAccessor<am::ApplicationSet>(app_set_, lock_ptr_)));
  ON_CALL(app_mngr_, pending_applications())
      .WillByDefault(Return(DataAccessor<am::AppsWaitRegistrationSet>(
          pending_app_set_, pending_lock_ptr_)));

  MockAppPtr mock_app2 = CreateBasicMockedApp();

  connection_handler::DeviceHandle device_id = 1;
  ON_CALL(mock_connection_handler_,
          GetDataOnSessionKey(kConnectionKey, _, _, _))
      .WillByDefault(DoAll(SetArgPointee<3>(device_id), Return(0)));

  ON_CALL(mock_session_observer_, GetDataOnDeviceID(device_id, _, _, _, _))
      .WillByDefault(DoAll(SetArgPointee<3>(kMacAddress1), Return(0)));

  EXPECT_CALL(app_mngr_, reregister_application_by_policy_id(kAppId1))
      .WillOnce(Return(ApplicationSharedPtr()));
  EXPECT_CALL(app_mngr_, application(kMacAddress1, kAppId1))
      .WillOnce(Return(mock_app1));

  EXPECT_CALL(mock_rpc_service_, ManageHMICommand(_, _)).Times(0);

  EXPECT_CALL(mock_rpc_service_,
              ManageMobileCommand(
                  MobileResultCodeIs(
                      mobile_apis::Result::APPLICATION_REGISTERED_ALREADY),
                  am::commands::Command::SOURCE_SDL));

  ASSERT_TRUE(command_->Init());
  command_->Run();
}

TEST_F(RegisterAppInterfaceRequestTest,
       RegisterApp_SameAppId_DifferentDevicesSuccess) {
  MockAppPtr mock_app1 = CreateBasicMockedApp();
  const connection_handler::DeviceHandle device_id1 = 1u;
  ON_CALL(*mock_app1, device()).WillByDefault(Return(device_id1));
  app_set_.insert(mock_app1);
  ON_CALL(app_mngr_, applications())
      .WillByDefault(
          Return(DataAccessor<am::ApplicationSet>(app_set_, lock_ptr_)));
  ON_CALL(app_mngr_, pending_applications())
      .WillByDefault(Return(DataAccessor<am::AppsWaitRegistrationSet>(
          pending_app_set_, pending_lock_ptr_)));

  InitBasicMessage();
  (*msg_)[am::strings::params][am::strings::connection_key] = kConnectionKey2;

  ON_CALL(app_mngr_, WaitForHmiIsReady()).WillByDefault(Return(true));
  EXPECT_CALL(app_mngr_, IsApplicationForbidden(kConnectionKey2, kAppId1))
      .WillOnce(Return(false));

  connection_handler::DeviceHandle device_id2 = 2u;
  ON_CALL(mock_connection_handler_,
          GetDataOnSessionKey(kConnectionKey2, _, _, _))
      .WillByDefault(DoAll(SetArgPointee<3>(device_id2), Return(0)));

  ON_CALL(mock_session_observer_, GetDataOnDeviceID(device_id2, _, _, _, _))
      .WillByDefault(DoAll(SetArgPointee<3>(kMacAddress2), Return(0)));

  MockAppPtr mock_app2 = CreateBasicMockedApp();

  ON_CALL(*mock_app2, device()).WillByDefault(Return(device_id2));
  ON_CALL(*mock_app2, mac_address()).WillByDefault(ReturnRef(kMacAddress2));
  EXPECT_CALL(app_mngr_, application(kMacAddress2, kAppId1))
      .WillRepeatedly(Return(ApplicationSharedPtr()));
  EXPECT_CALL(app_mngr_, reregister_application_by_policy_id(kAppId1))
      .WillOnce(Return(ApplicationSharedPtr()));

  ON_CALL(app_mngr_, application(kConnectionKey2))
      .WillByDefault(Return(mock_app2));

  ON_CALL(mock_policy_handler_, PolicyEnabled()).WillByDefault(Return(true));
  ON_CALL(mock_policy_handler_, GetInitialAppData(kAppId1, _, _))
      .WillByDefault(Return(true));
  ON_CALL(mock_policy_handler_, AddApplication(kMacAddress2, kAppId1, _))
      .WillByDefault(Return(notify_upd_manager_));

  EXPECT_CALL(app_mngr_, RegisterApplication(msg_)).WillOnce(Return(mock_app2));

  EXPECT_CALL(mock_hmi_interfaces_, GetInterfaceState(_))
      .WillRepeatedly(Return(am::HmiInterfaces::STATE_AVAILABLE));

  EXPECT_CALL(mock_rpc_service_,
              ManageHMICommand(
                  HMIResultCodeIs(
                      hmi_apis::FunctionID::BasicCommunication_OnAppRegistered),
                  _))
      .WillOnce(Return(true));
  EXPECT_CALL(
      mock_rpc_service_,
      ManageHMICommand(
          HMIResultCodeIs(hmi_apis::FunctionID::Buttons_OnButtonSubscription),
          _))
      .WillOnce(Return(true));
  EXPECT_CALL(
      mock_rpc_service_,
      ManageHMICommand(
          HMIResultCodeIs(hmi_apis::FunctionID::VR_ChangeRegistration), _))
      .WillOnce(Return(true));
  EXPECT_CALL(
      mock_rpc_service_,
      ManageHMICommand(
          HMIResultCodeIs(hmi_apis::FunctionID::TTS_ChangeRegistration), _))
      .WillOnce(Return(true));
  EXPECT_CALL(
      mock_rpc_service_,
      ManageHMICommand(
          HMIResultCodeIs(hmi_apis::FunctionID::UI_ChangeRegistration), _))
      .WillOnce(Return(true));

  application_manager::DisplayCapabilitiesBuilder builder(*mock_app2);
  ON_CALL(*mock_app2, display_capabilities_builder())
      .WillByDefault(ReturnRef(builder));

  EXPECT_CALL(mock_rpc_service_,
              ManageMobileCommand(_, am::commands::Command::SOURCE_SDL))
      .Times(2);
  EXPECT_CALL(app_mngr_, SendDriverDistractionState(_));

  ASSERT_TRUE(command_->Init());
  command_->Run();
}

TEST_F(RegisterAppInterfaceRequestTest,
       RegisterAppWith_WEB_VIEW_HMITypeSuccess) {
  SetCommonPreconditionsToRegisterApplication();

  InitBasicMessage();
  (*msg_)[am::strings::msg_params][am::strings::app_hmi_type][0] =
      mobile_apis::AppHMIType::WEB_VIEW;

  MockAppPtr mock_app = CreateBasicMockedApp();
  SetCommonPreconditionsToCheckWithPolicyData(mock_app);
  std::vector<std::string> present_hmi_types_in_policy{"WEB_VIEW"};
  ON_CALL(mock_policy_handler_, GetInitialAppData(kAppId1, _, _))
      .WillByDefault(
          DoAll(SetArgPointee<2>(present_hmi_types_in_policy), Return(true)));

  auto app = std::static_pointer_cast<am::Application>(mock_app);

  EXPECT_CALL(*mock_app, set_webengine_projection_enabled(true));
  EXPECT_CALL(app_mngr_, OnApplicationRegistered(app));
  EXPECT_CALL(mock_policy_handler_,
              OnAppRegisteredOnMobile(kMacAddress1, kAppId1));
  EXPECT_CALL(mock_rpc_service_,
              ManageMobileCommand(_, am::commands::Command::SOURCE_SDL))
      .Times(2);
  EXPECT_CALL(app_mngr_, SendDriverDistractionState(app));

  ASSERT_TRUE(command_->Init());
  command_->Run();
}

TEST_F(RegisterAppInterfaceRequestTest,
       RegisterApp_WithoutAppHMITypes_SuccessfulRegistration) {
  SetCommonPreconditionsToRegisterApplication();
  InitBasicMessage();

  MockAppPtr mock_app = CreateBasicMockedApp();
  SetCommonPreconditionsToCheckWithPolicyData(mock_app);
  std::vector<std::string> present_hmi_types_in_policy{"NAVIGATION"};
  ON_CALL(mock_policy_handler_, GetInitialAppData(kAppId1, _, _))
      .WillByDefault(
          DoAll(SetArgPointee<2>(present_hmi_types_in_policy), Return(true)));

  auto response_to_mobile = CreateMessage();
  EXPECT_CALL(
      mock_rpc_service_,
      ManageMobileCommand(_, am::commands::Command::CommandSource::SOURCE_SDL))
      .WillOnce(DoAll(SaveArg<0>(&response_to_mobile), Return(true)))
      .WillOnce(Return(true));

  ASSERT_TRUE(command_->Init());
  command_->Run();

  const auto result_code = static_cast<mobile_apis::Result::eType>(
      (*response_to_mobile)[am::strings::msg_params][am::strings::result_code]
          .asUInt());

  EXPECT_EQ(mobile_apis::Result::SUCCESS, result_code);
}

TEST_F(
    RegisterAppInterfaceRequestTest,
    RegisterAppInterface_AppHMITypesDontExistInPTForSpecifiedApp_SuccessfulRegistration) {
  SetCommonPreconditionsToRegisterApplication();
  InitBasicMessage();
  (*msg_)[am::strings::msg_params][am::strings::app_hmi_type][0] =
      mobile_apis::AppHMIType::PROJECTION;
  (*msg_)[am::strings::msg_params][am::strings::app_hmi_type][1] =
      mobile_apis::AppHMIType::INFORMATION;
  (*msg_)[am::strings::msg_params][am::strings::app_hmi_type][2] =
      mobile_apis::AppHMIType::COMMUNICATION;
  (*msg_)[am::strings::msg_params][am::strings::app_hmi_type][3] =
      mobile_apis::AppHMIType::NAVIGATION;
  (*msg_)[am::strings::msg_params][am::strings::app_hmi_type][4] =
      mobile_apis::AppHMIType::MESSAGING;

  MockAppPtr mock_app = CreateBasicMockedApp();
  SetCommonPreconditionsToCheckWithPolicyData(mock_app);
  std::vector<std::string> present_hmi_types_in_policy;
  ON_CALL(mock_policy_handler_, GetInitialAppData(kAppId1, _, _))
      .WillByDefault(
          DoAll(SetArgPointee<2>(present_hmi_types_in_policy), Return(true)));

  auto response_to_mobile = CreateMessage();
  EXPECT_CALL(
      mock_rpc_service_,
      ManageMobileCommand(_, am::commands::Command::CommandSource::SOURCE_SDL))
      .WillOnce(DoAll(SaveArg<0>(&response_to_mobile), Return(true)))
      .WillOnce(Return(true));

  ASSERT_TRUE(command_->Init());
  command_->Run();

  const auto result_code = static_cast<mobile_apis::Result::eType>(
      (*response_to_mobile)[am::strings::msg_params][am::strings::result_code]
          .asUInt());
  EXPECT_EQ(mobile_apis::Result::SUCCESS, result_code);
}

TEST_F(
    RegisterAppInterfaceRequestTest,
    RegisterApp_AppHMITypeNotCoincidedWithCurrentNonEmptyDataStoredInPT_SuccessfulRegistrationWithWarning) {
  SetCommonPreconditionsToRegisterApplication();
  InitBasicMessage();
  (*msg_)[am::strings::msg_params][am::strings::app_hmi_type][0] =
      mobile_apis::AppHMIType::PROJECTION;
  (*msg_)[am::strings::msg_params][am::strings::app_hmi_type][1] =
      mobile_apis::AppHMIType::INFORMATION;
  (*msg_)[am::strings::msg_params][am::strings::app_hmi_type][2] =
      mobile_apis::AppHMIType::COMMUNICATION;
  (*msg_)[am::strings::msg_params][am::strings::app_hmi_type][3] =
      mobile_apis::AppHMIType::NAVIGATION;
  (*msg_)[am::strings::msg_params][am::strings::app_hmi_type][4] =
      mobile_apis::AppHMIType::MESSAGING;

  MockAppPtr mock_app = CreateBasicMockedApp();
  SetCommonPreconditionsToCheckWithPolicyData(mock_app);
  std::vector<std::string> present_hmi_types_in_policy{"NAVIGATION"};
  ON_CALL(mock_policy_handler_, GetInitialAppData(kAppId1, _, _))
      .WillByDefault(
          DoAll(SetArgPointee<2>(present_hmi_types_in_policy), Return(true)));

  auto response_to_mobile = CreateMessage();
  EXPECT_CALL(
      mock_rpc_service_,
      ManageMobileCommand(_, am::commands::Command::CommandSource::SOURCE_SDL))
      .WillOnce(DoAll(SaveArg<0>(&response_to_mobile), Return(true)))
      .WillOnce(Return(true));

  ASSERT_TRUE(command_->Init());
  command_->Run();

  const auto result_code = static_cast<mobile_apis::Result::eType>(
      (*response_to_mobile)[am::strings::msg_params][am::strings::result_code]
          .asUInt());
  EXPECT_EQ(mobile_apis::Result::WARNINGS, result_code);

  // Regarding to requairements, response info string should contain all
  // AppHMITypes that are not contained in the policy table.
  std::vector<std::string> expected_app_hmi_types_in_info_string{
      "PROJECTION", "INFORMATION", "COMMUNICATION", "MESSAGING"};

  const std::string info_string_in_response =
      (*response_to_mobile)[am::strings::msg_params][am::strings::info]
          .asString();

  for (const auto& app_hmi_type : expected_app_hmi_types_in_info_string) {
    EXPECT_TRUE(info_string_in_response.find(app_hmi_type) != std::string::npos)
        << "AppHMItype absent in the response string: " << app_hmi_type;
  }
}

TEST_F(RegisterAppInterfaceRequestTest,
       RegisterAppWithWebView_WebViewNotAllowedInPT_RejectRegistration) {
  SetCommonPreconditionsToRegisterApplication();
  InitBasicMessage();
  (*msg_)[am::strings::msg_params][am::strings::app_hmi_type][0] =
      mobile_apis::AppHMIType::WEB_VIEW;
  (*msg_)[am::strings::msg_params][am::strings::app_hmi_type][1] =
      mobile_apis::AppHMIType::INFORMATION;
  (*msg_)[am::strings::msg_params][am::strings::app_hmi_type][2] =
      mobile_apis::AppHMIType::COMMUNICATION;

  MockAppPtr mock_app = CreateBasicMockedApp();
  SetCommonPreconditionsToCheckWithPolicyData(mock_app);
  std::vector<std::string> present_hmi_types_in_policy{"INFORMATION",
                                                       "COMMUNICATION"};
  ON_CALL(mock_policy_handler_, GetInitialAppData(kAppId1, _, _))
      .WillByDefault(
          DoAll(SetArgPointee<2>(present_hmi_types_in_policy), Return(true)));

  EXPECT_CALL(*mock_app, set_webengine_projection_enabled(true)).Times(0);

  auto response_to_mobile = CreateMessage();
  EXPECT_CALL(
      mock_rpc_service_,
      ManageMobileCommand(_, am::commands::Command::CommandSource::SOURCE_SDL))
      .WillOnce(DoAll(SaveArg<0>(&response_to_mobile), Return(true)));

  ASSERT_TRUE(command_->Init());
  command_->Run();

  const auto result_code = static_cast<mobile_apis::Result::eType>(
      (*response_to_mobile)[am::strings::msg_params][am::strings::result_code]
          .asUInt());

  EXPECT_EQ(mobile_apis::Result::DISALLOWED, result_code);
}

TEST_F(RegisterAppInterfaceRequestTest,
       RegisterAppWithWebView_EmptyPT_RejectRegistration) {
  SetCommonPreconditionsToRegisterApplication();
  InitBasicMessage();
  (*msg_)[am::strings::msg_params][am::strings::app_hmi_type][0] =
      mobile_apis::AppHMIType::PROJECTION;
  (*msg_)[am::strings::msg_params][am::strings::app_hmi_type][1] =
      mobile_apis::AppHMIType::INFORMATION;
  (*msg_)[am::strings::msg_params][am::strings::app_hmi_type][2] =
      mobile_apis::AppHMIType::WEB_VIEW;
  (*msg_)[am::strings::msg_params][am::strings::app_hmi_type][3] =
      mobile_apis::AppHMIType::NAVIGATION;
  (*msg_)[am::strings::msg_params][am::strings::app_hmi_type][4] =
      mobile_apis::AppHMIType::MESSAGING;

  MockAppPtr mock_app = CreateBasicMockedApp();
  SetCommonPreconditionsToCheckWithPolicyData(mock_app);
  std::vector<std::string> present_hmi_types_in_policy;
  ON_CALL(mock_policy_handler_, GetInitialAppData(kAppId1, _, _))
      .WillByDefault(
          DoAll(SetArgPointee<2>(present_hmi_types_in_policy), Return(true)));

  auto response_to_mobile = CreateMessage();
  EXPECT_CALL(
      mock_rpc_service_,
      ManageMobileCommand(_, am::commands::Command::CommandSource::SOURCE_SDL))
      .WillOnce(DoAll(SaveArg<0>(&response_to_mobile), Return(true)));

  ASSERT_TRUE(command_->Init());
  command_->Run();

  const auto result_code = static_cast<mobile_apis::Result::eType>(
      (*response_to_mobile)[am::strings::msg_params][am::strings::result_code]
          .asUInt());
  EXPECT_EQ(mobile_apis::Result::DISALLOWED, result_code);
}

TEST_F(RegisterAppInterfaceRequestTest,
       RegisterAppWithWebViewOnly_WebViewNotAllowedInPT_RejectRegistration) {
  SetCommonPreconditionsToRegisterApplication();
  InitBasicMessage();
  (*msg_)[am::strings::msg_params][am::strings::app_hmi_type][0] =
      mobile_apis::AppHMIType::WEB_VIEW;

  MockAppPtr mock_app = CreateBasicMockedApp();
  SetCommonPreconditionsToCheckWithPolicyData(mock_app);
  std::vector<std::string> present_hmi_types_in_policy{"INFORMATION",
                                                       "COMMUNICATION"};
  ON_CALL(mock_policy_handler_, GetInitialAppData(kAppId1, _, _))
      .WillByDefault(
          DoAll(SetArgPointee<2>(present_hmi_types_in_policy), Return(true)));

  EXPECT_CALL(*mock_app, set_webengine_projection_enabled(true)).Times(0);

  auto response_to_mobile = CreateMessage();
  EXPECT_CALL(
      mock_rpc_service_,
      ManageMobileCommand(_, am::commands::Command::CommandSource::SOURCE_SDL))
      .WillOnce(DoAll(SaveArg<0>(&response_to_mobile), Return(true)));

  ASSERT_TRUE(command_->Init());
  command_->Run();

  const auto result_code = static_cast<mobile_apis::Result::eType>(
      (*response_to_mobile)[am::strings::msg_params][am::strings::result_code]
          .asUInt());

  EXPECT_EQ(mobile_apis::Result::DISALLOWED, result_code);
}

}  // namespace register_app_interface_request
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
