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

#include "gtest/gtest.h"
#include "utils/shared_ptr.h"
#include "smart_objects/smart_object.h"
#include "application_manager/smart_object_keys.h"
#include "application_manager/commands/commands_test.h"
#include "application_manager/commands/command.h"
#include "hmi/allow_app_request.h"
#include "hmi/allow_all_apps_request.h"
#include "hmi/basic_communication_system_request.h"
#include "hmi/button_get_capabilities_request.h"
#include "hmi/navi_alert_maneuver_request.h"
#include "hmi/navi_audio_stop_stream_request.h"
#include "hmi/navi_get_way_points_request.h"
#include "hmi/navi_is_ready_request.h"
#include "hmi/navi_send_location_request.h"
#include "hmi/navi_show_constant_tbt_request.h"
#include "hmi/navi_stop_stream_request.h"
#include "hmi/navi_subscribe_way_points_request.h"
#include "hmi/navi_unsubscribe_way_points_request.h"
#include "hmi/navi_update_turn_list_request.h"
#include "hmi/sdl_activate_app_response.h"
#include "hmi/sdl_get_list_of_permissions_response.h"
#include "hmi/sdl_get_status_update_response.h"
#include "hmi/ui_scrollable_message_request.h"
#include "hmi/ui_set_app_icon_request.h"
#include "hmi/ui_set_display_layout_request.h"
#include "hmi/ui_set_global_properties_request.h"
#include "hmi/request_to_hmi.h"
#include "hmi/vi_get_vehicle_type_request.h"
#include "hmi/vi_is_ready_request.h"
#include "hmi/vi_read_did_request.h"
#include "hmi/vi_subscribe_vehicle_data_request.h"
#include "hmi/dial_number_request.h"
#include "hmi/tts_is_ready_request.h"
#include "hmi/tts_set_global_properties_request.h"
#include "hmi/tts_speak_request.h"
#include "hmi/tts_stop_speaking_request.h"
#include "hmi/tts_get_supported_languages_request.h"
#include "hmi/tts_change_registration_request.h"
#include "hmi/tts_get_capabilities_request.h"
#include "hmi/tts_get_language_request.h"
#include "hmi/close_popup_request.h"
#include "hmi/ui_add_command_request.h"
#include "hmi/ui_add_submenu_request.h"
#include "hmi/ui_alert_request.h"
#include "hmi/ui_change_registration_request.h"
#include "hmi/ui_delete_command_request.h"
#include "hmi/ui_delete_submenu_request.h"
#include "hmi/ui_end_audio_pass_thru_request.h"
#include "hmi/ui_get_capabilities_request.h"
#include "hmi/ui_get_language_request.h"
#include "hmi/ui_get_supported_languages_request.h"
#include "hmi/ui_is_ready_request.h"
#include "hmi/ui_perform_audio_pass_thru_request.h"
#include "hmi/ui_perform_interaction_request.h"
#include "hmi/vi_diagnostic_message_request.h"
#include "hmi/vi_get_dtcs_request.h"
#include "hmi/vi_get_vehicle_data_request.h"
#include "hmi/ui_set_media_clock_timer_request.h"
#include "hmi/ui_show_request.h"
#include "hmi/ui_slider_request.h"
#include "hmi/vi_unsubscribe_vehicle_data_request.h"
#include "hmi/vr_add_command_request.h"
#include "hmi/vr_change_registration_request.h"
#include "hmi/vr_delete_command_request.h"
#include "hmi/vr_get_capabilities_request.h"
#include "hmi/vr_get_supported_languages_request.h"
#include "hmi/vr_get_language_request.h"
#include "hmi/vr_is_ready_request.h"
#include "hmi/vr_perform_interaction_request.h"

namespace test {
namespace components {
namespace commands_test {
namespace hmi_commands_test {

using ::testing::_;
using ::testing::Types;
using ::testing::NotNull;

using ::utils::SharedPtr;

namespace commands = ::application_manager::commands;
using commands::MessageSharedPtr;

class RequestToHMITest : public CommandsTest<CommandsTestMocks::kIsNice> {};

TEST_F(RequestToHMITest, BasicMethodsOverloads_SUCCESS) {
  SharedPtr<commands::RequestToHMI> command(
      CreateCommand<commands::RequestToHMI>());

  // Current implementation always return `true`
  EXPECT_TRUE(command->Init());
  EXPECT_NO_THROW(command->Run());
  EXPECT_TRUE(command->CleanUp());
}

TEST_F(RequestToHMITest, SendRequest_SUCCESS) {
  SharedPtr<commands::RequestToHMI> command(
      CreateCommand<commands::RequestToHMI>());

  EXPECT_CALL(mock_app_manager_, SendMessageToHMI(NotNull()));

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

typedef Types<commands::VIIsReadyRequest,
              commands::VIGetVehicleTypeRequest,
              commands::VIReadDIDRequest,
              commands::VISubscribeVehicleDataRequest,
              commands::hmi::DialNumberRequest,
              commands::ClosePopupRequest,
              commands::TTSIsReadyRequest,
              commands::TTSSetGlobalPropertiesRequest,
              commands::TTSSpeakRequest,
              commands::TTSStopSpeakingRequest,
              commands::TTSGetSupportedLanguagesRequest,
              commands::UIAddCommandRequest,
              commands::UIAddSubmenuRequest,
              commands::UIAlertRequest,
              commands::UIChangeRegistrationRequest,
              commands::UIDeleteCommandRequest,
              commands::UIDeleteSubmenuRequest,
              commands::UIEndAudioPassThruRequest,
              commands::UIGetCapabilitiesRequest,
              commands::UIGetLanguageRequest,
              commands::UIGetSupportedLanguagesRequest,
              commands::UIIsReadyRequest,
              commands::UIPerformAudioPassThruRequest,
              commands::UIPerformInteractionRequest,
              commands::VIDiagnosticMessageRequest,
              commands::VIGetDTCsRequest,
              commands::VIGetVehicleDataRequest,
              commands::UISetMediaClockTimerRequest,
              commands::UIShowRequest,
              commands::VIUnsubscribeVehicleDataRequest,
              commands::VRAddCommandRequest,
              commands::VRChangeRegistrationRequest,
              commands::VRDeleteCommandRequest,
              commands::UISliderRequest,
              commands::TTSChangeRegistrationRequest,
              commands::TTSGetCapabilitiesRequest,
              commands::TTSGetLanguageRequest,
              commands::AllowAllAppsRequest,
              commands::BasicCommunicationSystemRequest,
              commands::ButtonGetCapabilitiesRequest,
              commands::NaviSendLocationRequest,
              commands::NaviShowConstantTBTRequest,
              commands::NaviStopStreamRequest,
              commands::NaviSubscribeWayPointsRequest,
              commands::NaviAlertManeuverRequest,
              commands::AudioStopStreamRequest,
              commands::NaviGetWayPointsRequest,
              commands::NaviIsReadyRequest,
              commands::UIScrollableMessageRequest,
              commands::UISetGlobalPropertiesRequest> RequestCommandsList;

typedef Types<commands::VRGetCapabilitiesRequest,
              commands::UISetAppIconRequest,
              commands::UiSetDisplayLayoutRequest,
              commands::VRGetSupportedLanguagesRequest,
              commands::VRGetLanguageRequest,
              commands::VRPerformInteractionRequest,
              commands::VRIsReadyRequest> RequestCommandsList2;

TYPED_TEST_CASE(RequestToHMICommandsTest, RequestCommandsList);
TYPED_TEST_CASE(RequestToHMICommandsTest2, RequestCommandsList2);

TYPED_TEST(RequestToHMICommandsTest, Run_SendMessageToHMI_SUCCESS) {
  typedef typename TestFixture::CommandType CommandType;

  SharedPtr<CommandType> command = this->template CreateCommand<CommandType>();

  EXPECT_CALL(this->mock_app_manager_, SendMessageToHMI(NotNull()));

  command->Run();
}

TYPED_TEST(RequestToHMICommandsTest2, Run_SendMessageToHMI_SUCCESS) {
  typedef typename TestFixture::CommandType CommandType;

  SharedPtr<CommandType> command = this->template CreateCommand<CommandType>();

  EXPECT_CALL(this->mock_app_manager_, SendMessageToHMI(NotNull()));

  command->Run();
}

}  // namespace hmi_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
