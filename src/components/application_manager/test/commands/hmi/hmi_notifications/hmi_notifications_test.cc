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
#include <vector>
#include "gtest/gtest.h"
#include "commands/command_request_test.h"

#include "utils/lock.h"
#include "utils/data_accessor.h"
#include "utils/signals.h"
#include "utils/shared_ptr.h"
#include "utils/make_shared.h"
#include "utils/file_system.h"
#include "smart_objects/smart_object.h"
#include "interfaces/HMI_API.h"
#include "application_manager/smart_object_keys.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/mock_state_controller.h"
#include "application_manager/mock_application.h"
#include "application_manager/mock_event_dispatcher.h"
#include "application_manager/hmi_capabilities_impl.h"
#include "application_manager/mock_hmi_capabilities.h"
#include "transport_manager/mock_transport_manager.h"
#include "connection_handler/mock_connection_handler.h"
#include "connection_handler/mock_connection_handler_settings.h"
#include "test/application_manager/mock_application_manager_settings.h"
#include "application_manager/mock_policy_handler_interface.h"
#include "application_manager/mock_message_helper.h"

#ifdef HMI_DBUS_API
#include "hmi/on_vi_acc_pedal_position_notification.h"
#include "hmi/on_vi_belt_status_notification.h"
#include "hmi/on_vi_body_information_notification.h"
#include "hmi/on_vi_device_status_notification.h"
#include "hmi/on_vi_driver_braking_notification.h"
#include "hmi/on_vi_engine_torque_notification.h"
#include "hmi/on_vi_external_temperature_notification.h"
#include "hmi/on_vi_fuel_level_notification.h"
#include "hmi/on_vi_fuel_level_state_notification.h"
#include "hmi/on_vi_gps_data_notification.h"
#include "hmi/on_vi_head_lamp_status_notification.h"
#include "hmi/on_vi_instant_fuel_consumption_notification.h"
#include "hmi/on_vi_my_key_notification.h"
#include "hmi/on_vi_odometer_notification.h"
#include "hmi/on_vi_prndl_notification.h"
#include "hmi/on_vi_rpm_notification.h"
#include "hmi/on_vi_speed_notification.h"
#include "hmi/on_vi_steering_wheel_angle_notification.h"
#include "hmi/on_vi_tire_pressure_notification.h"
#include "hmi/on_vi_vin_notification.h"
#include "hmi/on_vi_wiper_status_notification.h"
#else
#include "hmi/on_vi_vehicle_data_notification.h"
#endif  // HMI_DBUS_API

#include "hmi/on_button_event_notification.h"
#include "hmi/on_navi_tbt_client_state_notification.h"
#include "hmi/on_navi_way_point_change_notification.h"
#include "hmi/on_ui_command_notification.h"
#include "hmi/on_ui_keyboard_input_notification.h"
#include "hmi/on_ui_touch_event_notification.h"
#include "hmi/on_app_permission_changed_notification.h"
#include "hmi/on_app_registered_notification.h"
#include "hmi/on_audio_data_streaming_notification.h"
#include "hmi/on_button_subscription_notification.h"
#include "hmi/on_file_removed_notification.h"
#include "hmi/on_put_file_notification.h"
#include "hmi/on_resume_audio_source_notification.h"
#include "hmi/on_sdl_close_notification.h"
#include "hmi/on_sdl_consent_needed_notification.h"
#include "hmi/on_sdl_persistence_complete_notification.h"
#include "hmi/on_status_update_notification.h"
#include "hmi/on_video_data_streaming_notification.h"
#include "hmi/on_record_start_notification.h"
#include "hmi/on_app_activated_notification.h"
#include "hmi/on_app_deactivated_notification.h"
#include "hmi/on_app_unregistered_notification.h"
#include "hmi/on_button_press_notification.h"
#include "hmi/on_event_changed_notification.h"
#include "hmi/on_ready_notification.h"
#include "hmi/on_tts_reset_timeout_notification.h"
#include "hmi/on_tts_started_notification.h"
#include "hmi/on_tts_stopped_notification.h"
#include "hmi/on_ui_reset_timeout_notification.h"
#include "hmi/on_vr_started_notification.h"
#include "hmi/on_vr_stopped_notification.h"
#include "hmi/on_app_permission_consent_notification.h"
#include "hmi/on_ignition_cycle_over_notification.h"
#include "hmi/on_policy_update.h"
#include "hmi/on_received_policy_update.h"
#include "hmi/on_system_error_notification.h"
#include "hmi/on_system_info_changed_notification.h"
#include "hmi/on_allow_sdl_functionality_notification.h"
#include "hmi/on_device_state_changed_notification.h"
#include "hmi/on_exit_all_applications_notification.h"
#include "hmi/on_exit_application_notification.h"
#include "hmi/on_vr_command_notification.h"
#include "hmi/on_vr_language_change_notification.h"
#include "hmi/on_start_device_discovery.h"
#include "hmi/on_device_chosen_notification.h"
#include "hmi/on_system_context_notification.h"
#include "hmi/on_system_request_notification.h"
#include "hmi/on_tts_language_change_notification.h"
#include "hmi/on_ui_language_change_notification.h"
#include "hmi/on_device_connection_status_notification.h"

namespace test {
namespace components {
namespace commands_test {
namespace hmi_commands_test {
namespace hmi_notifications_test {

namespace am = application_manager;

using ::testing::_;
using ::testing::Test;
using ::testing::Types;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::NiceMock;
using ::utils::SharedPtr;
using ::smart_objects::SmartObject;
using testing::Mock;

namespace am = ::application_manager;
namespace strings = am::strings;

using am::commands::MessageSharedPtr;
using am::ApplicationSharedPtr;

using namespace am::commands;

ACTION_P(GetEventId, event_id) {
  *event_id = arg0.id();
}

ACTION_P(GetArg, arg) {
  *arg = arg0;
}

ACTION_P2(GetConnectIdPermissionConsent, connect_id, consent) {
  *connect_id = arg0;
  std::vector<policy::FunctionalGroupPermission>::const_iterator it =
      arg1.group_permissions.begin();
  for (; it != arg1.group_permissions.end(); ++it) {
    consent->group_permissions.push_back(*it);
  }
  consent->consent_source = arg1.consent_source;
}

ACTION_P2(GetBoolValueAndString, value, str) {
  *value = arg0;
  *str = arg1;
}

ACTION_P(GetMessage, message) {
  (*message) = (*arg0);
}

namespace {
const uint32_t kCountCommandsManageMobile_ = 22u;
const uint32_t kCountCommandsToHmi_ = 12u;
const int32_t kHmiProtocolType_ = 1;
const int32_t kMobileProtocolType_ = 0;
const int32_t kProtocolVersion_ = 3;
const uint32_t kCorrelationId_ = 1939u;
const uint32_t kAppId = 2014u;
const uint32_t kConnectionKey = 0u;
// Following variable represents MAC adress so it must contain 12 symbols.
const std::string kCustomDeviceInternalId = "255255255001";
const std::string kEmptyDeviceInternalId = "";
const std::string kDeviceId = "device_id";
}  // namespace

class HMICommandsNotificationsTest
    : public components::commands_test::CommandRequestTest<
          CommandsTestMocks::kIsNice> {
 public:
  HMICommandsNotificationsTest()
      : mock_app_(CreateMockApp())
      , message_helper_mock_(NULL)
      , message_(CreateMessage(smart_objects::SmartType_Map))
      , applications_(application_set_, applications_lock_) {}

  typedef Command CommandType;

  MockAppPtr mock_app_;
  am::MockMessageHelper* message_helper_mock_;

  MessageSharedPtr message_;
  am::ApplicationSet application_set_;
  sync_primitives::Lock applications_lock_;
  DataAccessor<am::ApplicationSet> applications_;
  NiceMock<application_manager_test::MockHMICapabilities>
      mock_hmi_capabilities_;

  typename IsNiceMock<policy_test::MockPolicyHandlerInterface,
                      kAreMocksNice>::Result mock_policy_handler_;

  typename IsNiceMock<application_manager_test::MockStateController,
                      kAreMocksNice>::Result mock_state_controller_;

 protected:
  void SetUp() OVERRIDE {
    message_helper_mock_ = am::MockMessageHelper::message_helper_mock();
    Mock::VerifyAndClearExpectations(&message_helper_mock_);
    ConfigureApp(kAppId, false, false, false);
    ON_CALL(mock_app_manager_, GetPolicyHandler())
        .WillByDefault(ReturnRef(mock_policy_handler_));
    ON_CALL(mock_app_manager_, application_by_hmi_app(_))
        .WillByDefault(Return(mock_app_));
  }

  void TearDown() OVERRIDE {
    Mock::VerifyAndClearExpectations(&message_helper_mock_);
    Mock::VerifyAndClearExpectations(&mock_app_manager_);
  }

  void ConfigureApp(uint32_t app_id, bool media, bool navi, bool vc) {
    ON_CALL(*mock_app_, app_id()).WillByDefault(Return(app_id));
    ON_CALL(*mock_app_, is_media()).WillByDefault(Return(media));
    ON_CALL(*mock_app_, is_navi()).WillByDefault(Return(navi));
    ON_CALL(*mock_app_, is_voice_communication()).WillByDefault(Return(vc));
    ON_CALL(*mock_app_, is_audio()).WillByDefault(Return(media || navi || vc));
  }
};

template <class Command>
class HMIOnViNotifications
    : public commands_test::CommandsTest<CommandsTestMocks::kIsNice> {
 public:
  typedef Command CommandType;
};

template <class Command>
class HMIOnNotificationsListToHMI
    : public commands_test::CommandsTest<CommandsTestMocks::kIsNice> {
 public:
  typedef Command CommandType;
};

template <class CommandT, hmi_apis::FunctionID::eType kCommandId>
struct CommandPair {
  typedef CommandT CommandType;
  enum { kHMICommandId = kCommandId };
};

template <class Command>
class HMIOnNotificationsEventDispatcher
    : public commands_test::CommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
  typedef Command CommandType;
};

typedef Types<
#ifdef HMI_DBUS_API
    OnVIAccPedalPositionNotification,
    OnVIBeltStatusNotification,
    OnVIBodyInformationNotification,
    OnVIDeviceStatusNotification,
    OnVIDriverBrakingNotification,
    OnVIEngineTorqueNotification,
    OnVIExternalTemperatureNotification,
    OnVIFuelLevelNotification,
    OnVIFuelLevelStateNotification,
    OnVIGpsDataNotification,
    OnVIHeadLampStatusNotification,
    OnVIInstantFuelConsumptionNotification,
    OnVIMyKeyNotification,
    OnVIOdometerNotification,
    OnVIPrndlNotification,
    OnVIRpmNotification,
    OnVISpeedNotification,
    OnVISteeringWheelAngleNotification,
    OnVITirePressureNotification,
    OnVIVinNotification,
    OnVIWiperStatusNotification
#else
    OnVIVehicleDataNotification
#endif  // HMI_DBUS_API
    > HMIOnViNotificationsTypes;

typedef Types<OnAppPermissionChangedNotification,
              OnAudioDataStreamingNotification,
              hmi::OnButtonSubscriptionNotification,
              OnFileRemovedNotification,
              OnPutFileNotification,
              OnResumeAudioSourceNotification,
              OnSDLCloseNotification,
              OnSDLConsentNeededNotification,
              OnSDLPersistenceCompleteNotification,
              OnStatusUpdateNotification,
              OnVideoDataStreamingNotification,
              OnRecordStartdNotification,
              OnAppRegisteredNotification> HMIOnNotificationsListToHMITypes;

typedef Types<
    CommandPair<OnAppActivatedNotification,
                hmi_apis::FunctionID::BasicCommunication_OnAppActivated>,
    CommandPair<OnAppDeactivatedNotification,
                hmi_apis::FunctionID::BasicCommunication_OnAppDeactivated>,
    CommandPair<OnEventChangedNotification,
                hmi_apis::FunctionID::BasicCommunication_OnEventChanged>,
    CommandPair<hmi::OnTTSResetTimeoutNotification,
                hmi_apis::FunctionID::TTS_OnResetTimeout>,
    CommandPair<OnTTSStartedNotification, hmi_apis::FunctionID::TTS_Started>,
    CommandPair<OnTTSStoppedNotification, hmi_apis::FunctionID::TTS_Stopped>,
    CommandPair<hmi::OnUIResetTimeoutNotification,
                hmi_apis::FunctionID::UI_OnResetTimeout>,
    CommandPair<OnVRStartedNotification, hmi_apis::FunctionID::VR_Started>,
    CommandPair<OnVRStoppedNotification, hmi_apis::FunctionID::VR_Stopped> >
    HMIOnNotificationsEventDispatcherTypes;

TYPED_TEST_CASE(HMIOnViNotifications, HMIOnViNotificationsTypes);
TYPED_TEST_CASE(HMIOnNotificationsListToHMI, HMIOnNotificationsListToHMITypes);
TYPED_TEST_CASE(HMIOnNotificationsEventDispatcher,
                HMIOnNotificationsEventDispatcherTypes);

TYPED_TEST(HMIOnViNotifications, CommandsSendNotificationToMobile) {
  MessageSharedPtr message = this->CreateMessage();
  utils::SharedPtr<typename TestFixture::CommandType> command =
      this->template CreateCommand<typename TestFixture::CommandType>(message);
  EXPECT_CALL(commands_test::CommandsTest<
                  CommandsTestMocks::kIsNice>::mock_app_manager_,
              ManageMobileCommand(_, Command::CommandOrigin::ORIGIN_SDL));
  command->Run();
  EXPECT_EQ(
      static_cast<int32_t>(mobile_apis::FunctionID::eType::OnVehicleDataID),
      (*message)[strings::params][strings::function_id].asInt());
  EXPECT_EQ(static_cast<int32_t>(am::MessageType::kNotification),
            (*message)[strings::params][strings::message_type].asInt());
}

TYPED_TEST(HMIOnNotificationsListToHMI, CommandsSendNotificationToHmi) {
  MessageSharedPtr message = this->CreateMessage();
  utils::SharedPtr<typename TestFixture::CommandType> command =
      this->template CreateCommand<typename TestFixture::CommandType>(message);
  EXPECT_CALL(commands_test::CommandsTest<
                  CommandsTestMocks::kIsNice>::mock_app_manager_,
              SendMessageToHMI(_));
  command->Run();
  EXPECT_EQ(static_cast<int32_t>(kHmiProtocolType_),
            (*message)[strings::params][strings::protocol_type].asInt());
  EXPECT_EQ(static_cast<int32_t>(kProtocolVersion_),
            (*message)[strings::params][strings::protocol_version].asInt());
}

TYPED_TEST(HMIOnNotificationsEventDispatcher,
           CommandsNotificationEventDispatcher) {
  int32_t event_id = hmi_apis::FunctionID::INVALID_ENUM;
  MessageSharedPtr message = this->CreateMessage();
  utils::SharedPtr<typename TestFixture::CommandType::CommandType> command =
      this->template CreateCommand<
          typename TestFixture::CommandType::CommandType>(message);
  EXPECT_CALL(commands_test::CommandsTest<
                  CommandsTestMocks::kIsNice>::mock_app_manager_,
              event_dispatcher())
      .WillOnce(ReturnRef(this->mock_event_dispatcher_));
  EXPECT_CALL(this->mock_event_dispatcher_, raise_event(_))
      .WillOnce(GetEventId(&event_id));
  command->Run();
  EXPECT_EQ(TestFixture::CommandType::kHMICommandId, event_id);
}

TEST_F(HMICommandsNotificationsTest, OnButtonEventSendNotificationToMobile) {
  utils::SharedPtr<Command> command =
      CreateCommand<hmi::OnButtonEventNotification>(message_);

  EXPECT_CALL(mock_app_manager_,
              ManageMobileCommand(_, Command::CommandOrigin::ORIGIN_SDL));
  command->Run();
  EXPECT_EQ(static_cast<int32_t>(mobile_apis::FunctionID::OnButtonEventID),
            (*message_)[strings::params][strings::function_id].asInt());
  EXPECT_EQ(static_cast<int32_t>(am::MessageType::kNotification),
            (*message_)[strings::params][strings::message_type].asInt());
}

TEST_F(HMICommandsNotificationsTest, OnNaviTBTClientSendNotificationToMobile) {
  utils::SharedPtr<Command> command =
      CreateCommand<OnNaviTBTClientStateNotification>(message_);

  EXPECT_CALL(mock_app_manager_,
              ManageMobileCommand(_, Command::CommandOrigin::ORIGIN_SDL));
  command->Run();
  EXPECT_EQ(static_cast<int32_t>(mobile_apis::FunctionID::OnTBTClientStateID),
            (*message_)[strings::params][strings::function_id].asInt());
  EXPECT_EQ(static_cast<int32_t>(am::MessageType::kNotification),
            (*message_)[strings::params][strings::message_type].asInt());
}

TEST_F(HMICommandsNotificationsTest,
       OnNaviWayPointChangeSendNotificationToMobile) {
  utils::SharedPtr<Command> command =
      CreateCommand<OnNaviWayPointChangeNotification>(message_);

  EXPECT_CALL(mock_app_manager_,
              ManageMobileCommand(_, Command::CommandOrigin::ORIGIN_SDL));
  command->Run();
  EXPECT_EQ(static_cast<int32_t>(mobile_apis::FunctionID::OnWayPointChangeID),
            (*message_)[strings::params][strings::function_id].asInt());
  EXPECT_EQ(static_cast<int32_t>(am::MessageType::kNotification),
            (*message_)[strings::params][strings::message_type].asInt());
}

TEST_F(HMICommandsNotificationsTest, OnUICommandSendNotificationToMobile) {
  utils::SharedPtr<Command> command =
      CreateCommand<OnUICommandNotification>(message_);

  EXPECT_CALL(mock_app_manager_,
              ManageMobileCommand(_, Command::CommandOrigin::ORIGIN_SDL));
  command->Run();
  EXPECT_EQ(static_cast<int32_t>(mobile_apis::FunctionID::eType::OnCommandID),
            (*message_)[strings::params][strings::function_id].asInt());
  EXPECT_EQ(static_cast<int32_t>(mobile_apis::TriggerSource::TS_MENU),
            (*message_)[strings::msg_params][strings::trigger_source].asInt());
  EXPECT_EQ(static_cast<int32_t>(am::MessageType::kNotification),
            (*message_)[strings::params][strings::message_type].asInt());
}

TEST_F(HMICommandsNotificationsTest,
       OnUIKeyBoardInputSendNotificationToMobile) {
  utils::SharedPtr<Command> command =
      CreateCommand<hmi::OnUIKeyBoardInputNotification>(message_);

  EXPECT_CALL(mock_app_manager_,
              ManageMobileCommand(_, Command::CommandOrigin::ORIGIN_SDL));
  command->Run();
  EXPECT_EQ(static_cast<int32_t>(mobile_apis::FunctionID::OnKeyboardInputID),
            (*message_)[strings::params][strings::function_id].asInt());
  EXPECT_EQ(static_cast<int32_t>(am::MessageType::kNotification),
            (*message_)[strings::params][strings::message_type].asInt());
}

TEST_F(HMICommandsNotificationsTest, OnUITouchEventSendNotificationToMobile) {
  utils::SharedPtr<Command> command =
      CreateCommand<hmi::OnUITouchEventNotification>(message_);

  EXPECT_CALL(mock_app_manager_,
              ManageMobileCommand(_, Command::CommandOrigin::ORIGIN_SDL));
  command->Run();
  EXPECT_EQ(static_cast<int32_t>(mobile_apis::FunctionID::OnTouchEventID),
            (*message_)[strings::params][strings::function_id].asInt());
  EXPECT_EQ(static_cast<int32_t>(am::MessageType::kNotification),
            (*message_)[strings::params][strings::message_type].asInt());
}

TEST_F(HMICommandsNotificationsTest,
       OnAppUnregisteredNotificationEventDispatcher) {
  int32_t event_id = hmi_apis::FunctionID::INVALID_ENUM;

  utils::SharedPtr<Command> command =
      CreateCommand<OnAppUnregisteredNotification>(message_);

  EXPECT_CALL(mock_app_manager_, SendMessageToHMI(_));
  EXPECT_CALL(mock_app_manager_, event_dispatcher());
  EXPECT_CALL(mock_event_dispatcher_, raise_event(_))
      .WillOnce(GetEventId(&event_id));
  command->Run();
  EXPECT_EQ(static_cast<int32_t>(
                hmi_apis::FunctionID::BasicCommunication_OnAppUnregistered),
            event_id);
  EXPECT_EQ(kHmiProtocolType_,
            (*message_)[strings::params][strings::protocol_type].asInt());
  EXPECT_EQ(kProtocolVersion_,
            (*message_)[strings::params][strings::protocol_version].asInt());
}

TEST_F(HMICommandsNotificationsTest, OnButtonPressNotificationEventDispatcher) {
  int32_t event_id = hmi_apis::FunctionID::INVALID_ENUM;

  utils::SharedPtr<Command> command =
      CreateCommand<hmi::OnButtonPressNotification>(message_);

  EXPECT_CALL(mock_app_manager_,
              ManageMobileCommand(_, Command::CommandOrigin::ORIGIN_SDL));
  EXPECT_CALL(mock_app_manager_, event_dispatcher());
  EXPECT_CALL(mock_event_dispatcher_, raise_event(_))
      .WillOnce(GetEventId(&event_id));
  command->Run();
  EXPECT_EQ(static_cast<int32_t>(hmi_apis::FunctionID::Buttons_OnButtonPress),
            event_id);
  EXPECT_EQ(static_cast<int>(mobile_apis::FunctionID::eType::OnButtonPressID),
            (*message_)[strings::params][strings::function_id].asInt());
}

TEST_F(HMICommandsNotificationsTest, OnReadyNotificationEventDispatcher) {
  int32_t event_id = hmi_apis::FunctionID::INVALID_ENUM;

  utils::SharedPtr<Command> command =
      CreateCommand<OnReadyNotification>(message_);

  EXPECT_CALL(mock_app_manager_, OnHMIStartedCooperation());
  EXPECT_CALL(mock_app_manager_, event_dispatcher());
  EXPECT_CALL(mock_event_dispatcher_, raise_event(_))
      .WillOnce(GetEventId(&event_id));
  command->Run();
  EXPECT_EQ(hmi_apis::FunctionID::BasicCommunication_OnReady, event_id);
}

TEST_F(HMICommandsNotificationsTest,
       OnIgnitionCycleOverNotificationPolicyHandler) {
  utils::SharedPtr<Command> command =
      CreateCommand<OnIgnitionCycleOverNotification>(message_);

  EXPECT_CALL(mock_policy_handler_, OnIgnitionCycleOver());
  command->Run();
}

TEST_F(HMICommandsNotificationsTest, OnPolicyUpdateNotificationPolicyHandler) {
  utils::SharedPtr<Command> command = CreateCommand<OnPolicyUpdate>(message_);

  EXPECT_CALL(mock_policy_handler_, OnPTExchangeNeeded());
  command->Run();
}

TEST_F(HMICommandsNotificationsTest,
       OnReceivePolicyUpdateNotification_SUCCESS) {
  const std::string kFile = "./test_file.txt";
  EXPECT_TRUE(file_system::CreateFile(kFile));
  uint8_t tmp[] = {1u, 2u, 3u, 4u};
  std::vector<uint8_t> data(tmp, tmp + 4);
  EXPECT_TRUE(file_system::WriteBinaryFile(kFile, data));

  (*message_)[strings::msg_params][am::hmi_notification::policyfile] = kFile;
  utils::SharedPtr<Command> command =
      CreateCommand<OnReceivedPolicyUpdate>(message_);

  EXPECT_CALL(mock_policy_handler_, ReceiveMessageFromSDK(kFile, data));
  command->Run();
  EXPECT_TRUE(file_system::DeleteFile(kFile));
}

TEST_F(HMICommandsNotificationsTest,
       OnReceivePolicyUpdateNotification_UNSUCCESS) {
  utils::SharedPtr<Command> command =
      CreateCommand<OnReceivedPolicyUpdate>(message_);

  EXPECT_CALL(mock_app_manager_, GetPolicyHandler()).Times(0);
  EXPECT_CALL(mock_policy_handler_, ReceiveMessageFromSDK(_, _)).Times(0);
  command->Run();
}

TEST_F(HMICommandsNotificationsTest,
       OnAppPermissionConsentNotificationPolicyHandlerNoAppId) {
  (*message_)[strings::msg_params]["consentedFunctions"] =
      smart_objects::SmartObject(smart_objects::SmartType_Array);
  SmartObject& applications =
      (*message_)[strings::msg_params]["consentedFunctions"];

  smart_objects::SmartObject hmi_application_temp(smart_objects::SmartType_Map);
  applications[0] = hmi_application_temp;

  utils::SharedPtr<Command> command =
      CreateCommand<OnAppPermissionConsentNotification>(message_);
  EXPECT_CALL(mock_policy_handler_,
              OnAppPermissionConsent(kConnectionKey, _, _));
  command->Run();
}

TEST_F(HMICommandsNotificationsTest,
       OnAppPermissionConsentNotificationPolicyHandlerWithAppId) {
  (*message_)[strings::msg_params][strings::app_id] = kAppId;
  (*message_)[strings::msg_params]["consentedFunctions"] =
      smart_objects::SmartObject(smart_objects::SmartType_Array);

  smart_objects::SmartObjectSPtr consented_function =
      utils::MakeShared<smart_objects::SmartObject>();
  (*message_)[strings::msg_params]["consentedFunctions"][0] =
      *consented_function;

  utils::SharedPtr<Command> command =
      CreateCommand<OnAppPermissionConsentNotification>(message_);

  uint32_t connection_id = 0u;
  policy::PermissionConsent permission_consent;
  EXPECT_CALL(mock_policy_handler_, OnAppPermissionConsent(_, _, _))
      .WillOnce(
          GetConnectIdPermissionConsent(&connection_id, &permission_consent));
  command->Run();
  EXPECT_EQ(kAppId, connection_id);

  std::vector<policy::FunctionalGroupPermission>::const_iterator it =
      permission_consent.group_permissions.begin();

  for (; it != permission_consent.group_permissions.end(); ++it) {
    EXPECT_EQ(policy::kGroupUndefined, (*it).state);
  }
}

TEST_F(HMICommandsNotificationsTest,
       OnAppPermissionConsentNotificationPolicyHandlerAppIdAllowTrue) {
  (*message_)[strings::msg_params]["consentedFunctions"] =
      smart_objects::SmartObject(smart_objects::SmartType_Array);
  (*message_)[strings::msg_params][strings::app_id] = kAppId;
  (*message_)[strings::msg_params]["source"] = "test_content_source";

  smart_objects::SmartObjectSPtr consented_function =
      utils::MakeShared<smart_objects::SmartObject>();
  (*consented_function)["allowed"] = true;
  (*consented_function)[strings::id] = 999;
  (*consented_function)[strings::name] = "test_group_alias";
  (*message_)[strings::msg_params]["consentedFunctions"][0] =
      *consented_function;

  utils::SharedPtr<Command> command =
      CreateCommand<OnAppPermissionConsentNotification>(message_);

  uint32_t connection_id = 0u;
  policy::PermissionConsent permission_consent;
  EXPECT_CALL(mock_policy_handler_, OnAppPermissionConsent(_, _, _))
      .WillOnce(
          GetConnectIdPermissionConsent(&connection_id, &permission_consent));
  command->Run();
  EXPECT_EQ(kAppId, connection_id);

  std::vector<policy::FunctionalGroupPermission>::const_iterator it =
      permission_consent.group_permissions.begin();
  for (; it != permission_consent.group_permissions.end(); ++it) {
    EXPECT_EQ(999, (*it).group_id);
    EXPECT_EQ("test_group_alias", (*it).group_alias);
    EXPECT_EQ(policy::kGroupAllowed, (*it).state);
  }
  EXPECT_EQ("test_content_source", permission_consent.consent_source);
}

TEST_F(HMICommandsNotificationsTest,
       OnAppPermissionConsentNotificationPolicyHandlerAppIdAllowFalse) {
  (*message_)[strings::msg_params]["consentedFunctions"] =
      smart_objects::SmartObject(smart_objects::SmartType_Array);
  (*message_)[strings::msg_params][strings::app_id] = kAppId;
  (*message_)[strings::msg_params]["source"] = "test_content_source";

  smart_objects::SmartObjectSPtr consented_function =
      utils::MakeShared<smart_objects::SmartObject>();
  (*consented_function)["allowed"] = false;
  (*consented_function)[strings::id] = 999;
  (*consented_function)[strings::name] = "test_group_alias";
  (*message_)[strings::msg_params]["consentedFunctions"][0] =
      *consented_function;

  utils::SharedPtr<Command> command =
      CreateCommand<OnAppPermissionConsentNotification>(message_);

  uint32_t connection_id = 0u;
  policy::PermissionConsent permission_consent;
  EXPECT_CALL(mock_policy_handler_, OnAppPermissionConsent(_, _, _))
      .WillOnce(
          GetConnectIdPermissionConsent(&connection_id, &permission_consent));
  command->Run();
  EXPECT_EQ(kAppId, connection_id);

  std::vector<policy::FunctionalGroupPermission>::const_iterator it =
      permission_consent.group_permissions.begin();
  for (; it != permission_consent.group_permissions.end(); ++it) {
    EXPECT_EQ(999, (*it).group_id);
    EXPECT_EQ("test_group_alias", (*it).group_alias);
    EXPECT_EQ(policy::kGroupDisallowed, (*it).state);
  }
  EXPECT_EQ("test_content_source", permission_consent.consent_source);
}

TEST_F(HMICommandsNotificationsTest,
       OnSystemErrorNotificationOnSystemErrorCode) {
  (*message_)[strings::msg_params][am::hmi_notification::error] =
      hmi_apis::Common_SystemError::SYNC_REBOOTED;

  utils::SharedPtr<Command> command =
      CreateCommand<OnSystemErrorNotification>(message_);

  int32_t code = hmi_apis::Common_SystemError::INVALID_ENUM;
  EXPECT_CALL(mock_policy_handler_, OnSystemError(_)).WillOnce(GetArg(&code));
  command->Run();
  EXPECT_EQ(static_cast<int32_t>(hmi_apis::Common_SystemError::SYNC_REBOOTED),
            code);
}

TEST_F(HMICommandsNotificationsTest,
       OnSystemInfoChangedNotificationCheckLanguage) {
  const uint32_t kLangCode = 5u;

  (*message_)[strings::msg_params][strings::language] = kLangCode;

  utils::SharedPtr<Command> command =
      CreateCommand<OnSystemInfoChangedNotification>(message_);

  EXPECT_CALL(*message_helper_mock_, CommonLanguageToString(_));
  EXPECT_CALL(mock_policy_handler_, OnSystemInfoChanged(_));
  command->Run();
}

TEST_F(HMICommandsNotificationsTest,
       OnAllowSDLFunctionalityNotificationDeviceKeyNotExist) {
  (*message_)[strings::msg_params][am::hmi_response::allowed] = true;
  utils::SharedPtr<Command> command =
      CreateCommand<OnAllowSDLFunctionalityNotification>(message_);

  bool value = false;
  std::string str;
  EXPECT_CALL(mock_policy_handler_, OnAllowSDLFunctionalityNotification(_, _))
      .WillOnce(GetBoolValueAndString(&value, &str));
  command->Run();
  EXPECT_EQ(true, value);
  EXPECT_EQ(kEmptyDeviceInternalId, str);
}

TEST_F(HMICommandsNotificationsTest,
       OnAllowSDLFunctionalityNotificationDeviceKeyExist) {
  (*message_)[strings::msg_params][am::hmi_response::allowed] = true;
  (*message_)[strings::msg_params][strings::device][strings::id] = kDeviceId;
  utils::SharedPtr<Command> command =
      CreateCommand<OnAllowSDLFunctionalityNotification>(message_);

  bool value;
  std::string str;
  EXPECT_CALL(mock_policy_handler_, OnAllowSDLFunctionalityNotification(_, _))
      .WillOnce(GetBoolValueAndString(&value, &str));
  command->Run();
  EXPECT_EQ(true, value);
  EXPECT_EQ(kDeviceId, str);
}

TEST_F(HMICommandsNotificationsTest,
       OnDeviceStateChangedNotificationDeviceStateNotUnpaired) {
  (*message_)[strings::msg_params]["deviceState"] =
      hmi_apis::Common_DeviceState::INVALID_ENUM;
  utils::SharedPtr<Command> command =
      CreateCommand<OnDeviceStateChangedNotification>(message_);

  EXPECT_CALL(mock_policy_handler_, RemoveDevice(_)).Times(0);
  command->Run();
}

TEST_F(HMICommandsNotificationsTest,
       OnDeviceStateChangedNotificationDeviceStateUnpaired) {
  (*message_)[strings::msg_params]["deviceState"] =
      hmi_apis::Common_DeviceState::UNPAIRED;
  (*message_)[strings::msg_params]["deviceInternalId"] =
      kCustomDeviceInternalId;

  utils::SharedPtr<Command> command =
      CreateCommand<OnDeviceStateChangedNotification>(message_);

  // If the "deviceInternalId" key is valid the MAC address will be hashed
  EXPECT_CALL(mock_policy_handler_, RemoveDevice(_));
  command->Run();
}

TEST_F(HMICommandsNotificationsTest,
       OnDeviceStateChangedNotificationDeviceStateEmptyDeviceId) {
  (*message_)[strings::msg_params]["deviceState"] =
      hmi_apis::Common_DeviceState::UNPAIRED;
  (*message_)[strings::msg_params]["deviceInternalId"] = kEmptyDeviceInternalId;

  utils::SharedPtr<Command> command =
      CreateCommand<OnDeviceStateChangedNotification>(message_);

  std::string result_device_id = "default_id";
  EXPECT_CALL(mock_policy_handler_, RemoveDevice(_))
      .WillOnce(GetArg(&result_device_id));
  command->Run();
  EXPECT_EQ(kEmptyDeviceInternalId, result_device_id);
}

TEST_F(HMICommandsNotificationsTest,
       OnDeviceStateChangedNotificationDeviceStateDeviceIdFromId) {
  MessageSharedPtr message = CreateMessage();
  (*message)[strings::msg_params]["deviceState"] =
      hmi_apis::Common_DeviceState::UNPAIRED;
  (*message)[strings::msg_params]["deviceInternalId"] = kEmptyDeviceInternalId;
  (*message)[strings::msg_params]["deviceId"][strings::id] = kDeviceId;

  utils::SharedPtr<Command> command =
      CreateCommand<OnDeviceStateChangedNotification>(message);

  std::string result_device_id = "default_id";
  EXPECT_CALL(mock_policy_handler_, RemoveDevice(_))
      .WillOnce(GetArg(&result_device_id));
  command->Run();
  EXPECT_EQ(kDeviceId, result_device_id);
}

MATCHER_P(ReasonCodeEqualTo, kCode, "") {
  return kCode == arg;
}

void DummySigHandler(int) {
  return;
}

TEST_F(HMICommandsNotificationsTest,
       OnExitAllApplicationsNotificationReasonIgnitionOff) {
  (*message_)[strings::msg_params][am::hmi_request::reason] =
      hmi_apis::Common_ApplicationsCloseReason::IGNITION_OFF;

  utils::SharedPtr<am::commands::Command> command =
      CreateCommand<am::commands::OnExitAllApplicationsNotification>(message_);

  EXPECT_CALL(
      mock_app_manager_,
      SetUnregisterAllApplicationsReason(ReasonCodeEqualTo(
          am::mobile_api::AppInterfaceUnregisteredReason::IGNITION_OFF)));
  EXPECT_CALL(mock_app_manager_, HeadUnitReset(_)).Times(0);

  utils::UnsibscribeFromTermination();
  command->Run();
  utils::WaitTerminationSignals(&DummySigHandler);

  EXPECT_CALL(mock_app_manager_, SetUnregisterAllApplicationsReason(_))
      .Times(0);
}

TEST_F(HMICommandsNotificationsTest,
       OnExitAllApplicationsNotificationReasonMasterResetAndFactoryDefaults) {
  std::vector<hmi_apis::Common_ApplicationsCloseReason::eType> reason_list;
  reason_list.push_back(hmi_apis::Common_ApplicationsCloseReason::MASTER_RESET);
  reason_list.push_back(
      hmi_apis::Common_ApplicationsCloseReason::FACTORY_DEFAULTS);

  std::vector<mobile_apis::AppInterfaceUnregisteredReason::eType>
      mob_reason_list;
  mob_reason_list.push_back(
      mobile_apis::AppInterfaceUnregisteredReason::MASTER_RESET);
  mob_reason_list.push_back(
      mobile_apis::AppInterfaceUnregisteredReason::FACTORY_DEFAULTS);

  std::vector<hmi_apis::Common_ApplicationsCloseReason::eType>::iterator
      it_reason = reason_list.begin();
  std::vector<mobile_apis::AppInterfaceUnregisteredReason::eType>::iterator
      it_mob_reason = mob_reason_list.begin();

  for (; it_reason != reason_list.end(); ++it_reason, ++it_mob_reason) {
    (*message_)[strings::msg_params][am::hmi_request::reason] = *it_reason;

    utils::SharedPtr<am::commands::Command> command =
        CreateCommand<am::commands::OnExitAllApplicationsNotification>(
            message_);

    am::mobile_api::AppInterfaceUnregisteredReason::eType mob_reason =
        *it_mob_reason;

    EXPECT_CALL(mock_app_manager_,
                SetUnregisterAllApplicationsReason(mob_reason));
    EXPECT_CALL(mock_app_manager_, HeadUnitReset(mob_reason));
    utils::UnsibscribeFromTermination();
    command->Run();
    utils::WaitTerminationSignals(&DummySigHandler);
    EXPECT_CALL(mock_app_manager_, SetUnregisterAllApplicationsReason(_))
        .Times(0);
    EXPECT_CALL(mock_app_manager_, HeadUnitReset(_)).Times(0);
  }
}

TEST_F(HMICommandsNotificationsTest,
       OnExitAllApplicationsNotificationReasonSuspend) {
  (*message_)[strings::msg_params][am::hmi_request::reason] =
      hmi_apis::Common_ApplicationsCloseReason::SUSPEND;

  utils::SharedPtr<am::commands::Command> command =
      CreateCommand<am::commands::OnExitAllApplicationsNotification>(message_);

  MessageSharedPtr ethalon_message =
      CreateMessage(smart_objects::SmartType_Map);
  (*ethalon_message)[strings::params][strings::function_id] =
      hmi_apis::FunctionID::BasicCommunication_OnSDLPersistenceComplete;
  (*ethalon_message)[strings::params][strings::message_type] =
      am::MessageType::kNotification;
  (*ethalon_message)[strings::params][strings::correlation_id] =
      kCorrelationId_;
  MessageSharedPtr temp_message = CreateMessage();

  EXPECT_CALL(mock_app_manager_, GetNextHMICorrelationID())
      .WillOnce(Return(kCorrelationId_));
  EXPECT_CALL(mock_app_manager_, ManageHMICommand(_))
      .WillOnce(DoAll(GetMessage(temp_message), Return(true)));
  command->Run();
  EXPECT_CALL(mock_app_manager_, GetNextHMICorrelationID()).Times(0);
  EXPECT_CALL(mock_app_manager_, ManageHMICommand(_)).Times(0);
  EXPECT_EQ((*temp_message)[strings::params][strings::function_id].asInt(),
            hmi_apis::FunctionID::BasicCommunication_OnSDLPersistenceComplete);
  EXPECT_EQ((*temp_message)[strings::params][strings::message_type].asInt(),
            am::MessageType::kNotification);
  EXPECT_EQ((*temp_message)[strings::params][strings::correlation_id].asInt(),
            kCorrelationId_);
}

TEST_F(HMICommandsNotificationsTest,
       OnExitAllApplicationsNotificationReasonInvalidEnum) {
  (*message_)[strings::msg_params][am::hmi_request::reason] =
      hmi_apis::Common_ApplicationsCloseReason::INVALID_ENUM;

  utils::SharedPtr<am::commands::Command> command =
      CreateCommand<am::commands::OnExitAllApplicationsNotification>(message_);

  EXPECT_CALL(mock_app_manager_, SetUnregisterAllApplicationsReason(_))
      .Times(0);
  EXPECT_CALL(mock_app_manager_, HeadUnitReset(_)).Times(0);
  EXPECT_CALL(mock_app_manager_, GetNextHMICorrelationID()).Times(0);
  EXPECT_CALL(mock_app_manager_, ManageHMICommand(_)).Times(0);
  command->Run();
}

TEST_F(HMICommandsNotificationsTest,
       OnExitApplicationNotificationManageMobileCommand) {
  (*message_)[strings::msg_params][strings::app_id] = kAppId;
  smart_objects::SmartObjectSPtr notification =
      utils::MakeShared<smart_objects::SmartObject>();
  (*notification)[strings::params][strings::function_id] =

      mobile_apis::FunctionID::OnAppInterfaceUnregisteredID;
  (*notification)[strings::params][strings::message_type] =
      am::MessageType::kNotification;
  (*notification)[strings::params][strings::connection_key] = kAppId;

  std::vector<hmi_apis::Common_ApplicationExitReason::eType> reason_list;
  reason_list.push_back(hmi_apis::Common_ApplicationExitReason::
                            UNAUTHORIZED_TRANSPORT_REGISTRATION);
  reason_list.push_back(
      hmi_apis::Common_ApplicationExitReason::UNSUPPORTED_HMI_RESOURCE);

  std::vector<mobile_apis::AppInterfaceUnregisteredReason::eType>
      mobile_reason_list;
  mobile_reason_list.push_back(
      mobile_apis::AppInterfaceUnregisteredReason::APP_UNAUTHORIZED);
  mobile_reason_list.push_back(
      mobile_apis::AppInterfaceUnregisteredReason::UNSUPPORTED_HMI_RESOURCE);

  std::vector<mobile_apis::AppInterfaceUnregisteredReason::eType>::iterator
      it_mobile_reason = mobile_reason_list.begin();
  std::vector<hmi_apis::Common_ApplicationExitReason::eType>::iterator
      it_reason = reason_list.begin();

  for (; it_reason != reason_list.end(); ++it_reason, ++it_mobile_reason) {
    (*message_)[strings::msg_params][strings::reason] = *it_reason;
    utils::SharedPtr<Command> command =
        CreateCommand<OnExitApplicationNotification>(message_);

    (*notification)[strings::msg_params][strings::reason] =
        static_cast<int32_t>(*it_mobile_reason);

    EXPECT_CALL(mock_app_manager_, application(kAppId))
        .WillRepeatedly(Return(mock_app_));
    EXPECT_CALL(*message_helper_mock_,
                GetOnAppInterfaceUnregisteredNotificationToMobile(
                    kAppId, *it_mobile_reason)).WillOnce(Return(notification));
    EXPECT_CALL(mock_app_manager_,
                ManageMobileCommand(notification, Command::ORIGIN_SDL));
    EXPECT_CALL(mock_app_manager_,
                UnregisterApplication(
                    kAppId, mobile_apis::Result::SUCCESS, false, false));
    command->Run();
  }
}

TEST_F(HMICommandsNotificationsTest,
       OnExitApplicationNotificationUnhandledReason) {
  (*message_)[strings::msg_params][strings::app_id] = kAppId;

  (*message_)[strings::msg_params][strings::reason] =
      hmi_apis::Common_ApplicationExitReason::INVALID_ENUM;

  utils::SharedPtr<Command> command =
      CreateCommand<OnExitApplicationNotification>(message_);

  EXPECT_CALL(mock_app_manager_, application(_)).Times(0);
  EXPECT_CALL(mock_app_manager_, ManageMobileCommand(_, _)).Times(0);
  EXPECT_CALL(mock_app_manager_, UnregisterApplication(_, _, _, _)).Times(0);
  EXPECT_CALL(mock_app_manager_, state_controller()).Times(0);
  EXPECT_CALL(mock_app_manager_, application(kAppId))
      .WillOnce(Return(mock_app_));
  command->Run();
}

TEST_F(HMICommandsNotificationsTest, OnExitApplicationNotificationInvalidApp) {
  (*message_)[strings::msg_params][strings::app_id] = kAppId;

  utils::SharedPtr<Command> command =
      CreateCommand<OnExitApplicationNotification>(message_);

  am::ApplicationSharedPtr invalid_app;
  EXPECT_CALL(mock_app_manager_, application(_)).Times(0);
  EXPECT_CALL(mock_app_manager_, ManageMobileCommand(_, _)).Times(0);
  EXPECT_CALL(mock_app_manager_, UnregisterApplication(_, _, _, _)).Times(0);
  EXPECT_CALL(mock_app_manager_, state_controller()).Times(0);
  EXPECT_CALL(mock_app_manager_, application(kAppId))
      .WillOnce(Return(invalid_app));
  command->Run();
}

TEST_F(HMICommandsNotificationsTest, Run_InvalidApp_UNSUCCESS) {
  (*message_)[strings::msg_params][strings::app_id] = kAppId;
  (*message_)[strings::msg_params][strings::reason] =
      hmi_apis::Common_ApplicationExitReason::DRIVER_DISTRACTION_VIOLATION;
  utils::SharedPtr<Command> command =
      CreateCommand<OnExitApplicationNotification>(message_);

  MockAppPtr valid_app = CreateMockApp();
  EXPECT_CALL(mock_app_manager_, application(kAppId))
      .WillOnce(Return(valid_app))
      .WillOnce(Return(MockAppPtr()));
  EXPECT_CALL(mock_app_manager_, ManageMobileCommand(_, _)).Times(0);
  EXPECT_CALL(mock_app_manager_, UnregisterApplication(_, _, _, _)).Times(0);
  EXPECT_CALL(mock_app_manager_, state_controller()).Times(0);

  command->Run();
}

TEST_F(HMICommandsNotificationsTest,
       OnExitApplicationNotificationDriverDistractionValidApp) {
  (*message_)[strings::msg_params][strings::app_id] = kAppId;
  (*message_)[strings::msg_params][strings::reason] =
      hmi_apis::Common_ApplicationExitReason::DRIVER_DISTRACTION_VIOLATION;
  utils::SharedPtr<Command> command =
      CreateCommand<OnExitApplicationNotification>(message_);

  smart_objects::SmartObjectSPtr notification =
      utils::MakeShared<smart_objects::SmartObject>();
  (*notification)[strings::params][strings::function_id] =

      mobile_apis::FunctionID::OnAppInterfaceUnregisteredID;
  (*notification)[strings::params][strings::message_type] =
      am::MessageType::kNotification;
  (*notification)[strings::params][strings::connection_key] = kAppId;
  (*notification)[strings::msg_params][strings::reason] =
      mobile_apis::AppInterfaceUnregisteredReason::DRIVER_DISTRACTION_VIOLATION;

  EXPECT_CALL(mock_app_manager_, application(kAppId))
      .WillRepeatedly(Return(mock_app_));

  EXPECT_CALL(mock_app_manager_, state_controller())
      .WillOnce(ReturnRef(mock_state_controller_));
  EXPECT_CALL(mock_state_controller_,
              SetRegularState(static_cast<am::ApplicationSharedPtr>(mock_app_),
                              mobile_apis::HMILevel::HMI_NONE,
                              mobile_apis::AudioStreamingState::NOT_AUDIBLE,
                              false));
  command->Run();
}

TEST_F(HMICommandsNotificationsTest,
       OnExitApplicationNotificationDriverDistractionInvalidApp) {
  (*message_)[strings::msg_params][strings::app_id] = kAppId;

  utils::SharedPtr<Command> command =
      CreateCommand<OnExitApplicationNotification>(message_);

  EXPECT_CALL(mock_app_manager_, application(kAppId))
      .WillRepeatedly(Return(am::ApplicationSharedPtr()));

  EXPECT_CALL(mock_app_manager_, ManageMobileCommand(_, _)).Times(0);

  command->Run();
}

TEST_F(HMICommandsNotificationsTest,
       OnExitApplicationNotificationUserExitValidApp) {
  (*message_)[strings::msg_params][strings::app_id] = kAppId;
  (*message_)[strings::msg_params][strings::reason] =
      hmi_apis::Common_ApplicationExitReason::USER_EXIT;
  utils::SharedPtr<Command> command =
      CreateCommand<OnExitApplicationNotification>(message_);

  EXPECT_CALL(mock_app_manager_, application(kAppId))
      .WillRepeatedly(Return(mock_app_));
  EXPECT_CALL(mock_app_manager_, ManageMobileCommand(_, _)).Times(0);
  EXPECT_CALL(mock_app_manager_, UnregisterApplication(_, _, _, _)).Times(0);
  EXPECT_CALL(mock_app_manager_, state_controller())
      .WillOnce(ReturnRef(mock_state_controller_));
  EXPECT_CALL(mock_state_controller_,
              SetRegularState(static_cast<am::ApplicationSharedPtr>(mock_app_),
                              mobile_apis::HMILevel::HMI_NONE,
                              mobile_apis::AudioStreamingState::NOT_AUDIBLE,
                              false));
  command->Run();
}

TEST_F(HMICommandsNotificationsTest,
       OnVRCommandNotificationSwitchedAndValidApp) {
  const uint32_t cmnd_id = 12u;
  const uint32_t max_cmd_id = 10u;

  (*message_)[strings::msg_params][strings::cmd_id] = cmnd_id;
  utils::SharedPtr<Command> command =
      CreateCommand<OnVRCommandNotification>(message_);

  EXPECT_CALL(mock_app_manager_, application(cmnd_id - max_cmd_id))
      .WillOnce(Return(mock_app_));

  EXPECT_CALL(mock_app_manager_, get_settings())
      .WillOnce(ReturnRef(mock_app_manager_settings_));
  EXPECT_CALL(mock_app_manager_settings_, max_cmd_id())
      .WillOnce(ReturnRef(max_cmd_id));

  EXPECT_CALL(mock_app_manager_, state_controller())
      .WillOnce(ReturnRef(mock_state_controller_));
  EXPECT_CALL(mock_state_controller_,
              SetRegularState(static_cast<am::ApplicationSharedPtr>(mock_app_),
                              mobile_apis::HMILevel::HMI_FULL,
                              true));

  command->Run();
}

TEST_F(HMICommandsNotificationsTest,
       OnVRCommandNotificationSwitchedAndInvalidApp) {
  const uint32_t kCmdId = 12u;
  const uint32_t kMaxCmdId = 10u;

  (*message_)[strings::msg_params][strings::cmd_id] = kCmdId;
  utils::SharedPtr<Command> command =
      CreateCommand<OnVRCommandNotification>(message_);

  am::ApplicationSharedPtr invalid_app;
  EXPECT_CALL(mock_app_manager_, application(_))
      .WillRepeatedly(Return(invalid_app));
  EXPECT_CALL(mock_app_manager_, state_controller()).Times(0);
  EXPECT_CALL(mock_app_manager_, get_settings())
      .WillOnce(ReturnRef(mock_app_manager_settings_));
  EXPECT_CALL(mock_app_manager_settings_, max_cmd_id())
      .WillOnce(ReturnRef(kMaxCmdId));
  command->Run();
}

TEST_F(HMICommandsNotificationsTest,
       OnVRCommandNotificationCmdIdEqualToMaxCmdId) {
  const uint32_t kCmdId = 11u;
  const uint32_t kMaxCmdId = 10u;

  (*message_)[strings::msg_params][strings::cmd_id] = kCmdId;
  utils::SharedPtr<Command> command =
      CreateCommand<OnVRCommandNotification>(message_);

  EXPECT_CALL(mock_app_manager_, application(_)).Times(0);
  EXPECT_CALL(mock_app_manager_settings_, max_cmd_id())
      .WillOnce(ReturnRef(kMaxCmdId));
  EXPECT_CALL(mock_app_manager_, get_settings())
      .WillOnce(ReturnRef(mock_app_manager_settings_));
  command->Run();
}

TEST_F(HMICommandsNotificationsTest,
       OnVRCommandNotificationCmdIdLessMaxCmdIsInvalidApp) {
  const uint32_t kCmdId = 8u;
  const uint32_t kMaxCmdId = 10u;

  (*message_)[strings::msg_params][strings::cmd_id] = kCmdId;
  (*message_)[strings::msg_params][strings::grammar_id] = 0;
  utils::SharedPtr<Command> command =
      CreateCommand<OnVRCommandNotification>(message_);

  EXPECT_CALL(mock_app_manager_, get_settings())
      .WillOnce(ReturnRef(mock_app_manager_settings_));
  EXPECT_CALL(mock_app_manager_settings_, max_cmd_id())
      .WillOnce(ReturnRef(kMaxCmdId));

  EXPECT_CALL(mock_app_manager_, applications())
      .WillOnce(Return(applications_));

  EXPECT_CALL(mock_app_manager_, ManageMobileCommand(_, _)).Times(0);
  command->Run();
}

TEST_F(HMICommandsNotificationsTest,
       OnVRCommandNotificationActivePerformIteraction) {
  const uint32_t kCmdId = 8u;
  const uint32_t kMaxCmdId = 10u;
  const uint32_t kIsPerformInteractionActive = 1u;
  int32_t event_id = hmi_apis::FunctionID::INVALID_ENUM;

  (*message_)[strings::msg_params][strings::cmd_id] = kCmdId;
  utils::SharedPtr<Command> command =
      CreateCommand<OnVRCommandNotification>(message_);

  application_set_.insert(mock_app_);
  EXPECT_CALL(mock_app_manager_, applications())
      .WillOnce(Return(applications_));
  EXPECT_CALL(mock_app_manager_settings_, max_cmd_id())
      .WillOnce(ReturnRef(kMaxCmdId));
  EXPECT_CALL(mock_app_manager_, get_settings())
      .WillOnce(ReturnRef(mock_app_manager_settings_));
  EXPECT_CALL(*mock_app_, is_perform_interaction_active())
      .WillOnce(Return(kIsPerformInteractionActive));
  EXPECT_CALL(mock_app_manager_, event_dispatcher());
  EXPECT_CALL(mock_event_dispatcher_, raise_event(_))
      .WillOnce(GetEventId(&event_id));
  EXPECT_CALL(mock_app_manager_, ManageMobileCommand(_, _)).Times(0);
  command->Run();
  EXPECT_EQ(hmi_apis::FunctionID::VR_OnCommand, event_id);
}

TEST_F(HMICommandsNotificationsTest,
       OnVRCommandNotificationNotActivePerformIteraction) {
  const uint32_t kCmdId = 8u;
  const uint32_t kMaxCmdId = 10u;
  const uint32_t kIsPerformInteractionActive = 0u;

  (*message_)[strings::msg_params][strings::cmd_id] = kCmdId;
  (*message_)[strings::msg_params][strings::function_id] =
      mobile_apis::FunctionID::eType::OnCommandID;
  utils::SharedPtr<Command> command =
      CreateCommand<OnVRCommandNotification>(message_);

  application_set_.insert(mock_app_);
  EXPECT_CALL(mock_app_manager_, applications())
      .WillOnce(Return(applications_));
  EXPECT_CALL(mock_app_manager_settings_, max_cmd_id())
      .WillOnce(ReturnRef(kMaxCmdId));
  EXPECT_CALL(mock_app_manager_, get_settings())
      .WillOnce(ReturnRef(mock_app_manager_settings_));
  EXPECT_CALL(*mock_app_, is_perform_interaction_active())
      .WillOnce(Return(kIsPerformInteractionActive));

  EXPECT_CALL(mock_app_manager_, event_dispatcher()).Times(0);
  EXPECT_CALL(mock_app_manager_,
              ManageMobileCommand(_, Command::CommandOrigin::ORIGIN_SDL));
  command->Run();
  EXPECT_EQ(static_cast<int32_t>(mobile_apis::FunctionID::eType::OnCommandID),
            (*message_)[strings::params][strings::function_id].asInt());
  EXPECT_EQ(static_cast<int32_t>(mobile_apis::TriggerSource::TS_VR),
            (*message_)[strings::msg_params][strings::trigger_source].asInt());
  EXPECT_EQ(static_cast<int32_t>(am::MessageType::kNotification),
            (*message_)[strings::params][strings::message_type].asInt());
}

TEST_F(HMICommandsNotificationsTest, OnVRLanguageChangeNotificationEmptyData) {
  const mobile_apis::Language::eType& kLang = mobile_apis::Language::EN_GB;

  (*message_)[strings::msg_params][strings::language] = kLang;
  utils::SharedPtr<Command> command =
      CreateCommand<OnVRLanguageChangeNotification>(message_);

  EXPECT_CALL(mock_hmi_capabilities_, active_ui_language())
      .WillOnce(Return(hmi_apis::Common_Language::EN_AU));
  EXPECT_CALL(mock_hmi_capabilities_, set_active_vr_language(_));
  EXPECT_CALL(mock_app_manager_, hmi_capabilities())
      .WillOnce(ReturnRef(mock_hmi_capabilities_));
  EXPECT_CALL(mock_app_manager_, applications())
      .WillOnce(Return(applications_));
  EXPECT_CALL(mock_app_manager_, ManageMobileCommand(_, _)).Times(0);
  EXPECT_CALL(*mock_app_, app_id()).Times(0);
  EXPECT_CALL(*mock_app_, language()).Times(0);
  command->Run();
}

TEST_F(HMICommandsNotificationsTest,
       OnVRLanguageChangeNotificationAppLangEqualMessageLang) {
  const mobile_apis::Language::eType& kLang = mobile_apis::Language::EN_GB;

  (*message_)[strings::msg_params][strings::language] = kLang;
  utils::SharedPtr<Command> command =
      CreateCommand<OnVRLanguageChangeNotification>(message_);

  application_set_.insert(mock_app_);
  EXPECT_CALL(mock_app_manager_, applications())
      .WillOnce(Return(applications_));
  EXPECT_CALL(mock_hmi_capabilities_, active_ui_language())
      .WillOnce(Return(hmi_apis::Common_Language::EN_AU));
  EXPECT_CALL(mock_hmi_capabilities_, set_active_vr_language(_));
  EXPECT_CALL(mock_app_manager_, hmi_capabilities())
      .WillOnce(ReturnRef(mock_hmi_capabilities_));
  EXPECT_CALL(mock_app_manager_,
              ManageMobileCommand(_, Command::CommandOrigin::ORIGIN_SDL));
  ON_CALL(*mock_app_, app_id()).WillByDefault(Return(kAppId));
  ON_CALL(*mock_app_, language()).WillByDefault(ReturnRef(kLang));

  command->Run();
  EXPECT_EQ(static_cast<int32_t>(mobile_apis::FunctionID::OnLanguageChangeID),
            (*message_)[strings::params][strings::function_id].asInt());
  EXPECT_EQ(kAppId,
            (*message_)[strings::params][strings::connection_key].asUInt());
  EXPECT_EQ(static_cast<int32_t>(am::MessageType::kNotification),
            (*message_)[strings::params][strings::message_type].asInt());
  EXPECT_EQ(
      static_cast<int32_t>(hmi_apis::Common_Language::EN_AU),
      (*message_)[strings::msg_params][strings::hmi_display_language].asInt());
}

TEST_F(HMICommandsNotificationsTest,
       OnVRLanguageChangeNotificationAppLangNotEqualMessageLang) {
  const mobile_apis::Language::eType& kLang = mobile_apis::Language::EN_GB;

  (*message_)[strings::msg_params][strings::language] =
      mobile_apis::Language::EN_US;
  utils::SharedPtr<Command> command =
      CreateCommand<OnVRLanguageChangeNotification>(message_);

  application_set_.insert(mock_app_);
  smart_objects::SmartObjectSPtr notification =
      utils::MakeShared<smart_objects::SmartObject>();
  (*notification)[strings::params][strings::function_id] =
      mobile_apis::FunctionID::OnLanguageChangeID;
  (*notification)[strings::params][strings::message_type] =
      am::MessageType::kNotification;
  (*notification)[strings::params][strings::connection_key] = kAppId;
  (*notification)[strings::msg_params][strings::reason] =
      mobile_apis::AppInterfaceUnregisteredReason::LANGUAGE_CHANGE;

  EXPECT_CALL(mock_app_manager_, applications())
      .WillOnce(Return(applications_));
  EXPECT_CALL(mock_hmi_capabilities_, active_ui_language())
      .WillOnce(Return(hmi_apis::Common_Language::EN_AU));
  EXPECT_CALL(mock_hmi_capabilities_, set_active_vr_language(_));
  EXPECT_CALL(mock_app_manager_, hmi_capabilities())
      .WillOnce(ReturnRef(mock_hmi_capabilities_));
  EXPECT_CALL(mock_app_manager_,
              ManageMobileCommand(_, Command::CommandOrigin::ORIGIN_SDL));
  EXPECT_CALL(*mock_app_, app_id()).WillRepeatedly(Return(kAppId));
  EXPECT_CALL(*mock_app_, language()).WillRepeatedly(ReturnRef(kLang));
  EXPECT_CALL(mock_app_manager_, state_controller())
      .WillOnce(ReturnRef(mock_state_controller_));
  EXPECT_CALL(mock_state_controller_,
              SetRegularState(static_cast<am::ApplicationSharedPtr>(mock_app_),
                              mobile_apis::HMILevel::HMI_NONE,
                              false));
  EXPECT_CALL(
      *message_helper_mock_,
      GetOnAppInterfaceUnregisteredNotificationToMobile(
          kAppId, mobile_apis::AppInterfaceUnregisteredReason::LANGUAGE_CHANGE))
      .WillOnce(Return(notification));
  EXPECT_CALL(mock_app_manager_,
              ManageMobileCommand(notification, Command::ORIGIN_SDL));
  EXPECT_CALL(mock_app_manager_,
              UnregisterApplication(
                  kAppId, mobile_apis::Result::SUCCESS, false, false));
  command->Run();
  EXPECT_EQ(static_cast<int32_t>(mobile_apis::FunctionID::OnLanguageChangeID),
            (*message_)[strings::params][strings::function_id].asInt());
  EXPECT_EQ(kAppId,
            (*message_)[strings::params][strings::connection_key].asUInt());
  EXPECT_EQ(static_cast<int32_t>(am::MessageType::kNotification),
            (*message_)[strings::params][strings::message_type].asInt());
  EXPECT_EQ(
      static_cast<int32_t>(hmi_apis::Common_Language::EN_AU),
      (*message_)[strings::msg_params][strings::hmi_display_language].asInt());
}

TEST_F(HMICommandsNotificationsTest, OnStartDeviceDiscoveryRun) {
  utils::SharedPtr<Command> command =
      CreateCommand<OnStartDeviceDiscovery>(message_);
  EXPECT_CALL(mock_app_manager_, StartDevicesDiscovery());
  command->Run();
}

TEST_F(HMICommandsNotificationsTest,
       OnDeviceChosenNotificationDeviceInfoExists) {
  (*message_)[strings::msg_params][strings::device_info][strings::id] = "2014";
  utils::SharedPtr<Command> command =
      CreateCommand<OnDeviceChosenNotification>(message_);
  EXPECT_CALL(
      mock_app_manager_,
      ConnectToDevice(
          (*message_)[strings::msg_params][strings::device_info][strings::id]
              .asString()));
  command->Run();
}

TEST_F(HMICommandsNotificationsTest,
       OnDeviceChosenNotificationDeviceInfoNotExists) {
  utils::SharedPtr<Command> command =
      CreateCommand<OnDeviceChosenNotification>(message_);
  EXPECT_CALL(mock_app_manager_, ConnectToDevice(_)).Times(0);
  command->Run();
}

TEST_F(HMICommandsNotificationsTest,
       OnSystemContextNotificationValidActiveApplication) {
  std::vector<am::mobile_api::SystemContext::eType> system_context_list;
  system_context_list.push_back(
      am::mobile_api::SystemContext::SYSCTXT_VRSESSION);
  system_context_list.push_back(am::mobile_api::SystemContext::SYSCTXT_MENU);
  system_context_list.push_back(
      am::mobile_api::SystemContext::SYSCTXT_HMI_OBSCURED);

  std::vector<am::mobile_api::SystemContext::eType>::iterator it =
      system_context_list.begin();
  for (; it != system_context_list.end(); ++it) {
    (*message_)[strings::msg_params][am::hmi_notification::system_context] =
        *it;
    utils::SharedPtr<Command> command =
        CreateCommand<OnSystemContextNotification>(message_);
    EXPECT_CALL(mock_app_manager_, active_application())
        .WillOnce(Return(mock_app_));
    EXPECT_CALL(mock_app_manager_, state_controller())
        .WillOnce(ReturnRef(mock_state_controller_));
    EXPECT_CALL(
        mock_state_controller_,
        SetRegularState(static_cast<am::ApplicationSharedPtr>(mock_app_), *it));
    command->Run();
  }
}

TEST_F(HMICommandsNotificationsTest,
       OnSystemContextNotificationInvalidActiveApplication) {
  (*message_)[strings::msg_params][am::hmi_notification::system_context] =
      am::mobile_api::SystemContext::SYSCTXT_VRSESSION;
  utils::SharedPtr<Command> command =
      CreateCommand<OnSystemContextNotification>(message_);
  ApplicationSharedPtr invalid_app;
  EXPECT_CALL(mock_app_manager_, active_application())
      .WillOnce(Return(invalid_app));
  EXPECT_CALL(mock_app_manager_, state_controller()).Times(0);
  command->Run();
}

TEST_F(HMICommandsNotificationsTest,
       OnSystemContextNotificationInvalidSystemContext) {
  (*message_)[strings::msg_params][am::hmi_notification::system_context] =
      am::mobile_api::SystemContext::INVALID_ENUM;
  utils::SharedPtr<Command> command =
      CreateCommand<OnSystemContextNotification>(message_);
  EXPECT_CALL(mock_app_manager_, active_application()).Times(0);
  EXPECT_CALL(mock_app_manager_, application(_)).Times(0);
  EXPECT_CALL(mock_app_manager_, state_controller()).Times(0);
  command->Run();
}

TEST_F(HMICommandsNotificationsTest,
       OnSystemContextNotificationValidApplication) {
  (*message_)[strings::msg_params][strings::app_id] = kAppId;
  std::vector<am::mobile_api::SystemContext::eType> system_context_list;
  system_context_list.push_back(am::mobile_api::SystemContext::SYSCTXT_ALERT);
  system_context_list.push_back(am::mobile_api::SystemContext::SYSCTXT_MAIN);

  std::vector<am::mobile_api::SystemContext::eType>::iterator it =
      system_context_list.begin();
  for (; it != system_context_list.end(); ++it) {
    (*message_)[strings::msg_params][am::hmi_notification::system_context] =
        *it;
    utils::SharedPtr<Command> command =
        CreateCommand<OnSystemContextNotification>(message_);
    EXPECT_CALL(mock_app_manager_, application(_)).WillOnce(Return(mock_app_));
    EXPECT_CALL(mock_app_manager_, state_controller())
        .WillOnce(ReturnRef(mock_state_controller_));
    EXPECT_CALL(
        mock_state_controller_,
        SetRegularState(static_cast<am::ApplicationSharedPtr>(mock_app_), *it));
    command->Run();
  }
}

TEST_F(HMICommandsNotificationsTest,
       OnSystemContextNotificationAppIdDoesntExists) {
  (*message_)[strings::msg_params][am::hmi_notification::system_context] =
      am::mobile_api::SystemContext::SYSCTXT_ALERT;
  utils::SharedPtr<Command> command =
      CreateCommand<OnSystemContextNotification>(message_);
  EXPECT_CALL(mock_app_manager_, application(_)).Times(0);
  EXPECT_CALL(mock_app_manager_, state_controller()).Times(0);
  command->Run();
}

TEST_F(HMICommandsNotificationsTest,
       OnSystemRequestNotificationAppIdExistsAndValidApp) {
  (*message_)[strings::msg_params][strings::app_id] = kAppId;
  utils::SharedPtr<Command> command =
      CreateCommand<OnSystemRequestNotification>(message_);

  EXPECT_CALL(mock_app_manager_, application(kAppId))
      .WillOnce(Return(mock_app_));
  ON_CALL(*mock_app_, app_id()).WillByDefault(Return(kAppId));
  EXPECT_CALL(mock_app_manager_,
              ManageMobileCommand(_, Command::CommandOrigin::ORIGIN_SDL));
  command->Run();
  EXPECT_EQ(static_cast<int32_t>(am::MessageType::kNotification),
            (*message_)[strings::params][strings::message_type].asInt());
  EXPECT_EQ(
      static_cast<int32_t>(mobile_apis::FunctionID::eType::OnSystemRequestID),
      (*message_)[strings::params][strings::function_id].asInt());
  EXPECT_EQ(kAppId,
            (*message_)[strings::params][strings::connection_key].asUInt());
}

TEST_F(HMICommandsNotificationsTest,
       OnSystemRequestNotificationAppIdExistsAndInvalidApp) {
  (*message_)[strings::msg_params][strings::app_id] = kAppId;
  utils::SharedPtr<Command> command =
      CreateCommand<OnSystemRequestNotification>(message_);
  ApplicationSharedPtr invalid_app;
  EXPECT_CALL(mock_app_manager_, application(kAppId))
      .WillOnce(Return(invalid_app));
  EXPECT_CALL(*mock_app_, app_id()).Times(0);
  EXPECT_CALL(mock_app_manager_, ManageMobileCommand(_, _)).Times(0);
  command->Run();
  EXPECT_EQ(
      static_cast<int32_t>(mobile_apis::FunctionID::eType::OnSystemRequestID),
      (*message_)[strings::params][strings::function_id].asInt());
}

TEST_F(HMICommandsNotificationsTest,
       OnSystemRequestNotificationAppIdDoesntExistsAndValidApp) {
  utils::SharedPtr<Command> command =
      CreateCommand<OnSystemRequestNotification>(message_);
  EXPECT_CALL(mock_policy_handler_, GetAppIdForSending())
      .WillOnce(Return(kAppId));
  EXPECT_CALL(mock_app_manager_, application(_)).WillOnce(Return(mock_app_));
  EXPECT_CALL(mock_app_manager_,
              ManageMobileCommand(_, Command::CommandOrigin::ORIGIN_SDL));
  command->Run();
  EXPECT_EQ(static_cast<int32_t>(am::MessageType::kNotification),
            (*message_)[strings::params][strings::message_type].asInt());
  EXPECT_EQ(
      static_cast<int32_t>(mobile_apis::FunctionID::eType::OnSystemRequestID),
      (*message_)[strings::params][strings::function_id].asInt());
  EXPECT_EQ(kAppId,
            (*message_)[strings::params][strings::connection_key].asUInt());
}

TEST_F(HMICommandsNotificationsTest,
       OnSystemRequestNotificationAppIdDoesntExistsAndNullAppId) {
  const uint32_t kNullApppId = 0u;

  utils::SharedPtr<Command> command =
      CreateCommand<OnSystemRequestNotification>(message_);
  EXPECT_CALL(mock_policy_handler_, GetAppIdForSending())
      .WillOnce(Return(kNullApppId));
  EXPECT_CALL(mock_app_manager_, application(_)).Times(0);
  EXPECT_CALL(mock_app_manager_, ManageMobileCommand(_, _)).Times(0);
  command->Run();
}

TEST_F(HMICommandsNotificationsTest, OnTTSLanguageChangeNotificationEmptyData) {
  const mobile_apis::Language::eType& kLang = mobile_apis::Language::EN_GB;

  (*message_)[strings::msg_params][strings::language] = kLang;
  utils::SharedPtr<Command> command =
      CreateCommand<OnTTSLanguageChangeNotification>(message_);

  EXPECT_CALL(mock_hmi_capabilities_, set_active_tts_language(_));
  EXPECT_CALL(mock_hmi_capabilities_, set_active_vr_language(_));
  EXPECT_CALL(mock_hmi_capabilities_, active_ui_language())
      .WillOnce(Return(hmi_apis::Common_Language::EN_AU));
  EXPECT_CALL(mock_app_manager_, hmi_capabilities())
      .WillOnce(ReturnRef(mock_hmi_capabilities_));
  EXPECT_CALL(mock_app_manager_, applications())
      .WillOnce(Return(applications_));
  EXPECT_CALL(mock_app_manager_, ManageMobileCommand(_, _)).Times(0);
  EXPECT_CALL(*mock_app_, app_id()).Times(0);
  EXPECT_CALL(*mock_app_, language()).Times(0);
  command->Run();
}

TEST_F(HMICommandsNotificationsTest,
       OnTTSLanguageChangeNotificationAppLangEqualMessageLang) {
  const mobile_apis::Language::eType& kLang = mobile_apis::Language::EN_GB;

  (*message_)[strings::msg_params][strings::language] = kLang;
  utils::SharedPtr<Command> command =
      CreateCommand<OnTTSLanguageChangeNotification>(message_);

  application_set_.insert(mock_app_);
  EXPECT_CALL(mock_app_manager_, applications())
      .WillOnce(Return(applications_));
  EXPECT_CALL(mock_hmi_capabilities_, active_ui_language())
      .WillOnce(Return(hmi_apis::Common_Language::EN_AU));
  EXPECT_CALL(mock_hmi_capabilities_, set_active_vr_language(_));
  EXPECT_CALL(mock_hmi_capabilities_, set_active_tts_language(_));
  EXPECT_CALL(mock_app_manager_, hmi_capabilities())
      .WillOnce(ReturnRef(mock_hmi_capabilities_));
  EXPECT_CALL(mock_app_manager_,
              ManageMobileCommand(_, Command::CommandOrigin::ORIGIN_SDL));
  ON_CALL(*mock_app_, app_id()).WillByDefault(Return(kAppId));
  ON_CALL(*mock_app_, language()).WillByDefault(ReturnRef(kLang));

  command->Run();
  EXPECT_EQ(static_cast<int32_t>(mobile_apis::FunctionID::OnLanguageChangeID),
            (*message_)[strings::params][strings::function_id].asInt());
  EXPECT_EQ(kAppId,
            (*message_)[strings::params][strings::connection_key].asUInt());
  EXPECT_EQ(static_cast<int32_t>(am::MessageType::kNotification),
            (*message_)[strings::params][strings::message_type].asInt());
  EXPECT_EQ(
      static_cast<int32_t>(hmi_apis::Common_Language::EN_AU),
      (*message_)[strings::msg_params][strings::hmi_display_language].asInt());
}

TEST_F(HMICommandsNotificationsTest,
       OnTTSLanguageChangeNotificationAppLangNotEqualMessageLang) {
  const mobile_apis::Language::eType& kLang = mobile_apis::Language::EN_GB;

  (*message_)[strings::msg_params][strings::language] =
      mobile_apis::Language::EN_US;
  utils::SharedPtr<Command> command =
      CreateCommand<OnTTSLanguageChangeNotification>(message_);

  application_set_.insert(mock_app_);
  smart_objects::SmartObjectSPtr notification =
      utils::MakeShared<smart_objects::SmartObject>();
  (*notification)[strings::params][strings::function_id] =
      mobile_apis::FunctionID::OnLanguageChangeID;
  (*notification)[strings::params][strings::message_type] =
      am::MessageType::kNotification;
  (*notification)[strings::params][strings::connection_key] = kAppId;
  (*notification)[strings::msg_params][strings::reason] =
      mobile_apis::AppInterfaceUnregisteredReason::LANGUAGE_CHANGE;

  EXPECT_CALL(mock_app_manager_, applications())
      .WillOnce(Return(applications_));
  EXPECT_CALL(mock_hmi_capabilities_, active_ui_language())
      .WillOnce(Return(hmi_apis::Common_Language::EN_AU));
  EXPECT_CALL(mock_hmi_capabilities_, set_active_vr_language(_));
  EXPECT_CALL(mock_hmi_capabilities_, set_active_tts_language(_));
  EXPECT_CALL(mock_app_manager_, hmi_capabilities())
      .WillOnce(ReturnRef(mock_hmi_capabilities_));
  EXPECT_CALL(mock_app_manager_,
              ManageMobileCommand(_, Command::CommandOrigin::ORIGIN_SDL));
  EXPECT_CALL(*mock_app_, app_id()).WillRepeatedly(Return(kAppId));
  EXPECT_CALL(*mock_app_, language()).WillRepeatedly(ReturnRef(kLang));
  EXPECT_CALL(
      *message_helper_mock_,
      GetOnAppInterfaceUnregisteredNotificationToMobile(
          kAppId, mobile_apis::AppInterfaceUnregisteredReason::LANGUAGE_CHANGE))
      .WillOnce(Return(notification));
  EXPECT_CALL(mock_app_manager_,
              ManageMobileCommand(notification, Command::ORIGIN_SDL));
  EXPECT_CALL(mock_app_manager_,
              UnregisterApplication(
                  kAppId, mobile_apis::Result::SUCCESS, false, false));
  command->Run();
  EXPECT_EQ(static_cast<int32_t>(mobile_apis::FunctionID::OnLanguageChangeID),
            (*message_)[strings::params][strings::function_id].asInt());
  EXPECT_EQ(kAppId,
            (*message_)[strings::params][strings::connection_key].asUInt());
  EXPECT_EQ(static_cast<int32_t>(am::MessageType::kNotification),
            (*message_)[strings::params][strings::message_type].asInt());
  EXPECT_EQ(
      static_cast<int32_t>(hmi_apis::Common_Language::EN_AU),
      (*message_)[strings::msg_params][strings::hmi_display_language].asInt());
}

TEST_F(HMICommandsNotificationsTest, OnUILanguageChangeNotificationEmptyData) {
  const mobile_apis::Language::eType& kLang = mobile_apis::Language::EN_GB;

  (*message_)[strings::msg_params][strings::language] = kLang;
  utils::SharedPtr<Command> command =
      CreateCommand<OnUILanguageChangeNotification>(message_);

  EXPECT_CALL(mock_hmi_capabilities_, set_active_ui_language(_));
  EXPECT_CALL(mock_hmi_capabilities_, active_vr_language())
      .WillOnce(Return(hmi_apis::Common_Language::EN_AU));
  EXPECT_CALL(mock_app_manager_, hmi_capabilities())
      .WillOnce(ReturnRef(mock_hmi_capabilities_));
  EXPECT_CALL(mock_app_manager_, applications())
      .WillOnce(Return(applications_));
  EXPECT_CALL(mock_app_manager_, ManageMobileCommand(_, _)).Times(0);
  EXPECT_CALL(*mock_app_, app_id()).Times(0);
  EXPECT_CALL(*mock_app_, ui_language()).Times(0);
  command->Run();
}

TEST_F(HMICommandsNotificationsTest,
       OnUILanguageChangeNotificationAppLangEqualMessageLang) {
  const mobile_apis::Language::eType& kLang = mobile_apis::Language::EN_GB;
  (*message_)[strings::msg_params][strings::language] = kLang;
  utils::SharedPtr<Command> command =
      CreateCommand<OnUILanguageChangeNotification>(message_);

  application_set_.insert(mock_app_);
  EXPECT_CALL(mock_app_manager_, applications())
      .WillOnce(Return(applications_));
  EXPECT_CALL(mock_hmi_capabilities_, active_vr_language())
      .WillOnce(Return(hmi_apis::Common_Language::EN_AU));
  EXPECT_CALL(mock_hmi_capabilities_, set_active_ui_language(_));
  EXPECT_CALL(mock_app_manager_, hmi_capabilities())
      .WillOnce(ReturnRef(mock_hmi_capabilities_));
  EXPECT_CALL(mock_app_manager_,
              ManageMobileCommand(_, Command::CommandOrigin::ORIGIN_SDL));
  ON_CALL(*mock_app_, app_id()).WillByDefault(Return(kAppId));
  ON_CALL(*mock_app_, ui_language()).WillByDefault(ReturnRef(kLang));

  command->Run();
  EXPECT_EQ(static_cast<int32_t>(mobile_apis::FunctionID::OnLanguageChangeID),
            (*message_)[strings::params][strings::function_id].asInt());
  EXPECT_EQ(kAppId,
            (*message_)[strings::params][strings::connection_key].asUInt());
  EXPECT_EQ(static_cast<int32_t>(am::MessageType::kNotification),
            (*message_)[strings::params][strings::message_type].asInt());
  EXPECT_EQ(
      static_cast<int32_t>(kLang),
      (*message_)[strings::msg_params][strings::hmi_display_language].asInt());
}

TEST_F(HMICommandsNotificationsTest,
       OnUILanguageChangeNotificationAppLangNotEqualMessageLang) {
  const mobile_apis::Language::eType& kLang = mobile_apis::Language::EN_GB;

  (*message_)[strings::msg_params][strings::language] =
      mobile_apis::Language::EN_US;
  utils::SharedPtr<Command> command =
      CreateCommand<OnUILanguageChangeNotification>(message_);

  application_set_.insert(mock_app_);
  smart_objects::SmartObjectSPtr notification =
      utils::MakeShared<smart_objects::SmartObject>();
  (*notification)[strings::params][strings::function_id] =
      mobile_apis::FunctionID::OnLanguageChangeID;
  (*notification)[strings::params][strings::message_type] =
      am::MessageType::kNotification;
  (*notification)[strings::params][strings::connection_key] = kAppId;
  (*notification)[strings::msg_params][strings::reason] =
      mobile_apis::AppInterfaceUnregisteredReason::LANGUAGE_CHANGE;

  EXPECT_CALL(mock_app_manager_, applications())
      .WillOnce(Return(applications_));
  EXPECT_CALL(mock_hmi_capabilities_, active_vr_language())
      .WillOnce(Return(hmi_apis::Common_Language::EN_AU));
  EXPECT_CALL(mock_hmi_capabilities_, set_active_ui_language(_));
  EXPECT_CALL(mock_app_manager_, hmi_capabilities())
      .WillOnce(ReturnRef(mock_hmi_capabilities_));
  EXPECT_CALL(mock_app_manager_,
              ManageMobileCommand(_, Command::CommandOrigin::ORIGIN_SDL));
  EXPECT_CALL(*mock_app_, app_id()).WillRepeatedly(Return(kAppId));
  EXPECT_CALL(*mock_app_, ui_language()).WillRepeatedly(ReturnRef(kLang));
  EXPECT_CALL(
      *message_helper_mock_,
      GetOnAppInterfaceUnregisteredNotificationToMobile(
          kAppId, mobile_apis::AppInterfaceUnregisteredReason::LANGUAGE_CHANGE))
      .WillOnce(Return(notification));
  EXPECT_CALL(mock_app_manager_,
              ManageMobileCommand(notification, Command::ORIGIN_SDL));
  EXPECT_CALL(mock_app_manager_,
              UnregisterApplication(
                  kAppId, mobile_apis::Result::SUCCESS, false, false));
  command->Run();
  EXPECT_EQ(static_cast<int32_t>(mobile_apis::FunctionID::OnLanguageChangeID),
            (*message_)[strings::params][strings::function_id].asInt());
  EXPECT_EQ(kAppId,
            (*message_)[strings::params][strings::connection_key].asUInt());
  EXPECT_EQ(static_cast<int32_t>(am::MessageType::kNotification),
            (*message_)[strings::params][strings::message_type].asInt());
  EXPECT_EQ(
      static_cast<int32_t>(mobile_apis::Language::EN_US),
      (*message_)[strings::msg_params][strings::hmi_display_language].asInt());
}

TEST_F(
    HMICommandsNotificationsTest,
    OnDeviceConnectionStatusNotification_DeviceKeyNotExist_IgnoreNotification) {
  utils::SharedPtr<Command> command =
      CreateCommand<hmi::OnDeviceConnectionStatusNotification>(message_);

  EXPECT_CALL(mock_app_manager_, GetDeviceConnectionType(_)).Times(0);
  EXPECT_CALL(mock_policy_handler_, OnDeviceConnectionStatus(_, _)).Times(0);
  command->Run();
}

TEST_F(
    HMICommandsNotificationsTest,
    OnDeviceConnectionStatusNotification_MandatoryParamsMissing_IgnoreNotification) {
  (*message_)[strings::msg_params][strings::device] = SmartObject();
  utils::SharedPtr<Command> command =
      CreateCommand<hmi::OnDeviceConnectionStatusNotification>(message_);

  EXPECT_CALL(mock_app_manager_, GetDeviceConnectionType(_)).Times(0);
  EXPECT_CALL(mock_policy_handler_, OnDeviceConnectionStatus(_, _)).Times(0);
  command->Run();
}

TEST_F(
    HMICommandsNotificationsTest,
    OnDeviceConnectionStatusNotification_MandatoryIntegerParamsMissing_IgnoreNotification) {
  const hmi_apis::Common_UserSetting::eType usb_transport_status =
      hmi_apis::Common_UserSetting::ENABLED;

  SmartObject& device_so = (*message_)[strings::msg_params][strings::device];
  device_so[strings::id] = kDeviceId;
  device_so[strings::usb_transport_status] = usb_transport_status;

  utils::SharedPtr<Command> command =
      CreateCommand<hmi::OnDeviceConnectionStatusNotification>(message_);

  EXPECT_CALL(mock_app_manager_, GetDeviceConnectionType(_)).Times(0);
  EXPECT_CALL(mock_policy_handler_, OnDeviceConnectionStatus(_, _)).Times(0);
  command->Run();
}

TEST_F(
    HMICommandsNotificationsTest,
    OnDeviceConnectionStatusNotification_MandatoryStringParamsMissing_IgnoreNotification) {
  const hmi_apis::Common_TransportType::eType transport_type =
      hmi_apis::Common_TransportType::USB_AOA;
  const hmi_apis::Common_UserSetting::eType usb_transport_status =
      hmi_apis::Common_UserSetting::ENABLED;

  SmartObject& device_so = (*message_)[strings::msg_params][strings::device];
  device_so[strings::transport_type] = transport_type;
  device_so[strings::usb_transport_status] = usb_transport_status;

  utils::SharedPtr<Command> command =
      CreateCommand<hmi::OnDeviceConnectionStatusNotification>(message_);

  EXPECT_CALL(mock_app_manager_, GetDeviceConnectionType(_)).Times(0);
  EXPECT_CALL(mock_policy_handler_, OnDeviceConnectionStatus(_, _)).Times(0);
  command->Run();
}

TEST_F(
    HMICommandsNotificationsTest,
    OnDeviceConnectionStatusNotification_StringParamsEmpty_IgnoreNotification) {
  const hmi_apis::Common_TransportType::eType transport_type =
      hmi_apis::Common_TransportType::USB_AOA;
  const std::string connection_type = "USB_AOA";
  const hmi_apis::Common_UserSetting::eType usb_transport_status =
      hmi_apis::Common_UserSetting::ENABLED;

  SmartObject& device_so = (*message_)[strings::msg_params][strings::device];
  device_so[strings::transport_type] = transport_type;
  device_so[strings::id] = kEmptyDeviceInternalId;
  device_so[strings::usb_transport_status] = usb_transport_status;

  utils::SharedPtr<Command> command =
      CreateCommand<hmi::OnDeviceConnectionStatusNotification>(message_);

  EXPECT_CALL(mock_app_manager_, GetDeviceConnectionType(_)).Times(0);
  EXPECT_CALL(mock_policy_handler_, OnDeviceConnectionStatus(_, _)).Times(0);
  command->Run();
}

TEST_F(
    HMICommandsNotificationsTest,
    OnDeviceConnectionStatusNotification_InvalidTransportType_IgnoreNotification) {
  const hmi_apis::Common_TransportType::eType transport_type_value =
      hmi_apis::Common_TransportType::INVALID_ENUM;
  const std::string connection_type;
  const hmi_apis::Common_UserSetting::eType usb_transport_status =
      hmi_apis::Common_UserSetting::ENABLED;

  SmartObject& device_so = (*message_)[strings::msg_params][strings::device];
  device_so[strings::transport_type] = transport_type_value;
  device_so[strings::id] = kDeviceId;
  device_so[strings::usb_transport_status] = usb_transport_status;

  utils::SharedPtr<Command> command =
      CreateCommand<hmi::OnDeviceConnectionStatusNotification>(message_);

  EXPECT_CALL(mock_app_manager_, GetDeviceConnectionType(transport_type_value))
      .WillOnce(Return(connection_type));
  EXPECT_CALL(mock_policy_handler_, OnDeviceConnectionStatus(_, _)).Times(0);
  command->Run();
}

TEST_F(
    HMICommandsNotificationsTest,
    OnDeviceConnectionStatusNotification_USBTransportStatusInvalid_IgnoreNotification) {
  const hmi_apis::Common_TransportType::eType transport_type_value =
      hmi_apis::Common_TransportType::USB_AOA;
  const std::string connection_type = "USB_AOA";
  const hmi_apis::Common_UserSetting::eType usb_transport_status =
      hmi_apis::Common_UserSetting::INVALID_ENUM;

  SmartObject& device_so = (*message_)[strings::msg_params][strings::device];
  device_so[strings::transport_type] = transport_type_value;
  device_so[strings::id] = kDeviceId;
  device_so[strings::usb_transport_status] = usb_transport_status;

  utils::SharedPtr<Command> command =
      CreateCommand<hmi::OnDeviceConnectionStatusNotification>(message_);

  EXPECT_CALL(mock_app_manager_, GetDeviceConnectionType(transport_type_value))
      .WillOnce(Return(connection_type));
  EXPECT_CALL(mock_policy_handler_, OnDeviceConnectionStatus(_, _)).Times(0);
  command->Run();
}

TEST_F(
    HMICommandsNotificationsTest,
    OnDeviceConnectionStatusNotification_NotificationValidUSBTransportEnable_SendNotificationToPolicy) {
  const hmi_apis::Common_TransportType::eType transport_type_value =
      hmi_apis::Common_TransportType::USB_AOA;
  const std::string connection_type = "USB_AOA";
  const hmi_apis::Common_UserSetting::eType usb_transport_status =
      hmi_apis::Common_UserSetting::ENABLED;

  SmartObject& device_so = (*message_)[strings::msg_params][strings::device];
  device_so[strings::transport_type] = transport_type_value;
  device_so[strings::id] = kDeviceId;
  device_so[strings::usb_transport_status] = usb_transport_status;

  utils::SharedPtr<Command> command =
      CreateCommand<hmi::OnDeviceConnectionStatusNotification>(message_);

  EXPECT_CALL(mock_app_manager_, GetDeviceConnectionType(transport_type_value))
      .WillOnce(Return(connection_type));
  EXPECT_CALL(mock_policy_handler_,
              OnDeviceConnectionStatus(kDeviceId, usb_transport_status));
  command->Run();
}

TEST_F(
    HMICommandsNotificationsTest,
    OnDeviceConnectionStatusNotification_NotificationValidUSBTransportDisable_SendNotificationToPolicy) {
  const hmi_apis::Common_TransportType::eType transport_type_value =
      hmi_apis::Common_TransportType::USB_AOA;
  const std::string connection_type = "USB_AOA";
  const hmi_apis::Common_UserSetting::eType usb_transport_status =
      hmi_apis::Common_UserSetting::DISABLED;

  SmartObject& device_so = (*message_)[strings::msg_params][strings::device];
  device_so[strings::transport_type] = transport_type_value;
  device_so[strings::id] = kDeviceId;
  device_so[strings::usb_transport_status] = usb_transport_status;

  utils::SharedPtr<Command> command =
      CreateCommand<hmi::OnDeviceConnectionStatusNotification>(message_);

  EXPECT_CALL(mock_app_manager_, GetDeviceConnectionType(transport_type_value))
      .WillOnce(Return(connection_type));
  EXPECT_CALL(mock_policy_handler_,
              OnDeviceConnectionStatus(kDeviceId, usb_transport_status));
  command->Run();
}

}  // namespace hmi_notifications_test
}  // namespace hmi_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
