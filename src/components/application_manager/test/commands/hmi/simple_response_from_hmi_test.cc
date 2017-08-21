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

#include "gtest/gtest.h"
#include "utils/shared_ptr.h"
#include "smart_objects/smart_object.h"
#include "application_manager/smart_object_keys.h"
#include "application_manager/commands/commands_test.h"
#include "application_manager/commands/command.h"
#include "hmi/activate_app_response.h"
#include "hmi/basic_communication_system_response.h"
#include "hmi/navi_alert_maneuver_response.h"
#include "hmi/navi_audio_start_stream_response.h"
#include "hmi/navi_audio_stop_stream_response.h"
#include "hmi/navi_get_way_points_response.h"
#include "hmi/navi_send_location_response.h"
#include "hmi/navi_show_constant_tbt_response.h"
#include "hmi/navi_start_stream_response.h"
#include "hmi/navi_stop_stream_response.h"
#include "hmi/navi_subscribe_way_points_response.h"
#include "hmi/navi_unsubscribe_way_points_response.h"
#include "hmi/navi_update_turn_list_response.h"
#include "hmi/tts_change_registration_response.h"
#include "hmi/ui_set_app_icon_response.h"
#include "hmi/ui_set_display_layout_response.h"
#include "hmi/ui_set_global_properties_response.h"
#include "hmi/ui_scrollable_message_response.h"
#include "application_manager/mock_event_dispatcher.h"
#include "application_manager/mock_hmi_capabilities.h"
#include "application_manager/policies/mock_policy_handler_interface.h"
#include "hmi/vi_read_did_response.h"
#include "hmi/vi_subscribe_vehicle_data_response.h"
#include "hmi/vi_get_vehicle_type_response.h"
#include "hmi/vi_is_ready_response.h"
#include "hmi/dial_number_response.h"
#include "hmi/close_popup_response.h"
#include "hmi/tts_set_global_properties_response.h"
#include "hmi/tts_speak_response.h"
#include "hmi/tts_stop_speaking_response.h"
#include "hmi/tts_change_registration_response.h"
#include "hmi/ui_add_command_response.h"
#include "hmi/ui_add_submenu_response.h"
#include "hmi/ui_alert_response.h"
#include "hmi/ui_change_registration_response.h"
#include "hmi/ui_delete_command_response.h"
#include "hmi/ui_delete_submenu_response.h"
#include "hmi/ui_end_audio_pass_thru_response.h"
#include "hmi/ui_perform_audio_pass_thru_response.h"
#include "hmi/ui_perform_interaction_response.h"
#include "hmi/vi_diagnostic_message_response.h"
#include "hmi/vi_get_dtcs_response.h"
#include "hmi/ui_set_media_clock_timer_response.h"
#include "hmi/ui_show_response.h"
#include "hmi/ui_slider_response.h"
#include "hmi/vi_unsubscribe_vehicle_data_response.h"
#include "hmi/vr_add_command_response.h"
#include "hmi/vr_change_registration_response.h"
#include "hmi/vr_delete_command_response.h"
#include "hmi/vr_perform_interaction_response.h"
#include "hmi/activate_app_response.h"
#include "hmi/basic_communication_system_response.h"
#include "hmi/navi_unsubscribe_way_points_response.h"
#include "hmi/navi_update_turn_list_response.h"
#include "hmi/navi_send_location_response.h"
#include "hmi/navi_show_constant_tbt_response.h"
#include "hmi/navi_start_stream_response.h"
#include "hmi/navi_subscribe_way_points_response.h"
#include "hmi/on_find_applications.h"
#include "hmi/on_update_device_list.h"
#include "hmi/sdl_policy_update_response.h"
#include "hmi/update_app_list_response.h"
#include "hmi/update_device_list_response.h"
#include "hmi/notification_from_hmi.h"

namespace test {
namespace components {
namespace commands_test {
namespace hmi_commands_test {
namespace simple_response_from_hmi_test {

using ::testing::_;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::Types;
using ::testing::Eq;

using ::utils::SharedPtr;
using ::test::components::event_engine_test::MockEventDispatcher;

namespace am = ::application_manager;
namespace commands = am::commands;
using commands::MessageSharedPtr;

template <class CommandD>
class ResponseFromHMICommandsTest
    : public CommandsTest<CommandsTestMocks::kIsNice> {
 public:
  typedef CommandD CommandData;
  MockEventDispatcher event_dispatcher_;

  ResponseFromHMICommandsTest() {
    ON_CALL(app_mngr_, event_dispatcher())
        .WillByDefault(ReturnRef(event_dispatcher_));
  }
};

template <class Command>
class EmptyResponseFromHMICommandsTest
    : public CommandsTest<CommandsTestMocks::kIsNice> {
 public:
  typedef Command CommandType;
};

template <class Command, hmi_apis::FunctionID::eType kExpectedEventId>
struct CommandData {
  typedef Command CommandType;
  enum { kEventId = kExpectedEventId };
};

typedef Types<
    CommandData<commands::VIReadDIDResponse,
                hmi_apis::FunctionID::VehicleInfo_ReadDID>,
    CommandData<commands::TTSSpeakResponse, hmi_apis::FunctionID::TTS_Speak>,
    CommandData<commands::VISubscribeVehicleDataResponse,
                hmi_apis::FunctionID::VehicleInfo_SubscribeVehicleData>,
    CommandData<commands::hmi::DialNumberResponse,
                hmi_apis::FunctionID::BasicCommunication_DialNumber>,
    CommandData<commands::UIDeleteSubmenuResponse,
                hmi_apis::FunctionID::UI_DeleteSubMenu>,
    CommandData<commands::UIEndAudioPassThruResponse,
                hmi_apis::FunctionID::UI_EndAudioPassThru>,
    CommandData<commands::TTSSetGlobalPropertiesResponse,
                hmi_apis::FunctionID::TTS_SetGlobalProperties>,
    CommandData<commands::TTSStopSpeakingResponse,
                hmi_apis::FunctionID::TTS_StopSpeaking>,
    CommandData<commands::UIAddCommandResponse,
                hmi_apis::FunctionID::UI_AddCommand>,
    CommandData<commands::UIAddSubmenuResponse,
                hmi_apis::FunctionID::UI_AddSubMenu>,
    CommandData<commands::UIAlertResponse, hmi_apis::FunctionID::UI_Alert>,
    CommandData<commands::UIChangeRegistratioResponse,
                hmi_apis::FunctionID::UI_ChangeRegistration>,
    CommandData<commands::UIDeleteCommandResponse,
                hmi_apis::FunctionID::UI_DeleteCommand>,
    CommandData<commands::UIPerformAudioPassThruResponse,
                hmi_apis::FunctionID::UI_PerformAudioPassThru>,
    CommandData<commands::UIPerformInteractionResponse,
                hmi_apis::FunctionID::UI_PerformInteraction>,
    CommandData<commands::UIDeleteSubmenuResponse,
                hmi_apis::FunctionID::UI_DeleteSubMenu>,
    CommandData<commands::UIEndAudioPassThruResponse,
                hmi_apis::FunctionID::UI_EndAudioPassThru>,
    CommandData<commands::VIDiagnosticMessageResponse,
                hmi_apis::FunctionID::VehicleInfo_DiagnosticMessage>,
    CommandData<commands::VIGetDTCsResponse,
                hmi_apis::FunctionID::VehicleInfo_GetDTCs>,
    CommandData<commands::UISetMediaClockTimerResponse,
                hmi_apis::FunctionID::UI_SetMediaClockTimer>,
    CommandData<commands::UIShowResponse, hmi_apis::FunctionID::UI_Show>,
    CommandData<commands::VIUnsubscribeVehicleDataResponse,
                hmi_apis::FunctionID::VehicleInfo_UnsubscribeVehicleData>,
    CommandData<commands::VRAddCommandResponse,
                hmi_apis::FunctionID::VR_AddCommand>,
    CommandData<commands::VRChangeRegistrationResponse,
                hmi_apis::FunctionID::VR_ChangeRegistration>,
    CommandData<commands::VRDeleteCommandResponse,
                hmi_apis::FunctionID::VR_DeleteCommand>,
    CommandData<commands::UISliderResponse, hmi_apis::FunctionID::UI_Slider>,
    CommandData<commands::TTSChangeRegistratioResponse,
                hmi_apis::FunctionID::TTS_ChangeRegistration>,
    CommandData<commands::ActivateAppResponse,
                hmi_apis::FunctionID::BasicCommunication_ActivateApp>,
    CommandData<commands::BasicCommunicationSystemResponse,
                hmi_apis::FunctionID::BasicCommunication_SystemRequest>,
    CommandData<commands::NaviAlertManeuverResponse,
                hmi_apis::FunctionID::Navigation_AlertManeuver>,
    CommandData<commands::AudioStartStreamResponse,
                hmi_apis::FunctionID::Navigation_StartAudioStream>,
    CommandData<commands::NaviGetWayPointsResponse,
                hmi_apis::FunctionID::Navigation_GetWayPoints>,
    CommandData<commands::NaviSendLocationResponse,
                hmi_apis::FunctionID::Navigation_SendLocation>,
    CommandData<commands::NaviShowConstantTBTResponse,
                hmi_apis::FunctionID::Navigation_ShowConstantTBT>,
    CommandData<commands::NaviStartStreamResponse,
                hmi_apis::FunctionID::Navigation_StartStream>,
    CommandData<commands::NaviSubscribeWayPointsResponse,
                hmi_apis::FunctionID::Navigation_SubscribeWayPoints>,
    CommandData<commands::NaviUnsubscribeWayPointsResponse,
                hmi_apis::FunctionID::Navigation_UnsubscribeWayPoints>,
    CommandData<commands::NaviUpdateTurnListResponse,
                hmi_apis::FunctionID::Navigation_UpdateTurnList>,
    CommandData<commands::UISetAppIconResponse,
                hmi_apis::FunctionID::UI_SetAppIcon>,
    CommandData<commands::UiSetDisplayLayoutResponse,
                hmi_apis::FunctionID::UI_SetDisplayLayout>,
    CommandData<commands::UISetGlobalPropertiesResponse,
                hmi_apis::FunctionID::UI_SetGlobalProperties>,
    CommandData<commands::UISetGlobalPropertiesResponse,
                hmi_apis::FunctionID::UI_SetGlobalProperties>,
    CommandData<commands::VRPerformInteractionResponse,
                hmi_apis::FunctionID::VR_PerformInteraction>,
    CommandData<commands::UIScrollableMessageResponse,
                hmi_apis::FunctionID::UI_ScrollableMessage> >
    ResponseCommandsList;

typedef Types<commands::AudioStopStreamResponse,
              commands::NaviStopStreamResponse,
              commands::OnFindApplications,
              commands::OnUpdateDeviceList,
              commands::SDLPolicyUpdateResponse,
              commands::UpdateAppListResponse,
              commands::UpdateDeviceListResponse> EmptyResponseCommandsList;

TYPED_TEST_CASE(ResponseFromHMICommandsTest, ResponseCommandsList);

TYPED_TEST_CASE(EmptyResponseFromHMICommandsTest, EmptyResponseCommandsList);

MATCHER_P(EventIdIsEqualTo, function_id, "") {
  return static_cast<hmi_apis::FunctionID::eType>(function_id) == arg.id();
}

TYPED_TEST(ResponseFromHMICommandsTest, Run_SendMessageToHMI_SUCCESS) {
  typedef typename TestFixture::CommandData CommandData;
  typedef typename CommandData::CommandType CommandType;

  SharedPtr<CommandType> command = this->template CreateCommand<CommandType>();

  EXPECT_CALL(this->event_dispatcher_,
              raise_event(EventIdIsEqualTo(CommandData::kEventId)));

  command->Run();
}

TYPED_TEST(EmptyResponseFromHMICommandsTest, Run_SUCCESS) {
  typedef typename TestFixture::CommandType CommandType;

  SharedPtr<CommandType> command = this->template CreateCommand<CommandType>();

  command->Run();
}

class OtherResponseFromHMICommandsTest
    : public CommandsTest<CommandsTestMocks::kIsNice> {};

MATCHER_P(VehicleTypeIsEqualTo, vehicle_type, "") {
  return (*vehicle_type) == arg.asString();
}

TEST_F(OtherResponseFromHMICommandsTest, VIGetVehicleTypeResponse_Run_SUCCESS) {
  const std::string kVehicleType = "Test";

  MessageSharedPtr command_msg(CreateMessage(smart_objects::SmartType_Map));
  (*command_msg)[am::strings::msg_params][am::hmi_response::vehicle_type] =
      kVehicleType;

  SharedPtr<commands::VIGetVehicleTypeResponse> command(
      CreateCommand<commands::VIGetVehicleTypeResponse>(command_msg));

  application_manager_test::MockHMICapabilities hmi_capabilities;
  EXPECT_CALL(app_mngr_, hmi_capabilities())
      .WillOnce(ReturnRef(hmi_capabilities));

  EXPECT_CALL(hmi_capabilities,
              set_vehicle_type(VehicleTypeIsEqualTo(&kVehicleType)));

  command->Run();
}

TEST_F(OtherResponseFromHMICommandsTest, VIIsReadyResponse_Run_SUCCESS) {
  SharedPtr<commands::VIIsReadyResponse> command(
      CreateCommand<commands::VIIsReadyResponse>());

  MockEventDispatcher mock_event_dispatcher;
  EXPECT_CALL(app_mngr_, event_dispatcher())
      .WillOnce(ReturnRef(mock_event_dispatcher));
  EXPECT_CALL(mock_event_dispatcher, raise_event(_));

  command->Run();
}

MATCHER_P(CheckMsgType, msg_type, "") {
  return msg_type ==
         static_cast<int32_t>(
             (*arg)[am::strings::params][am::strings::message_type].asInt());
}

class NotificationFromHMITest
    : public CommandsTest<CommandsTestMocks::kIsNice> {};

TEST_F(NotificationFromHMITest, BasicMethodsOverloads_SUCCESS) {
  SharedPtr<commands::NotificationFromHMI> command(
      CreateCommand<commands::NotificationFromHMI>());
  // Current implementation always return `true`
  EXPECT_TRUE(command->Init());
  EXPECT_TRUE(command->CleanUp());
  EXPECT_NO_THROW(command->Run());
}

TEST_F(NotificationFromHMITest, SendNotificationToMobile_SUCCESS) {
  MessageSharedPtr command_msg(CreateMessage(smart_objects::SmartType_Map));
  (*command_msg)[am::strings::params][am::strings::message_type] =
      static_cast<int32_t>(am::MessageType::kNotification);

  SharedPtr<commands::NotificationFromHMI> command(
      CreateCommand<commands::NotificationFromHMI>());

  EXPECT_CALL(
      app_mngr_,
      ManageMobileCommand(CheckMsgType(am::MessageType::kNotification),
                          am::commands::Command::CommandOrigin::ORIGIN_SDL));

  command->SendNotificationToMobile(command_msg);
}

TEST_F(NotificationFromHMITest, CreateHMIRequest_UNSUCCESS) {
  MessageSharedPtr command_msg(CreateMessage(smart_objects::SmartType_Map));
  (*command_msg)[am::strings::msg_params] = 0;
  SharedPtr<commands::NotificationFromHMI> command(
      CreateCommand<commands::NotificationFromHMI>(command_msg));

  const uint32_t correlation_id = 1u;
  EXPECT_CALL(app_mngr_, GetNextHMICorrelationID())
      .WillOnce(Return(correlation_id));
  EXPECT_CALL(app_mngr_,
              ManageHMICommand(CheckMsgType(am::MessageType::kRequest)))
      .WillOnce(Return(false));

  command->CreateHMIRequest(hmi_apis::FunctionID::INVALID_ENUM,
                            (*command_msg)[am::strings::msg_params]);
}

}  // namespace simple_response_from_hmi_test
}  // namespace hmi_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
