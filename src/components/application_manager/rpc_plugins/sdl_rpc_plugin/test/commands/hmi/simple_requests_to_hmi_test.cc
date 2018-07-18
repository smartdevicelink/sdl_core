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

#include "smart_objects/smart_object.h"
#include "application_manager/smart_object_keys.h"
#include "application_manager/commands/commands_test.h"
#include "application_manager/commands/command_request_test.h"
#include "application_manager/commands/command.h"
#include "sdl_rpc_plugin/commands/hmi/allow_app_request.h"
#include "sdl_rpc_plugin/commands/hmi/allow_all_apps_request.h"
#include "sdl_rpc_plugin/commands/hmi/basic_communication_system_request.h"
#include "sdl_rpc_plugin/commands/hmi/button_get_capabilities_request.h"
#include "sdl_rpc_plugin/commands/hmi/navi_alert_maneuver_request.h"
#include "sdl_rpc_plugin/commands/hmi/navi_audio_stop_stream_request.h"
#include "sdl_rpc_plugin/commands/hmi/navi_get_way_points_request.h"
#include "sdl_rpc_plugin/commands/hmi/navi_is_ready_request.h"
#include "sdl_rpc_plugin/commands/hmi/navi_send_location_request.h"
#include "sdl_rpc_plugin/commands/hmi/navi_show_constant_tbt_request.h"
#include "sdl_rpc_plugin/commands/hmi/navi_stop_stream_request.h"
#include "sdl_rpc_plugin/commands/hmi/navi_subscribe_way_points_request.h"
#include "sdl_rpc_plugin/commands/hmi/navi_unsubscribe_way_points_request.h"
#include "sdl_rpc_plugin/commands/hmi/navi_update_turn_list_request.h"
#include "sdl_rpc_plugin/commands/hmi/sdl_activate_app_response.h"
#include "sdl_rpc_plugin/commands/hmi/sdl_get_list_of_permissions_response.h"
#include "sdl_rpc_plugin/commands/hmi/sdl_get_status_update_response.h"
#include "sdl_rpc_plugin/commands/hmi/ui_scrollable_message_request.h"
#include "sdl_rpc_plugin/commands/hmi/ui_set_app_icon_request.h"
#include "sdl_rpc_plugin/commands/hmi/ui_set_display_layout_request.h"
#include "sdl_rpc_plugin/commands/hmi/ui_set_global_properties_request.h"
#include "application_manager/commands/request_to_hmi.h"
#include "sdl_rpc_plugin/commands/hmi/dial_number_request.h"
#include "sdl_rpc_plugin/commands/hmi/tts_is_ready_request.h"
#include "sdl_rpc_plugin/commands/hmi/tts_set_global_properties_request.h"
#include "sdl_rpc_plugin/commands/hmi/tts_speak_request.h"
#include "sdl_rpc_plugin/commands/hmi/tts_stop_speaking_request.h"
#include "sdl_rpc_plugin/commands/hmi/tts_get_supported_languages_request.h"
#include "sdl_rpc_plugin/commands/hmi/tts_change_registration_request.h"
#include "sdl_rpc_plugin/commands/hmi/tts_get_capabilities_request.h"
#include "sdl_rpc_plugin/commands/hmi/tts_get_language_request.h"
#include "sdl_rpc_plugin/commands/hmi/close_popup_request.h"
#include "sdl_rpc_plugin/commands/hmi/ui_add_command_request.h"
#include "sdl_rpc_plugin/commands/hmi/ui_add_submenu_request.h"
#include "sdl_rpc_plugin/commands/hmi/ui_alert_request.h"
#include "sdl_rpc_plugin/commands/hmi/ui_change_registration_request.h"
#include "sdl_rpc_plugin/commands/hmi/ui_delete_command_request.h"
#include "sdl_rpc_plugin/commands/hmi/ui_delete_submenu_request.h"
#include "sdl_rpc_plugin/commands/hmi/ui_end_audio_pass_thru_request.h"
#include "sdl_rpc_plugin/commands/hmi/ui_get_capabilities_request.h"
#include "sdl_rpc_plugin/commands/hmi/ui_get_language_request.h"
#include "sdl_rpc_plugin/commands/hmi/ui_get_supported_languages_request.h"
#include "sdl_rpc_plugin/commands/hmi/ui_is_ready_request.h"
#include "sdl_rpc_plugin/commands/hmi/ui_perform_audio_pass_thru_request.h"
#include "sdl_rpc_plugin/commands/hmi/ui_perform_interaction_request.h"
#include "sdl_rpc_plugin/commands/hmi/ui_set_media_clock_timer_request.h"
#include "sdl_rpc_plugin/commands/hmi/ui_show_request.h"
#include "sdl_rpc_plugin/commands/hmi/ui_slider_request.h"
#include "sdl_rpc_plugin/commands/hmi/vr_add_command_request.h"
#include "sdl_rpc_plugin/commands/hmi/vr_change_registration_request.h"
#include "sdl_rpc_plugin/commands/hmi/vr_delete_command_request.h"
#include "sdl_rpc_plugin/commands/hmi/vr_get_capabilities_request.h"
#include "sdl_rpc_plugin/commands/hmi/vr_get_supported_languages_request.h"
#include "sdl_rpc_plugin/commands/hmi/vr_get_language_request.h"
#include "sdl_rpc_plugin/commands/hmi/vr_is_ready_request.h"
#include "sdl_rpc_plugin/commands/hmi/vr_perform_interaction_request.h"
#include "sdl_rpc_plugin/commands/hmi/allow_all_apps_request.h"
#include "sdl_rpc_plugin/commands/hmi/basic_communication_system_request.h"
#include "sdl_rpc_plugin/commands/hmi/button_get_capabilities_request.h"
#include "sdl_rpc_plugin/commands/hmi/allow_app_request.h"
#include "sdl_rpc_plugin/commands/hmi/navi_send_location_request.h"
#include "sdl_rpc_plugin/commands/hmi/navi_unsubscribe_way_points_request.h"
#include "sdl_rpc_plugin/commands/hmi/navi_update_turn_list_request.h"
#include "sdl_rpc_plugin/commands/hmi/navi_show_constant_tbt_request.h"
#include "sdl_rpc_plugin/commands/hmi/navi_stop_stream_request.h"
#include "sdl_rpc_plugin/commands/hmi/navi_subscribe_way_points_request.h"
#include "sdl_rpc_plugin/commands/hmi/sdl_policy_update.h"
#include "sdl_rpc_plugin/commands/hmi/ui_set_icon_request.h"
#include "sdl_rpc_plugin/commands/hmi/dial_number_request.h"
#include "sdl_rpc_plugin/commands/hmi/ui_send_haptic_data_request.h"

#include "application_manager/mock_event_dispatcher.h"

namespace test {
namespace components {
namespace commands_test {
namespace hmi_commands_test {
namespace simple_requests_to_hmi_test {

using ::testing::_;
using ::testing::Types;
using ::testing::NotNull;

namespace am_commands = application_manager::commands;
using am_commands::MessageSharedPtr;
using event_engine_test::MockEventDispatcher;

class RequestToHMITest : public CommandsTest<CommandsTestMocks::kIsNice> {};

TEST_F(RequestToHMITest, BasicMethodsOverloads_SUCCESS) {
  std::shared_ptr<am_commands::RequestToHMI> command(
      CreateCommand<am_commands::RequestToHMI>());

  // Current implementation always return `true`
  EXPECT_TRUE(command->Init());
  EXPECT_NO_THROW(command->Run());
  EXPECT_TRUE(command->CleanUp());
}

TEST_F(RequestToHMITest, SendRequest_SUCCESS) {
  std::shared_ptr<am_commands::RequestToHMI> command(
      CreateCommand<am_commands::RequestToHMI>());
  EXPECT_CALL(mock_rpc_service_, SendMessageToHMI(NotNull()));

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

typedef Types<sdl_rpc_plugin::commands::hmi::DialNumberRequest,
              sdl_rpc_plugin::commands::ClosePopupRequest,
              sdl_rpc_plugin::commands::TTSSetGlobalPropertiesRequest,
              sdl_rpc_plugin::commands::TTSSpeakRequest,
              sdl_rpc_plugin::commands::TTSStopSpeakingRequest,
              sdl_rpc_plugin::commands::TTSGetSupportedLanguagesRequest,
              sdl_rpc_plugin::commands::UIAddCommandRequest,
              sdl_rpc_plugin::commands::UIAddSubmenuRequest,
              sdl_rpc_plugin::commands::UIAlertRequest,
              sdl_rpc_plugin::commands::UIChangeRegistrationRequest,
              sdl_rpc_plugin::commands::UIDeleteCommandRequest,
              sdl_rpc_plugin::commands::UIDeleteSubmenuRequest,
              sdl_rpc_plugin::commands::UIEndAudioPassThruRequest,
              sdl_rpc_plugin::commands::UIGetCapabilitiesRequest,
              sdl_rpc_plugin::commands::UIGetLanguageRequest,
              sdl_rpc_plugin::commands::UIGetSupportedLanguagesRequest,
              sdl_rpc_plugin::commands::UIPerformAudioPassThruRequest,
              sdl_rpc_plugin::commands::UIPerformInteractionRequest,
              sdl_rpc_plugin::commands::UISetMediaClockTimerRequest,
              sdl_rpc_plugin::commands::UIShowRequest,
              sdl_rpc_plugin::commands::VRAddCommandRequest,
              sdl_rpc_plugin::commands::VRChangeRegistrationRequest,
              sdl_rpc_plugin::commands::VRDeleteCommandRequest,
              sdl_rpc_plugin::commands::UISliderRequest,
              sdl_rpc_plugin::commands::TTSChangeRegistrationRequest,
              sdl_rpc_plugin::commands::TTSGetCapabilitiesRequest,
              sdl_rpc_plugin::commands::TTSGetLanguageRequest,
              sdl_rpc_plugin::commands::AllowAllAppsRequest,
              sdl_rpc_plugin::commands::BasicCommunicationSystemRequest,
              sdl_rpc_plugin::commands::ButtonGetCapabilitiesRequest,
              sdl_rpc_plugin::commands::NaviSendLocationRequest,
              sdl_rpc_plugin::commands::NaviUnSubscribeWayPointsRequest,
              sdl_rpc_plugin::commands::NaviUpdateTurnListRequest,
              sdl_rpc_plugin::commands::NaviShowConstantTBTRequest,
              sdl_rpc_plugin::commands::NaviStopStreamRequest,
              sdl_rpc_plugin::commands::NaviSubscribeWayPointsRequest,
              sdl_rpc_plugin::commands::NaviAlertManeuverRequest,
              sdl_rpc_plugin::commands::AudioStopStreamRequest,
              sdl_rpc_plugin::commands::NaviGetWayPointsRequest,
              sdl_rpc_plugin::commands::UISetGlobalPropertiesRequest,
              sdl_rpc_plugin::commands::UISendHapticDataRequest>
    RequestCommandsList;

typedef Types<sdl_rpc_plugin::commands::UIScrollableMessageRequest,
              sdl_rpc_plugin::commands::VRGetCapabilitiesRequest,
              sdl_rpc_plugin::commands::UISetAppIconRequest,
              sdl_rpc_plugin::commands::UiSetDisplayLayoutRequest,
              sdl_rpc_plugin::commands::VRGetSupportedLanguagesRequest,
              sdl_rpc_plugin::commands::VRGetLanguageRequest,
              sdl_rpc_plugin::commands::VRPerformInteractionRequest,
              sdl_rpc_plugin::commands::AllowAppRequest,
// TODO (OKozlov). Need to clarify why UT fails
// for UISetIconRequest
// am_commands::UISetIconRequest,
#if defined(PROPRIETARY_MODE) || defined(EXTERNAL_PROPRIETARY_MODE)
              sdl_rpc_plugin::commands::SDLPolicyUpdate,
#endif
              sdl_rpc_plugin::commands::hmi::DialNumberRequest>
    RequestCommandsList2;

typedef Types<sdl_rpc_plugin::commands::TTSIsReadyRequest,
              sdl_rpc_plugin::commands::UIIsReadyRequest,
              sdl_rpc_plugin::commands::NaviIsReadyRequest,
              sdl_rpc_plugin::commands::VRIsReadyRequest> RequestCommandsList3;

TYPED_TEST_CASE(RequestToHMICommandsTest, RequestCommandsList);
TYPED_TEST_CASE(RequestToHMICommandsTest2, RequestCommandsList2);
TYPED_TEST_CASE(RequestToHMICommandsTest3, RequestCommandsList3);

TYPED_TEST(RequestToHMICommandsTest, Run_SendMessageToHMI_SUCCESS) {
  typedef typename TestFixture::CommandType CommandType;

  std::shared_ptr<CommandType> command =
      this->template CreateCommand<CommandType>();

  EXPECT_CALL(this->mock_rpc_service_, SendMessageToHMI(NotNull()));

  command->Run();
}

TYPED_TEST(RequestToHMICommandsTest2, Run_SendMessageToHMI_SUCCESS) {
  typedef typename TestFixture::CommandType CommandType;

  std::shared_ptr<CommandType> command =
      this->template CreateCommand<CommandType>();
  EXPECT_CALL(this->mock_rpc_service_, SendMessageToHMI(NotNull()));

  command->Run();
}

TYPED_TEST(RequestToHMICommandsTest3, Run_SendMessageToHMI_SUCCESS) {
  typedef typename TestFixture::CommandType CommandType;

  std::shared_ptr<CommandType> command =
      this->template CreateCommand<CommandType>();
  EXPECT_CALL(this->mock_rpc_service_, SendMessageToHMI(NotNull()));

  command->Run();
}

}  // namespace simple_requests_to_hmi_test
}  // namespace hmi_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
