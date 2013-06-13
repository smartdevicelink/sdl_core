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
#include "application_manager/commands/register_app_interface_command.h"
#include "application_manager/commands/unregister_app_interface_command.h"
#include "application_manager/commands/register_app_interface_response_command.h"
#include "application_manager/commands/unregister_app_interface_response_command.h"
#include "application_manager/commands/generic_response_command.h"
#include "application_manager/commands/set_global_properties_command.h"
#include "application_manager/commands/set_global_properties_response_command.h"
#include "application_manager/commands/reset_global_properties_command.h"
#include "application_manager/commands/reset_global_properties_response_command.h"
#include "application_manager/commands/add_command_request.h"
#include "application_manager/commands/add_command_response.h"
#include "application_manager/commands/delete_sub_menu_command.h"
#include "application_manager/commands/delete_sub_menu_response_command.h"
#include "application_manager/commands/add_sub_menu_command.h"
#include "application_manager/commands/add_sub_menu_response_command.h"
#include "application_manager/commands/delete_interaction_choice_set_command.h"
#include "application_manager/commands/delete_interaction_choice_set_response_command.h"
#include "application_manager/commands/alert_command.h"
#include "application_manager/commands/alert_response_command.h"
#include "application_manager/commands/perform_audio_pass_thru_command.h"
#include "application_manager/commands/perform_audio_pass_thru_response_command.h"
#include "application_manager/commands/end_audio_pass_thru_command.h"
#include "application_manager/commands/end_audio_pass_thru_response_command.h"
#include "application_manager/commands/create_interaction_choice_set_command.h"
#include "application_manager/commands/create_interaction_choice_set_command_response.h"
#include "application_manager/commands/perform_interaction_command.h"
#include "application_manager/commands/perform_interaction_response_command.h"
#include "application_manager/commands/put_file_command.h"
#include "application_manager/commands/put_file_response_command.h"
#include "application_manager/commands/delete_file_command.h"
#include "application_manager/commands/delete_file_response_command.h"
#include "application_manager/commands/list_files_command.h"
#include "application_manager/commands/list_files_response_command.h"
#include "application_manager/commands/subscribe_button_command.h"
#include "application_manager/commands/subscribe_button_response_command.h"
#include "application_manager/commands/unsubscribe_button_command.h"
#include "application_manager/commands/unsubscribe_button_response_command.h"
#include "application_manager/commands/show_constant_tbt_command.h"
#include "application_manager/commands/show_constant_tbt_response_command.h"
#include "application_manager/commands/subscribe_vehicle_data_command.h"
#include "application_manager/commands/subscribe_vehicle_data_response_command.h"
#include "application_manager/commands/unsubscribe_vehicle_data_command.h"
#include "application_manager/commands/unsubscribe_vehicle_data_response_command.h"
#include "application_manager/commands/read_did_command.h"
#include "application_manager/commands/read_did_response_command.h"
#include "application_manager/commands/get_vehicle_data_command.h"
#include "application_manager/commands/get_vehicle_data_response_command.h"
#include "application_manager/commands/alert_maneuver_command.h"
#include "application_manager/commands/alert_maneuver_response_command.h"
#include "application_manager/commands/set_icon_command.h"
#include "application_manager/commands/set_icon_response_command.h"
#include "application_manager/commands/set_display_layout_command.h"
#include "application_manager/commands/set_display_layout_response_command.h"
#include "application_manager/commands/update_turn_list_command.h"
#include "application_manager/commands/update_turn_list_response_command.h"
#include "application_manager/commands/on_app_interface_unregistered_command.h"
#include "application_manager/commands/change_registration_command.h"
#include "application_manager/commands/change_registration_response_command.h"
#include "application_manager/commands/on_audio_pass_thru_command.h"
#include "application_manager/commands/on_button_event_command.h"
#include "application_manager/commands/on_button_press_command.h"
#include "application_manager/commands/on_vehicle_data_command.h"
#include "application_manager/commands/on_menu_entry_command.h"
#include "application_manager/commands/on_tbt_client_state_command.h"
#include "application_manager/commands/on_driver_distraction_command.h"
#include "application_manager/commands/on_language_change_command.h"
#include "application_manager/commands/on_permissions_change_command.h"
#include "application_manager/commands/on_hmi_status_command.h"
#include "application_manager/commands/get_dtcs_command.h"
#include "application_manager/commands/get_dtcs_response_command.h"


#include "interfaces/v4_protocol_v2_0_revT.h"

namespace application_manager {

CommandSharedPtr MobileCommandFactory::CreateCommand(
  const MessageSharedPtr& message) {
  CommandSharedPtr command(NULL);

  switch (static_cast<int>((*message)[strings::params][strings::function_id])) {
    case NsSmartDeviceLinkRPC::V2::FunctionID::eType::RegisterAppInterfaceID: {
      if ((*message)[strings::params][strings::message_type] ==
          MessageType::kRequest) {
        command.reset(new commands::RegisterAppInterfaceCommand(message));
      } else
        command.reset(
          new commands::RegisterAppInterfaceResponseCommand(message));
      break;
    }
    case NsSmartDeviceLinkRPC::V2::FunctionID::eType::UnregisterAppInterfaceID: {
      if ((*message)[strings::params][strings::message_type] ==
          MessageType::kRequest) {
        command.reset(new commands::UnregisterAppInterfaceCommand(message));
      } else
        command.reset(
          new commands::UnregisterAppInterfaceResponseCommand(message));
      break;
    }
    case NsSmartDeviceLinkRPC::V2::FunctionID::eType::SetGlobalPropertiesID: {
      if ((*message)[strings::params][strings::message_type] == MessageType::kResponse) {
        command.reset(new commands::SetGlobalPropertiesResponseCommand(message));
      } else {
        command.reset(new commands::SetGlobalPropertiesCommand(message));
      }
      break;
    }
    case NsSmartDeviceLinkRPC::V2::FunctionID::eType::ResetGlobalPropertiesID: {
      if ((*message)[strings::params][strings::message_type] == MessageType::kResponse) {
        command.reset(new commands::ResetGlobalPropertiesResponseCommand(message));
      } else {
        command.reset(new commands::ResetGlobalPropertiesCommand(message));
      }
      break;
    }
    case NsSmartDeviceLinkRPC::V2::FunctionID::eType::AddCommandID: {
      if ((*message)[strings::params][strings::message_type] == MessageType::kResponse) {
        command.reset(new commands::AddCommandResponse(message));
      } else {
        command.reset(new commands::AddCommandRequest(message));
      }
      break;
    }
    case NsSmartDeviceLinkRPC::V2::FunctionID::eType::AddSubMenuID: {
      if ((*message)[strings::params][strings::message_type] == MessageType::kResponse) {
        command.reset(new commands::AddSubMenuResponseCommand(message));
      } else {
        command.reset(new commands::AddSubMenuCommand(message));
      }
      break;
    }
    case NsSmartDeviceLinkRPC::V2::FunctionID::eType::DeleteSubMenuID: {
      if ((*message)[strings::params][strings::message_type] == MessageType::kResponse) {
        command.reset(new commands::DeleteSubMenuResponseCommand(message));
      } else {
        command.reset(new commands::DeleteSubMenuCommand(message));
      }
      break;
    }
    case NsSmartDeviceLinkRPC::V2::FunctionID::eType::DeleteInteractionChoiceSetID: {
      if ((*message)[strings::params][strings::message_type] == MessageType::kResponse) {
        command.reset(new commands::DeleteInteractionChoiceSetResponse(message));
      } else {
        command.reset(new commands::DeleteInteractionChoiceSetCommand(message));
      }
      break;
    }
    case NsSmartDeviceLinkRPC::V2::FunctionID::eType::AlertID: {
      if ((*message)[strings::params][strings::message_type] == MessageType::kResponse) {
        command.reset(new commands::AlertCommandResponse(message));
      } else {
        command.reset(new commands::AlertCommandRequest(message));
      }
      break;
    }
    case NsSmartDeviceLinkRPC::V2::FunctionID::eType::PerformAudioPassThruID: {
      if ((*message)[strings::params][strings::message_type] == MessageType::kResponse) {
        command.reset(new commands::PerformAudioPassThruCommandResponse(message));
      } else {
        command.reset(new commands::PerformAudioPassThruCommandRequest(message));
      }
      break;
    }
    case NsSmartDeviceLinkRPC::V2::FunctionID::eType::CreateInteractionChoiceSetID: {
      if ((*message)[strings::params][strings::message_type] == MessageType::kResponse) {
        command.reset(new commands::CreateInteractionChoiceSetResponseCommand(message));
      } else {
        command.reset(new commands::CreateInteractionChoiceSetCommand(message));
      }
      break;
    }
    case NsSmartDeviceLinkRPC::V2::FunctionID::eType::PerformInteractionID: {
      if ((*message)[strings::params][strings::message_type] == MessageType::kResponse) {
        command.reset(new commands::PerformInteractionResponseCommand(message));
      } else {
        command.reset(new commands::PerformInteractionCommand(message));
      }
      break;
    }
    case NsSmartDeviceLinkRPC::V2::FunctionID::eType::EndAudioPassThruID: {
      if ((*message)[strings::params][strings::message_type] == MessageType::kResponse) {
        command.reset(new commands::EndAudioPassThruCommandResponse(message));
      } else {
        command.reset(new commands::EndAudioPassThruCommandRequest(message));
      }
      break;
    }
    case NsSmartDeviceLinkRPC::V2::FunctionID::eType::PutFileID: {
      if ((*message)[strings::params][strings::message_type] == MessageType::kResponse) {
        command.reset(new commands::PutFileResponseCommand(message));
      } else {
        command.reset(new commands::PutFileCommand(message));
      }
      break;
    }
    case NsSmartDeviceLinkRPC::V2::FunctionID::eType::DeleteFileID: {
      if ((*message)[strings::params][strings::message_type] == MessageType::kResponse) {
        command.reset(new commands::DeleteFileResponseCommand(message));
      } else {
        command.reset(new commands::DeleteFileCommand(message));
      }
      break;
    }
    case NsSmartDeviceLinkRPC::V2::FunctionID::eType::ListFilesID: {
      if ((*message)[strings::params][strings::message_type] == MessageType::kResponse) {
        command.reset(new commands::ListFilesResponseCommand(message));
      } else {
        command.reset(new commands::ListFilesCommand(message));
      }
      break;
    }
    case NsSmartDeviceLinkRPC::V2::FunctionID::eType::SubscribeButtonID: {
      if ((*message)[strings::params][strings::message_type] == MessageType::kResponse) {
        command.reset(new commands::SubscribeButtonCommandResponse(message));
      } else {
        command.reset(new commands::SubscribeButtonCommandRequest(message));
      }
      break;
    }
    case NsSmartDeviceLinkRPC::V2::FunctionID::eType::UnsubscribeButtonID: {
      if ((*message)[strings::params][strings::message_type] == MessageType::kResponse) {
        command.reset(new commands::UnsubscribeButtonCommandResponse(message));
      } else {
        command.reset(new commands::UnsubscribeButtonCommandRequest(message));
      }
      break;
    }
    case NsSmartDeviceLinkRPC::V2::FunctionID::eType::ShowConstantTBTID: {
      if ((*message)[strings::params][strings::message_type] == MessageType::kResponse) {
        command.reset(new commands::ShowConstantTBTResponseCommand(message));
      } else {
        command.reset(new commands::ShowConstantTBTCommand(message));
      }
      break;
    }
    case NsSmartDeviceLinkRPC::V2::FunctionID::eType::SubscribeVehicleDataID: {
      if ((*message)[strings::params][strings::message_type] == MessageType::kResponse) {
        command.reset(new commands::SubscribeVehicleDataCommandResponse(message));
      } else {
        command.reset(new commands::SubscribeVehicleDataCommandRequest(message));
      }
      break;
    }
    case NsSmartDeviceLinkRPC::V2::FunctionID::eType::UnsubscribeVehicleDataID: {
      if ((*message)[strings::params][strings::message_type] == MessageType::kResponse) {
        command.reset(new commands::UnsubscribeVehicleDataCommandResponse(message));
      } else {
        command.reset(new commands::UnsubscribeVehicleDataCommandRequest(message));
      }
      break;
    }
    case NsSmartDeviceLinkRPC::V2::FunctionID::eType::ReadDIDID: {
      if ((*message)[strings::params][strings::message_type] == MessageType::kResponse) {
        command.reset(new commands::ReadDIDCommandResponse(message));
      } else {
        command.reset(new commands::ReadDIDCommandRequest(message));
      }
      break;
    }
    case NsSmartDeviceLinkRPC::V2::FunctionID::eType::GetVehicleDataID: {
      if ((*message)[strings::params][strings::message_type] == MessageType::kResponse) {
        command.reset(new commands::GetVehicleDataCommandResponse(message));
      } else {
        command.reset(new commands::GetVehicleDataCommandRequest(message));
      }
      break;
    }
    case NsSmartDeviceLinkRPC::V2::FunctionID::eType::AlertManeuverID: {
      if ((*message)[strings::params][strings::message_type] == MessageType::kResponse) {
        command.reset(new commands::AlertManeuverResponseCommand(message));
      } else {
        command.reset(new commands::AlertManeuverCommand(message));
      }
      break;
    }
    case NsSmartDeviceLinkRPC::V2::FunctionID::eType::SetAppIconID: {
      if ((*message)[strings::params][strings::message_type] == MessageType::kResponse) {
        command.reset(new commands::SetIconResponseCommand(message));
      } else {
        command.reset(new commands::SetIconCommand(message));
      }
      break;
    }
    case NsSmartDeviceLinkRPC::V2::FunctionID::eType::SetDisplayLayoutID: {
      if ((*message)[strings::params][strings::message_type] == MessageType::kResponse) {
        command.reset(new commands::SetDisplayLayoutResponseCommand(message));
      } else {
        command.reset(new commands::SetDisplayLayoutCommand(message));
      }
      break;
    }
    case NsSmartDeviceLinkRPC::V2::FunctionID::eType::UpdateTurnListID: {
      if ((*message)[strings::params][strings::message_type] == MessageType::kResponse) {
        command.reset(new commands::UpdateTurnListResponseCommand(message));
      } else {
        command.reset(new commands::UpdateTurnListCommand(message));
      }
      break;
    }
    case NsSmartDeviceLinkRPC::V2::FunctionID::eType::ChangeRegistrationID: {
      if ((*message)[strings::params][strings::message_type] == MessageType::kResponse) {
        command.reset(new commands::ChangeRegistrationResponseCommand(message));
      } else {
        command.reset(new commands::ChangeRegistrationCommand(message));
      }
      break;
    }
    case NsSmartDeviceLinkRPC::V2::FunctionID::eType::GetDTCsID: {
      if ((*message)[strings::params][strings::message_type] == MessageType::kResponse) {
        command.reset(new commands::GetDTCsCommand(message));
      } else {
        command.reset(new commands::GetDTCsCommand(message));
      }
      break;
    }
    case NsSmartDeviceLinkRPC::V2::FunctionID::eType::OnButtonEventID: {
      command.reset(new commands::OnButtonEventCommand(message));
      break;
    }
    case NsSmartDeviceLinkRPC::V2::FunctionID::eType::OnButtonPressID: {
      command.reset(new commands::OnButtonPressCommand(message));
      break;
    }
    case NsSmartDeviceLinkRPC::V2::FunctionID::eType::OnAudioPassThruID: {
      command.reset(new commands::OnAudioPassThruCommand(message));
      break;
    }
    case NsSmartDeviceLinkRPC::V2::FunctionID::eType::OnVehicleDataID: {
      command.reset(new commands::OnVehicleDataCommand(message));
      break;
    }
    case NsSmartDeviceLinkRPC::V2::FunctionID::eType::OnAppInterfaceUnregisteredID: {
       command.reset(new commands::OnAppInterfaceUnregisteredCommand(message));
       break;
     }
    case NsSmartDeviceLinkRPC::V2::FunctionID::eType::OnCommandID: {
       command.reset(new commands::OnMenuEntryCommand(message));
       break;
     }
    case NsSmartDeviceLinkRPC::V2::FunctionID::eType::OnTBTClientStateID: {
       command.reset(new commands::OnTBTClientStateCommand(message));
       break;
    }
    case NsSmartDeviceLinkRPC::V2::FunctionID::eType::OnDriverDistractionID: {
       command.reset(new commands::OnDriverDistractionCommand(message));
       break;
    }
    case NsSmartDeviceLinkRPC::V2::FunctionID::eType::OnLanguageChangeID: {
       command.reset(new commands::OnLanguageChangeCommand(message));
       break;
    }
    case NsSmartDeviceLinkRPC::V2::FunctionID::eType::OnPermissionsChangeID: {
       command.reset(new commands::OnPermissionsChangeCommand(message));
       break;
    }
    case NsSmartDeviceLinkRPC::V2::FunctionID::eType::OnHMIStatusID: {
       command.reset(new commands::OnHMIStatusCommand(message));
       break;
    }
    default: {
      command.reset(new commands::GenericResponseCommand(message));
      break;
    }
  }

  return command;
}

}  // namespace application_manager
