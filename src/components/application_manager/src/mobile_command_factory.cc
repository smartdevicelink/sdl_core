/*
 Copyright (c) 2013, Ford Motor Company
 All rights reserved.

 Copyright (c) 2017 Xevo Inc.
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following
 disclaimer in the documentation and/or other materials provided with the
 distribution.

 Neither the name of the copyright holders nor the names of their contributors
 may be used to endorse or promote products derived from this software
 without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
 */

#include "application_manager/mobile_command_factory.h"
#include "application_manager/commands/mobile/add_command_request.h"
#include "application_manager/commands/mobile/add_command_response.h"
#include "application_manager/commands/mobile/delete_command_request.h"
#include "application_manager/commands/mobile/delete_command_response.h"
#include "application_manager/commands/mobile/add_sub_menu_request.h"
#include "application_manager/commands/mobile/add_sub_menu_response.h"
#include "application_manager/commands/mobile/alert_request.h"
#include "application_manager/commands/mobile/alert_response.h"
#include "application_manager/commands/mobile/alert_maneuver_request.h"
#include "application_manager/commands/mobile/alert_maneuver_response.h"
#include "application_manager/commands/mobile/change_registration_request.h"
#include "application_manager/commands/mobile/change_registration_response.h"
#include "application_manager/commands/mobile/create_interaction_choice_set_request.h"
#include "application_manager/commands/mobile/create_interaction_choice_set_response.h"
#include "application_manager/commands/mobile/delete_file_request.h"
#include "application_manager/commands/mobile/delete_file_response.h"
#include "application_manager/commands/mobile/delete_interaction_choice_set_request.h"
#include "application_manager/commands/mobile/delete_interaction_choice_set_response.h"
#include "application_manager/commands/mobile/delete_sub_menu_request.h"
#include "application_manager/commands/mobile/delete_sub_menu_response.h"
#include "application_manager/commands/mobile/end_audio_pass_thru_request.h"
#include "application_manager/commands/mobile/end_audio_pass_thru_response.h"
#include "application_manager/commands/mobile/generic_response.h"
#include "application_manager/commands/mobile/get_dtcs_request.h"
#include "application_manager/commands/mobile/get_dtcs_response.h"
#include "application_manager/commands/mobile/get_system_capability_request.h"
#include "application_manager/commands/mobile/get_system_capability_response.h"
#include "application_manager/commands/mobile/get_vehicle_data_request.h"
#include "application_manager/commands/mobile/get_vehicle_data_response.h"
#include "application_manager/commands/mobile/get_way_points_request.h"
#include "application_manager/commands/mobile/get_way_points_response.h"
#include "application_manager/commands/mobile/list_files_request.h"
#include "application_manager/commands/mobile/list_files_response.h"
#include "application_manager/commands/mobile/on_app_interface_unregistered_notification.h"
#include "application_manager/commands/mobile/on_audio_pass_thru_notification.h"
#include "application_manager/commands/mobile/on_button_event_notification.h"
#include "application_manager/commands/mobile/on_button_press_notification.h"
#include "application_manager/commands/mobile/on_driver_distraction_notification.h"
#include "application_manager/commands/mobile/on_hmi_status_notification.h"
#include "application_manager/commands/mobile/on_hmi_status_notification_from_mobile.h"
#include "application_manager/commands/mobile/on_language_change_notification.h"
#include "application_manager/commands/mobile/on_command_notification.h"
#include "application_manager/commands/mobile/on_permissions_change_notification.h"
#include "application_manager/commands/mobile/on_tbt_client_state_notification.h"
#include "application_manager/commands/mobile/on_vehicle_data_notification.h"
#include "application_manager/commands/mobile/on_hash_change_notification.h"
#include "application_manager/commands/mobile/on_way_point_change_notification.h"
#include "application_manager/commands/mobile/perform_audio_pass_thru_request.h"
#include "application_manager/commands/mobile/perform_audio_pass_thru_response.h"
#include "application_manager/commands/mobile/perform_interaction_request.h"
#include "application_manager/commands/mobile/perform_interaction_response.h"
#include "application_manager/commands/mobile/put_file_request.h"
#include "application_manager/commands/mobile/put_file_response.h"
#include "application_manager/commands/mobile/read_did_request.h"
#include "application_manager/commands/mobile/read_did_response.h"
#include "application_manager/commands/mobile/register_app_interface_request.h"
#include "application_manager/commands/mobile/register_app_interface_response.h"
#include "application_manager/commands/mobile/reset_global_properties_request.h"
#include "application_manager/commands/mobile/reset_global_properties_response.h"
#include "application_manager/commands/mobile/scrollable_message_request.h"
#include "application_manager/commands/mobile/scrollable_message_response.h"
#include "application_manager/commands/mobile/set_display_layout_request.h"
#include "application_manager/commands/mobile/set_display_layout_response.h"
#include "application_manager/commands/mobile/set_global_properties_request.h"
#include "application_manager/commands/mobile/set_global_properties_response.h"
#include "application_manager/commands/mobile/set_app_icon_request.h"
#include "application_manager/commands/mobile/set_app_icon_response.h"
#include "application_manager/commands/mobile/set_media_clock_timer_request.h"
#include "application_manager/commands/mobile/set_media_clock_timer_response.h"
#include "application_manager/commands/mobile/show_constant_tbt_request.h"
#include "application_manager/commands/mobile/show_constant_tbt_response.h"
#include "application_manager/commands/mobile/show_request.h"
#include "application_manager/commands/mobile/show_response.h"
#include "application_manager/commands/mobile/slider_request.h"
#include "application_manager/commands/mobile/slider_response.h"
#include "application_manager/commands/mobile/speak_request.h"
#include "application_manager/commands/mobile/speak_response.h"
#include "application_manager/commands/mobile/subscribe_button_request.h"
#include "application_manager/commands/mobile/subscribe_button_response.h"
#include "application_manager/commands/mobile/subscribe_vehicle_data_request.h"
#include "application_manager/commands/mobile/subscribe_vehicle_data_response.h"
#include "application_manager/commands/mobile/subscribe_way_points_request.h"
#include "application_manager/commands/mobile/subscribe_way_points_response.h"
#include "application_manager/commands/mobile/unsubscribe_way_points_request.h"
#include "application_manager/commands/mobile/unsubscribe_way_points_response.h"
#include "application_manager/commands/mobile/unregister_app_interface_request.h"
#include "application_manager/commands/mobile/unregister_app_interface_response.h"
#include "application_manager/commands/mobile/unsubscribe_button_request.h"
#include "application_manager/commands/mobile/unsubscribe_button_response.h"
#include "application_manager/commands/mobile/unsubscribe_vehicle_data_request.h"
#include "application_manager/commands/mobile/unsubscribe_vehicle_data_response.h"
#include "application_manager/commands/mobile/update_turn_list_request.h"
#include "application_manager/commands/mobile/update_turn_list_response.h"
#include "application_manager/commands/mobile/system_request.h"
#include "application_manager/commands/mobile/system_response.h"
#include "application_manager/commands/mobile/on_keyboard_input_notification.h"
#include "application_manager/commands/mobile/on_touch_event_notification.h"
#include "application_manager/commands/mobile/on_system_request_notification.h"
#include "application_manager/commands/mobile/diagnostic_message_request.h"
#include "application_manager/commands/mobile/diagnostic_message_response.h"
#include "application_manager/commands/mobile/send_location_request.h"
#include "application_manager/commands/mobile/send_location_response.h"
#include "application_manager/commands/mobile/dial_number_request.h"
#include "application_manager/commands/mobile/dial_number_response.h"
#include "application_manager/commands/mobile/send_haptic_data_request.h"
#include "application_manager/commands/mobile/send_haptic_data_response.h"
#include "interfaces/MOBILE_API.h"
#include <memory>

namespace application_manager {

CommandSharedPtr MobileCommandFactory::CreateCommand(
    const commands::MessageSharedPtr& message,
    commands::Command::CommandOrigin origin,
    ApplicationManager& application_manager) {
  CommandSharedPtr command;

  switch ((*message)[strings::params][strings::function_id].asInt()) {
    case mobile_apis::FunctionID::RegisterAppInterfaceID: {
      if ((*message)[strings::params][strings::message_type] ==
          static_cast<int>(application_manager::MessageType::kRequest)) {
        command.reset(new commands::RegisterAppInterfaceRequest(
            message, application_manager));

      } else {
        command.reset(new commands::RegisterAppInterfaceResponse(
            message, application_manager));
      }
      break;
    }
    case mobile_apis::FunctionID::UnregisterAppInterfaceID: {
      if ((*message)[strings::params][strings::message_type] ==
          static_cast<int>(application_manager::MessageType::kRequest)) {
        command.reset(new commands::UnregisterAppInterfaceRequest(
            message, application_manager));
      } else {
        command.reset(new commands::UnregisterAppInterfaceResponse(
            message, application_manager));
      }
      break;
    }
    case mobile_apis::FunctionID::SetGlobalPropertiesID: {
      if ((*message)[strings::params][strings::message_type] ==
          static_cast<int>(application_manager::MessageType::kResponse)) {
        command.reset(new commands::SetGlobalPropertiesResponse(
            message, application_manager));
      } else {
        command.reset(new commands::SetGlobalPropertiesRequest(
            message, application_manager));
      }
      break;
    }
    case mobile_apis::FunctionID::ResetGlobalPropertiesID: {
      if ((*message)[strings::params][strings::message_type] ==
          static_cast<int>(application_manager::MessageType::kResponse)) {
        command.reset(new commands::ResetGlobalPropertiesResponse(
            message, application_manager));
      } else {
        command.reset(new commands::ResetGlobalPropertiesRequest(
            message, application_manager));
      }
      break;
    }
    case mobile_apis::FunctionID::AddCommandID: {
      if ((*message)[strings::params][strings::message_type] ==
          static_cast<int>(application_manager::MessageType::kResponse)) {
        command.reset(
            new commands::AddCommandResponse(message, application_manager));
      } else {
        command.reset(
            new commands::AddCommandRequest(message, application_manager));
      }
      break;
    }
    case mobile_apis::FunctionID::DeleteCommandID: {
      if ((*message)[strings::params][strings::message_type] ==
          static_cast<int>(application_manager::MessageType::kResponse)) {
        command.reset(
            new commands::DeleteCommandResponse(message, application_manager));
      } else {
        command.reset(
            new commands::DeleteCommandRequest(message, application_manager));
      }
      break;
    }
    case mobile_apis::FunctionID::AddSubMenuID: {
      if ((*message)[strings::params][strings::message_type] ==
          static_cast<int>(application_manager::MessageType::kResponse)) {
        command.reset(
            new commands::AddSubMenuResponse(message, application_manager));
      } else {
        command.reset(
            new commands::AddSubMenuRequest(message, application_manager));
      }
      break;
    }
    case mobile_apis::FunctionID::DeleteSubMenuID: {
      if ((*message)[strings::params][strings::message_type] ==
          static_cast<int>(application_manager::MessageType::kResponse)) {
        command.reset(
            new commands::DeleteSubMenuResponse(message, application_manager));
      } else {
        command.reset(
            new commands::DeleteSubMenuRequest(message, application_manager));
      }
      break;
    }
    case mobile_apis::FunctionID::DeleteInteractionChoiceSetID: {
      if ((*message)[strings::params][strings::message_type] ==
          static_cast<int>(application_manager::MessageType::kResponse)) {
        command.reset(new commands::DeleteInteractionChoiceSetResponse(
            message, application_manager));
      } else {
        command.reset(new commands::DeleteInteractionChoiceSetRequest(
            message, application_manager));
      }
      break;
    }
    case mobile_apis::FunctionID::AlertID: {
      if ((*message)[strings::params][strings::message_type] ==
          static_cast<int>(application_manager::MessageType::kResponse)) {
        command.reset(
            new commands::AlertResponse(message, application_manager));
      } else {
        command.reset(new commands::AlertRequest(message, application_manager));
      }
      break;
    }
    case mobile_apis::FunctionID::SpeakID: {
      if ((*message)[strings::params][strings::message_type] ==
          static_cast<int>(application_manager::MessageType::kResponse)) {
        command.reset(
            new commands::SpeakResponse(message, application_manager));
      } else {
        command.reset(new commands::SpeakRequest(message, application_manager));
      }
      break;
    }
    case mobile_apis::FunctionID::SliderID: {
      if ((*message)[strings::params][strings::message_type] ==
          static_cast<int>(application_manager::MessageType::kResponse)) {
        command.reset(
            new commands::SliderResponse(message, application_manager));
      } else {
        command.reset(
            new commands::SliderRequest(message, application_manager));
      }
      break;
    }
    case mobile_apis::FunctionID::PerformAudioPassThruID: {
      if ((*message)[strings::params][strings::message_type] ==
          static_cast<int>(application_manager::MessageType::kResponse)) {
        command.reset(new commands::PerformAudioPassThruResponse(
            message, application_manager));
      } else {
        command.reset(new commands::PerformAudioPassThruRequest(
            message, application_manager));
      }
      break;
    }
    case mobile_apis::FunctionID::CreateInteractionChoiceSetID: {
      if ((*message)[strings::params][strings::message_type] ==
          static_cast<int>(application_manager::MessageType::kResponse)) {
        command.reset(new commands::CreateInteractionChoiceSetResponse(
            message, application_manager));
      } else {
        command.reset(new commands::CreateInteractionChoiceSetRequest(
            message, application_manager));
      }
      break;
    }
    case mobile_apis::FunctionID::PerformInteractionID: {
      if ((*message)[strings::params][strings::message_type] ==
          static_cast<int>(application_manager::MessageType::kResponse)) {
        command.reset(new commands::PerformInteractionResponse(
            message, application_manager));
      } else {
        command.reset(new commands::PerformInteractionRequest(
            message, application_manager));
      }
      break;
    }
    case mobile_apis::FunctionID::EndAudioPassThruID: {
      if ((*message)[strings::params][strings::message_type] ==
          static_cast<int>(application_manager::MessageType::kResponse)) {
        command.reset(new commands::EndAudioPassThruResponse(
            message, application_manager));
      } else {
        command.reset(new commands::EndAudioPassThruRequest(
            message, application_manager));
      }
      break;
    }
    case mobile_apis::FunctionID::PutFileID: {
      if ((*message)[strings::params][strings::message_type] ==
          static_cast<int>(application_manager::MessageType::kResponse)) {
        command.reset(
            new commands::PutFileResponse(message, application_manager));
      } else {
        command.reset(
            new commands::PutFileRequest(message, application_manager));
      }
      break;
    }
    case mobile_apis::FunctionID::DeleteFileID: {
      if ((*message)[strings::params][strings::message_type] ==
          static_cast<int>(application_manager::MessageType::kResponse)) {
        command.reset(
            new commands::DeleteFileResponse(message, application_manager));
      } else {
        command.reset(
            new commands::DeleteFileRequest(message, application_manager));
      }
      break;
    }
    case mobile_apis::FunctionID::ListFilesID: {
      if ((*message)[strings::params][strings::message_type] ==
          static_cast<int>(application_manager::MessageType::kResponse)) {
        command.reset(
            new commands::ListFilesResponse(message, application_manager));
      } else {
        command.reset(
            new commands::ListFilesRequest(message, application_manager));
      }
      break;
    }
    case mobile_apis::FunctionID::SubscribeButtonID: {
      if ((*message)[strings::params][strings::message_type] ==
          static_cast<int>(application_manager::MessageType::kResponse)) {
        command.reset(new commands::SubscribeButtonResponse(
            message, application_manager));
      } else {
        command.reset(
            new commands::SubscribeButtonRequest(message, application_manager));
      }
      break;
    }
    case mobile_apis::FunctionID::UnsubscribeButtonID: {
      if ((*message)[strings::params][strings::message_type] ==
          static_cast<int>(application_manager::MessageType::kResponse)) {
        command.reset(new commands::UnsubscribeButtonResponse(
            message, application_manager));
      } else {
        command.reset(new commands::UnsubscribeButtonRequest(
            message, application_manager));
      }
      break;
    }
    case mobile_apis::FunctionID::ShowConstantTBTID: {
      if ((*message)[strings::params][strings::message_type] ==
          static_cast<int>(application_manager::MessageType::kResponse)) {
        command.reset(new commands::ShowConstantTBTResponse(
            message, application_manager));
      } else {
        command.reset(
            new commands::ShowConstantTBTRequest(message, application_manager));
      }
      break;
    }
    case mobile_apis::FunctionID::ShowID: {
      if ((*message)[strings::params][strings::message_type] ==
          static_cast<int>(application_manager::MessageType::kResponse)) {
        command.reset(new commands::ShowResponse(message, application_manager));
      } else {
        command.reset(new commands::ShowRequest(message, application_manager));
      }
      break;
    }
    case mobile_apis::FunctionID::GetWayPointsID: {
      if ((*message)[strings::params][strings::message_type] ==
          static_cast<int>(application_manager::MessageType::kResponse)) {
        command = std::make_shared<commands::GetWayPointsResponse>(
            message, application_manager);
      } else {
        command = std::make_shared<commands::GetWayPointsRequest>(
            message, application_manager);
      }
      break;
    }
    case mobile_apis::FunctionID::SubscribeVehicleDataID: {
      if ((*message)[strings::params][strings::message_type] ==
          static_cast<int>(application_manager::MessageType::kResponse)) {
        command.reset(new commands::SubscribeVehicleDataResponse(
            message, application_manager));
      } else {
        command.reset(new commands::SubscribeVehicleDataRequest(
            message, application_manager));
      }
      break;
    }
    case mobile_apis::FunctionID::UnsubscribeVehicleDataID: {
      if ((*message)[strings::params][strings::message_type] ==
          static_cast<int>(application_manager::MessageType::kResponse)) {
        command.reset(new commands::UnsubscribeVehicleDataResponse(
            message, application_manager));
      } else {
        command.reset(new commands::UnsubscribeVehicleDataRequest(
            message, application_manager));
      }
      break;
    }
    case mobile_apis::FunctionID::SubscribeWayPointsID: {
      if ((*message)[strings::params][strings::message_type] ==
          static_cast<int>(application_manager::MessageType::kResponse)) {
        command = std::make_shared<commands::SubscribeWayPointsResponse>(
            message, application_manager);
      } else {
        command = std::make_shared<commands::SubscribeWayPointsRequest>(
            message, application_manager);
      }
      break;
    }
    case mobile_apis::FunctionID::UnsubscribeWayPointsID: {
      if ((*message)[strings::params][strings::message_type] ==
          static_cast<int>(application_manager::MessageType::kResponse)) {
        command = std::make_shared<commands::UnsubscribeWayPointsResponse>(
            message, application_manager);
      } else {
        command = std::make_shared<commands::UnSubscribeWayPointsRequest>(
            message, application_manager);
      }
      break;
    }
    case mobile_apis::FunctionID::GetSystemCapabilityID: {
      if ((*message)[strings::params][strings::message_type] ==
          static_cast<int>(application_manager::MessageType::kResponse)) {
        command.reset(new commands::GetSystemCapabilityResponse(
            message, application_manager));
      } else {
        command.reset(new commands::GetSystemCapabilityRequest(
            message, application_manager));
      }
      break;
    }
    case mobile_apis::FunctionID::ReadDIDID: {
      if ((*message)[strings::params][strings::message_type] ==
          static_cast<int>(application_manager::MessageType::kResponse)) {
        command.reset(
            new commands::ReadDIDResponse(message, application_manager));
      } else {
        command.reset(
            new commands::ReadDIDRequest(message, application_manager));
      }
      break;
    }
    case mobile_apis::FunctionID::GetVehicleDataID: {
      if ((*message)[strings::params][strings::message_type] ==
          static_cast<int>(application_manager::MessageType::kResponse)) {
        command.reset(
            new commands::GetVehicleDataResponse(message, application_manager));
      } else {
        command.reset(
            new commands::GetVehicleDataRequest(message, application_manager));
      }
      break;
    }
    case mobile_apis::FunctionID::ScrollableMessageID: {
      if ((*message)[strings::params][strings::message_type] ==
          static_cast<int>(application_manager::MessageType::kResponse)) {
        command.reset(new commands::ScrollableMessageResponse(
            message, application_manager));
      } else {
        command.reset(new commands::ScrollableMessageRequest(
            message, application_manager));
      }
      break;
    }
    case mobile_apis::FunctionID::AlertManeuverID: {
      if ((*message)[strings::params][strings::message_type] ==
          static_cast<int>(application_manager::MessageType::kResponse)) {
        command.reset(
            new commands::AlertManeuverResponse(message, application_manager));
      } else {
        command.reset(
            new commands::AlertManeuverRequest(message, application_manager));
      }
      break;
    }
    case mobile_apis::FunctionID::SetAppIconID: {
      if ((*message)[strings::params][strings::message_type] ==
          static_cast<int>(application_manager::MessageType::kResponse)) {
        command.reset(
            new commands::SetAppIconResponse(message, application_manager));
      } else {
        command.reset(
            new commands::SetAppIconRequest(message, application_manager));
      }
      break;
    }
    case mobile_apis::FunctionID::SetDisplayLayoutID: {
      if ((*message)[strings::params][strings::message_type] ==
          static_cast<int>(application_manager::MessageType::kResponse)) {
        command.reset(new commands::SetDisplayLayoutResponse(
            message, application_manager));
      } else {
        command.reset(new commands::SetDisplayLayoutRequest(
            message, application_manager));
      }
      break;
    }
    case mobile_apis::FunctionID::UpdateTurnListID: {
      if ((*message)[strings::params][strings::message_type] ==
          static_cast<int>(application_manager::MessageType::kResponse)) {
        command.reset(
            new commands::UpdateTurnListResponse(message, application_manager));
      } else {
        command.reset(
            new commands::UpdateTurnListRequest(message, application_manager));
      }
      break;
    }
    case mobile_apis::FunctionID::ChangeRegistrationID: {
      if ((*message)[strings::params][strings::message_type] ==
          static_cast<int>(application_manager::MessageType::kResponse)) {
        command.reset(new commands::ChangeRegistrationResponse(
            message, application_manager));
      } else {
        command.reset(new commands::ChangeRegistrationRequest(
            message, application_manager));
      }
      break;
    }
    case mobile_apis::FunctionID::GetDTCsID: {
      if ((*message)[strings::params][strings::message_type] ==
          static_cast<int>(application_manager::MessageType::kResponse)) {
        command.reset(
            new commands::GetDTCsResponse(message, application_manager));
      } else {
        command.reset(
            new commands::GetDTCsRequest(message, application_manager));
      }
      break;
    }
    case mobile_apis::FunctionID::DiagnosticMessageID: {
      if ((*message)[strings::params][strings::message_type] ==
          static_cast<int>(application_manager::MessageType::kResponse)) {
        command.reset(new commands::DiagnosticMessageResponse(
            message, application_manager));
      } else {
        command.reset(new commands::DiagnosticMessageRequest(
            message, application_manager));
      }
      break;
    }
    case mobile_apis::FunctionID::SetMediaClockTimerID: {
      if ((*message)[strings::params][strings::message_type] ==
          static_cast<int>(application_manager::MessageType::kResponse)) {
        command.reset(new commands::SetMediaClockTimerResponse(
            message, application_manager));
      } else {
        command.reset(
            new commands::SetMediaClockRequest(message, application_manager));
      }
      break;
    }
    case mobile_apis::FunctionID::SystemRequestID: {
      if ((*message)[strings::params][strings::message_type] ==
          static_cast<int>(application_manager::MessageType::kResponse)) {
        command.reset(
            new commands::SystemResponse(message, application_manager));
      } else {
        command.reset(
            new commands::SystemRequest(message, application_manager));
      }
      break;
    }
    case mobile_apis::FunctionID::SendLocationID: {
      if ((*message)[strings::params][strings::message_type] ==
          static_cast<int>(application_manager::MessageType::kResponse)) {
        command.reset(
            new commands::SendLocationResponse(message, application_manager));
      } else {
        command.reset(
            new commands::SendLocationRequest(message, application_manager));
      }
      break;
    }
    case mobile_apis::FunctionID::DialNumberID: {
      if ((*message)[strings::params][strings::message_type] ==
          static_cast<int>(application_manager::MessageType::kResponse)) {
        command.reset(
            new commands::DialNumberResponse(message, application_manager));
      } else {
        command.reset(
            new commands::DialNumberRequest(message, application_manager));
      }
      break;
    }
    case mobile_apis::FunctionID::SendHapticDataID: {
      if ((*message)[strings::params][strings::message_type] ==
          static_cast<int>(application_manager::MessageType::kResponse)) {
        command.reset(
            new commands::SendHapticDataResponse(message, application_manager));
      } else {
        command.reset(
            new commands::SendHapticDataRequest(message, application_manager));
      }
      break;
    }
    case mobile_apis::FunctionID::OnButtonEventID: {
      command.reset(new commands::mobile::OnButtonEventNotification(
          message, application_manager));
      break;
    }
    case mobile_apis::FunctionID::OnButtonPressID: {
      command.reset(new commands::mobile::OnButtonPressNotification(
          message, application_manager));
      break;
    }
    case mobile_apis::FunctionID::OnAudioPassThruID: {
      command.reset(new commands::OnAudioPassThruNotification(
          message, application_manager));
      break;
    }
    case mobile_apis::FunctionID::OnVehicleDataID: {
      command.reset(new commands::OnVehicleDataNotification(
          message, application_manager));
      break;
    }
    case mobile_apis::FunctionID::OnAppInterfaceUnregisteredID: {
      command.reset(new commands::OnAppInterfaceUnregisteredNotification(
          message, application_manager));
      break;
    }
    case mobile_apis::FunctionID::OnCommandID: {
      command.reset(
          new commands::OnCommandNotification(message, application_manager));
      break;
    }
    case mobile_apis::FunctionID::OnTBTClientStateID: {
      command.reset(new commands::OnTBTClientStateNotification(
          message, application_manager));
      break;
    }
    case mobile_apis::FunctionID::OnDriverDistractionID: {
      command.reset(new commands::mobile::OnDriverDistractionNotification(
          message, application_manager));
      break;
    }
    case mobile_apis::FunctionID::OnLanguageChangeID: {
      command.reset(new commands::OnLanguageChangeNotification(
          message, application_manager));
      break;
    }
    case mobile_apis::FunctionID::OnPermissionsChangeID: {
      command.reset(new commands::OnPermissionsChangeNotification(
          message, application_manager));
      break;
    }
    case mobile_apis::FunctionID::OnHMIStatusID: {
      if (commands::Command::ORIGIN_SDL == origin) {
        command.reset(new commands::OnHMIStatusNotification(
            message, application_manager));
      } else {
        command.reset(new commands::OnHMIStatusNotificationFromMobile(
            message, application_manager));
      }
      break;
    }
    case mobile_apis::FunctionID::OnKeyboardInputID: {
      command.reset(new commands::mobile::OnKeyBoardInputNotification(
          message, application_manager));
      break;
    }
    case mobile_apis::FunctionID::OnTouchEventID: {
      command.reset(new commands::mobile::OnTouchEventNotification(
          message, application_manager));
      break;
    }
    case mobile_apis::FunctionID::OnSystemRequestID: {
      command.reset(new commands::mobile::OnSystemRequestNotification(
          message, application_manager));
      break;
    }
    case mobile_apis::FunctionID::OnHashChangeID: {
      command.reset(new commands::mobile::OnHashChangeNotification(
          message, application_manager));
      break;
    }
    case mobile_apis::FunctionID::OnWayPointChangeID: {
      command = std::make_shared<commands::OnWayPointChangeNotification>(
          message, application_manager);
      break;
    }
    default: {
      (*message)[strings::params][strings::function_id] =
          static_cast<int32_t>(mobile_apis::FunctionID::GenericResponseID);
      command.reset(
          new commands::GenericResponse(message, application_manager));
    }
  }
  return command;
}

}  // namespace application_manager
