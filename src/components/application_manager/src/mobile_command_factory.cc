/*
 Copyright (c) 2013, Ford Motor Company
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following
 disclaimer in the documentation and/or other materials provided with the
 distribution.

 Neither the name of the Ford Motor Company nor the names of its contributors
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
#include "application_manager/commands/mobile/get_vehicle_data_request.h"
#include "application_manager/commands/mobile/get_vehicle_data_response.h"
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
#include "interfaces/MOBILE_API.h"
#include "utils/make_shared.h"

namespace application_manager {

CommandSharedPtr MobileCommandFactory::CreateCommand(
    const commands::MessageSharedPtr& message,
    commands::Command::CommandOrigin origin) {

    CommandSharedPtr command;

  switch ((*message)[strings::params][strings::function_id].asInt()) {
    case mobile_apis::FunctionID::RegisterAppInterfaceID: {
      if ((*message)[strings::params][strings::message_type]
          == static_cast<int>(application_manager::MessageType::kRequest)) {
        command = utils::MakeShared<commands::RegisterAppInterfaceRequest>(message);
      } else {
        command = utils::MakeShared<commands::RegisterAppInterfaceResponse>(message);
      }
      break;
    }
    case mobile_apis::FunctionID::UnregisterAppInterfaceID: {
      if ((*message)[strings::params][strings::message_type]
          == static_cast<int>(application_manager::MessageType::kRequest)) {
        command = utils::MakeShared<commands::UnregisterAppInterfaceRequest>(message);
      } else {
        command = utils::MakeShared<commands::UnregisterAppInterfaceResponse>(message);
      }
      break;
    }
    case mobile_apis::FunctionID::SetGlobalPropertiesID: {
      if ((*message)[strings::params][strings::message_type]
          == static_cast<int>(application_manager::MessageType::kResponse)) {
        command = utils::MakeShared<commands::SetGlobalPropertiesResponse>(message);
      } else {
        command = utils::MakeShared<commands::SetGlobalPropertiesRequest>(message);
      }
      break;
    }
    case mobile_apis::FunctionID::ResetGlobalPropertiesID: {
      if ((*message)[strings::params][strings::message_type]
          == static_cast<int>(application_manager::MessageType::kResponse)) {
        command = utils::MakeShared<commands::ResetGlobalPropertiesResponse>(message);
      } else {
        command = utils::MakeShared<commands::ResetGlobalPropertiesRequest>(message);
      }
      break;
    }
    case mobile_apis::FunctionID::AddCommandID: {
      if ((*message)[strings::params][strings::message_type]
          == static_cast<int>(application_manager::MessageType::kResponse)) {
        command = utils::MakeShared<commands::AddCommandResponse>(message);
      } else {
        command = utils::MakeShared<commands::AddCommandRequest>(message);
      }
      break;
    }
    case mobile_apis::FunctionID::DeleteCommandID: {
      if ((*message)[strings::params][strings::message_type]
          == static_cast<int>(application_manager::MessageType::kResponse)) {
        command = utils::MakeShared<commands::DeleteCommandResponse>(message);
      } else {
        command = utils::MakeShared<commands::DeleteCommandRequest>(message);
      }
      break;
    }
    case mobile_apis::FunctionID::AddSubMenuID: {
      if ((*message)[strings::params][strings::message_type]
          == static_cast<int>(application_manager::MessageType::kResponse)) {
        command = utils::MakeShared<commands::AddSubMenuResponse>(message);
      } else {
        command = utils::MakeShared<commands::AddSubMenuRequest>(message);
      }
      break;
    }
    case mobile_apis::FunctionID::DeleteSubMenuID: {
      if ((*message)[strings::params][strings::message_type]
          == static_cast<int>(application_manager::MessageType::kResponse)) {
        command = utils::MakeShared<commands::DeleteSubMenuResponse>(message);
      } else {
        command = utils::MakeShared<commands::DeleteSubMenuRequest>(message);
      }
      break;
    }
    case mobile_apis::FunctionID::DeleteInteractionChoiceSetID: {
      if ((*message)[strings::params][strings::message_type]
          == static_cast<int>(application_manager::MessageType::kResponse)) {
        command = utils::MakeShared<commands::DeleteInteractionChoiceSetResponse>(message);
      } else {
        command = utils::MakeShared<commands::DeleteInteractionChoiceSetRequest>(message);
      }
      break;
    }
    case mobile_apis::FunctionID::AlertID: {
      if ((*message)[strings::params][strings::message_type]
          == static_cast<int>(application_manager::MessageType::kResponse)) {
        command = utils::MakeShared<commands::AlertResponse>(message);
      } else {
        command = utils::MakeShared<commands::AlertRequest>(message);
      }
      break;
    }
    case mobile_apis::FunctionID::SpeakID: {
      if ((*message)[strings::params][strings::message_type]
          == static_cast<int>(application_manager::MessageType::kResponse)) {
        command = utils::MakeShared<commands::SpeakResponse>(message);
      } else {
        command = utils::MakeShared<commands::SpeakRequest>(message);
      }
      break;
    }
    case mobile_apis::FunctionID::SliderID: {
      if ((*message)[strings::params][strings::message_type]
          == static_cast<int>(application_manager::MessageType::kResponse)) {
        command = utils::MakeShared<commands::SliderResponse>(message);
      } else {
        command = utils::MakeShared<commands::SliderRequest>(message);
      }
      break;
    }
    case mobile_apis::FunctionID::PerformAudioPassThruID: {
      if ((*message)[strings::params][strings::message_type]
          == static_cast<int>(application_manager::MessageType::kResponse)) {
        command = utils::MakeShared<commands::PerformAudioPassThruResponse>(message);
      } else {
        command = utils::MakeShared<commands::PerformAudioPassThruRequest>(message);
      }
      break;
    }
    case mobile_apis::FunctionID::CreateInteractionChoiceSetID: {
      if ((*message)[strings::params][strings::message_type]
          == static_cast<int>(application_manager::MessageType::kResponse)) {
        command = utils::MakeShared<commands::CreateInteractionChoiceSetResponse>(message);
      } else {
        command = utils::MakeShared<commands::CreateInteractionChoiceSetRequest>(message);
      }
      break;
    }
    case mobile_apis::FunctionID::PerformInteractionID: {
      if ((*message)[strings::params][strings::message_type]
          == static_cast<int>(application_manager::MessageType::kResponse)) {
        command = utils::MakeShared<commands::PerformInteractionResponse>(message);
      } else {
        command = utils::MakeShared<commands::PerformInteractionRequest>(message);
      }
      break;
    }
    case mobile_apis::FunctionID::EndAudioPassThruID: {
      if ((*message)[strings::params][strings::message_type]
          == static_cast<int>(application_manager::MessageType::kResponse)) {
        command = utils::MakeShared<commands::EndAudioPassThruResponse>(message);
      } else {
        command = utils::MakeShared<commands::EndAudioPassThruRequest>(message);
      }
      break;
    }
    case mobile_apis::FunctionID::PutFileID: {
      if ((*message)[strings::params][strings::message_type]
          == static_cast<int>(application_manager::MessageType::kResponse)) {
        command = utils::MakeShared<commands::PutFileResponse>(message);
      } else {
        command = utils::MakeShared<commands::PutFileRequest>(message);
      }
      break;
    }
    case mobile_apis::FunctionID::DeleteFileID: {
      if ((*message)[strings::params][strings::message_type]
          == static_cast<int>(application_manager::MessageType::kResponse)) {
        command = utils::MakeShared<commands::DeleteFileResponse>(message);
      } else {
        command = utils::MakeShared<commands::DeleteFileRequest>(message);
      }
      break;
    }
    case mobile_apis::FunctionID::ListFilesID: {
      if ((*message)[strings::params][strings::message_type]
          == static_cast<int>(application_manager::MessageType::kResponse)) {
        command = utils::MakeShared<commands::ListFilesResponse>(message);
      } else {
        command = utils::MakeShared<commands::ListFilesRequest>(message);
      }
      break;
    }
    case mobile_apis::FunctionID::SubscribeButtonID: {
      if ((*message)[strings::params][strings::message_type]
          == static_cast<int>(application_manager::MessageType::kResponse)) {
        command = utils::MakeShared<commands::SubscribeButtonResponse>(message);
      } else {
        command = utils::MakeShared<commands::SubscribeButtonRequest>(message);
      }
      break;
    }
    case mobile_apis::FunctionID::UnsubscribeButtonID: {
      if ((*message)[strings::params][strings::message_type]
          == static_cast<int>(application_manager::MessageType::kResponse)) {
        command = utils::MakeShared<commands::UnsubscribeButtonResponse>(message);
      } else {
        command = utils::MakeShared<commands::UnsubscribeButtonRequest>(message);
      }
      break;
    }
    case mobile_apis::FunctionID::ShowConstantTBTID: {
      if ((*message)[strings::params][strings::message_type]
          == static_cast<int>(application_manager::MessageType::kResponse)) {
        command = utils::MakeShared<commands::ShowConstantTBTResponse>(message);
      } else {
        command = utils::MakeShared<commands::ShowConstantTBTRequest>(message);
      }
      break;
    }
    case mobile_apis::FunctionID::ShowID: {
      if ((*message)[strings::params][strings::message_type]
          == static_cast<int>(application_manager::MessageType::kResponse)) {
        command = utils::MakeShared<commands::ShowResponse>(message);
      } else {
        command = utils::MakeShared<commands::ShowRequest>(message);
      }
      break;
    }
    case mobile_apis::FunctionID::SubscribeVehicleDataID: {
      if ((*message)[strings::params][strings::message_type]
          == static_cast<int>(application_manager::MessageType::kResponse)) {
        command = utils::MakeShared<commands::SubscribeVehicleDataResponse>(message);
      } else {
        command = utils::MakeShared<commands::SubscribeVehicleDataRequest>(message);
      }
      break;
    }
    case mobile_apis::FunctionID::UnsubscribeVehicleDataID: {
      if ((*message)[strings::params][strings::message_type]
          == static_cast<int>(application_manager::MessageType::kResponse)) {
        command = utils::MakeShared<commands::UnsubscribeVehicleDataResponse>(message);
      } else {
        command = utils::MakeShared<commands::UnsubscribeVehicleDataRequest>(message);
      }
      break;
    }
    case mobile_apis::FunctionID::ReadDIDID: {
      if ((*message)[strings::params][strings::message_type]
          == static_cast<int>(application_manager::MessageType::kResponse)) {
        command = utils::MakeShared<commands::ReadDIDResponse>(message);
      } else {
        command = utils::MakeShared<commands::ReadDIDRequest>(message);
      }
      break;
    }
    case mobile_apis::FunctionID::GetVehicleDataID: {
      if ((*message)[strings::params][strings::message_type]
          == static_cast<int>(application_manager::MessageType::kResponse)) {
        command = utils::MakeShared<commands::GetVehicleDataResponse>(message);
      } else {
        command = utils::MakeShared<commands::GetVehicleDataRequest>(message);
      }
      break;
    }
    case mobile_apis::FunctionID::ScrollableMessageID: {
      if ((*message)[strings::params][strings::message_type]
          == static_cast<int>(application_manager::MessageType::kResponse)) {
        command = utils::MakeShared<commands::ScrollableMessageResponse>(message);
      } else {
        command = utils::MakeShared<commands::ScrollableMessageRequest>(message);
      }
      break;
    }
    case mobile_apis::FunctionID::AlertManeuverID: {
      if ((*message)[strings::params][strings::message_type]
          == static_cast<int>(application_manager::MessageType::kResponse)) {
        command = utils::MakeShared<commands::AlertManeuverResponse>(message);
      } else {
        command = utils::MakeShared<commands::AlertManeuverRequest>(message);
      }
      break;
    }
    case mobile_apis::FunctionID::SetAppIconID: {
      if ((*message)[strings::params][strings::message_type]
          == static_cast<int>(application_manager::MessageType::kResponse)) {
        command = utils::MakeShared<commands::SetAppIconResponse>(message);
      } else {
        command = utils::MakeShared<commands::SetAppIconRequest>(message);
      }
      break;
    }
    case mobile_apis::FunctionID::SetDisplayLayoutID: {
      if ((*message)[strings::params][strings::message_type]
          == static_cast<int>(application_manager::MessageType::kResponse)) {
        command = utils::MakeShared<commands::SetDisplayLayoutResponse>(message);
      } else {
        command = utils::MakeShared<commands::SetDisplayLayoutRequest>(message);
      }
      break;
    }
    case mobile_apis::FunctionID::UpdateTurnListID: {
      if ((*message)[strings::params][strings::message_type]
          == static_cast<int>(application_manager::MessageType::kResponse)) {
        command = utils::MakeShared<commands::UpdateTurnListResponse>(message);
      } else {
        command = utils::MakeShared<commands::UpdateTurnListRequest>(message);
      }
      break;
    }
    case mobile_apis::FunctionID::ChangeRegistrationID: {
      if ((*message)[strings::params][strings::message_type]
          == static_cast<int>(application_manager::MessageType::kResponse)) {
        command = utils::MakeShared<commands::ChangeRegistrationResponse>(message);
      } else {
        command = utils::MakeShared<commands::ChangeRegistrationRequest>(message);
      }
      break;
    }
    case mobile_apis::FunctionID::GetDTCsID: {
      if ((*message)[strings::params][strings::message_type]
          == static_cast<int>(application_manager::MessageType::kResponse)) {
        command = utils::MakeShared<commands::GetDTCsResponse>(message);
      } else {
        command = utils::MakeShared<commands::GetDTCsRequest>(message);
      }
      break;
    }
    case mobile_apis::FunctionID::DiagnosticMessageID: {
      if ((*message)[strings::params][strings::message_type]
          == static_cast<int>(application_manager::MessageType::kResponse)) {
        command = utils::MakeShared<commands::DiagnosticMessageResponse>(message);
      } else {
        command = utils::MakeShared<commands::DiagnosticMessageRequest>(message);
      }
      break;
    }
    case mobile_apis::FunctionID::SetMediaClockTimerID: {
      if ((*message)[strings::params][strings::message_type]
          == static_cast<int>(application_manager::MessageType::kResponse)) {
        command = utils::MakeShared<commands::SetMediaClockTimerResponse>(message);
      } else {
        command = utils::MakeShared<commands::SetMediaClockRequest>(message);
      }
      break;
    }
    case mobile_apis::FunctionID::SystemRequestID: {
      if ((*message)[strings::params][strings::message_type]
          == static_cast<int>(application_manager::MessageType::kResponse)) {
        command = utils::MakeShared<commands::SystemResponse>(message);
      } else {
        command = utils::MakeShared<commands::SystemRequest>(message);
      }
      break;
    }
    case mobile_apis::FunctionID::SendLocationID: {
      if ((*message)[strings::params][strings::message_type]
          == static_cast<int>(application_manager::MessageType::kResponse)) {
        command = utils::MakeShared<commands::SendLocationResponse>(message);
      } else {
        command = utils::MakeShared<commands::SendLocationRequest>(message);
      }
      break;
    }
    case mobile_apis::FunctionID::DialNumberID: {
      if ((*message)[strings::params][strings::message_type]
          == static_cast<int>(application_manager::MessageType::kResponse)) {
        return  new commands::DialNumberResponse(message);
      } else {
        return new commands::DialNumberRequest(message);
      }
      break;
    }
    case mobile_apis::FunctionID::OnButtonEventID: {
      command = utils::MakeShared<commands::mobile::OnButtonEventNotification>(message);
      break;
    }
    case mobile_apis::FunctionID::OnButtonPressID: {
      command = utils::MakeShared<commands::mobile::OnButtonPressNotification>(message);
      break;
    }
    case mobile_apis::FunctionID::OnAudioPassThruID: {
      command = utils::MakeShared<commands::OnAudioPassThruNotification>(message);
      break;
    }
    case mobile_apis::FunctionID::OnVehicleDataID: {
      command = utils::MakeShared<commands::OnVehicleDataNotification>(message);
      break;
    }
    case mobile_apis::FunctionID::OnAppInterfaceUnregisteredID: {
      command = utils::MakeShared<commands::OnAppInterfaceUnregisteredNotification>(message);
      break;
    }
    case mobile_apis::FunctionID::OnCommandID: {
      command = utils::MakeShared<commands::OnCommandNotification>(message);
      break;
    }
    case mobile_apis::FunctionID::OnTBTClientStateID: {
      command = utils::MakeShared<commands::OnTBTClientStateNotification>(message);
      break;
    }
    case mobile_apis::FunctionID::OnDriverDistractionID: {
      command = utils::MakeShared<commands::mobile::OnDriverDistractionNotification>(message);
      break;
    }
    case mobile_apis::FunctionID::OnLanguageChangeID: {
      command = utils::MakeShared<commands::OnLanguageChangeNotification>(message);
      break;
    }
    case mobile_apis::FunctionID::OnPermissionsChangeID: {
      command = utils::MakeShared<commands::OnPermissionsChangeNotification>(message);
      break;
    }
    case mobile_apis::FunctionID::OnHMIStatusID: {
      if (origin == commands::Command::ORIGIN_MOBILE) {
        command = utils::MakeShared<commands::OnHMIStatusNotificationFromMobile>(message);
      } else {
        command = utils::MakeShared<commands::OnHMIStatusNotification>(message);
      }
      break;
    }
    case mobile_apis::FunctionID::OnKeyboardInputID: {
      command = utils::MakeShared<commands::mobile::OnKeyBoardInputNotification>(message);
      break;
    }
    case mobile_apis::FunctionID::OnTouchEventID: {
      command = utils::MakeShared<commands::mobile::OnTouchEventNotification>(message);
      break;
    }
    case mobile_apis::FunctionID::OnSystemRequestID: {
      command = utils::MakeShared<commands::mobile::OnSystemRequestNotification>(message);
      break;
    }
    case mobile_apis::FunctionID::OnHashChangeID: {
      command = utils::MakeShared<commands::mobile::OnHashChangeNotification>(message);
      break;
    }
    default: {
      (*message)[strings::params][strings::function_id] =
          static_cast<int32_t>(mobile_apis::FunctionID::GenericResponseID);
      command = utils::MakeShared<commands::GenericResponse>(message);
    }
  }
  return command;
}

}  // namespace application_manager
