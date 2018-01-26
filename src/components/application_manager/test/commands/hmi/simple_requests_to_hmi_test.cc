/*
 * Copyright (c) 2016, Ford Motor Company
 * All rights reserved.
 *
 * Copyright (c) 2017 Xevo Inc.
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
 * Neither the name of the copyright holders nor the names of their contributors
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
#include "utils/shared_ptr.h"
#include "smart_objects/smart_object.h"
#include "application_manager/smart_object_keys.h"
#include "application_manager/commands/commands_test.h"
#include "application_manager/commands/command_request_test.h"
#include "application_manager/commands/command.h"
#include "application_manager/commands/hmi/allow_app_request.h"
#include "application_manager/commands/hmi/allow_all_apps_request.h"
#include "application_manager/commands/hmi/basic_communication_system_request.h"
#include "application_manager/commands/hmi/button_get_capabilities_request.h"
#include "application_manager/commands/hmi/navi_alert_maneuver_request.h"
#include "application_manager/commands/hmi/navi_audio_stop_stream_request.h"
#include "application_manager/commands/hmi/navi_get_way_points_request.h"
#include "application_manager/commands/hmi/navi_is_ready_request.h"
#include "application_manager/commands/hmi/navi_send_location_request.h"
#include "application_manager/commands/hmi/navi_show_constant_tbt_request.h"
#include "application_manager/commands/hmi/navi_stop_stream_request.h"
#include "application_manager/commands/hmi/navi_subscribe_way_points_request.h"
#include "application_manager/commands/hmi/navi_unsubscribe_way_points_request.h"
#include "application_manager/commands/hmi/navi_update_turn_list_request.h"
#include "application_manager/commands/hmi/sdl_activate_app_response.h"
#include "application_manager/commands/hmi/sdl_get_list_of_permissions_response.h"
#include "application_manager/commands/hmi/sdl_get_status_update_response.h"
#include "application_manager/commands/hmi/ui_scrollable_message_request.h"
#include "application_manager/commands/hmi/ui_set_app_icon_request.h"
#include "application_manager/commands/hmi/ui_set_display_layout_request.h"
#include "application_manager/commands/hmi/ui_set_global_properties_request.h"
#include "application_manager/commands/hmi/request_to_hmi.h"
#include "application_manager/commands/hmi/vi_get_vehicle_type_request.h"
#include "application_manager/commands/hmi/vi_is_ready_request.h"
#include "application_manager/commands/hmi/vi_read_did_request.h"
#include "application_manager/commands/hmi/vi_subscribe_vehicle_data_request.h"
#include "application_manager/commands/hmi/dial_number_request.h"
#include "application_manager/commands/hmi/tts_is_ready_request.h"
#include "application_manager/commands/hmi/tts_set_global_properties_request.h"
#include "application_manager/commands/hmi/tts_speak_request.h"
#include "application_manager/commands/hmi/tts_stop_speaking_request.h"
#include "application_manager/commands/hmi/tts_get_supported_languages_request.h"
#include "application_manager/commands/hmi/tts_change_registration_request.h"
#include "application_manager/commands/hmi/tts_get_capabilities_request.h"
#include "application_manager/commands/hmi/tts_get_language_request.h"
#include "application_manager/commands/hmi/close_popup_request.h"
#include "application_manager/commands/hmi/ui_add_command_request.h"
#include "application_manager/commands/hmi/ui_add_submenu_request.h"
#include "application_manager/commands/hmi/ui_alert_request.h"
#include "application_manager/commands/hmi/ui_change_registration_request.h"
#include "application_manager/commands/hmi/ui_delete_command_request.h"
#include "application_manager/commands/hmi/ui_delete_submenu_request.h"
#include "application_manager/commands/hmi/ui_end_audio_pass_thru_request.h"
#include "application_manager/commands/hmi/ui_get_capabilities_request.h"
#include "application_manager/commands/hmi/ui_get_language_request.h"
#include "application_manager/commands/hmi/ui_get_supported_languages_request.h"
#include "application_manager/commands/hmi/ui_is_ready_request.h"
#include "application_manager/commands/hmi/ui_perform_audio_pass_thru_request.h"
#include "application_manager/commands/hmi/ui_perform_interaction_request.h"
#include "application_manager/commands/hmi/vi_diagnostic_message_request.h"
#include "application_manager/commands/hmi/vi_get_dtcs_request.h"
#include "application_manager/commands/hmi/vi_get_vehicle_data_request.h"
#include "application_manager/commands/hmi/ui_set_media_clock_timer_request.h"
#include "application_manager/commands/hmi/ui_show_request.h"
#include "application_manager/commands/hmi/ui_slider_request.h"
#include "application_manager/commands/hmi/vi_unsubscribe_vehicle_data_request.h"
#include "application_manager/commands/hmi/vr_add_command_request.h"
#include "application_manager/commands/hmi/vr_change_registration_request.h"
#include "application_manager/commands/hmi/vr_delete_command_request.h"
#include "application_manager/commands/hmi/vr_get_capabilities_request.h"
#include "application_manager/commands/hmi/vr_get_supported_languages_request.h"
#include "application_manager/commands/hmi/vr_get_language_request.h"
#include "application_manager/commands/hmi/vr_is_ready_request.h"
#include "application_manager/commands/hmi/vr_perform_interaction_request.h"
#include "application_manager/commands/hmi/allow_all_apps_request.h"
#include "application_manager/commands/hmi/basic_communication_system_request.h"
#include "application_manager/commands/hmi/button_get_capabilities_request.h"
#include "application_manager/commands/hmi/allow_app_request.h"
#include "application_manager/commands/hmi/navi_send_location_request.h"
#include "application_manager/commands/hmi/navi_unsubscribe_way_points_request.h"
#include "application_manager/commands/hmi/navi_update_turn_list_request.h"
#include "application_manager/commands/hmi/navi_show_constant_tbt_request.h"
#include "application_manager/commands/hmi/navi_stop_stream_request.h"
#include "application_manager/commands/hmi/navi_subscribe_way_points_request.h"
#include "application_manager/commands/hmi/sdl_policy_update.h"
#include "application_manager/commands/hmi/ui_set_icon_request.h"
#include "application_manager/commands/hmi/dial_number_request.h"
#include "application_manager/commands/hmi/ui_send_haptic_data_request.h"

#include "application_manager/test/include/application_manager/mock_event_dispatcher.h"

namespace test {
namespace components {
namespace commands_test {
namespace hmi_commands_test {
namespace simple_requests_to_hmi_test {

using ::testing::_;
using ::testing::Types;
using ::testing::NotNull;

using ::utils::SharedPtr;

namespace am_commands = application_manager::commands;
using am_commands::MessageSharedPtr;
using event_engine_test::MockEventDispatcher;

class RequestToHMITest : public CommandsTest<CommandsTestMocks::kIsNice> {};

TEST_F(RequestToHMITest, BasicMethodsOverloads_SUCCESS) {
  SharedPtr<am_commands::RequestToHMI> command(
      CreateCommand<am_commands::RequestToHMI>());

  // Current implementation always return `true`
  EXPECT_TRUE(command->Init());
  EXPECT_NO_THROW(command->Run());
  EXPECT_TRUE(command->CleanUp());
}

TEST_F(RequestToHMITest, SendRequest_SUCCESS) {
  SharedPtr<am_commands::RequestToHMI> command(
      CreateCommand<am_commands::RequestToHMI>());
  ON_CALL(app_mngr_, GetRPCService()).WillByDefault(ReturnRef(rpc_service_));
  EXPECT_CALL(rpc_service_, SendMessageToHMI(NotNull()));

  command->SendRequest();
}

template <typename Command>
class RequestToHMICommandsTest
    : public CommandsTest<CommandsTestMocks::kIsNice> {
 public:
  typedef Command CommandType;
};

template <typename Command>
class RequestToHMICommandsTest2 : public RequestToHMICommandsTest<Command> {};

template <typename Command>
class RequestToHMICommandsTest3
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
  typedef Command CommandType;
};

typedef Types<am_commands::VIGetVehicleTypeRequest,
              am_commands::VIReadDIDRequest,
              am_commands::VISubscribeVehicleDataRequest,
              am_commands::hmi::DialNumberRequest,
              am_commands::ClosePopupRequest,
              am_commands::TTSSetGlobalPropertiesRequest,
              am_commands::TTSSpeakRequest,
              am_commands::TTSStopSpeakingRequest,
              am_commands::TTSGetSupportedLanguagesRequest,
              am_commands::UIAddCommandRequest,
              am_commands::UIAddSubmenuRequest,
              am_commands::UIAlertRequest,
              am_commands::UIChangeRegistrationRequest,
              am_commands::UIDeleteCommandRequest,
              am_commands::UIDeleteSubmenuRequest,
              am_commands::UIEndAudioPassThruRequest,
              am_commands::UIGetCapabilitiesRequest,
              am_commands::UIGetLanguageRequest,
              am_commands::UIGetSupportedLanguagesRequest,
              am_commands::UIPerformAudioPassThruRequest,
              am_commands::UIPerformInteractionRequest,
              am_commands::VIDiagnosticMessageRequest,
              am_commands::VIGetDTCsRequest,
              am_commands::VIGetVehicleDataRequest,
              am_commands::UISetMediaClockTimerRequest,
              am_commands::UIShowRequest,
              am_commands::VIUnsubscribeVehicleDataRequest,
              am_commands::VRAddCommandRequest,
              am_commands::VRChangeRegistrationRequest,
              am_commands::VRDeleteCommandRequest,
              am_commands::UISliderRequest,
              am_commands::TTSChangeRegistrationRequest,
              am_commands::TTSGetCapabilitiesRequest,
              am_commands::TTSGetLanguageRequest,
              am_commands::AllowAllAppsRequest,
              am_commands::BasicCommunicationSystemRequest,
              am_commands::ButtonGetCapabilitiesRequest,
              am_commands::NaviSendLocationRequest,
              am_commands::NaviUnSubscribeWayPointsRequest,
              am_commands::NaviUpdateTurnListRequest,
              am_commands::NaviShowConstantTBTRequest,
              am_commands::NaviStopStreamRequest,
              am_commands::NaviSubscribeWayPointsRequest,
              am_commands::NaviAlertManeuverRequest,
              am_commands::AudioStopStreamRequest,
              am_commands::NaviGetWayPointsRequest,
              am_commands::UISetGlobalPropertiesRequest,
              am_commands::UISendHapticDataRequest> RequestCommandsList;

typedef Types<am_commands::UIScrollableMessageRequest,
              am_commands::VRGetCapabilitiesRequest,
              am_commands::UISetAppIconRequest,
              am_commands::UiSetDisplayLayoutRequest,
              am_commands::VRGetSupportedLanguagesRequest,
              am_commands::VRGetLanguageRequest,
              am_commands::VRPerformInteractionRequest,
              am_commands::AllowAppRequest,
// TODO (OKozlov). Need to clarify why UT fails
// for UISetIconRequest
// am_commands::UISetIconRequest,
#if defined(PROPRIETARY_MODE) || defined(EXTERNAL_PROPRIETARY_MODE)
              am_commands::SDLPolicyUpdate,
#endif
              am_commands::hmi::DialNumberRequest> RequestCommandsList2;

typedef Types<am_commands::VIIsReadyRequest,
              am_commands::TTSIsReadyRequest,
              am_commands::UIIsReadyRequest,
              am_commands::NaviIsReadyRequest,
              am_commands::VRIsReadyRequest> RequestCommandsList3;

TYPED_TEST_CASE(RequestToHMICommandsTest, RequestCommandsList);
TYPED_TEST_CASE(RequestToHMICommandsTest2, RequestCommandsList2);
TYPED_TEST_CASE(RequestToHMICommandsTest3, RequestCommandsList3);

TYPED_TEST(RequestToHMICommandsTest, Run_SendMessageToHMI_SUCCESS) {
  typedef typename TestFixture::CommandType CommandType;

  SharedPtr<CommandType> command = this->template CreateCommand<CommandType>();
  EXPECT_CALL(this->app_mngr_, GetRPCService())
      .WillOnce(ReturnRef(this->rpc_service_));
  EXPECT_CALL(this->rpc_service_, SendMessageToHMI(NotNull()));

  command->Run();
}

TYPED_TEST(RequestToHMICommandsTest2, Run_SendMessageToHMI_SUCCESS) {
  typedef typename TestFixture::CommandType CommandType;

  SharedPtr<CommandType> command = this->template CreateCommand<CommandType>();
  EXPECT_CALL(this->app_mngr_, GetRPCService())
      .WillOnce(ReturnRef(this->rpc_service_));
  EXPECT_CALL(this->rpc_service_, SendMessageToHMI(NotNull()));

  command->Run();
}

TYPED_TEST(RequestToHMICommandsTest3, Run_SendMessageToHMI_SUCCESS) {
  typedef typename TestFixture::CommandType CommandType;

  SharedPtr<CommandType> command = this->template CreateCommand<CommandType>();
  EXPECT_CALL(this->app_mngr_, GetRPCService())
      .WillOnce(ReturnRef(this->rpc_service_));
  EXPECT_CALL(this->rpc_service_, SendMessageToHMI(NotNull()));

  command->Run();
}

}  // namespace simple_requests_to_hmi_test
}  // namespace hmi_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
