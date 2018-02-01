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

#include "application_manager/commands/command_request_test.h"

#include <stdint.h>
#include <string>
#include <vector>
#include "gtest/gtest.h"

#include "application_manager/commands/hmi/allow_all_apps_request.h"
#include "application_manager/commands/hmi/allow_all_apps_response.h"
#include "application_manager/commands/hmi/allow_app_request.h"
#include "application_manager/commands/hmi/allow_app_response.h"
#include "application_manager/commands/hmi/navi_audio_start_stream_request.h"
#include "application_manager/commands/hmi/navi_audio_start_stream_response.h"
#include "application_manager/commands/hmi/navi_audio_stop_stream_request.h"
#include "application_manager/commands/hmi/navi_audio_stop_stream_response.h"
#include "application_manager/commands/hmi/update_device_list_request.h"
#include "application_manager/commands/hmi/update_device_list_response.h"
#include "application_manager/commands/hmi/on_update_device_list.h"
#include "application_manager/commands/hmi/on_start_device_discovery.h"
#include "application_manager/commands/hmi/update_app_list_request.h"
#include "application_manager/commands/hmi/update_app_list_response.h"
#include "application_manager/commands/hmi/on_find_applications.h"
#include "application_manager/commands/hmi/sdl_get_list_of_permissions_request.h"
#include "application_manager/commands/hmi/sdl_get_list_of_permissions_response.h"
#include "application_manager/commands/hmi/sdl_get_user_friendly_message_request.h"
#include "application_manager/commands/hmi/sdl_get_user_friendly_message_response.h"
#include "application_manager/commands/hmi/sdl_get_status_update_request.h"
#include "application_manager/commands/hmi/sdl_get_status_update_response.h"
#include "application_manager/commands/hmi/on_status_update_notification.h"
#include "application_manager/commands/hmi/update_sdl_request.h"
#include "application_manager/commands/hmi/update_sdl_response.h"
#include "application_manager/commands/hmi/activate_app_request.h"
#include "application_manager/commands/hmi/activate_app_response.h"
#include "application_manager/commands/hmi/mixing_audio_supported_request.h"
#include "application_manager/commands/hmi/mixing_audio_supported_response.h"
#include "application_manager/commands/hmi/on_allow_sdl_functionality_notification.h"
#include "application_manager/commands/hmi/on_app_permission_changed_notification.h"
#include "application_manager/commands/hmi/on_app_permission_consent_notification.h"
#include "application_manager/commands/hmi/on_app_activated_notification.h"
#include "application_manager/commands/hmi/on_audio_data_streaming_notification.h"
#include "application_manager/commands/hmi/on_video_data_streaming_notification.h"
#include "application_manager/commands/hmi/on_sdl_consent_needed_notification.h"
#include "application_manager/commands/hmi/on_exit_all_applications_notification.h"
#include "application_manager/commands/hmi/on_exit_application_notification.h"
#include "application_manager/commands/hmi/on_put_file_notification.h"
#include "application_manager/commands/hmi/on_resume_audio_source_notification.h"
#include "application_manager/commands/hmi/on_ignition_cycle_over_notification.h"
#include "application_manager/commands/hmi/on_system_info_changed_notification.h"
#include "application_manager/commands/hmi/get_system_info_request.h"
#include "application_manager/commands/hmi/get_system_info_response.h"
#include "application_manager/commands/hmi/close_popup_request.h"
#include "application_manager/commands/hmi/close_popup_response.h"
#include "application_manager/commands/hmi/button_get_capabilities_request.h"
#include "application_manager/commands/hmi/button_get_capabilities_response.h"
#include "application_manager/commands/hmi/ui_add_command_request.h"
#include "application_manager/commands/hmi/ui_add_command_response.h"
#include "application_manager/commands/hmi/ui_delete_command_request.h"
#include "application_manager/commands/hmi/ui_delete_command_response.h"
#include "application_manager/commands/hmi/ui_add_submenu_request.h"
#include "application_manager/commands/hmi/ui_add_submenu_response.h"
#include "application_manager/commands/hmi/ui_delete_submenu_request.h"
#include "application_manager/commands/hmi/ui_delete_submenu_response.h"
#include "application_manager/commands/hmi/ui_get_supported_languages_request.h"
#include "application_manager/commands/hmi/ui_get_supported_languages_response.h"
#include "application_manager/commands/hmi/ui_get_language_request.h"
#include "application_manager/commands/hmi/ui_get_language_response.h"
#include "application_manager/commands/hmi/ui_get_capabilities_request.h"
#include "application_manager/commands/hmi/ui_get_capabilities_response.h"
#include "application_manager/commands/hmi/ui_change_registration_request.h"
#include "application_manager/commands/hmi/ui_change_registration_response.h"
#include "application_manager/commands/hmi/ui_show_request.h"
#include "application_manager/commands/hmi/ui_show_response.h"
#include "application_manager/commands/hmi/ui_alert_request.h"
#include "application_manager/commands/hmi/ui_alert_response.h"
#include "application_manager/commands/hmi/ui_is_ready_request.h"
#include "application_manager/commands/hmi/ui_is_ready_response.h"
#include "application_manager/commands/hmi/ui_slider_request.h"
#include "application_manager/commands/hmi/ui_slider_response.h"
#include "application_manager/commands/hmi/ui_set_media_clock_timer_request.h"
#include "application_manager/commands/hmi/ui_set_media_clock_timer_response.h"
#include "application_manager/commands/hmi/ui_set_global_properties_request.h"
#include "application_manager/commands/hmi/ui_set_global_properties_response.h"
#include "application_manager/commands/hmi/ui_scrollable_message_request.h"
#include "application_manager/commands/hmi/ui_scrollable_message_response.h"
#include "application_manager/commands/hmi/ui_set_app_icon_request.h"
#include "application_manager/commands/hmi/ui_set_app_icon_response.h"
#include "application_manager/commands/hmi/ui_perform_audio_pass_thru_response.h"
#include "application_manager/commands/hmi/ui_perform_audio_pass_thru_request.h"
#include "application_manager/commands/hmi/ui_end_audio_pass_thru_request.h"
#include "application_manager/commands/hmi/ui_end_audio_pass_thru_response.h"
#include "application_manager/commands/hmi/ui_perform_interaction_request.h"
#include "application_manager/commands/hmi/ui_perform_interaction_response.h"
#include "application_manager/commands/hmi/vr_is_ready_request.h"
#include "application_manager/commands/hmi/vr_is_ready_response.h"
#include "application_manager/commands/hmi/vr_add_command_request.h"
#include "application_manager/commands/hmi/vr_add_command_response.h"
#include "application_manager/commands/hmi/vr_delete_command_request.h"
#include "application_manager/commands/hmi/vr_delete_command_response.h"
#include "application_manager/commands/hmi/vr_change_registration_request.h"
#include "application_manager/commands/hmi/vr_change_registration_response.h"
#include "application_manager/commands/hmi/vr_get_supported_languages_request.h"
#include "application_manager/commands/hmi/vr_get_supported_languages_response.h"
#include "application_manager/commands/hmi/vr_get_language_request.h"
#include "application_manager/commands/hmi/vr_get_language_response.h"
#include "application_manager/commands/hmi/vr_get_capabilities_request.h"
#include "application_manager/commands/hmi/vr_get_capabilities_response.h"
#include "application_manager/commands/hmi/tts_is_ready_request.h"
#include "application_manager/commands/hmi/tts_is_ready_response.h"
#include "application_manager/commands/hmi/tts_change_registration_request.h"
#include "application_manager/commands/hmi/tts_change_registration_response.h"
#include "application_manager/commands/hmi/tts_get_supported_languages_request.h"
#include "application_manager/commands/hmi/tts_get_supported_languages_response.h"
#include "application_manager/commands/hmi/tts_get_language_request.h"
#include "application_manager/commands/hmi/tts_get_language_response.h"
#include "application_manager/commands/hmi/tts_stop_speaking_request.h"
#include "application_manager/commands/hmi/tts_stop_speaking_response.h"
#include "application_manager/commands/hmi/tts_speak_request.h"
#include "application_manager/commands/hmi/tts_speak_response.h"
#include "application_manager/commands/hmi/tts_set_global_properties_request.h"
#include "application_manager/commands/hmi/tts_set_global_properties_response.h"
#include "application_manager/commands/hmi/tts_get_capabilities_request.h"
#include "application_manager/commands/hmi/tts_get_capabilities_response.h"
#include "application_manager/commands/hmi/vr_perform_interaction_request.h"
#include "application_manager/commands/hmi/vr_perform_interaction_response.h"
#include "application_manager/commands/hmi/vi_is_ready_request.h"
#include "application_manager/commands/hmi/vi_is_ready_response.h"
#include "application_manager/commands/hmi/vi_read_did_request.h"
#include "application_manager/commands/hmi/vi_read_did_response.h"
#include "application_manager/commands/hmi/sdl_activate_app_request.h"
#include "application_manager/commands/hmi/sdl_activate_app_response.h"
#include "application_manager/commands/hmi/on_app_permission_changed_notification.h"
#include "application_manager/commands/hmi/on_event_changed_notification.h"
#include "application_manager/commands/hmi/vi_get_vehicle_data_request.h"
#include "application_manager/commands/hmi/vi_get_vehicle_data_response.h"
#include "application_manager/commands/hmi/on_vi_vehicle_data_notification.h"
#include "application_manager/commands/hmi/vi_subscribe_vehicle_data_request.h"
#include "application_manager/commands/hmi/vi_subscribe_vehicle_data_response.h"
#include "application_manager/commands/hmi/vi_unsubscribe_vehicle_data_request.h"
#include "application_manager/commands/hmi/vi_unsubscribe_vehicle_data_response.h"
#include "application_manager/commands/hmi/vi_get_dtcs_request.h"
#include "application_manager/commands/hmi/vi_get_dtcs_response.h"
#include "application_manager/commands/hmi/vi_diagnostic_message_request.h"
#include "application_manager/commands/hmi/vi_diagnostic_message_response.h"
#include "application_manager/commands/hmi/vi_get_vehicle_type_request.h"
#include "application_manager/commands/hmi/vi_get_vehicle_type_response.h"
#include "application_manager/commands/hmi/navi_is_ready_request.h"
#include "application_manager/commands/hmi/navi_show_constant_tbt_request.h"
#include "application_manager/commands/hmi/navi_show_constant_tbt_response.h"
#include "application_manager/commands/hmi/navi_is_ready_response.h"
#include "application_manager/commands/hmi/navi_alert_maneuver_request.h"
#include "application_manager/commands/hmi/navi_alert_maneuver_response.h"
#include "application_manager/commands/hmi/navi_update_turn_list_request.h"
#include "application_manager/commands/hmi/navi_update_turn_list_response.h"
#include "application_manager/commands/hmi/navi_subscribe_way_points_request.h"
#include "application_manager/commands/hmi/navi_subscribe_way_points_response.h"
#include "application_manager/commands/hmi/navi_unsubscribe_way_points_request.h"
#include "application_manager/commands/hmi/navi_unsubscribe_way_points_response.h"
#include "application_manager/commands/hmi/navi_get_way_points_request.h"
#include "application_manager/commands/hmi/navi_get_way_points_response.h"
#include "application_manager/commands/hmi/on_ready_notification.h"
#include "application_manager/commands/hmi/on_device_chosen_notification.h"
#include "application_manager/commands/hmi/on_file_removed_notification.h"
#include "application_manager/commands/hmi/on_system_context_notification.h"
#include "application_manager/commands/hmi/on_app_registered_notification.h"
#include "application_manager/commands/hmi/on_app_unregistered_notification.h"
#include "application_manager/commands/hmi/on_driver_distraction_notification.h"
#include "application_manager/commands/hmi/on_tts_started_notification.h"
#include "application_manager/commands/hmi/on_tts_stopped_notification.h"
#include "application_manager/commands/hmi/on_vr_started_notification.h"
#include "application_manager/commands/hmi/on_vr_stopped_notification.h"
#include "application_manager/commands/hmi/on_vr_command_notification.h"
#include "application_manager/commands/hmi/on_ui_command_notification.h"
#include "application_manager/commands/hmi/on_app_deactivated_notification.h"
#include "application_manager/commands/hmi/on_ui_language_change_notification.h"
#include "application_manager/commands/hmi/on_vr_language_change_notification.h"
#include "application_manager/commands/hmi/on_tts_language_change_notification.h"
#include "application_manager/commands/hmi/on_navi_tbt_client_state_notification.h"
#include "application_manager/commands/hmi/on_navi_way_point_change_notification.h"
#include "application_manager/commands/hmi/on_button_event_notification.h"
#include "application_manager/commands/hmi/on_button_press_notification.h"
#include "application_manager/commands/hmi/on_button_subscription_notification.h"
#include "application_manager/commands/hmi/on_vi_vehicle_data_notification.h"
#include "application_manager/commands/hmi/on_ui_keyboard_input_notification.h"
#include "application_manager/commands/hmi/on_ui_touch_event_notification.h"
#include "application_manager/commands/hmi/on_ui_reset_timeout_notification.h"
#include "application_manager/commands/hmi/navi_start_stream_request.h"
#include "application_manager/commands/hmi/navi_start_stream_response.h"
#include "application_manager/commands/hmi/navi_stop_stream_request.h"
#include "application_manager/commands/hmi/navi_stop_stream_response.h"
#include "application_manager/commands/hmi/on_system_request_notification.h"
#include "application_manager/commands/hmi/ui_set_display_layout_request.h"
#include "application_manager/commands/hmi/ui_set_display_layout_response.h"
#include "application_manager/commands/hmi/on_sdl_close_notification.h"
#include "application_manager/commands/hmi/on_sdl_persistence_complete_notification.h"
#include "application_manager/commands/hmi/on_record_start_notification.h"
#include "application_manager/commands/hmi/add_statistics_info_notification.h"
#include "application_manager/commands/hmi/on_system_error_notification.h"
#include "application_manager/commands/hmi/basic_communication_system_request.h"
#include "application_manager/commands/hmi/basic_communication_system_response.h"
#include "application_manager/commands/hmi/sdl_policy_update.h"
#include "application_manager/commands/hmi/sdl_policy_update_response.h"
#include "application_manager/commands/hmi/on_received_policy_update.h"
#include "application_manager/commands/hmi/on_policy_update.h"
#include "application_manager/commands/hmi/get_urls.h"
#include "application_manager/commands/hmi/get_urls_response.h"
#include "application_manager/commands/hmi/on_device_state_changed_notification.h"
#include "application_manager/commands/hmi/navi_send_location_request.h"
#include "application_manager/commands/hmi/navi_send_location_response.h"
#include "application_manager/commands/hmi/on_tts_reset_timeout_notification.h"
#include "application_manager/commands/hmi/dial_number_request.h"
#include "application_manager/commands/hmi/dial_number_response.h"
#include "application_manager/commands/hmi/ui_set_icon_request.h"

#include "application_manager/mock_application.h"
#include "application_manager/mock_application_manager.h"
#include "test/application_manager/mock_application_manager_settings.h"
#include "application_manager/mock_event_dispatcher.h"

namespace am = application_manager;

namespace test {
namespace components {
namespace commands_test {
namespace hmi_commands_test {
namespace dummy_hmi_commands_test {

namespace commands = ::application_manager::commands;

using ::testing::_;
using ::testing::NotNull;
using ::testing::Types;
using commands::MessageSharedPtr;
using ::test::components::event_engine_test::MockEventDispatcher;
using ::test::components::application_manager_test::MockApplicationManager;
using ::test::components::application_manager_test::
    MockApplicationManagerSettings;
using ::application_manager::ApplicationSharedPtr;
using ::test::components::application_manager_test::MockApplication;

template <class Command>
class HMICommandsTest : public components::commands_test::CommandRequestTest<
                            CommandsTestMocks::kIsNice> {
 public:
  typedef Command CommandType;

  void InitCommand(const uint32_t& timeout) OVERRIDE {
    stream_retry_.first = 0;
    stream_retry_.second = 0;
    EXPECT_CALL(app_mngr_settings_, default_timeout())
        .WillOnce(ReturnRef(timeout));
    ON_CALL(app_mngr_, event_dispatcher())
        .WillByDefault(ReturnRef(event_dispatcher_));
    ON_CALL(app_mngr_, get_settings())
        .WillByDefault(ReturnRef(app_mngr_settings_));
    ON_CALL(app_mngr_settings_, start_stream_retry_amount())
        .WillByDefault(ReturnRef(stream_retry_));
  }

 protected:
  std::pair<uint32_t, int32_t> stream_retry_;
};

template <class Command>
class HMICommandsTestFirst : public HMICommandsTest<Command> {
 public:
  using typename HMICommandsTest<Command>::CommandType;
};

template <class Command>
class HMICommandsTestSecond : public HMICommandsTest<Command> {
 public:
  using typename HMICommandsTest<Command>::CommandType;
};

template <class Command>
class HMICommandsTestThird : public HMICommandsTest<Command> {
 public:
  using typename HMICommandsTest<Command>::CommandType;
};

template <class Command>
class HMICommandsTestFourth : public HMICommandsTest<Command> {
 public:
  using typename HMICommandsTest<Command>::CommandType;
};

template <class Command>
class HMICommandsTestFifth : public HMICommandsTest<Command> {
 public:
  using typename HMICommandsTest<Command>::CommandType;
};

/* macro TYPED_TEST_CASE takes max 50 args. That is why there are few
 * TYPED_TEST_CASE for HMI and mobile commands
 */

typedef Types<commands::OnStartDeviceDiscovery,
              commands::UpdateDeviceListResponse,
              commands::UpdateDeviceListRequest,
              commands::ActivateAppResponse,
              commands::ActivateAppRequest,
              commands::GetSystemInfoResponse,
              commands::GetSystemInfoRequest,
              commands::SDLActivateAppResponse,
              commands::SDLActivateAppRequest,
              commands::SDLPolicyUpdateResponse,
              commands::SDLPolicyUpdate,
              commands::GetUrlsResponse,
              commands::GetUrls,
              commands::OnAppPermissionChangedNotification,
              commands::SDLGetListOfPermissionsResponse,
              commands::SDLGetListOfPermissionsRequest,
              commands::SDLGetUserFriendlyMessageResponse,
              commands::SDLGetUserFriendlyMessageRequest,
              commands::SDLGetStatusUpdateResponse,
              commands::SDLGetStatusUpdateRequest,
              commands::OnStatusUpdateNotification,
              commands::OnAppPermissionConsentNotification,
              commands::MixingAudioSupportedResponse,
              commands::MixingAudioSupportedRequest,
              commands::OnExitAllApplicationsNotification,
              commands::UIAddCommandResponse,
              commands::UIAddCommandRequest,
              commands::UIDeleteCommandResponse,
              commands::UIDeleteCommandRequest,
              commands::UIAddSubmenuResponse,
              commands::UIAddSubmenuRequest,
              commands::UIDeleteSubmenuResponse,
              commands::UIDeleteSubmenuRequest,
              commands::UISetMediaClockTimerResponse,
              commands::UISetMediaClockTimerRequest,
              commands::UIPerformInteractionResponse,
              commands::UIPerformInteractionRequest,
              commands::UISetGlobalPropertiesResponse,
              commands::UISetGlobalPropertiesRequest,
              commands::UIScrollableMessageResponse,
              commands::UIScrollableMessageRequest,
              commands::UISetAppIconResponse,
              commands::UISetAppIconRequest,
              commands::UIGetSupportedLanguagesResponse,
              commands::UIGetSupportedLanguagesRequest,
              commands::UIGetLanguageResponse,
              commands::UIGetLanguageRequest,
              commands::UIGetCapabilitiesResponse,
              commands::UIGetCapabilitiesRequest,
              commands::UIChangeRegistratioResponse> HMICommandsListFirst;

typedef Types<commands::UIChangeRegistrationRequest,
              commands::UIPerformAudioPassThruResponse,
              commands::UIPerformAudioPassThruRequest,
              commands::UIEndAudioPassThruResponse,
              commands::UIEndAudioPassThruRequest,
              commands::UIAlertResponse,
              commands::UIAlertRequest,
              commands::VRIsReadyResponse,
              commands::VRIsReadyRequest,
              commands::VRAddCommandResponse,
              commands::VRAddCommandRequest,
              commands::VRDeleteCommandResponse,
              commands::VRDeleteCommandRequest,
              commands::VRChangeRegistrationResponse,
              commands::VRChangeRegistrationRequest,
              commands::VRGetSupportedLanguagesResponse,
              commands::VRGetSupportedLanguagesRequest,
              commands::VRGetLanguageResponse,
              commands::VRGetLanguageRequest,
              commands::VRGetCapabilitiesResponse,
              commands::VRGetCapabilitiesRequest,
              commands::TTSIsReadyResponse,
              commands::TTSIsReadyRequest,
              commands::TTSChangeRegistratioResponse,
              commands::TTSChangeRegistrationRequest,
              commands::TTSGetSupportedLanguagesResponse,
              commands::TTSGetSupportedLanguagesRequest,
              commands::TTSStopSpeakingResponse,
              commands::TTSStopSpeakingRequest,
              commands::TTSGetLanguageResponse,
              commands::TTSGetLanguageRequest,
              commands::TTSSpeakResponse,
              commands::TTSSpeakRequest,
              commands::TTSSetGlobalPropertiesResponse,
              commands::TTSSetGlobalPropertiesRequest,
              commands::TTSGetCapabilitiesResponse,
              commands::TTSGetCapabilitiesRequest,
              commands::OnTTSStartedNotification,
              commands::OnTTSStoppedNotification,
              commands::OnAppActivatedNotification,
              commands::OnExitApplicationNotification,
              commands::UIShowResponse,
              commands::UIShowRequest,
              commands::UISliderResponse,
              commands::UISliderRequest,
              commands::ClosePopupResponse,
              commands::ClosePopupRequest,
              commands::UIIsReadyResponse,
              commands::UIIsReadyRequest,
              commands::VIIsReadyResponse> HMICommandsListSecond;

typedef Types<commands::VIIsReadyRequest,
              commands::VIReadDIDResponse,
              commands::VIReadDIDRequest,
              commands::VIGetVehicleDataResponse,
              commands::VIGetVehicleDataRequest,
              commands::VIGetDTCsResponse,
              commands::VIGetDTCsRequest,
              commands::VIDiagnosticMessageResponse,
              commands::VIDiagnosticMessageRequest,
              commands::VIGetVehicleTypeResponse,
              commands::VIGetVehicleTypeRequest,
              commands::NaviIsReadyResponse,
              commands::NaviIsReadyRequest,
              commands::NaviAlertManeuverResponse,
              commands::NaviAlertManeuverRequest,
              commands::NaviGetWayPointsResponse,
              commands::NaviGetWayPointsRequest,
              commands::NaviUpdateTurnListResponse,
              commands::NaviUpdateTurnListRequest,
              commands::NaviShowConstantTBTResponse,
              commands::NaviShowConstantTBTRequest,
              commands::NaviSubscribeWayPointsResponse,
              commands::NaviSubscribeWayPointsRequest,
              commands::NaviUnsubscribeWayPointsResponse,
              commands::NaviUnSubscribeWayPointsRequest,
              commands::ButtonGetCapabilitiesResponse,
              commands::ButtonGetCapabilitiesRequest,
              commands::OnAllowSDLFunctionalityNotification,
              commands::OnSDLConsentNeededNotification,
              commands::UpdateSDLResponse,
              commands::UpdateSDLRequest,
              commands::OnIgnitionCycleOverNotification,
              commands::OnSystemInfoChangedNotification,
              commands::OnReadyNotification,
              commands::OnDeviceChosenNotification,
              commands::OnSystemContextNotification,
              commands::hmi::OnDriverDistractionNotification,
              commands::OnUpdateDeviceList,
              commands::OnAppRegisteredNotification,
              commands::OnAppUnregisteredNotification,
              commands::OnFindApplications,
              commands::UpdateAppListResponse,
              commands::UpdateAppListRequest,
              commands::OnVRStartedNotification,
              commands::OnVRStoppedNotification,
              commands::OnVRCommandNotification,
              commands::OnUICommandNotification,
              commands::OnAppDeactivatedNotification> HMICommandsListThird;

typedef Types<commands::hmi::OnButtonEventNotification,
              commands::hmi::OnButtonPressNotification,
              commands::hmi::OnButtonSubscriptionNotification,
              commands::VISubscribeVehicleDataResponse,
              commands::VISubscribeVehicleDataRequest,
              commands::VIUnsubscribeVehicleDataResponse,
              commands::VIUnsubscribeVehicleDataRequest,
              commands::OnVIVehicleDataNotification,
              commands::OnNaviTBTClientStateNotification,
              commands::hmi::OnUIKeyBoardInputNotification,
              commands::hmi::OnUITouchEventNotification,
              commands::hmi::OnUIResetTimeoutNotification,
              commands::NaviStartStreamResponse,
              commands::NaviStartStreamRequest,
              commands::NaviStopStreamResponse,
              commands::NaviStopStreamRequest,
              commands::AudioStartStreamResponse,
              commands::AudioStartStreamRequest,
              commands::AudioStopStreamResponse,
              commands::AudioStopStreamRequest,
              commands::OnAudioDataStreamingNotification,
              commands::OnVideoDataStreamingNotification,
              commands::VRPerformInteractionResponse,
              commands::VRPerformInteractionRequest,
              commands::OnSystemRequestNotification,
              commands::OnPutFileNotification,
              commands::OnResumeAudioSourceNotification,
              commands::UiSetDisplayLayoutResponse,
              commands::UiSetDisplayLayoutRequest,
              commands::OnSDLCloseNotification,
              commands::OnSDLPersistenceCompleteNotification,
              commands::OnFileRemovedNotification,
              commands::OnRecordStartdNotification,
              commands::BasicCommunicationSystemResponse,
              commands::BasicCommunicationSystemRequest,
              commands::NaviSendLocationResponse,
              commands::NaviSendLocationRequest,
              commands::AddStatisticsInfoNotification,
              commands::OnSystemErrorNotification,
              commands::OnReceivedPolicyUpdate,
              commands::OnPolicyUpdate,
              commands::OnDeviceStateChangedNotification,
              commands::hmi::OnTTSResetTimeoutNotification,
              commands::hmi::DialNumberResponse,
              commands::hmi::DialNumberRequest,
              commands::OnEventChangedNotification,
              commands::OnNaviWayPointChangeNotification,
              commands::OnUILanguageChangeNotification,
              commands::OnVRLanguageChangeNotification,
              commands::OnTTSLanguageChangeNotification> HMICommandsListFourth;

typedef Types<commands::AllowAllAppsRequest,
              commands::AllowAllAppsResponse,
              commands::AllowAppRequest,
              commands::AllowAppResponse> HMICommandsListFifth;

TYPED_TEST_CASE(HMICommandsTestFirst, HMICommandsListFirst);
TYPED_TEST_CASE(HMICommandsTestSecond, HMICommandsListSecond);
TYPED_TEST_CASE(HMICommandsTestThird, HMICommandsListThird);
TYPED_TEST_CASE(HMICommandsTestFourth, HMICommandsListFourth);
TYPED_TEST_CASE(HMICommandsTestFifth, HMICommandsListFifth);

TYPED_TEST(HMICommandsTestFirst, CtorAndDtorCall) {
  utils::SharedPtr<typename TestFixture::CommandType> command =
      this->template CreateCommand<typename TestFixture::CommandType>();
  UNUSED(command);
}

TYPED_TEST(HMICommandsTestSecond, CtorAndDtorCall) {
  utils::SharedPtr<typename TestFixture::CommandType> command =
      this->template CreateCommand<typename TestFixture::CommandType>();
  UNUSED(command);
}
TYPED_TEST(HMICommandsTestThird, CtorAndDtorCall) {
  utils::SharedPtr<typename TestFixture::CommandType> command =
      this->template CreateCommand<typename TestFixture::CommandType>();
  UNUSED(command);
}

TYPED_TEST(HMICommandsTestFourth, CtorAndDtorCall) {
  utils::SharedPtr<typename TestFixture::CommandType> command =
      this->template CreateCommand<typename TestFixture::CommandType>();
  UNUSED(command);
}

TYPED_TEST(HMICommandsTestFifth, CtorAndDtorCall) {
  utils::SharedPtr<typename TestFixture::CommandType> command =
      this->template CreateCommand<typename TestFixture::CommandType>();
  UNUSED(command);
}

}  // namespace dummy_hmi_commands_test
}  // namespace hmi_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
