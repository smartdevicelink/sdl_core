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
#include "application_manager/commands/commands_test.h"
#include "gtest/gtest.h"
#include "sdl_rpc_plugin/commands/hmi/on_allow_sdl_functionality_notification.h"
#include "sdl_rpc_plugin/commands/hmi/on_app_activated_notification.h"
#include "sdl_rpc_plugin/commands/hmi/on_app_deactivated_notification.h"
#include "sdl_rpc_plugin/commands/hmi/on_app_permission_changed_notification.h"
#include "sdl_rpc_plugin/commands/hmi/on_app_permission_consent_notification.h"
#include "sdl_rpc_plugin/commands/hmi/on_app_registered_notification.h"
#include "sdl_rpc_plugin/commands/hmi/on_app_unregistered_notification.h"
#include "sdl_rpc_plugin/commands/hmi/on_audio_data_streaming_notification.h"
#include "sdl_rpc_plugin/commands/hmi/on_button_event_notification.h"
#include "sdl_rpc_plugin/commands/hmi/on_button_press_notification.h"
#include "sdl_rpc_plugin/commands/hmi/on_button_subscription_notification.h"
#include "sdl_rpc_plugin/commands/hmi/on_device_chosen_notification.h"
#include "sdl_rpc_plugin/commands/hmi/on_device_state_changed_notification.h"
#include "sdl_rpc_plugin/commands/hmi/on_driver_distraction_notification.h"
#include "sdl_rpc_plugin/commands/hmi/on_event_changed_notification.h"
#include "sdl_rpc_plugin/commands/hmi/on_exit_all_applications_notification.h"
#include "sdl_rpc_plugin/commands/hmi/on_exit_application_notification.h"
#include "sdl_rpc_plugin/commands/hmi/on_file_removed_notification.h"
#include "sdl_rpc_plugin/commands/hmi/on_ignition_cycle_over_notification.h"
#include "sdl_rpc_plugin/commands/hmi/on_navi_tbt_client_state_notification.h"
#include "sdl_rpc_plugin/commands/hmi/on_navi_way_point_change_notification.h"
#include "sdl_rpc_plugin/commands/hmi/on_policy_update.h"
#include "sdl_rpc_plugin/commands/hmi/on_put_file_notification.h"
#include "sdl_rpc_plugin/commands/hmi/on_ready_notification.h"
#include "sdl_rpc_plugin/commands/hmi/on_received_policy_update.h"
#include "sdl_rpc_plugin/commands/hmi/on_record_start_notification.h"
#include "sdl_rpc_plugin/commands/hmi/on_resume_audio_source_notification.h"
#include "sdl_rpc_plugin/commands/hmi/on_sdl_close_notification.h"
#include "sdl_rpc_plugin/commands/hmi/on_sdl_consent_needed_notification.h"
#include "sdl_rpc_plugin/commands/hmi/on_sdl_persistence_complete_notification.h"
#include "sdl_rpc_plugin/commands/hmi/on_start_device_discovery.h"
#include "sdl_rpc_plugin/commands/hmi/on_status_update_notification.h"
#include "sdl_rpc_plugin/commands/hmi/on_system_context_notification.h"
#include "sdl_rpc_plugin/commands/hmi/on_system_error_notification.h"
#include "sdl_rpc_plugin/commands/hmi/on_system_info_changed_notification.h"
#include "sdl_rpc_plugin/commands/hmi/on_system_request_notification.h"
#include "sdl_rpc_plugin/commands/hmi/on_tts_language_change_notification.h"
#include "sdl_rpc_plugin/commands/hmi/on_tts_reset_timeout_notification.h"
#include "sdl_rpc_plugin/commands/hmi/on_tts_started_notification.h"
#include "sdl_rpc_plugin/commands/hmi/on_tts_stopped_notification.h"
#include "sdl_rpc_plugin/commands/hmi/on_ui_command_notification.h"
#include "sdl_rpc_plugin/commands/hmi/on_ui_keyboard_input_notification.h"
#include "sdl_rpc_plugin/commands/hmi/on_ui_language_change_notification.h"
#include "sdl_rpc_plugin/commands/hmi/on_ui_reset_timeout_notification.h"
#include "sdl_rpc_plugin/commands/hmi/on_ui_subtle_alert_pressed_notification.h"
#include "sdl_rpc_plugin/commands/hmi/on_ui_touch_event_notification.h"
#include "sdl_rpc_plugin/commands/hmi/on_video_data_streaming_notification.h"
#include "sdl_rpc_plugin/commands/hmi/on_vr_command_notification.h"
#include "sdl_rpc_plugin/commands/hmi/on_vr_language_change_notification.h"
#include "sdl_rpc_plugin/commands/hmi/on_vr_started_notification.h"
#include "sdl_rpc_plugin/commands/hmi/on_vr_stopped_notification.h"

#include "utils/data_accessor.h"
#include "utils/lock.h"
#include "utils/signals.h"

#include "application_manager/hmi_capabilities_impl.h"
#include "application_manager/mock_app_service_manager.h"
#include "application_manager/mock_application.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/mock_event_dispatcher.h"
#include "application_manager/mock_hmi_capabilities.h"
#include "application_manager/mock_message_helper.h"
#include "application_manager/mock_resume_ctrl.h"
#include "application_manager/mock_rpc_plugin_manager.h"
#include "application_manager/mock_state_controller.h"
#include "application_manager/policies/mock_policy_handler_interface.h"
#include "application_manager/resumption/resumption_data_processor.h"
#include "application_manager/smart_object_keys.h"
#include "connection_handler/mock_connection_handler.h"
#include "connection_handler/mock_connection_handler_settings.h"
#include "protocol_handler/mock_session_observer.h"
#include "resumption/last_state_wrapper_impl.h"
#include "resumption/mock_last_state.h"
#include "smart_objects/smart_object.h"
#include "test/application_manager/mock_application_manager_settings.h"
#include "transport_manager/mock_transport_manager.h"
#include "utils/file_system.h"

namespace am = application_manager;

namespace test {
namespace components {
namespace commands_test {
namespace hmi_commands_test {
namespace hmi_notifications_test {

using ::application_manager::ApplicationSharedPtr;
using ::application_manager::MockMessageHelper;
using ::application_manager::commands::MessageSharedPtr;
using ::smart_objects::SmartObject;
using ::test::components::application_manager_test::MockApplication;
using ::test::components::application_manager_test::MockApplicationManager;
using ::test::components::application_manager_test::
    MockApplicationManagerSettings;
using ::test::components::application_manager_test::MockAppServiceManager;
using ::test::components::event_engine_test::MockEventDispatcher;
using ::testing::_;
using ::testing::InSequence;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::Test;
using ::testing::Types;
using namespace sdl_rpc_plugin::commands;
using namespace am::commands;

typedef std::shared_ptr<MockApplication> MockAppPtr;
typedef NiceMock<
    ::test::components::application_manager_test::MockHMICapabilities>
    MockHMICapabilities;

#define MEDIA true
#define NOT_MEDIA false
#define VC true
#define NOT_VC false
#define NAVI true
#define NOT_NAVI false

namespace {
const am::WindowID kDefaultWindowId =
    mobile_apis::PredefinedWindows::DEFAULT_WINDOW;
}

ACTION_P(GetEventId, event_id) {
  *event_id = arg0.id();
}
ACTION_P(GetArg, arg) {
  *arg = arg0;
}

ACTION_P(GetArg4, result) {
  arg4 = *result;
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
  (*message)[am::strings::params][am::strings::function_id] =
      (*arg0)[am::strings::params][am::strings::function_id].asInt();
  (*message)[am::strings::params][am::strings::message_type] =
      (*arg0)[am::strings::params][am::strings::message_type].asInt();
  (*message)[am::strings::params][am::strings::correlation_id] =
      (*arg0)[am::strings::params][am::strings::correlation_id].asInt();
  (*message)[am::strings::params][am::strings::connection_key] =
      (*arg0)[am::strings::params][am::strings::connection_key].asInt();
  return (*message)[am::strings::params][am::strings::correlation_id].asInt();
}

namespace {
const uint32_t kCountCommandsManageMobile_ = 22u;
const uint32_t kCountCommandsToHmi_ = 12u;
const int32_t kHmiProtocolType_ = 1;
const int32_t kMobileProtocolType_ = 0;
const int32_t kProtocolVersion_ = 3;
const uint32_t kCorrelationId_ = 1939u;
const uint32_t kAppId_ = 2014u;
const std::string kDefaultLanguage = "en-us";
const mobile_apis::Language::eType kMobileLanguage =
    mobile_apis::Language::EN_US;

// LSDW - lock screen dismissal warning
const std::string kLockScreenDismissalWarningMessage_en =
    "Swipe down to dismiss, acknowledging that you are not the driver";
const uint32_t kConnectionKey = 2u;
}  // namespace

class HMICommandsNotificationsTest
    : public components::commands_test::CommandsTest<
          CommandsTestMocks::kIsNice> {
 public:
  HMICommandsNotificationsTest()
      : applications_lock_(std::make_shared<sync_primitives::Lock>())
      , applications_(application_set_, applications_lock_)
      , app_ptr_(NULL)
      , mock_last_state_(std::make_shared<resumption_test::MockLastState>())
      , last_state_wrapper_(std::make_shared<resumption::LastStateWrapperImpl>(
            mock_last_state_))
      , mock_app_service_manager_(app_mngr_, last_state_wrapper_) {}

  ~HMICommandsNotificationsTest() {
    // Fix DataAccessor release and WinQt crash
    Mock::VerifyAndClearExpectations(&app_mngr_);
  }
  typedef Command CommandType;

 protected:
  am::ApplicationSet application_set_;
  std::shared_ptr<sync_primitives::Lock> applications_lock_;
  DataAccessor<am::ApplicationSet> applications_;

  NiceMock<event_engine_test::MockEventDispatcher> mock_event_dispatcher_;

  application_manager_test::MockStateController mock_state_controller_;

  am::ApplicationSharedPtr app_;
  NiceMock<MockApplication>* app_ptr_;

  typedef IsNiceMock<connection_handler_test::MockConnectionHandler,
                     kMocksAreNice>::Result MockConnectionHandler;

  typedef IsNiceMock<protocol_handler_test::MockSessionObserver,
                     kMocksAreNice>::Result MockSessionObserver;

  MockConnectionHandler mock_connection_handler_;
  MockSessionObserver mock_session_observer_;
  std::shared_ptr<resumption_test::MockLastState> mock_last_state_;
  resumption::LastStateWrapperPtr last_state_wrapper_;
  MockAppServiceManager mock_app_service_manager_;

  void InitCommand(const uint32_t& default_timeout) OVERRIDE {
    app_ = ConfigureApp(&app_ptr_, kAppId_, NOT_MEDIA, NOT_NAVI, NOT_VC);
    EXPECT_CALL(app_mngr_, get_settings())
        .WillOnce(ReturnRef(app_mngr_settings_));
    EXPECT_CALL(app_mngr_settings_, default_timeout())
        .WillOnce(ReturnRef(default_timeout));
    ON_CALL(app_mngr_, event_dispatcher())
        .WillByDefault(ReturnRef(mock_event_dispatcher_));
    ON_CALL(app_mngr_, application_by_hmi_app(_)).WillByDefault(Return(app_));
    ON_CALL(*app_ptr_, app_id()).WillByDefault(Return(kAppId_));
    ON_CALL(app_mngr_, application(kConnectionKey)).WillByDefault(Return(app_));
    ON_CALL(app_mngr_, GetAppServiceManager())
        .WillByDefault(ReturnRef(mock_app_service_manager_));
    ON_CALL(app_mngr_, connection_handler())
        .WillByDefault(ReturnRef(mock_connection_handler_));
  }

  am::ApplicationSharedPtr ConfigureApp(NiceMock<MockApplication>** app_mock,
                                        uint32_t app_id,
                                        bool media,
                                        bool navi,
                                        bool vc) {
    *app_mock = new NiceMock<MockApplication>;

    Mock::AllowLeak(*app_mock);  // WorkAround for gogletest bug
    am::ApplicationSharedPtr app(*app_mock);

    ON_CALL(**app_mock, app_id()).WillByDefault(Return(app_id));
    ON_CALL(**app_mock, is_media_application()).WillByDefault(Return(media));
    ON_CALL(**app_mock, is_navi()).WillByDefault(Return(navi));
    ON_CALL(**app_mock, is_voice_communication_supported())
        .WillByDefault(Return(vc));
    ON_CALL(**app_mock, IsAudioApplication())
        .WillByDefault(Return(media || navi || vc));
    ON_CALL(**app_mock, ui_language())
        .WillByDefault(ReturnRef(kMobileLanguage));
    return app;
  }
#if defined(OS_POSIX)
  void SubscribeForSignal() {
    sigset_t signal_set;
    sigemptyset(&signal_set);
    sigaddset(&signal_set, SIGINT);
    sigaddset(&signal_set, SIGTERM);
    pthread_sigmask(SIG_BLOCK, &signal_set, NULL);
  }
#endif
};

namespace {
void sig_handler(int sig) {
  switch (sig) {
    case SIGINT:
      break;
    case SIGTERM:
      break;
    case SIGSEGV:
      abort();
    default:
      exit(EXIT_FAILURE);
  }
}
}  //  namespace

template <class Command>
class HMIOnNotificationsListToHMI
    : public commands_test::CommandsTest<kIsNice> {
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
    : public commands_test::CommandsTest<kIsNice> {
 public:
  typedef Command CommandType;
  NiceMock<event_engine_test::MockEventDispatcher> mock_event_dispatcher_;
};

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
              OnRecordStartdNotification>
    HMIOnNotificationsListToHMITypes;

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

TYPED_TEST_CASE(HMIOnNotificationsListToHMI, HMIOnNotificationsListToHMITypes);
TYPED_TEST_CASE(HMIOnNotificationsEventDispatcher,
                HMIOnNotificationsEventDispatcherTypes);

TYPED_TEST(HMIOnNotificationsListToHMI, CommandsSendNotificationToHmi) {
  MessageSharedPtr message =
      commands_test::CommandsTest<kIsNice>::CreateMessage();
  std::shared_ptr<typename TestFixture::CommandType> command =
      this->template CreateCommand<typename TestFixture::CommandType>(message);
  EXPECT_CALL(commands_test::CommandsTest<kIsNice>::mock_rpc_service_,
              SendMessageToHMI(_));
  command->Run();
  EXPECT_EQ(
      static_cast<int32_t>(kHmiProtocolType_),
      (*message)[am::strings::params][am::strings::protocol_type].asInt());
  EXPECT_EQ(
      static_cast<int32_t>(kProtocolVersion_),
      (*message)[am::strings::params][am::strings::protocol_version].asInt());
}

TYPED_TEST(HMIOnNotificationsEventDispatcher,
           CommandsNotificationEventDispatcher) {
  int32_t event_id = hmi_apis::FunctionID::INVALID_ENUM;
  MessageSharedPtr message =
      commands_test::CommandsTest<kIsNice>::CreateMessage();
  std::shared_ptr<typename TestFixture::CommandType::CommandType> command =
      this->template CreateCommand<
          typename TestFixture::CommandType::CommandType>(message);
  EXPECT_CALL(commands_test::CommandsTest<kIsNice>::app_mngr_,
              event_dispatcher())
      .WillOnce(ReturnRef(this->mock_event_dispatcher_));
  EXPECT_CALL(this->mock_event_dispatcher_, raise_event(_))
      .WillOnce(GetEventId(&event_id));
  command->Run();
  EXPECT_EQ(TestFixture::CommandType::kHMICommandId, event_id);
}

// notifications(SendNotificationToMobile)
TEST_F(HMICommandsNotificationsTest, OnButtonEventSendNotificationToMobile) {
  MessageSharedPtr message = CreateMessage();
  std::shared_ptr<Command> command =
      CreateCommand<hmi::OnButtonEventNotification>(message);
  EXPECT_CALL(mock_rpc_service_,
              ManageMobileCommand(_, Command::CommandSource::SOURCE_SDL));
  command->Run();
  EXPECT_EQ(static_cast<int32_t>(mobile_apis::FunctionID::OnButtonEventID),
            (*message)[am::strings::params][am::strings::function_id].asInt());
  EXPECT_EQ(static_cast<int32_t>(am::MessageType::kNotification),
            (*message)[am::strings::params][am::strings::message_type].asInt());
}

TEST_F(HMICommandsNotificationsTest, OnNaviTBTClientSendNotificationToMobile) {
  MessageSharedPtr message = CreateMessage();
  std::shared_ptr<Command> command =
      CreateCommand<OnNaviTBTClientStateNotification>(message);
  EXPECT_CALL(mock_rpc_service_,
              ManageMobileCommand(_, Command::CommandSource::SOURCE_SDL));
  command->Run();
  EXPECT_EQ(static_cast<int32_t>(mobile_apis::FunctionID::OnTBTClientStateID),
            (*message)[am::strings::params][am::strings::function_id].asInt());
  EXPECT_EQ(static_cast<int32_t>(am::MessageType::kNotification),
            (*message)[am::strings::params][am::strings::message_type].asInt());
}

TEST_F(HMICommandsNotificationsTest,
       OnNaviWayPointChangeSendNotificationToMobile) {
  MessageSharedPtr message = CreateMessage();
  std::shared_ptr<Command> command =
      CreateCommand<OnNaviWayPointChangeNotification>(message);
  EXPECT_CALL(mock_app_service_manager_, FindWayPointsHandler())
      .WillOnce(Return(nullptr));
  EXPECT_CALL(mock_rpc_service_,
              ManageMobileCommand(_, Command::CommandSource::SOURCE_SDL));
  command->Run();
  EXPECT_EQ(static_cast<int32_t>(mobile_apis::FunctionID::OnWayPointChangeID),
            (*message)[am::strings::params][am::strings::function_id].asInt());
  EXPECT_EQ(static_cast<int32_t>(am::MessageType::kNotification),
            (*message)[am::strings::params][am::strings::message_type].asInt());
}

TEST_F(HMICommandsNotificationsTest, OnUICommandSendNotificationToMobile) {
  MessageSharedPtr message = CreateMessage();
  std::shared_ptr<Command> command =
      CreateCommand<OnUICommandNotification>(message);
  EXPECT_CALL(mock_rpc_service_,
              ManageMobileCommand(_, Command::CommandSource::SOURCE_SDL));
  command->Run();
  EXPECT_EQ(static_cast<int32_t>(mobile_apis::FunctionID::eType::OnCommandID),
            (*message)[am::strings::params][am::strings::function_id].asInt());
  EXPECT_EQ(
      static_cast<int32_t>(mobile_apis::TriggerSource::TS_MENU),
      (*message)[am::strings::msg_params][am::strings::trigger_source].asInt());
  EXPECT_EQ(static_cast<int32_t>(am::MessageType::kNotification),
            (*message)[am::strings::params][am::strings::message_type].asInt());
}

TEST_F(HMICommandsNotificationsTest,
       OnUIKeyBoardInputSendNotificationToMobile) {
  MessageSharedPtr message = CreateMessage();
  std::shared_ptr<Command> command =
      CreateCommand<hmi::OnUIKeyBoardInputNotification>(message);
  EXPECT_CALL(mock_rpc_service_,
              ManageMobileCommand(_, Command::CommandSource::SOURCE_SDL));
  command->Run();
  EXPECT_EQ(static_cast<int32_t>(mobile_apis::FunctionID::OnKeyboardInputID),
            (*message)[am::strings::params][am::strings::function_id].asInt());
  EXPECT_EQ(static_cast<int32_t>(am::MessageType::kNotification),
            (*message)[am::strings::params][am::strings::message_type].asInt());
}

TEST_F(HMICommandsNotificationsTest, OnUITouchEventSendNotificationToMobile) {
  MessageSharedPtr message = CreateMessage();
  std::shared_ptr<Command> command =
      CreateCommand<hmi::OnUITouchEventNotification>(message);
  EXPECT_CALL(mock_rpc_service_,
              ManageMobileCommand(_, Command::CommandSource::SOURCE_SDL));
  command->Run();
  EXPECT_EQ(static_cast<int32_t>(mobile_apis::FunctionID::OnTouchEventID),
            (*message)[am::strings::params][am::strings::function_id].asInt());
  EXPECT_EQ(static_cast<int32_t>(am::MessageType::kNotification),
            (*message)[am::strings::params][am::strings::message_type].asInt());
}

TEST_F(HMICommandsNotificationsTest,
       OnUISubtleAlertPressedSendNotificationToMobile) {
  MessageSharedPtr message = CreateMessage();
  std::shared_ptr<Command> command =
      CreateCommand<OnUISubtleAlertPressedNotification>(message);
  EXPECT_CALL(mock_rpc_service_,
              ManageMobileCommand(_, Command::CommandSource::SOURCE_SDL));
  command->Run();
  EXPECT_EQ(
      static_cast<int32_t>(mobile_apis::FunctionID::OnSubtleAlertPressedID),
      (*message)[am::strings::params][am::strings::function_id].asInt());
  EXPECT_EQ(static_cast<int32_t>(am::MessageType::kNotification),
            (*message)[am::strings::params][am::strings::message_type].asInt());
}

TEST_F(HMICommandsNotificationsTest,
       OnAppRegisteredNotificationSendNotificationToHmi) {
  int32_t event_id = hmi_apis::FunctionID::INVALID_ENUM;
  MessageSharedPtr message = CreateMessage();
  std::shared_ptr<Command> command =
      CreateCommand<OnAppRegisteredNotification>(message);
  EXPECT_CALL(mock_rpc_service_, SendMessageToHMI(_));
  EXPECT_CALL(app_mngr_, event_dispatcher());
  EXPECT_CALL(mock_event_dispatcher_, raise_event(_))
      .WillOnce(GetEventId(&event_id));
  command->Run();
  EXPECT_EQ(static_cast<int32_t>(
                hmi_apis::FunctionID::BasicCommunication_OnAppRegistered),
            event_id);
  EXPECT_EQ(
      kHmiProtocolType_,
      (*message)[am::strings::params][am::strings::protocol_type].asInt());
  EXPECT_EQ(
      kProtocolVersion_,
      (*message)[am::strings::params][am::strings::protocol_version].asInt());
}

TEST_F(HMICommandsNotificationsTest,
       OnAppUnregisteredNotificationEventDispatcher) {
  int32_t event_id = hmi_apis::FunctionID::INVALID_ENUM;
  MessageSharedPtr message = CreateMessage();
  std::shared_ptr<Command> command =
      CreateCommand<OnAppUnregisteredNotification>(message);
  EXPECT_CALL(mock_rpc_service_, SendMessageToHMI(_));
  EXPECT_CALL(app_mngr_, event_dispatcher());
  EXPECT_CALL(mock_event_dispatcher_, raise_event(_))
      .WillOnce(GetEventId(&event_id));
  command->Run();
  EXPECT_EQ(static_cast<int32_t>(
                hmi_apis::FunctionID::BasicCommunication_OnAppUnregistered),
            event_id);
  EXPECT_EQ(
      kHmiProtocolType_,
      (*message)[am::strings::params][am::strings::protocol_type].asInt());
  EXPECT_EQ(
      kProtocolVersion_,
      (*message)[am::strings::params][am::strings::protocol_version].asInt());
}

TEST_F(HMICommandsNotificationsTest, OnButtonPressNotificationEventDispatcher) {
  int32_t event_id = hmi_apis::FunctionID::INVALID_ENUM;
  MessageSharedPtr message = CreateMessage();
  std::shared_ptr<Command> command =
      CreateCommand<hmi::OnButtonPressNotification>(message);
  EXPECT_CALL(mock_rpc_service_,
              ManageMobileCommand(_, Command::CommandSource::SOURCE_SDL));
  EXPECT_CALL(app_mngr_, event_dispatcher());
  EXPECT_CALL(mock_event_dispatcher_, raise_event(_))
      .WillOnce(GetEventId(&event_id));
  command->Run();
  EXPECT_EQ(static_cast<int32_t>(hmi_apis::FunctionID::Buttons_OnButtonPress),
            event_id);
  EXPECT_EQ(static_cast<int>(mobile_apis::FunctionID::eType::OnButtonPressID),
            (*message)[am::strings::params][am::strings::function_id].asInt());
}

TEST_F(HMICommandsNotificationsTest, OnReadyNotificationEventDispatcher) {
  int32_t event_id = hmi_apis::FunctionID::INVALID_ENUM;
  MessageSharedPtr message = CreateMessage();
  std::shared_ptr<Command> command =
      CreateCommand<OnReadyNotification>(message);

  EXPECT_CALL(app_mngr_, OnHMIReady());
  EXPECT_CALL(app_mngr_, event_dispatcher());
  EXPECT_CALL(mock_event_dispatcher_, raise_event(_))
      .WillOnce(GetEventId(&event_id));
  command->Run();
  EXPECT_EQ(hmi_apis::FunctionID::BasicCommunication_OnReady, event_id);
}

// policy handler
TEST_F(HMICommandsNotificationsTest,
       OnIgnitionCycleOverNotificationPolicyHandler) {
  MessageSharedPtr message = CreateMessage();
  std::shared_ptr<Command> command =
      CreateCommand<OnIgnitionCycleOverNotification>(message);

  EXPECT_CALL(mock_policy_handler_, OnIgnitionCycleOver());
  command->Run();
}

TEST_F(HMICommandsNotificationsTest, OnPolicyUpdateNotificationPolicyHandler) {
  MessageSharedPtr message = CreateMessage();
  std::shared_ptr<Command> command = CreateCommand<OnPolicyUpdate>(message);

  EXPECT_CALL(mock_policy_handler_, OnPTExchangeNeeded());
  command->Run();
}

#if defined(PROPRIETARY_MODE) || defined(EXTERNAL_PROPRIETARY_MODE)
TEST_F(HMICommandsNotificationsTest,
       OnReceivePolicyUpdateNotification_SUCCESS) {
  const std::string kFile = "./test_file.txt";
  EXPECT_TRUE(file_system::CreateFile(kFile));
  uint8_t tmp[] = {1u, 2u, 3u, 4u};
  std::vector<uint8_t> data(tmp, tmp + 4);
  EXPECT_TRUE(file_system::WriteBinaryFile(kFile, data));

  MessageSharedPtr message = CreateMessage(smart_objects::SmartType_String);
  (*message)[am::strings::msg_params][am::hmi_notification::policyfile] = kFile;
  std::shared_ptr<Command> command =
      CreateCommand<OnReceivedPolicyUpdate>(message);

  EXPECT_CALL(mock_policy_handler_, ReceiveMessageFromSDK(kFile, data));
  command->Run();
  EXPECT_TRUE(file_system::DeleteFile(kFile));
}
#endif

TEST_F(HMICommandsNotificationsTest,
       OnReceivePolicyUpdateNotification_UNSUCCESS) {
  MessageSharedPtr message = CreateMessage(smart_objects::SmartType_String);
  std::shared_ptr<Command> command =
      CreateCommand<OnReceivedPolicyUpdate>(message);

  EXPECT_CALL(mock_policy_handler_, ReceiveMessageFromSDK(_, _)).Times(0);
  command->Run();
}

TEST_F(HMICommandsNotificationsTest,
       OnAppPermissionConsentNotificationPolicyHandlerNoAppId) {
  MessageSharedPtr message = CreateMessage(smart_objects::SmartType_Map);
  (*message)[am::strings::msg_params]["consentedFunctions"] =
      smart_objects::SmartObject(smart_objects::SmartType_Array);
  SmartObject& applications =
      (*message)[am::strings::msg_params]["consentedFunctions"];

  smart_objects::SmartObject hmi_application_temp(smart_objects::SmartType_Map);
  applications[0] = hmi_application_temp;

  std::shared_ptr<Command> command =
      CreateCommand<OnAppPermissionConsentNotification>(message);

  int32_t connection_id = -1;
#ifdef EXTERNAL_PROPRIETARY_MODE
  EXPECT_CALL(mock_policy_handler_, OnAppPermissionConsent(_, _, _))
#else
  EXPECT_CALL(mock_policy_handler_, OnAppPermissionConsent(_, _))
#endif
      .WillOnce(GetArg(&connection_id));
  command->Run();
  EXPECT_EQ(0, connection_id);
}

TEST_F(HMICommandsNotificationsTest,
       OnAppPermissionConsentNotificationPolicyHandlerWithAppId) {
  MessageSharedPtr message = CreateMessage(smart_objects::SmartType_Map);
  (*message)[am::strings::msg_params][am::strings::app_id] = kAppId_;
  (*message)[am::strings::msg_params]["consentedFunctions"] =
      smart_objects::SmartObject(smart_objects::SmartType_Array);

  smart_objects::SmartObjectSPtr consented_function =
      std::make_shared<smart_objects::SmartObject>();
  (*message)[am::strings::msg_params]["consentedFunctions"][0] =
      *consented_function;

  std::shared_ptr<Command> command =
      CreateCommand<OnAppPermissionConsentNotification>(message);

  int32_t connection_id = -1;
  policy::PermissionConsent permission_consent;
#ifdef EXTERNAL_PROPRIETARY_MODE
  EXPECT_CALL(mock_policy_handler_, OnAppPermissionConsent(_, _, _))
#else
  EXPECT_CALL(mock_policy_handler_, OnAppPermissionConsent(_, _))
#endif
      .WillOnce(
          GetConnectIdPermissionConsent(&connection_id, &permission_consent));
  command->Run();
  EXPECT_EQ(static_cast<int32_t>(kAppId_), connection_id);
  std::vector<policy::FunctionalGroupPermission>::const_iterator it =
      permission_consent.group_permissions.begin();
  for (; it != permission_consent.group_permissions.end(); ++it) {
    EXPECT_EQ(policy::kGroupUndefined, (*it).state);
  }
}

TEST_F(HMICommandsNotificationsTest,
       OnAppPermissionConsentNotificationPolicyHandlerAppIdAllowTrue) {
  MessageSharedPtr message = CreateMessage(smart_objects::SmartType_Map);
  (*message)[am::strings::msg_params]["consentedFunctions"] =
      smart_objects::SmartObject(smart_objects::SmartType_Array);
  (*message)[am::strings::msg_params][am::strings::app_id] = kAppId_;
  (*message)[am::strings::msg_params]["source"] = "test_content_source";

  smart_objects::SmartObjectSPtr consented_function =
      std::make_shared<smart_objects::SmartObject>();
  (*consented_function)["allowed"] = true;
  (*consented_function)[am::strings::id] = 999;
  (*consented_function)[am::strings::name] = "test_group_alias";
  (*message)[am::strings::msg_params]["consentedFunctions"][0] =
      *consented_function;

  std::shared_ptr<Command> command =
      CreateCommand<OnAppPermissionConsentNotification>(message);

  int32_t connection_id = -1;
  policy::PermissionConsent permission_consent;
#ifdef EXTERNAL_PROPRIETARY_MODE
  EXPECT_CALL(mock_policy_handler_, OnAppPermissionConsent(_, _, _))
#else
  EXPECT_CALL(mock_policy_handler_, OnAppPermissionConsent(_, _))
#endif
      .WillOnce(
          GetConnectIdPermissionConsent(&connection_id, &permission_consent));
  command->Run();
  EXPECT_EQ(static_cast<int32_t>(kAppId_), connection_id);

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
  MessageSharedPtr message = CreateMessage(smart_objects::SmartType_Map);
  (*message)[am::strings::msg_params]["consentedFunctions"] =
      smart_objects::SmartObject(smart_objects::SmartType_Array);
  (*message)[am::strings::msg_params][am::strings::app_id] = kAppId_;
  (*message)[am::strings::msg_params]["source"] = "test_content_source";

  smart_objects::SmartObjectSPtr consented_function =
      std::make_shared<smart_objects::SmartObject>();
  (*consented_function)["allowed"] = false;
  (*consented_function)[am::strings::id] = 999;
  (*consented_function)[am::strings::name] = "test_group_alias";
  (*message)[am::strings::msg_params]["consentedFunctions"][0] =
      *consented_function;

  std::shared_ptr<Command> command =
      CreateCommand<OnAppPermissionConsentNotification>(message);

  int32_t connection_id = -1;
  policy::PermissionConsent permission_consent;
#ifdef EXTERNAL_PROPRIETARY_MODE
  EXPECT_CALL(mock_policy_handler_, OnAppPermissionConsent(_, _, _))
#else
  EXPECT_CALL(mock_policy_handler_, OnAppPermissionConsent(_, _))
#endif
      .WillOnce(
          GetConnectIdPermissionConsent(&connection_id, &permission_consent));
  command->Run();
  EXPECT_EQ(static_cast<int32_t>(kAppId_), connection_id);

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
  MessageSharedPtr message = CreateMessage();
  (*message)[am::strings::msg_params][am::hmi_notification::error] =
      hmi_apis::Common_SystemError::SYNC_REBOOTED;

  std::shared_ptr<Command> command =
      CreateCommand<OnSystemErrorNotification>(message);

  int32_t code = hmi_apis::Common_SystemError::INVALID_ENUM;
  EXPECT_CALL(mock_policy_handler_, OnSystemError(_)).WillOnce(GetArg(&code));
  command->Run();
  EXPECT_EQ(static_cast<int32_t>(hmi_apis::Common_SystemError::SYNC_REBOOTED),
            code);
}

TEST_F(HMICommandsNotificationsTest,
       OnSystemInfoChangedNotificationCheckLanguage) {
  const uint32_t kLangCode = 5u;
  MessageSharedPtr message = CreateMessage();
  (*message)[am::strings::msg_params][am::strings::language] = kLangCode;

  std::shared_ptr<Command> command =
      CreateCommand<OnSystemInfoChangedNotification>(message);

  EXPECT_CALL(mock_policy_handler_, OnSystemInfoChanged(_));
  command->Run();
}

TEST_F(HMICommandsNotificationsTest,
       OnAllowSDLFunctionalityNotificationDeviceKeyNotExist) {
  const std::string kDeviceId = "";
  MessageSharedPtr message = CreateMessage();
  (*message)[am::strings::msg_params][am::hmi_response::allowed] = true;
  std::shared_ptr<Command> command =
      CreateCommand<OnAllowSDLFunctionalityNotification>(message);

  bool value = false;
  std::string str;
  EXPECT_CALL(mock_policy_handler_, OnAllowSDLFunctionalityNotification(_, _))
      .WillOnce(GetBoolValueAndString(&value, &str));
  command->Run();
  EXPECT_EQ(true, value);
  EXPECT_EQ(kDeviceId, str);
}

TEST_F(HMICommandsNotificationsTest,
       OnAllowSDLFunctionalityNotificationDeviceKeyExist) {
  const std::string kDeviceId = "device_id";
  MessageSharedPtr message = CreateMessage();
  (*message)[am::strings::msg_params][am::hmi_response::allowed] = true;
  (*message)[am::strings::msg_params]["device"]["id"] = kDeviceId;
  std::shared_ptr<Command> command =
      CreateCommand<OnAllowSDLFunctionalityNotification>(message);

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
  MessageSharedPtr message = CreateMessage();
  (*message)[am::strings::msg_params]["deviceState"] =
      hmi_apis::Common_DeviceState::INVALID_ENUM;
  std::shared_ptr<Command> command =
      CreateCommand<OnDeviceStateChangedNotification>(message);

  EXPECT_CALL(mock_policy_handler_, RemoveDevice(_)).Times(0);
  command->Run();
}

TEST_F(HMICommandsNotificationsTest,
       OnDeviceStateChangedNotificationDeviceStateUnpaired) {
  // Random MAC adress for test. It must contain 12 symbols.
  const std::string device_id = "AA15F2204D6B";
  MessageSharedPtr message = CreateMessage();
  (*message)[am::strings::msg_params]["deviceState"] =
      hmi_apis::Common_DeviceState::UNPAIRED;
  (*message)[am::strings::msg_params]["deviceInternalId"] = device_id;

  std::shared_ptr<Command> command =
      CreateCommand<OnDeviceStateChangedNotification>(message);

  EXPECT_CALL(mock_policy_handler_, RemoveDevice(_));
  command->Run();
}

TEST_F(HMICommandsNotificationsTest,
       OnDeviceStateChangedNotificationDeviceStateEmptyDeviceId) {
  const std::string empty_device_id = "";
  MessageSharedPtr message = CreateMessage();
  (*message)[am::strings::msg_params]["deviceState"] =
      hmi_apis::Common_DeviceState::UNPAIRED;
  (*message)[am::strings::msg_params]["deviceInternalId"] = empty_device_id;

  std::shared_ptr<Command> command =
      CreateCommand<OnDeviceStateChangedNotification>(message);

  std::string device_id = "default_id";
  EXPECT_CALL(mock_policy_handler_, RemoveDevice(_))
      .WillOnce(GetArg(&device_id));
  command->Run();
  EXPECT_EQ(empty_device_id, device_id);
}

TEST_F(HMICommandsNotificationsTest,
       OnDeviceStateChangedNotificationDeviceStateDeviceIdFromId) {
  const std::string empty_device_id = "";
  const std::string id = "id_string";
  MessageSharedPtr message = CreateMessage();
  (*message)[am::strings::msg_params]["deviceState"] =
      hmi_apis::Common_DeviceState::UNPAIRED;
  (*message)[am::strings::msg_params]["deviceInternalId"] = empty_device_id;
  (*message)[am::strings::msg_params]["deviceId"]["id"] = id;

  std::shared_ptr<Command> command =
      CreateCommand<OnDeviceStateChangedNotification>(message);

  std::string device_id = "default_id";
  EXPECT_CALL(mock_policy_handler_, RemoveDevice(_))
      .WillOnce(GetArg(&device_id));
  command->Run();
  EXPECT_EQ(id, device_id);
}

//~policy_handler
TEST_F(HMICommandsNotificationsTest,
       OnExitAllApplicationsNotificationReasonIgnitionOff) {
  MessageSharedPtr message = CreateMessage();
  (*message)[am::strings::msg_params][am::hmi_request::reason] =
      hmi_apis::Common_ApplicationsCloseReason::IGNITION_OFF;

  std::shared_ptr<Command> command =
      CreateCommand<OnExitAllApplicationsNotification>(message);
#if defined(OS_POSIX)
  am::mobile_api::AppInterfaceUnregisteredReason::eType mob_reason;

  EXPECT_CALL(app_mngr_, SetUnregisterAllApplicationsReason(_))
      .WillOnce(GetArg(&mob_reason));
  EXPECT_CALL(app_mngr_, HeadUnitReset(_)).Times(0);

  SubscribeForSignal();
  command->Run();
  utils::Signals::WaitTerminationSignals(sig_handler);

  EXPECT_EQ(am::mobile_api::AppInterfaceUnregisteredReason::IGNITION_OFF,
            mob_reason);
#endif
}

TEST_F(HMICommandsNotificationsTest,
       OnExitAllApplicationsNotificationReasonMasterResetAndFactoryDefaults) {
  MessageSharedPtr message = CreateMessage();

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
    (*message)[am::strings::msg_params][am::hmi_request::reason] = *it_reason;

    std::shared_ptr<Command> command =
        CreateCommand<OnExitAllApplicationsNotification>(message);
#if defined(OS_POSIX)
    am::mobile_api::AppInterfaceUnregisteredReason::eType mob_reason =
        *it_mob_reason;

    EXPECT_CALL(app_mngr_, SetUnregisterAllApplicationsReason(mob_reason));
    EXPECT_CALL(app_mngr_, HeadUnitReset(mob_reason));

    SubscribeForSignal();
    command->Run();
    utils::Signals::WaitTerminationSignals(sig_handler);
#endif
  }
}

TEST_F(HMICommandsNotificationsTest,
       OnExitAllApplicationsNotificationReasonSuspend) {
  MessageSharedPtr message = CreateMessage();
  (*message)[am::strings::msg_params][am::hmi_request::reason] =
      hmi_apis::Common_ApplicationsCloseReason::SUSPEND;

  std::shared_ptr<Command> command =
      CreateCommand<OnExitAllApplicationsNotification>(message);

  MessageSharedPtr ethalon_message =
      CreateMessage(smart_objects::SmartType_Map);
  (*ethalon_message)[am::strings::params][am::strings::function_id] =
      hmi_apis::FunctionID::BasicCommunication_OnSDLPersistenceComplete;
  (*ethalon_message)[am::strings::params][am::strings::message_type] =
      am::MessageType::kNotification;
  (*ethalon_message)[am::strings::params][am::strings::correlation_id] =
      kCorrelationId_;
  MessageSharedPtr temp_message = CreateMessage();

  resumption_test::MockResumeCtrl mock_resume_ctrl;
  EXPECT_CALL(app_mngr_, resume_controller())
      .WillOnce(ReturnRef(mock_resume_ctrl));
  EXPECT_CALL(mock_resume_ctrl, OnSuspend());

  EXPECT_CALL(app_mngr_, GetNextHMICorrelationID())
      .WillOnce(Return(kCorrelationId_));
  EXPECT_CALL(mock_rpc_service_, ManageHMICommand(_, _))
      .WillOnce(GetMessage(temp_message));

  command->Run();
  EXPECT_EQ(
      static_cast<uint32_t>(
          hmi_apis::FunctionID::BasicCommunication_OnSDLPersistenceComplete),
      (*temp_message)[am::strings::params][am::strings::function_id].asInt());
  EXPECT_EQ(
      static_cast<uint32_t>(am::MessageType::kNotification),
      (*temp_message)[am::strings::params][am::strings::message_type].asInt());
  EXPECT_EQ(static_cast<uint32_t>(kCorrelationId_),
            (*temp_message)[am::strings::params][am::strings::correlation_id]
                .asInt());
}

TEST_F(HMICommandsNotificationsTest,
       OnExitAllApplicationsNotificationReasonInvalidEnum) {
  MessageSharedPtr message = CreateMessage();
  (*message)[am::strings::msg_params][am::hmi_request::reason] =
      hmi_apis::Common_ApplicationsCloseReason::INVALID_ENUM;

  std::shared_ptr<Command> command =
      CreateCommand<OnExitAllApplicationsNotification>(message);

  EXPECT_CALL(app_mngr_, SetUnregisterAllApplicationsReason(_)).Times(0);
  EXPECT_CALL(app_mngr_, HeadUnitReset(_)).Times(0);
  EXPECT_CALL(app_mngr_, GetNextHMICorrelationID()).Times(0);
  EXPECT_CALL(mock_rpc_service_, ManageHMICommand(_, _)).Times(0);
  command->Run();
}

TEST_F(HMICommandsNotificationsTest,
       OnExitApplicationNotificationManageMobileCommand) {
  MessageSharedPtr message = CreateMessage();
  (*message)[am::strings::msg_params][am::strings::app_id] = kAppId_;
  smart_objects::SmartObjectSPtr notification =
      std::make_shared<smart_objects::SmartObject>();
  (*notification)[am::strings::params][am::strings::function_id] =
      static_cast<int32_t>(
          mobile_apis::FunctionID::OnAppInterfaceUnregisteredID);
  (*notification)[am::strings::params][am::strings::message_type] =
      static_cast<int32_t>(am::MessageType::kNotification);
  (*notification)[am::strings::params][am::strings::connection_key] = kAppId_;

  using ExitReason = hmi_apis::Common_ApplicationExitReason::eType;
  std::vector<ExitReason> reason_list = {
      ExitReason::UNAUTHORIZED_TRANSPORT_REGISTRATION,
      ExitReason::UNSUPPORTED_HMI_RESOURCE};

  using UnregisteredReason = mobile_apis::AppInterfaceUnregisteredReason::eType;
  std::vector<UnregisteredReason> mobile_reason_list = {
      UnregisteredReason::APP_UNAUTHORIZED,
      UnregisteredReason::UNSUPPORTED_HMI_RESOURCE};

  std::vector<mobile_apis::AppInterfaceUnregisteredReason::eType>::iterator
      it_mobile_reason = mobile_reason_list.begin();
  std::vector<hmi_apis::Common_ApplicationExitReason::eType>::iterator
      it_reason = reason_list.begin();

  for (; it_reason != reason_list.end(); ++it_reason, ++it_mobile_reason) {
    (*message)[am::strings::msg_params][am::strings::reason] = *it_reason;
    std::shared_ptr<Command> command =
        CreateCommand<OnExitApplicationNotification>(message);

    (*notification)[am::strings::msg_params][am::strings::reason] =
        static_cast<int32_t>(*it_mobile_reason);

    am::plugin_manager::MockRPCPluginManager mock_rpc_plugin_manager_;
    EXPECT_CALL(app_mngr_, GetPluginManager())
        .WillRepeatedly(ReturnRef(mock_rpc_plugin_manager_));

    EXPECT_CALL(app_mngr_, application(kAppId_)).WillRepeatedly(Return(app_));
    EXPECT_CALL(mock_message_helper_,
                GetOnAppInterfaceUnregisteredNotificationToMobile(
                    kAppId_, *it_mobile_reason))
        .WillOnce(Return(notification));
    EXPECT_CALL(mock_rpc_service_,
                ManageMobileCommand(notification, Command::SOURCE_SDL));
    EXPECT_CALL(app_mngr_,
                UnregisterApplication(
                    kAppId_, mobile_apis::Result::SUCCESS, false, false));
    command->Run();
  }
}

TEST_F(HMICommandsNotificationsTest,
       OnExitApplicationNotificationResourceConstraintReason) {
  auto message = CreateMessage();
  (*message)[am::strings::msg_params][am::strings::app_id] = kAppId_;
  const auto notification = std::make_shared<smart_objects::SmartObject>();
  (*notification)[am::strings::params][am::strings::function_id] =
      static_cast<int32_t>(
          mobile_apis::FunctionID::OnAppInterfaceUnregisteredID);
  (*notification)[am::strings::params][am::strings::message_type] =
      static_cast<int32_t>(am::MessageType::kNotification);
  (*notification)[am::strings::params][am::strings::connection_key] = kAppId_;

  using ExitReason = hmi_apis::Common_ApplicationExitReason::eType;
  auto hmi_reason = ExitReason::RESOURCE_CONSTRAINT;

  using UnregisteredReason = mobile_apis::AppInterfaceUnregisteredReason::eType;
  auto mobile_reason = UnregisteredReason::RESOURCE_CONSTRAINT;

  (*message)[am::strings::msg_params][am::strings::reason] = hmi_reason;
  const auto command = CreateCommand<OnExitApplicationNotification>(message);

  (*notification)[am::strings::msg_params][am::strings::reason] =
      static_cast<int32_t>(mobile_reason);

  am::plugin_manager::MockRPCPluginManager mock_rpc_plugin_manager_;
  EXPECT_CALL(app_mngr_, GetPluginManager())
      .WillRepeatedly(ReturnRef(mock_rpc_plugin_manager_));

  EXPECT_CALL(app_mngr_, application(kAppId_)).WillRepeatedly(Return(app_));
  EXPECT_CALL(
      mock_message_helper_,
      GetOnAppInterfaceUnregisteredNotificationToMobile(kAppId_, mobile_reason))
      .WillOnce(Return(notification));
  EXPECT_CALL(mock_rpc_service_,
              ManageMobileCommand(notification, Command::SOURCE_SDL));
  EXPECT_CALL(app_mngr_, UnregisterApplication(_, _, _, _)).Times(0);

  ASSERT_TRUE(command->Init());
  command->Run();
}

TEST_F(HMICommandsNotificationsTest,
       OnExitApplicationNotificationUnhandledReason) {
  MessageSharedPtr message = CreateMessage();
  (*message)[am::strings::msg_params][am::strings::app_id] = kAppId_;

  (*message)[am::strings::msg_params][am::strings::reason] =
      hmi_apis::Common_ApplicationExitReason::INVALID_ENUM;

  std::shared_ptr<Command> command =
      CreateCommand<OnExitApplicationNotification>(message);

  am::plugin_manager::MockRPCPluginManager mock_rpc_plugin_manager_;

  EXPECT_CALL(app_mngr_, application(_)).Times(0);
  EXPECT_CALL(mock_rpc_service_, ManageMobileCommand(_, _)).Times(0);
  EXPECT_CALL(app_mngr_, UnregisterApplication(_, _, _, _)).Times(0);
  EXPECT_CALL(app_mngr_, state_controller()).Times(0);
  EXPECT_CALL(app_mngr_, application(kAppId_)).WillOnce(Return(app_));
  EXPECT_CALL(app_mngr_, GetPluginManager())
      .WillOnce(ReturnRef(mock_rpc_plugin_manager_));
  EXPECT_CALL(mock_rpc_plugin_manager_, ForEachPlugin(_)).Times(1);
  command->Run();
}

TEST_F(HMICommandsNotificationsTest, OnExitApplicationNotificationInvalidApp) {
  MessageSharedPtr message = CreateMessage();
  (*message)[am::strings::msg_params][am::strings::app_id] = kAppId_;

  std::shared_ptr<Command> command =
      CreateCommand<OnExitApplicationNotification>(message);

  am::ApplicationSharedPtr invalid_app;
  EXPECT_CALL(app_mngr_, application(_)).Times(0);
  EXPECT_CALL(mock_rpc_service_, ManageMobileCommand(_, _)).Times(0);
  EXPECT_CALL(app_mngr_, UnregisterApplication(_, _, _, _)).Times(0);
  EXPECT_CALL(app_mngr_, state_controller()).Times(0);
  EXPECT_CALL(app_mngr_, application(kAppId_)).WillOnce(Return(invalid_app));
  command->Run();
}

TEST_F(HMICommandsNotificationsTest,
       DISABLED_OnExitApplicationNotificationDriverDistractionValidApp) {
  MessageSharedPtr message = CreateMessage();
  (*message)[am::strings::msg_params][am::strings::app_id] = kAppId_;
  (*message)[am::strings::msg_params][am::strings::reason] =
      hmi_apis::Common_ApplicationExitReason::DRIVER_DISTRACTION_VIOLATION;
  std::shared_ptr<Command> command =
      CreateCommand<OnExitApplicationNotification>(message);

  EXPECT_CALL(app_mngr_, application(kAppId_)).WillRepeatedly(Return(app_));

  EXPECT_CALL(app_mngr_, state_controller())
      .WillOnce(ReturnRef(mock_state_controller_));
  EXPECT_CALL(mock_state_controller_,
              SetRegularState(app_,
                              kDefaultWindowId,
                              mobile_apis::HMILevel::HMI_NONE,
                              mobile_apis::AudioStreamingState::NOT_AUDIBLE,
                              mobile_apis::VideoStreamingState::NOT_STREAMABLE,
                              false));
  command->Run();
}

TEST_F(HMICommandsNotificationsTest,
       OnExitApplicationNotificationrUserExitValidApp) {
  MessageSharedPtr message = CreateMessage();
  (*message)[am::strings::msg_params][am::strings::app_id] = kAppId_;
  (*message)[am::strings::msg_params][am::strings::reason] =
      hmi_apis::Common_ApplicationExitReason::USER_EXIT;
  std::shared_ptr<Command> command =
      CreateCommand<OnExitApplicationNotification>(message);
  am::plugin_manager::MockRPCPluginManager mock_rpc_plugin_manager_;
  EXPECT_CALL(app_mngr_, GetPluginManager())
      .WillOnce(ReturnRef(mock_rpc_plugin_manager_));
  EXPECT_CALL(mock_rpc_plugin_manager_, ForEachPlugin(_)).Times(1);
  EXPECT_CALL(app_mngr_, application(kAppId_)).WillRepeatedly(Return(app_));
  EXPECT_CALL(mock_rpc_service_, ManageMobileCommand(_, _)).Times(0);
  EXPECT_CALL(app_mngr_, UnregisterApplication(_, _, _, _)).Times(0);
  EXPECT_CALL(app_mngr_, state_controller())
      .WillOnce(ReturnRef(mock_state_controller_));
  EXPECT_CALL(mock_state_controller_, ExitDefaultWindow(app_));
  command->Run();
}

TEST_F(HMICommandsNotificationsTest,
       OnVRCommandNotificationSwitchedAndValidApp) {
  const uint32_t cmd_id = 12u;
  const uint32_t max_cmd_id = 10u;

  MessageSharedPtr message = CreateMessage();
  (*message)[am::strings::msg_params][am::strings::cmd_id] = cmd_id;
  std::shared_ptr<Command> command =
      CreateCommand<OnVRCommandNotification>(message);

  EXPECT_CALL(app_mngr_, application(_)).WillRepeatedly(Return(app_));
  EXPECT_CALL(app_mngr_, state_controller())
      .WillOnce(ReturnRef(mock_state_controller_));
  EXPECT_CALL(mock_state_controller_,
              SetRegularState(
                  _, kDefaultWindowId, mobile_apis::HMILevel::HMI_FULL, true));

  EXPECT_CALL(app_mngr_, get_settings())
      .WillOnce(ReturnRef(app_mngr_settings_));
  EXPECT_CALL(app_mngr_settings_, max_cmd_id()).WillOnce(ReturnRef(max_cmd_id));
  command->Run();
}

TEST_F(HMICommandsNotificationsTest,
       OnVRCommandNotificationSwitchedAndInvalidApp) {
  const uint32_t kCmdId = 12u;
  const uint32_t kMaxCmdId = 10u;

  MessageSharedPtr message = CreateMessage();
  (*message)[am::strings::msg_params][am::strings::cmd_id] = kCmdId;
  std::shared_ptr<Command> command =
      CreateCommand<OnVRCommandNotification>(message);

  am::ApplicationSharedPtr invalid_app;
  EXPECT_CALL(app_mngr_, application(_)).WillRepeatedly(Return(invalid_app));
  EXPECT_CALL(app_mngr_, state_controller()).Times(0);
  EXPECT_CALL(app_mngr_, get_settings())
      .WillOnce(ReturnRef(app_mngr_settings_));
  EXPECT_CALL(app_mngr_settings_, max_cmd_id()).WillOnce(ReturnRef(kMaxCmdId));
  command->Run();
}

TEST_F(HMICommandsNotificationsTest,
       OnVRCommandNotificationCmdIdEqualToMaxCmdId) {
  const uint32_t kCmdId = 11u;
  const uint32_t kMaxCmdId = 10u;

  MessageSharedPtr message = CreateMessage();
  (*message)[am::strings::msg_params][am::strings::cmd_id] = kCmdId;
  std::shared_ptr<Command> command =
      CreateCommand<OnVRCommandNotification>(message);

  EXPECT_CALL(app_mngr_, application(_)).Times(0);
  EXPECT_CALL(app_mngr_settings_, max_cmd_id()).WillOnce(ReturnRef(kMaxCmdId));
  EXPECT_CALL(app_mngr_, get_settings())
      .WillOnce(ReturnRef(app_mngr_settings_));
  command->Run();
}

TEST_F(HMICommandsNotificationsTest,
       OnVRCommandNotificationCmdIdLessMaxCmdIsInvalidApp) {
  const uint32_t kCmdId = 8u;
  const uint32_t kMaxCmdId = 10u;

  MessageSharedPtr message = CreateMessage();
  (*message)[am::strings::msg_params][am::strings::cmd_id] = kCmdId;
  std::shared_ptr<Command> command =
      CreateCommand<OnVRCommandNotification>(message);

  am::ApplicationSharedPtr invalid_app;
  EXPECT_CALL(app_mngr_, application(_)).WillOnce(Return(invalid_app));
  EXPECT_CALL(app_mngr_settings_, max_cmd_id()).WillOnce(ReturnRef(kMaxCmdId));
  EXPECT_CALL(app_mngr_, get_settings())
      .WillOnce(ReturnRef(app_mngr_settings_));
  EXPECT_CALL(app_mngr_, event_dispatcher()).Times(0);
  EXPECT_CALL(mock_rpc_service_, ManageMobileCommand(_, _)).Times(0);
  command->Run();
}

TEST_F(HMICommandsNotificationsTest,
       OnVRCommandNotificationActivePerformIteraction) {
  const uint32_t kCmdId = 8u;
  const uint32_t kMaxCmdId = 10u;
  const uint32_t kIsPerformInteractionActive = 1u;
  int32_t event_id = hmi_apis::FunctionID::INVALID_ENUM;
  MessageSharedPtr message = CreateMessage();
  (*message)[am::strings::msg_params][am::strings::cmd_id] = kCmdId;
  std::shared_ptr<Command> command =
      CreateCommand<OnVRCommandNotification>(message);

  EXPECT_CALL(app_mngr_, application(_)).WillOnce(Return(app_));
  EXPECT_CALL(app_mngr_settings_, max_cmd_id()).WillOnce(ReturnRef(kMaxCmdId));
  EXPECT_CALL(app_mngr_, get_settings())
      .WillOnce(ReturnRef(app_mngr_settings_));
  EXPECT_CALL(*app_ptr_, is_perform_interaction_active())
      .WillOnce(Return(kIsPerformInteractionActive));
  EXPECT_CALL(app_mngr_, event_dispatcher());
  EXPECT_CALL(mock_event_dispatcher_, raise_event(_))
      .WillOnce(GetEventId(&event_id));
  EXPECT_CALL(mock_rpc_service_, ManageMobileCommand(_, _)).Times(0);
  command->Run();
  EXPECT_EQ(hmi_apis::FunctionID::VR_OnCommand, event_id);
}

TEST_F(HMICommandsNotificationsTest,
       OnVRCommandNotificationNotActivePerformIteraction) {
  const uint32_t kCmdId = 8u;
  const uint32_t kMaxCmdId = 10u;
  const uint32_t kIsPerformInteractionActive = 0u;
  MessageSharedPtr message = CreateMessage();
  (*message)[am::strings::msg_params][am::strings::cmd_id] = kCmdId;
  (*message)[am::strings::msg_params][am::strings::function_id] =
      mobile_apis::FunctionID::eType::OnCommandID;
  std::shared_ptr<Command> command =
      CreateCommand<OnVRCommandNotification>(message);

  EXPECT_CALL(app_mngr_, application(_)).WillOnce(Return(app_));
  EXPECT_CALL(app_mngr_settings_, max_cmd_id()).WillOnce(ReturnRef(kMaxCmdId));
  EXPECT_CALL(app_mngr_, get_settings())
      .WillOnce(ReturnRef(app_mngr_settings_));
  EXPECT_CALL(*app_ptr_, is_perform_interaction_active())
      .WillOnce(Return(kIsPerformInteractionActive));

  EXPECT_CALL(app_mngr_, event_dispatcher()).Times(0);
  EXPECT_CALL(mock_rpc_service_,
              ManageMobileCommand(_, Command::CommandSource::SOURCE_SDL));
  command->Run();
  EXPECT_EQ(static_cast<int32_t>(mobile_apis::FunctionID::eType::OnCommandID),
            (*message)[am::strings::params][am::strings::function_id].asInt());
  EXPECT_EQ(
      static_cast<int32_t>(mobile_apis::TriggerSource::TS_VR),
      (*message)[am::strings::msg_params][am::strings::trigger_source].asInt());
  EXPECT_EQ(static_cast<int32_t>(am::MessageType::kNotification),
            (*message)[am::strings::params][am::strings::message_type].asInt());
}

TEST_F(HMICommandsNotificationsTest, OnVRLanguageChangeNotificationEmptyData) {
  const mobile_apis::Language::eType& kLang = mobile_apis::Language::EN_GB;
  MessageSharedPtr message = CreateMessage();
  (*message)[am::strings::msg_params][am::strings::language] = kLang;
  std::shared_ptr<Command> command =
      CreateCommand<OnVRLanguageChangeNotification>(message);

  EXPECT_CALL(mock_hmi_capabilities_, active_ui_language())
      .WillOnce(Return(hmi_apis::Common_Language::EN_AU));
  EXPECT_CALL(mock_hmi_capabilities_, set_active_vr_language(_));
  EXPECT_CALL(app_mngr_, applications()).WillOnce(Return(applications_));
  EXPECT_CALL(mock_rpc_service_, ManageMobileCommand(_, _)).Times(0);
  EXPECT_CALL(*app_ptr_, app_id()).Times(0);
  EXPECT_CALL(*app_ptr_, language()).Times(0);
  command->Run();
}

TEST_F(HMICommandsNotificationsTest,
       OnVRLanguageChangeNotificationAppLangEqualMessageLang) {
  const mobile_apis::Language::eType& kLang = mobile_apis::Language::EN_GB;
  MessageSharedPtr message = CreateMessage();
  (*message)[am::strings::msg_params][am::strings::language] = kLang;
  std::shared_ptr<Command> command =
      CreateCommand<OnVRLanguageChangeNotification>(message);

  application_set_.insert(app_);
  EXPECT_CALL(app_mngr_, applications()).WillOnce(Return(applications_));
  EXPECT_CALL(mock_hmi_capabilities_, active_ui_language())
      .WillOnce(Return(hmi_apis::Common_Language::EN_AU));
  EXPECT_CALL(mock_hmi_capabilities_, set_active_vr_language(_));
  EXPECT_CALL(mock_rpc_service_,
              ManageMobileCommand(_, Command::CommandSource::SOURCE_SDL));
  EXPECT_CALL(*app_ptr_, app_id()).WillOnce(Return(kAppId_));
  EXPECT_CALL(*app_ptr_, language()).WillRepeatedly(ReturnRef(kLang));

  command->Run();
  EXPECT_EQ(static_cast<int32_t>(mobile_apis::FunctionID::OnLanguageChangeID),
            (*message)[am::strings::params][am::strings::function_id].asInt());
  EXPECT_EQ(
      static_cast<int32_t>(kAppId_),
      (*message)[am::strings::params][am::strings::connection_key].asInt());
  EXPECT_EQ(static_cast<int32_t>(am::MessageType::kNotification),
            (*message)[am::strings::params][am::strings::message_type].asInt());
  EXPECT_EQ(
      static_cast<int32_t>(hmi_apis::Common_Language::EN_AU),
      (*message)[am::strings::msg_params][am::strings::hmi_display_language]
          .asInt());
}

TEST_F(HMICommandsNotificationsTest,
       OnVRLanguageChangeNotificationAppLangNotEqualMessageLang) {
  const mobile_apis::Language::eType& kLang = mobile_apis::Language::EN_GB;
  MessageSharedPtr message = CreateMessage();
  (*message)[am::strings::msg_params][am::strings::language] =
      mobile_apis::Language::EN_US;
  std::shared_ptr<Command> command =
      CreateCommand<OnVRLanguageChangeNotification>(message);

  application_set_.insert(app_);
  smart_objects::SmartObjectSPtr notification =
      std::make_shared<smart_objects::SmartObject>();
  (*notification)[am::strings::params][am::strings::function_id] =
      static_cast<int32_t>(mobile_apis::FunctionID::OnLanguageChangeID);
  (*notification)[am::strings::params][am::strings::message_type] =
      static_cast<int32_t>(am::MessageType::kNotification);
  (*notification)[am::strings::params][am::strings::connection_key] = kAppId_;
  (*notification)[am::strings::msg_params][am::strings::reason] =
      static_cast<int32_t>(
          mobile_apis::AppInterfaceUnregisteredReason::LANGUAGE_CHANGE);

  EXPECT_CALL(app_mngr_, applications()).WillOnce(Return(applications_));
  EXPECT_CALL(mock_hmi_capabilities_, active_ui_language())
      .WillOnce(Return(hmi_apis::Common_Language::EN_AU));
  EXPECT_CALL(mock_hmi_capabilities_, set_active_vr_language(_));
  EXPECT_CALL(mock_rpc_service_,
              ManageMobileCommand(_, Command::CommandSource::SOURCE_SDL));
  EXPECT_CALL(*app_ptr_, app_id()).WillRepeatedly(Return(kAppId_));
  EXPECT_CALL(*app_ptr_, language()).WillRepeatedly(ReturnRef(kLang));
  EXPECT_CALL(app_mngr_, state_controller())
      .WillOnce(ReturnRef(mock_state_controller_));
  EXPECT_CALL(
      mock_state_controller_,
      SetRegularState(
          app_, kDefaultWindowId, mobile_apis::HMILevel::HMI_NONE, false));
  EXPECT_CALL(mock_message_helper_,
              GetOnAppInterfaceUnregisteredNotificationToMobile(
                  kAppId_,
                  mobile_apis::AppInterfaceUnregisteredReason::LANGUAGE_CHANGE))
      .WillOnce(Return(notification));
  EXPECT_CALL(mock_rpc_service_,
              ManageMobileCommand(notification, Command::SOURCE_SDL));
  EXPECT_CALL(app_mngr_,
              UnregisterApplication(
                  kAppId_, mobile_apis::Result::SUCCESS, false, false));
  command->Run();
  EXPECT_EQ(static_cast<int32_t>(mobile_apis::FunctionID::OnLanguageChangeID),
            (*message)[am::strings::params][am::strings::function_id].asInt());
  EXPECT_EQ(
      static_cast<int32_t>(kAppId_),
      (*message)[am::strings::params][am::strings::connection_key].asInt());
  EXPECT_EQ(static_cast<int32_t>(am::MessageType::kNotification),
            (*message)[am::strings::params][am::strings::message_type].asInt());
  EXPECT_EQ(
      static_cast<int32_t>(hmi_apis::Common_Language::EN_AU),
      (*message)[am::strings::msg_params][am::strings::hmi_display_language]
          .asInt());
}

TEST_F(HMICommandsNotificationsTest, OnStartDeviceDiscoveryRun) {
  MessageSharedPtr message = CreateMessage();
  std::shared_ptr<Command> command =
      CreateCommand<OnStartDeviceDiscovery>(message);
  EXPECT_CALL(app_mngr_, StartDevicesDiscovery());
  command->Run();
}

TEST_F(HMICommandsNotificationsTest,
       OnDeviceChosenNotificationDeviceInfoExists) {
  MessageSharedPtr message = CreateMessage();
  (*message)[am::strings::msg_params][am::strings::device_info]
            [am::strings::id] = "2014";
  std::shared_ptr<Command> command =
      CreateCommand<OnDeviceChosenNotification>(message);
  EXPECT_CALL(
      app_mngr_,
      ConnectToDevice((*message)[am::strings::msg_params]
                                [am::strings::device_info][am::strings::id]
                                    .asString()));
  command->Run();
}

TEST_F(HMICommandsNotificationsTest,
       OnDeviceChosenNotificationDeviceInfoNotExists) {
  MessageSharedPtr message = CreateMessage();
  std::shared_ptr<Command> command =
      CreateCommand<OnDeviceChosenNotification>(message);
  EXPECT_CALL(app_mngr_, ConnectToDevice(_)).Times(0);
  command->Run();
}

TEST_F(HMICommandsNotificationsTest,
       OnSystemContextNotificationValidActiveApplication) {
  MessageSharedPtr message = CreateMessage();
  std::vector<am::mobile_api::SystemContext::eType> system_context_list;
  system_context_list.push_back(
      am::mobile_api::SystemContext::SYSCTXT_VRSESSION);
  system_context_list.push_back(am::mobile_api::SystemContext::SYSCTXT_MENU);
  system_context_list.push_back(
      am::mobile_api::SystemContext::SYSCTXT_HMI_OBSCURED);

  std::vector<am::mobile_api::SystemContext::eType>::iterator it =
      system_context_list.begin();
  for (; it != system_context_list.end(); ++it) {
    (*message)[am::strings::msg_params][am::hmi_notification::system_context] =
        *it;
    std::shared_ptr<Command> command =
        CreateCommand<OnSystemContextNotification>(message);
    EXPECT_CALL(app_mngr_, active_application()).WillOnce(Return(app_));
    EXPECT_CALL(app_mngr_, state_controller())
        .WillOnce(ReturnRef(mock_state_controller_));
    EXPECT_CALL(mock_state_controller_,
                SetRegularState(app_, kDefaultWindowId, *it));
    command->Run();
  }
}

TEST_F(HMICommandsNotificationsTest,
       OnSystemContextNotificationInvalidActiveApplication) {
  MessageSharedPtr message = CreateMessage();
  (*message)[am::strings::msg_params][am::hmi_notification::system_context] =
      am::mobile_api::SystemContext::SYSCTXT_VRSESSION;
  std::shared_ptr<Command> command =
      CreateCommand<OnSystemContextNotification>(message);
  ApplicationSharedPtr invalid_app;
  EXPECT_CALL(app_mngr_, active_application()).WillOnce(Return(invalid_app));
  EXPECT_CALL(app_mngr_, state_controller()).Times(0);
  command->Run();
}

TEST_F(HMICommandsNotificationsTest,
       OnSystemContextNotificationInvalidSystemContext) {
  MessageSharedPtr message = CreateMessage();
  (*message)[am::strings::msg_params][am::hmi_notification::system_context] =
      am::mobile_api::SystemContext::INVALID_ENUM;
  std::shared_ptr<Command> command =
      CreateCommand<OnSystemContextNotification>(message);
  EXPECT_CALL(app_mngr_, active_application()).Times(0);
  EXPECT_CALL(app_mngr_, application(_)).Times(0);
  EXPECT_CALL(app_mngr_, state_controller()).Times(0);
  command->Run();
}

TEST_F(HMICommandsNotificationsTest,
       OnSystemContextNotificationValidApplication) {
  MessageSharedPtr message = CreateMessage();
  (*message)[am::strings::msg_params][am::strings::app_id] = kAppId_;
  std::vector<am::mobile_api::SystemContext::eType> system_context_list;
  system_context_list.push_back(am::mobile_api::SystemContext::SYSCTXT_ALERT);
  system_context_list.push_back(am::mobile_api::SystemContext::SYSCTXT_MAIN);

  std::vector<am::mobile_api::SystemContext::eType>::iterator it =
      system_context_list.begin();
  for (; it != system_context_list.end(); ++it) {
    (*message)[am::strings::msg_params][am::hmi_notification::system_context] =
        *it;
    std::shared_ptr<Command> command =
        CreateCommand<OnSystemContextNotification>(message);
    EXPECT_CALL(app_mngr_, application(_)).WillOnce(Return(app_));
    EXPECT_CALL(app_mngr_, state_controller())
        .WillOnce(ReturnRef(mock_state_controller_));
    EXPECT_CALL(mock_state_controller_,
                SetRegularState(app_, kDefaultWindowId, *it));
    command->Run();
  }
}

TEST_F(HMICommandsNotificationsTest,
       OnSystemContextNotificationAppIdDoesntExists) {
  MessageSharedPtr message = CreateMessage();
  (*message)[am::strings::msg_params][am::hmi_notification::system_context] =
      am::mobile_api::SystemContext::SYSCTXT_ALERT;
  std::shared_ptr<Command> command =
      CreateCommand<OnSystemContextNotification>(message);
  EXPECT_CALL(app_mngr_, application(_)).Times(0);
  EXPECT_CALL(app_mngr_, state_controller()).Times(0);
  command->Run();
}

TEST_F(HMICommandsNotificationsTest,
       OnSystemRequestNotificationAppIdExistsAndValidApp) {
  MessageSharedPtr message = CreateMessage();
  (*message)[am::strings::msg_params][am::strings::app_id] = kAppId_;
  std::shared_ptr<Command> command =
      CreateCommand<OnSystemRequestNotification>(message);

  EXPECT_CALL(app_mngr_, application(kAppId_)).WillOnce(Return(app_));
  ON_CALL(app_mngr_, connection_handler())
      .WillByDefault(ReturnRef(mock_connection_handler_));
  ON_CALL(mock_connection_handler_, get_session_observer())
      .WillByDefault(ReturnRef(mock_session_observer_));
  const int32_t device_id = 1;
  ON_CALL(
      mock_session_observer_,
      GetDataOnDeviceID(
          testing::An<transport_manager::DeviceHandle>(), NULL, NULL, _, NULL))
      .WillByDefault(Return(device_id));

  EXPECT_CALL(mock_policy_handler_, GetUserConsentForDevice(_))
      .WillOnce(Return(policy::kDeviceAllowed));
  EXPECT_CALL(mock_rpc_service_,
              ManageMobileCommand(_, Command::CommandSource::SOURCE_SDL));
  command->Run();
  EXPECT_EQ(static_cast<int32_t>(am::MessageType::kNotification),
            (*message)[am::strings::params][am::strings::message_type].asInt());
  EXPECT_EQ(
      static_cast<int32_t>(mobile_apis::FunctionID::eType::OnSystemRequestID),
      (*message)[am::strings::params][am::strings::function_id].asInt());
  EXPECT_EQ(
      static_cast<int32_t>(kAppId_),
      (*message)[am::strings::params][am::strings::connection_key].asInt());
}

TEST_F(HMICommandsNotificationsTest,
       OnSystemRequestNotificationAppIdExistsAndInvalidApp) {
  MessageSharedPtr message = CreateMessage();
  (*message)[am::strings::msg_params][am::strings::app_id] = kAppId_;
  std::shared_ptr<Command> command =
      CreateCommand<OnSystemRequestNotification>(message);
  ApplicationSharedPtr invalid_app;
  EXPECT_CALL(app_mngr_, application(kAppId_)).WillOnce(Return(invalid_app));
  EXPECT_CALL(*app_ptr_, app_id()).Times(0);
  EXPECT_CALL(mock_rpc_service_, ManageMobileCommand(_, _)).Times(0);
  command->Run();
  EXPECT_EQ(
      static_cast<int32_t>(mobile_apis::FunctionID::eType::OnSystemRequestID),
      (*message)[am::strings::params][am::strings::function_id].asInt());
}

TEST_F(HMICommandsNotificationsTest,
       OnSystemRequestNotificationAppIdDoesntExistsAndValidApp) {
  MessageSharedPtr message = CreateMessage();
  std::shared_ptr<Command> command =
      CreateCommand<OnSystemRequestNotification>(message);

  EXPECT_CALL(mock_policy_handler_, GetAppIdForSending())
      .WillOnce(Return(kAppId_));
  EXPECT_CALL(app_mngr_, application(_)).WillOnce(Return(app_));
  ON_CALL(app_mngr_, connection_handler())
      .WillByDefault(ReturnRef(mock_connection_handler_));
  ON_CALL(mock_connection_handler_, get_session_observer())
      .WillByDefault(ReturnRef(mock_session_observer_));
  const int32_t device_id = 1;
  ON_CALL(
      mock_session_observer_,
      GetDataOnDeviceID(
          testing::An<transport_manager::DeviceHandle>(), NULL, NULL, _, NULL))
      .WillByDefault(Return(device_id));

  EXPECT_CALL(mock_policy_handler_, GetUserConsentForDevice(_))
      .WillOnce(Return(policy::kDeviceAllowed));
  EXPECT_CALL(mock_rpc_service_,
              ManageMobileCommand(_, Command::CommandSource::SOURCE_SDL));
  command->Run();
  EXPECT_EQ(static_cast<int32_t>(am::MessageType::kNotification),
            (*message)[am::strings::params][am::strings::message_type].asInt());
  EXPECT_EQ(
      static_cast<int32_t>(mobile_apis::FunctionID::eType::OnSystemRequestID),
      (*message)[am::strings::params][am::strings::function_id].asInt());
  EXPECT_EQ(
      static_cast<int32_t>(kAppId_),
      (*message)[am::strings::params][am::strings::connection_key].asInt());
}

TEST_F(HMICommandsNotificationsTest,
       OnSystemRequestNotificationAppIdDoesntExistsAndNullAppId) {
  const uint32_t kNullApppId = 0u;
  MessageSharedPtr message = CreateMessage();
  std::shared_ptr<Command> command =
      CreateCommand<OnSystemRequestNotification>(message);

  EXPECT_CALL(mock_policy_handler_, GetAppIdForSending())
      .WillOnce(Return(kNullApppId));
  EXPECT_CALL(app_mngr_, application(_)).Times(0);
  EXPECT_CALL(mock_rpc_service_, ManageMobileCommand(_, _)).Times(0);
  command->Run();
}

TEST_F(HMICommandsNotificationsTest, OnTTSLanguageChangeNotificationEmptyData) {
  const mobile_apis::Language::eType& kLang = mobile_apis::Language::EN_GB;
  MessageSharedPtr message = CreateMessage();
  (*message)[am::strings::msg_params][am::strings::language] = kLang;
  std::shared_ptr<Command> command =
      CreateCommand<OnTTSLanguageChangeNotification>(message);

  EXPECT_CALL(mock_hmi_capabilities_, set_active_tts_language(_));
  EXPECT_CALL(mock_hmi_capabilities_, set_active_vr_language(_));
  EXPECT_CALL(mock_hmi_capabilities_, active_ui_language())
      .WillOnce(Return(hmi_apis::Common_Language::EN_AU));
  EXPECT_CALL(app_mngr_, applications()).WillOnce(Return(applications_));
  EXPECT_CALL(mock_rpc_service_, ManageMobileCommand(_, _)).Times(0);
  EXPECT_CALL(*app_ptr_, app_id()).Times(0);
  EXPECT_CALL(*app_ptr_, language()).Times(0);
  command->Run();
}

TEST_F(HMICommandsNotificationsTest,
       OnTTSLanguageChangeNotificationAppLangEqualMessageLang) {
  const mobile_apis::Language::eType& kLang = mobile_apis::Language::EN_GB;
  MessageSharedPtr message = CreateMessage();
  (*message)[am::strings::msg_params][am::strings::language] = kLang;
  std::shared_ptr<Command> command =
      CreateCommand<OnTTSLanguageChangeNotification>(message);

  application_set_.insert(app_);
  EXPECT_CALL(app_mngr_, applications()).WillOnce(Return(applications_));
  EXPECT_CALL(mock_hmi_capabilities_, active_ui_language())
      .WillOnce(Return(hmi_apis::Common_Language::EN_AU));
  EXPECT_CALL(mock_hmi_capabilities_, set_active_vr_language(_));
  EXPECT_CALL(mock_hmi_capabilities_, set_active_tts_language(_));
  EXPECT_CALL(mock_rpc_service_,
              ManageMobileCommand(_, Command::CommandSource::SOURCE_SDL));
  EXPECT_CALL(*app_ptr_, app_id()).WillOnce(Return(kAppId_));
  EXPECT_CALL(*app_ptr_, language()).WillRepeatedly(ReturnRef(kLang));

  command->Run();
  EXPECT_EQ(static_cast<int32_t>(mobile_apis::FunctionID::OnLanguageChangeID),
            (*message)[am::strings::params][am::strings::function_id].asInt());
  EXPECT_EQ(
      static_cast<int32_t>(kAppId_),
      (*message)[am::strings::params][am::strings::connection_key].asInt());
  EXPECT_EQ(static_cast<int32_t>(am::MessageType::kNotification),
            (*message)[am::strings::params][am::strings::message_type].asInt());
  EXPECT_EQ(
      static_cast<int32_t>(hmi_apis::Common_Language::EN_AU),
      (*message)[am::strings::msg_params][am::strings::hmi_display_language]
          .asInt());
}

TEST_F(HMICommandsNotificationsTest,
       OnTTSLanguageChangeNotificationAppLangNotEqualMessageLang) {
  const mobile_apis::Language::eType& kLang = mobile_apis::Language::EN_GB;
  MessageSharedPtr message = CreateMessage();
  (*message)[am::strings::msg_params][am::strings::language] =
      mobile_apis::Language::EN_US;
  std::shared_ptr<Command> command =
      CreateCommand<OnTTSLanguageChangeNotification>(message);

  application_set_.insert(app_);
  smart_objects::SmartObjectSPtr notification =
      std::make_shared<smart_objects::SmartObject>();
  (*notification)[am::strings::params][am::strings::function_id] =
      static_cast<int32_t>(mobile_apis::FunctionID::OnLanguageChangeID);
  (*notification)[am::strings::params][am::strings::message_type] =
      static_cast<int32_t>(am::MessageType::kNotification);
  (*notification)[am::strings::params][am::strings::connection_key] = kAppId_;
  (*notification)[am::strings::msg_params][am::strings::reason] =
      static_cast<int32_t>(
          mobile_apis::AppInterfaceUnregisteredReason::LANGUAGE_CHANGE);

  EXPECT_CALL(app_mngr_, applications()).WillOnce(Return(applications_));
  EXPECT_CALL(mock_hmi_capabilities_, active_ui_language())
      .WillOnce(Return(hmi_apis::Common_Language::EN_AU));
  EXPECT_CALL(mock_hmi_capabilities_, set_active_vr_language(_));
  EXPECT_CALL(mock_hmi_capabilities_, set_active_tts_language(_));
  EXPECT_CALL(mock_rpc_service_,
              ManageMobileCommand(_, Command::CommandSource::SOURCE_SDL));
  EXPECT_CALL(*app_ptr_, app_id()).WillRepeatedly(Return(kAppId_));
  EXPECT_CALL(*app_ptr_, language()).WillRepeatedly(ReturnRef(kLang));
  EXPECT_CALL(mock_message_helper_,
              GetOnAppInterfaceUnregisteredNotificationToMobile(
                  kAppId_,
                  mobile_apis::AppInterfaceUnregisteredReason::LANGUAGE_CHANGE))
      .WillOnce(Return(notification));
  EXPECT_CALL(mock_rpc_service_,
              ManageMobileCommand(notification, Command::SOURCE_SDL));
  EXPECT_CALL(app_mngr_,
              UnregisterApplication(
                  kAppId_, mobile_apis::Result::SUCCESS, false, false));
  command->Run();
  EXPECT_EQ(static_cast<int32_t>(mobile_apis::FunctionID::OnLanguageChangeID),
            (*message)[am::strings::params][am::strings::function_id].asInt());
  EXPECT_EQ(
      static_cast<int32_t>(kAppId_),
      (*message)[am::strings::params][am::strings::connection_key].asInt());
  EXPECT_EQ(static_cast<int32_t>(am::MessageType::kNotification),
            (*message)[am::strings::params][am::strings::message_type].asInt());
  EXPECT_EQ(
      static_cast<int32_t>(hmi_apis::Common_Language::EN_AU),
      (*message)[am::strings::msg_params][am::strings::hmi_display_language]
          .asInt());
}

TEST_F(HMICommandsNotificationsTest, OnUILanguageChangeNotificationEmptyData) {
  const mobile_apis::Language::eType& kLang = mobile_apis::Language::EN_GB;
  MessageSharedPtr message = CreateMessage();
  (*message)[am::strings::msg_params][am::strings::language] = kLang;
  std::shared_ptr<Command> command =
      CreateCommand<OnUILanguageChangeNotification>(message);

  EXPECT_CALL(mock_hmi_capabilities_, set_active_ui_language(_));
  EXPECT_CALL(mock_hmi_capabilities_, active_vr_language())
      .WillOnce(Return(hmi_apis::Common_Language::EN_AU));
  EXPECT_CALL(app_mngr_, applications()).WillOnce(Return(applications_));
  EXPECT_CALL(mock_rpc_service_, ManageMobileCommand(_, _)).Times(0);
  EXPECT_CALL(*app_ptr_, app_id()).Times(0);
  EXPECT_CALL(*app_ptr_, ui_language()).Times(0);
  command->Run();
}

TEST_F(HMICommandsNotificationsTest,
       OnUILanguageChangeNotificationAppLangEqualMessageLang) {
  const mobile_apis::Language::eType& kLang = mobile_apis::Language::EN_GB;
  MessageSharedPtr message = CreateMessage();
  (*message)[am::strings::msg_params][am::strings::language] = kLang;
  std::shared_ptr<Command> command =
      CreateCommand<OnUILanguageChangeNotification>(message);

  application_set_.insert(app_);
  EXPECT_CALL(app_mngr_, applications()).WillOnce(Return(applications_));
  EXPECT_CALL(mock_hmi_capabilities_, active_vr_language())
      .WillOnce(Return(hmi_apis::Common_Language::EN_AU));
  EXPECT_CALL(mock_hmi_capabilities_, set_active_ui_language(_));
  EXPECT_CALL(mock_rpc_service_,
              ManageMobileCommand(_, Command::CommandSource::SOURCE_SDL));
  EXPECT_CALL(*app_ptr_, app_id()).WillOnce(Return(kAppId_));
  EXPECT_CALL(*app_ptr_, ui_language()).WillRepeatedly(ReturnRef(kLang));

  command->Run();
  EXPECT_EQ(static_cast<int32_t>(mobile_apis::FunctionID::OnLanguageChangeID),
            (*message)[am::strings::params][am::strings::function_id].asInt());
  EXPECT_EQ(
      static_cast<int32_t>(kAppId_),
      (*message)[am::strings::params][am::strings::connection_key].asInt());
  EXPECT_EQ(static_cast<int32_t>(am::MessageType::kNotification),
            (*message)[am::strings::params][am::strings::message_type].asInt());
  EXPECT_EQ(
      static_cast<int32_t>(kLang),
      (*message)[am::strings::msg_params][am::strings::hmi_display_language]
          .asInt());
}

TEST_F(HMICommandsNotificationsTest,
       OnUILanguageChangeNotificationAppLangNotEqualMessageLang) {
  const mobile_apis::Language::eType& kLang = mobile_apis::Language::EN_GB;
  MessageSharedPtr message = CreateMessage();
  (*message)[am::strings::msg_params][am::strings::language] =
      mobile_apis::Language::EN_US;
  std::shared_ptr<Command> command =
      CreateCommand<OnUILanguageChangeNotification>(message);

  application_set_.insert(app_);
  smart_objects::SmartObjectSPtr notification =
      std::make_shared<smart_objects::SmartObject>();
  (*notification)[am::strings::params][am::strings::function_id] =
      static_cast<int32_t>(mobile_apis::FunctionID::OnLanguageChangeID);
  (*notification)[am::strings::params][am::strings::message_type] =
      static_cast<int32_t>(am::MessageType::kNotification);
  (*notification)[am::strings::params][am::strings::connection_key] = kAppId_;
  (*notification)[am::strings::msg_params][am::strings::reason] =
      static_cast<int32_t>(
          mobile_apis::AppInterfaceUnregisteredReason::LANGUAGE_CHANGE);

  EXPECT_CALL(app_mngr_, applications()).WillOnce(Return(applications_));
  EXPECT_CALL(mock_hmi_capabilities_, active_vr_language())
      .WillOnce(Return(hmi_apis::Common_Language::EN_AU));
  EXPECT_CALL(mock_hmi_capabilities_, set_active_ui_language(_));
  EXPECT_CALL(mock_rpc_service_,
              ManageMobileCommand(_, Command::CommandSource::SOURCE_SDL));
  EXPECT_CALL(*app_ptr_, app_id()).WillRepeatedly(Return(kAppId_));
  EXPECT_CALL(*app_ptr_, ui_language()).WillRepeatedly(ReturnRef(kLang));
  EXPECT_CALL(mock_message_helper_,
              GetOnAppInterfaceUnregisteredNotificationToMobile(
                  kAppId_,
                  mobile_apis::AppInterfaceUnregisteredReason::LANGUAGE_CHANGE))
      .WillOnce(Return(notification));
  EXPECT_CALL(mock_rpc_service_,
              ManageMobileCommand(notification, Command::SOURCE_SDL));
  EXPECT_CALL(app_mngr_,
              UnregisterApplication(
                  kAppId_, mobile_apis::Result::SUCCESS, false, false));
  command->Run();
  EXPECT_EQ(static_cast<int32_t>(mobile_apis::FunctionID::OnLanguageChangeID),
            (*message)[am::strings::params][am::strings::function_id].asInt());
  EXPECT_EQ(
      static_cast<int32_t>(kAppId_),
      (*message)[am::strings::params][am::strings::connection_key].asInt());
  EXPECT_EQ(static_cast<int32_t>(am::MessageType::kNotification),
            (*message)[am::strings::params][am::strings::message_type].asInt());
  EXPECT_EQ(
      static_cast<int32_t>(mobile_apis::Language::EN_US),
      (*message)[am::strings::msg_params][am::strings::hmi_display_language]
          .asInt());
}

TEST_F(HMICommandsNotificationsTest, OnDriverDistractionNotificationEmptyData) {
  const hmi_apis::Common_DriverDistractionState::eType state =
      hmi_apis::Common_DriverDistractionState::DD_ON;
  MessageSharedPtr message = CreateMessage();
  (*message)[am::strings::msg_params][am::hmi_notification::state] = state;
  (*message)[am::strings::params][am::strings::connection_key] = kConnectionKey;
  std::shared_ptr<Command> command =
      CreateCommand<hmi::OnDriverDistractionNotification>(message);

  EXPECT_CALL(app_mngr_, set_driver_distraction_state(state));

  ON_CALL(app_mngr_, GetPolicyHandler())
      .WillByDefault(ReturnRef(mock_policy_handler_));
  typedef boost::optional<bool> OptionalBool;

  ON_CALL(mock_policy_handler_, LockScreenDismissalEnabledState())
      .WillByDefault(Return(OptionalBool(true)));
  std::string required_language = "EN-US";
  ON_CALL(mock_policy_handler_,
          LockScreenDismissalWarningMessage(required_language))
      .WillByDefault(Return(
          boost::optional<std::string>(kLockScreenDismissalWarningMessage_en)));

  ON_CALL(app_mngr_, applications()).WillByDefault(Return(applications_));

  EXPECT_CALL(mock_rpc_service_, ManageMobileCommand(_, _)).Times(0);
  EXPECT_CALL(*app_ptr_, app_id()).Times(0);
  command->Run();
}

TEST_F(HMICommandsNotificationsTest,
       OnDriverDistractionNotificationInvalidApp) {
  const auto state = hmi_apis::Common_DriverDistractionState::DD_ON;
  MessageSharedPtr message = CreateMessage();
  (*message)[am::strings::msg_params][am::hmi_notification::state] = state;
  (*message)[am::strings::params][am::strings::connection_key] = kConnectionKey;
  std::shared_ptr<Command> command =
      CreateCommand<hmi::OnDriverDistractionNotification>(message);

  ApplicationSharedPtr invalid_app;
  application_set_.insert(invalid_app);

  ON_CALL(app_mngr_, GetPolicyHandler())
      .WillByDefault(ReturnRef(mock_policy_handler_));
  typedef boost::optional<bool> OptionalBool;
  ON_CALL(mock_policy_handler_, LockScreenDismissalEnabledState())
      .WillByDefault(Return(OptionalBool(true)));
  std::string required_language = "EN-US";
  ON_CALL(mock_policy_handler_,
          LockScreenDismissalWarningMessage(required_language))
      .WillByDefault(Return(
          boost::optional<std::string>(kLockScreenDismissalWarningMessage_en)));
  ON_CALL(app_mngr_, applications()).WillByDefault(Return(applications_));

  EXPECT_CALL(mock_rpc_service_, ManageMobileCommand(_, _)).Times(0);
  EXPECT_CALL(*app_ptr_, app_id()).Times(0);
  command->Run();
}

TEST_F(HMICommandsNotificationsTest, OnDriverDistractionNotificationValidApp) {
  const hmi_apis::Common_DriverDistractionState::eType state =
      hmi_apis::Common_DriverDistractionState::DD_ON;
  MessageSharedPtr message = CreateMessage();
  (*message)[am::strings::msg_params][am::mobile_notification::state] = state;
  (*message)[am::strings::params][am::strings::connection_key] = kConnectionKey;
  std::shared_ptr<Command> command =
      CreateCommand<hmi::OnDriverDistractionNotification>(message);

  application_set_.insert(app_);

  ON_CALL(app_mngr_, GetPolicyHandler())
      .WillByDefault(ReturnRef(mock_policy_handler_));
  typedef boost::optional<bool> OptionalBool;
  ON_CALL(mock_policy_handler_, LockScreenDismissalEnabledState())
      .WillByDefault(Return(OptionalBool(true)));
  std::string required_language = "EN-US";
  ON_CALL(*app_ptr_, ui_language()).WillByDefault(ReturnRef(kMobileLanguage));

  ON_CALL(mock_policy_handler_,
          LockScreenDismissalWarningMessage(required_language))
      .WillByDefault(Return(
          boost::optional<std::string>(kLockScreenDismissalWarningMessage_en)));
  ON_CALL(app_mngr_, applications()).WillByDefault(Return(applications_));

  policy::CheckPermissionResult result;
  result.hmi_level_permitted = policy::kRpcAllowed;
  EXPECT_CALL(mock_policy_handler_, CheckPermissions(_, _, _, _, _))
      .WillOnce(GetArg4(&result));

  EXPECT_CALL(mock_rpc_service_,
              ManageMobileCommand(_, Command::CommandSource::SOURCE_SDL))
      .WillOnce(GetMessage(message));
  ON_CALL(*app_ptr_, app_id()).WillByDefault(Return(kAppId_));

  command->Run();
  EXPECT_EQ(
      static_cast<int32_t>(am::mobile_api::FunctionID::OnDriverDistractionID),
      (*message)[am::strings::params][am::strings::function_id].asInt());
  EXPECT_EQ(
      static_cast<int32_t>(kAppId_),
      (*message)[am::strings::params][am::strings::connection_key].asInt());
  EXPECT_EQ(static_cast<int32_t>(am::MessageType::kNotification),
            (*message)[am::strings::params][am::strings::message_type].asInt());
}

}  // namespace hmi_notifications_test
}  // namespace hmi_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
