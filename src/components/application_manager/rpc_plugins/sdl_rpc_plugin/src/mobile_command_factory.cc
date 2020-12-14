/*
 Copyright (c) 2018, Ford Motor Company
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

#include "sdl_rpc_plugin/mobile_command_factory.h"

#include "interfaces/MOBILE_API.h"
#include "sdl_rpc_plugin/commands/mobile/add_command_request.h"
#include "sdl_rpc_plugin/commands/mobile/add_command_response.h"
#include "sdl_rpc_plugin/commands/mobile/add_sub_menu_request.h"
#include "sdl_rpc_plugin/commands/mobile/add_sub_menu_response.h"
#include "sdl_rpc_plugin/commands/mobile/alert_maneuver_request.h"
#include "sdl_rpc_plugin/commands/mobile/alert_maneuver_response.h"
#include "sdl_rpc_plugin/commands/mobile/alert_request.h"
#include "sdl_rpc_plugin/commands/mobile/alert_response.h"
#include "sdl_rpc_plugin/commands/mobile/cancel_interaction_request.h"
#include "sdl_rpc_plugin/commands/mobile/cancel_interaction_response.h"
#include "sdl_rpc_plugin/commands/mobile/change_registration_request.h"
#include "sdl_rpc_plugin/commands/mobile/change_registration_response.h"
#include "sdl_rpc_plugin/commands/mobile/close_application_request.h"
#include "sdl_rpc_plugin/commands/mobile/close_application_response.h"
#include "sdl_rpc_plugin/commands/mobile/create_interaction_choice_set_request.h"
#include "sdl_rpc_plugin/commands/mobile/create_interaction_choice_set_response.h"
#include "sdl_rpc_plugin/commands/mobile/create_window_request.h"
#include "sdl_rpc_plugin/commands/mobile/create_window_response.h"
#include "sdl_rpc_plugin/commands/mobile/delete_command_request.h"
#include "sdl_rpc_plugin/commands/mobile/delete_command_response.h"
#include "sdl_rpc_plugin/commands/mobile/delete_file_request.h"
#include "sdl_rpc_plugin/commands/mobile/delete_file_response.h"
#include "sdl_rpc_plugin/commands/mobile/delete_interaction_choice_set_request.h"
#include "sdl_rpc_plugin/commands/mobile/delete_interaction_choice_set_response.h"
#include "sdl_rpc_plugin/commands/mobile/delete_sub_menu_request.h"
#include "sdl_rpc_plugin/commands/mobile/delete_sub_menu_response.h"
#include "sdl_rpc_plugin/commands/mobile/delete_window_request.h"
#include "sdl_rpc_plugin/commands/mobile/delete_window_response.h"
#include "sdl_rpc_plugin/commands/mobile/dial_number_request.h"
#include "sdl_rpc_plugin/commands/mobile/dial_number_response.h"
#include "sdl_rpc_plugin/commands/mobile/end_audio_pass_thru_request.h"
#include "sdl_rpc_plugin/commands/mobile/end_audio_pass_thru_response.h"
#include "sdl_rpc_plugin/commands/mobile/generic_response.h"
#include "sdl_rpc_plugin/commands/mobile/get_cloud_app_properties_request.h"
#include "sdl_rpc_plugin/commands/mobile/get_cloud_app_properties_response.h"
#include "sdl_rpc_plugin/commands/mobile/get_file_request.h"
#include "sdl_rpc_plugin/commands/mobile/get_file_response.h"
#include "sdl_rpc_plugin/commands/mobile/get_system_capability_request.h"
#include "sdl_rpc_plugin/commands/mobile/get_system_capability_response.h"
#include "sdl_rpc_plugin/commands/mobile/get_way_points_request.h"
#include "sdl_rpc_plugin/commands/mobile/get_way_points_response.h"
#include "sdl_rpc_plugin/commands/mobile/list_files_request.h"
#include "sdl_rpc_plugin/commands/mobile/list_files_response.h"
#include "sdl_rpc_plugin/commands/mobile/on_app_interface_unregistered_notification.h"
#include "sdl_rpc_plugin/commands/mobile/on_audio_pass_thru_notification.h"
#include "sdl_rpc_plugin/commands/mobile/on_button_event_notification.h"
#include "sdl_rpc_plugin/commands/mobile/on_button_press_notification.h"
#include "sdl_rpc_plugin/commands/mobile/on_command_notification.h"
#include "sdl_rpc_plugin/commands/mobile/on_driver_distraction_notification.h"
#include "sdl_rpc_plugin/commands/mobile/on_hash_change_notification.h"
#include "sdl_rpc_plugin/commands/mobile/on_hmi_status_notification.h"
#include "sdl_rpc_plugin/commands/mobile/on_hmi_status_notification_from_mobile.h"
#include "sdl_rpc_plugin/commands/mobile/on_keyboard_input_notification.h"
#include "sdl_rpc_plugin/commands/mobile/on_language_change_notification.h"
#include "sdl_rpc_plugin/commands/mobile/on_permissions_change_notification.h"
#include "sdl_rpc_plugin/commands/mobile/on_subtle_alert_pressed_notification.h"
#include "sdl_rpc_plugin/commands/mobile/on_system_capability_updated_notification.h"
#include "sdl_rpc_plugin/commands/mobile/on_system_request_notification.h"
#include "sdl_rpc_plugin/commands/mobile/on_tbt_client_state_notification.h"
#include "sdl_rpc_plugin/commands/mobile/on_touch_event_notification.h"
#include "sdl_rpc_plugin/commands/mobile/on_update_file_notification.h"
#include "sdl_rpc_plugin/commands/mobile/on_update_sub_menu_notification.h"
#include "sdl_rpc_plugin/commands/mobile/on_way_point_change_notification.h"
#include "sdl_rpc_plugin/commands/mobile/on_way_point_change_notification_from_mobile.h"
#include "sdl_rpc_plugin/commands/mobile/perform_audio_pass_thru_request.h"
#include "sdl_rpc_plugin/commands/mobile/perform_audio_pass_thru_response.h"
#include "sdl_rpc_plugin/commands/mobile/perform_interaction_request.h"
#include "sdl_rpc_plugin/commands/mobile/perform_interaction_response.h"
#include "sdl_rpc_plugin/commands/mobile/put_file_request.h"
#include "sdl_rpc_plugin/commands/mobile/put_file_response.h"
#include "sdl_rpc_plugin/commands/mobile/register_app_interface_request.h"
#include "sdl_rpc_plugin/commands/mobile/register_app_interface_response.h"
#include "sdl_rpc_plugin/commands/mobile/reset_global_properties_request.h"
#include "sdl_rpc_plugin/commands/mobile/reset_global_properties_response.h"
#include "sdl_rpc_plugin/commands/mobile/scrollable_message_request.h"
#include "sdl_rpc_plugin/commands/mobile/scrollable_message_response.h"
#include "sdl_rpc_plugin/commands/mobile/send_haptic_data_request.h"
#include "sdl_rpc_plugin/commands/mobile/send_haptic_data_response.h"
#include "sdl_rpc_plugin/commands/mobile/send_location_request.h"
#include "sdl_rpc_plugin/commands/mobile/send_location_response.h"
#include "sdl_rpc_plugin/commands/mobile/set_app_icon_request.h"
#include "sdl_rpc_plugin/commands/mobile/set_app_icon_response.h"
#include "sdl_rpc_plugin/commands/mobile/set_cloud_app_properties_request.h"
#include "sdl_rpc_plugin/commands/mobile/set_cloud_app_properties_response.h"
#include "sdl_rpc_plugin/commands/mobile/set_display_layout_request.h"
#include "sdl_rpc_plugin/commands/mobile/set_display_layout_response.h"
#include "sdl_rpc_plugin/commands/mobile/set_global_properties_request.h"
#include "sdl_rpc_plugin/commands/mobile/set_global_properties_response.h"
#include "sdl_rpc_plugin/commands/mobile/set_media_clock_timer_request.h"
#include "sdl_rpc_plugin/commands/mobile/set_media_clock_timer_response.h"
#include "sdl_rpc_plugin/commands/mobile/show_app_menu_request.h"
#include "sdl_rpc_plugin/commands/mobile/show_app_menu_response.h"
#include "sdl_rpc_plugin/commands/mobile/show_constant_tbt_request.h"
#include "sdl_rpc_plugin/commands/mobile/show_constant_tbt_response.h"
#include "sdl_rpc_plugin/commands/mobile/show_request.h"
#include "sdl_rpc_plugin/commands/mobile/show_response.h"
#include "sdl_rpc_plugin/commands/mobile/slider_request.h"
#include "sdl_rpc_plugin/commands/mobile/slider_response.h"
#include "sdl_rpc_plugin/commands/mobile/speak_request.h"
#include "sdl_rpc_plugin/commands/mobile/speak_response.h"
#include "sdl_rpc_plugin/commands/mobile/subscribe_button_request.h"
#include "sdl_rpc_plugin/commands/mobile/subscribe_button_response.h"
#include "sdl_rpc_plugin/commands/mobile/subscribe_way_points_request.h"
#include "sdl_rpc_plugin/commands/mobile/subscribe_way_points_response.h"
#include "sdl_rpc_plugin/commands/mobile/subtle_alert_request.h"
#include "sdl_rpc_plugin/commands/mobile/subtle_alert_response.h"
#include "sdl_rpc_plugin/commands/mobile/system_request.h"
#include "sdl_rpc_plugin/commands/mobile/system_response.h"
#include "sdl_rpc_plugin/commands/mobile/unregister_app_interface_request.h"
#include "sdl_rpc_plugin/commands/mobile/unregister_app_interface_response.h"
#include "sdl_rpc_plugin/commands/mobile/unsubscribe_button_request.h"
#include "sdl_rpc_plugin/commands/mobile/unsubscribe_button_response.h"
#include "sdl_rpc_plugin/commands/mobile/unsubscribe_way_points_request.h"
#include "sdl_rpc_plugin/commands/mobile/unsubscribe_way_points_response.h"
#include "sdl_rpc_plugin/commands/mobile/update_turn_list_request.h"
#include "sdl_rpc_plugin/commands/mobile/update_turn_list_response.h"

SDL_CREATE_LOG_VARIABLE("ApplicationManager")
namespace sdl_rpc_plugin {
using namespace application_manager;

CommandCreator& MobileCommandFactory::get_command_creator(
    const mobile_apis::FunctionID::eType id,
    const mobile_apis::messageType::eType message_type) const {
  CommandCreatorFactory factory(
      application_manager_, rpc_service_, hmi_capabilities_, policy_handler_);
  switch (id) {
    case mobile_apis::FunctionID::RegisterAppInterfaceID: {
      return mobile_api::messageType::request == message_type
                 ? factory.GetCreator<commands::RegisterAppInterfaceRequest>()
                 : factory.GetCreator<commands::RegisterAppInterfaceResponse>();
    }
    case mobile_apis::FunctionID::UnregisterAppInterfaceID: {
      return mobile_api::messageType::request == message_type
                 ? factory.GetCreator<commands::UnregisterAppInterfaceRequest>()
                 : factory
                       .GetCreator<commands::UnregisterAppInterfaceResponse>();
    }
    case mobile_apis::FunctionID::SetGlobalPropertiesID: {
      return mobile_api::messageType::request == message_type
                 ? factory.GetCreator<commands::SetGlobalPropertiesRequest>()
                 : factory.GetCreator<commands::SetGlobalPropertiesResponse>();
    }
    case mobile_apis::FunctionID::ResetGlobalPropertiesID: {
      return mobile_api::messageType::request == message_type
                 ? factory.GetCreator<commands::ResetGlobalPropertiesRequest>()
                 : factory
                       .GetCreator<commands::ResetGlobalPropertiesResponse>();
    }
    case mobile_apis::FunctionID::AddCommandID: {
      return mobile_api::messageType::request == message_type
                 ? factory.GetCreator<commands::AddCommandRequest>()
                 : factory.GetCreator<commands::AddCommandResponse>();
    }
    case mobile_apis::FunctionID::CreateWindowID: {
      return mobile_api::messageType::request == message_type
                 ? factory.GetCreator<commands::CreateWindowRequest>()
                 : factory.GetCreator<commands::CreateWindowResponse>();
    }
    case mobile_apis::FunctionID::DeleteCommandID: {
      return mobile_api::messageType::request == message_type
                 ? factory.GetCreator<commands::DeleteCommandRequest>()
                 : factory.GetCreator<commands::DeleteCommandResponse>();
    }
    case mobile_apis::FunctionID::AddSubMenuID: {
      return mobile_api::messageType::request == message_type
                 ? factory.GetCreator<commands::AddSubMenuRequest>()
                 : factory.GetCreator<commands::AddSubMenuResponse>();
    }
    case mobile_apis::FunctionID::DeleteSubMenuID: {
      return mobile_api::messageType::request == message_type
                 ? factory.GetCreator<commands::DeleteSubMenuRequest>()
                 : factory.GetCreator<commands::DeleteSubMenuResponse>();
    }
    case mobile_apis::FunctionID::ShowAppMenuID: {
      return mobile_api::messageType::request == message_type
                 ? factory.GetCreator<commands::ShowAppMenuRequest>()
                 : factory.GetCreator<commands::ShowAppMenuResponse>();
    }
    case mobile_apis::FunctionID::DeleteWindowID: {
      return mobile_api::messageType::request == message_type
                 ? factory.GetCreator<commands::DeleteWindowRequest>()
                 : factory.GetCreator<commands::DeleteWindowResponse>();
    }
    case mobile_apis::FunctionID::DeleteInteractionChoiceSetID: {
      return mobile_api::messageType::request == message_type
                 ? factory.GetCreator<
                       commands::DeleteInteractionChoiceSetRequest>()
                 : factory.GetCreator<
                       commands::DeleteInteractionChoiceSetResponse>();
    }
    case mobile_apis::FunctionID::AlertID: {
      return mobile_api::messageType::request == message_type
                 ? factory.GetCreator<commands::AlertRequest>()
                 : factory.GetCreator<commands::AlertResponse>();
    }
    case mobile_apis::FunctionID::SubtleAlertID: {
      return mobile_api::messageType::request == message_type
                 ? factory.GetCreator<commands::SubtleAlertRequest>()
                 : factory.GetCreator<commands::SubtleAlertResponse>();
    }
    case mobile_apis::FunctionID::SpeakID: {
      return mobile_api::messageType::request == message_type
                 ? factory.GetCreator<commands::SpeakRequest>()
                 : factory.GetCreator<commands::SpeakResponse>();
    }
    case mobile_apis::FunctionID::SliderID: {
      return mobile_api::messageType::request == message_type
                 ? factory.GetCreator<commands::SliderRequest>()
                 : factory.GetCreator<commands::SliderResponse>();
    }
    case mobile_apis::FunctionID::PerformAudioPassThruID: {
      return mobile_api::messageType::request == message_type
                 ? factory.GetCreator<commands::PerformAudioPassThruRequest>()
                 : factory.GetCreator<commands::PerformAudioPassThruResponse>();
    }
    case mobile_apis::FunctionID::CancelInteractionID: {
      return mobile_api::messageType::request == message_type
                 ? factory.GetCreator<commands::CancelInteractionRequest>()
                 : factory.GetCreator<commands::CancelInteractionResponse>();
    }
    case mobile_apis::FunctionID::CreateInteractionChoiceSetID: {
      return mobile_api::messageType::request == message_type
                 ? factory.GetCreator<
                       commands::CreateInteractionChoiceSetRequest>()
                 : factory.GetCreator<
                       commands::CreateInteractionChoiceSetResponse>();
    }
    case mobile_apis::FunctionID::PerformInteractionID: {
      return mobile_api::messageType::request == message_type
                 ? factory.GetCreator<commands::PerformInteractionRequest>()
                 : factory.GetCreator<commands::PerformInteractionResponse>();
    }
    case mobile_apis::FunctionID::EndAudioPassThruID: {
      return mobile_api::messageType::request == message_type
                 ? factory.GetCreator<commands::EndAudioPassThruRequest>()
                 : factory.GetCreator<commands::EndAudioPassThruResponse>();
    }
    case mobile_apis::FunctionID::PutFileID: {
      return mobile_api::messageType::request == message_type
                 ? factory.GetCreator<commands::PutFileRequest>()
                 : factory.GetCreator<commands::PutFileResponse>();
    }
    case mobile_apis::FunctionID::GetFileID: {
      return mobile_api::messageType::request == message_type
                 ? factory.GetCreator<commands::GetFileRequest>()
                 : factory.GetCreator<commands::GetFileResponse>();
    }
    case mobile_apis::FunctionID::DeleteFileID: {
      return mobile_api::messageType::request == message_type
                 ? factory.GetCreator<commands::DeleteFileRequest>()
                 : factory.GetCreator<commands::DeleteFileResponse>();
    }
    case mobile_apis::FunctionID::ListFilesID: {
      return mobile_api::messageType::request == message_type
                 ? factory.GetCreator<commands::ListFilesRequest>()
                 : factory.GetCreator<commands::ListFilesResponse>();
    }
    case mobile_apis::FunctionID::SubscribeButtonID: {
      return mobile_api::messageType::request == message_type
                 ? factory.GetCreator<commands::SubscribeButtonRequest>()
                 : factory.GetCreator<commands::SubscribeButtonResponse>();
    }
    case mobile_apis::FunctionID::UnsubscribeButtonID: {
      return mobile_api::messageType::request == message_type
                 ? factory.GetCreator<commands::UnsubscribeButtonRequest>()
                 : factory.GetCreator<commands::UnsubscribeButtonResponse>();
    }
    case mobile_apis::FunctionID::ShowConstantTBTID: {
      return mobile_api::messageType::request == message_type
                 ? factory.GetCreator<commands::ShowConstantTBTRequest>()
                 : factory.GetCreator<commands::ShowConstantTBTResponse>();
    }
    case mobile_apis::FunctionID::ShowID: {
      return mobile_api::messageType::request == message_type
                 ? factory.GetCreator<commands::ShowRequest>()
                 : factory.GetCreator<commands::ShowResponse>();
    }
    case mobile_apis::FunctionID::GetWayPointsID: {
      return mobile_api::messageType::request == message_type
                 ? factory.GetCreator<commands::GetWayPointsRequest>()
                 : factory.GetCreator<commands::GetWayPointsResponse>();
    }
    case mobile_apis::FunctionID::SubscribeWayPointsID: {
      return mobile_api::messageType::request == message_type
                 ? factory.GetCreator<commands::SubscribeWayPointsRequest>()
                 : factory.GetCreator<commands::SubscribeWayPointsResponse>();
    }
    case mobile_apis::FunctionID::UnsubscribeWayPointsID: {
      return mobile_api::messageType::request == message_type
                 ? factory.GetCreator<commands::UnsubscribeWayPointsRequest>()
                 : factory.GetCreator<commands::UnsubscribeWayPointsResponse>();
    }
    case mobile_apis::FunctionID::GetSystemCapabilityID: {
      return mobile_api::messageType::request == message_type
                 ? factory.GetCreator<commands::GetSystemCapabilityRequest>()
                 : factory.GetCreator<commands::GetSystemCapabilityResponse>();
    }
    case mobile_apis::FunctionID::ScrollableMessageID: {
      return mobile_api::messageType::request == message_type
                 ? factory.GetCreator<commands::ScrollableMessageRequest>()
                 : factory.GetCreator<commands::ScrollableMessageResponse>();
    }
    case mobile_apis::FunctionID::AlertManeuverID: {
      return mobile_api::messageType::request == message_type
                 ? factory.GetCreator<commands::AlertManeuverRequest>()
                 : factory.GetCreator<commands::AlertManeuverResponse>();
    }
    case mobile_apis::FunctionID::SetAppIconID: {
      return mobile_api::messageType::request == message_type
                 ? factory.GetCreator<commands::SetAppIconRequest>()
                 : factory.GetCreator<commands::SetAppIconResponse>();
    }
    case mobile_apis::FunctionID::SetDisplayLayoutID: {
      return mobile_api::messageType::request == message_type
                 ? factory.GetCreator<commands::SetDisplayLayoutRequest>()
                 : factory.GetCreator<commands::SetDisplayLayoutResponse>();
    }
    case mobile_apis::FunctionID::UpdateTurnListID: {
      return mobile_api::messageType::request == message_type
                 ? factory.GetCreator<commands::UpdateTurnListRequest>()
                 : factory.GetCreator<commands::UpdateTurnListResponse>();
    }
    case mobile_apis::FunctionID::ChangeRegistrationID: {
      return mobile_api::messageType::request == message_type
                 ? factory.GetCreator<commands::ChangeRegistrationRequest>()
                 : factory.GetCreator<commands::ChangeRegistrationResponse>();
    }
    case mobile_apis::FunctionID::SetMediaClockTimerID: {
      return mobile_api::messageType::request == message_type
                 ? factory.GetCreator<commands::SetMediaClockRequest>()
                 : factory.GetCreator<commands::SetMediaClockTimerResponse>();
    }
    case mobile_apis::FunctionID::SystemRequestID: {
      return mobile_api::messageType::request == message_type
                 ? factory.GetCreator<commands::SystemRequest>()
                 : factory.GetCreator<commands::SystemResponse>();
    }
    case mobile_apis::FunctionID::SendLocationID: {
      return mobile_api::messageType::request == message_type
                 ? factory.GetCreator<commands::SendLocationRequest>()
                 : factory.GetCreator<commands::SendLocationResponse>();
    }
    case mobile_apis::FunctionID::DialNumberID: {
      return mobile_api::messageType::request == message_type
                 ? factory.GetCreator<commands::DialNumberRequest>()
                 : factory.GetCreator<commands::DialNumberResponse>();
    }
    case mobile_apis::FunctionID::SendHapticDataID: {
      return mobile_api::messageType::request == message_type
                 ? factory.GetCreator<commands::SendHapticDataRequest>()
                 : factory.GetCreator<commands::SendHapticDataResponse>();
    }
    case mobile_apis::FunctionID::SetCloudAppPropertiesID: {
      return mobile_api::messageType::request == message_type
                 ? factory.GetCreator<commands::SetCloudAppPropertiesRequest>()
                 : factory
                       .GetCreator<commands::SetCloudAppPropertiesResponse>();
    }
    case mobile_apis::FunctionID::GetCloudAppPropertiesID: {
      return mobile_api::messageType::request == message_type
                 ? factory.GetCreator<commands::GetCloudAppPropertiesRequest>()
                 : factory
                       .GetCreator<commands::GetCloudAppPropertiesResponse>();
    }
    case mobile_apis::FunctionID::CloseApplicationID: {
      return mobile_api::messageType::request == message_type
                 ? factory.GetCreator<commands::CloseApplicationRequest>()
                 : factory.GetCreator<commands::CloseApplicationResponse>();
    }
    case mobile_apis::FunctionID::GenericResponseID: {
      using app_mngr::commands::Command;
      return factory.GetCreator<commands::GenericResponse>();
    }
    default: {}
  }
  return factory.GetCreator<InvalidCommand>();
}

CommandCreator& MobileCommandFactory::get_notification_creator(
    const mobile_apis::FunctionID::eType id) const {
  CommandCreatorFactory factory(
      application_manager_, rpc_service_, hmi_capabilities_, policy_handler_);
  switch (id) {
    case mobile_apis::FunctionID::OnButtonEventID: {
      return factory.GetCreator<commands::mobile::OnButtonEventNotification>();
    }
    case mobile_apis::FunctionID::OnButtonPressID: {
      return factory.GetCreator<commands::mobile::OnButtonPressNotification>();
    }
    case mobile_apis::FunctionID::OnAudioPassThruID: {
      return factory.GetCreator<commands::OnAudioPassThruNotification>();
    }
    case mobile_apis::FunctionID::OnAppInterfaceUnregisteredID: {
      return factory
          .GetCreator<commands::OnAppInterfaceUnregisteredNotification>();
    }
    case mobile_apis::FunctionID::OnCommandID: {
      return factory.GetCreator<commands::OnCommandNotification>();
    }
    case mobile_apis::FunctionID::OnTBTClientStateID: {
      return factory.GetCreator<commands::OnTBTClientStateNotification>();
    }
    case mobile_apis::FunctionID::OnDriverDistractionID: {
      return factory
          .GetCreator<commands::mobile::OnDriverDistractionNotification>();
    }
    case mobile_apis::FunctionID::OnLanguageChangeID: {
      return factory.GetCreator<commands::OnLanguageChangeNotification>();
    }
    case mobile_apis::FunctionID::OnPermissionsChangeID: {
      return factory.GetCreator<commands::OnPermissionsChangeNotification>();
    }
    case mobile_apis::FunctionID::OnHMIStatusID: {
      return factory.GetCreator<commands::OnHMIStatusNotification>();
    }
    case mobile_apis::FunctionID::OnKeyboardInputID: {
      return factory
          .GetCreator<commands::mobile::OnKeyBoardInputNotification>();
    }
    case mobile_apis::FunctionID::OnTouchEventID: {
      return factory.GetCreator<commands::mobile::OnTouchEventNotification>();
    }
    case mobile_apis::FunctionID::OnSystemCapabilityUpdatedID: {
      return factory.GetCreator<
          commands::mobile::OnSystemCapabilityUpdatedNotification>();
    }
    case mobile_apis::FunctionID::OnSystemRequestID: {
      return factory
          .GetCreator<commands::mobile::OnSystemRequestNotification>();
    }
    case mobile_apis::FunctionID::OnHashChangeID: {
      return factory.GetCreator<commands::mobile::OnHashChangeNotification>();
    }
    case mobile_apis::FunctionID::OnWayPointChangeID: {
      return factory.GetCreator<commands::OnWayPointChangeNotification>();
    }
    case mobile_apis::FunctionID::OnUpdateFileID: {
      return factory.GetCreator<commands::OnUpdateFileNotification>();
    }
    case mobile_apis::FunctionID::OnUpdateSubMenuID: {
      return factory.GetCreator<commands::OnUpdateSubMenuNotification>();
    }
    case mobile_apis::FunctionID::OnSubtleAlertPressedID: {
      return factory.GetCreator<commands::OnSubtleAlertPressedNotification>();
    }
    default: {}
  }
  return factory.GetCreator<InvalidCommand>();
}

CommandCreator& MobileCommandFactory::get_notification_from_mobile_creator(
    const mobile_apis::FunctionID::eType id) const {
  CommandCreatorFactory factory(
      application_manager_, rpc_service_, hmi_capabilities_, policy_handler_);
  switch (id) {
    case mobile_apis::FunctionID::OnHMIStatusID: {
      return factory.GetCreator<commands::OnHMIStatusNotificationFromMobile>();
    }
    case mobile_apis::FunctionID::OnWayPointChangeID: {
      return factory
          .GetCreator<commands::OnWayPointChangeNotificationFromMobile>();
    }
    default: {}
  }
  return factory.GetCreator<InvalidCommand>();
}

CommandCreator& MobileCommandFactory::get_creator_factory(
    const mobile_apis::FunctionID::eType id,
    const mobile_apis::messageType::eType message_type,
    const app_mngr::commands::Command::CommandSource source) const {
  switch (message_type) {
    case mobile_api::messageType::request: {
      if (app_mngr::commands::Command::CommandSource::SOURCE_MOBILE == source) {
        return get_command_creator(id, message_type);
      }
      break;
    }
    case mobile_api::messageType::response: {
      if (app_mngr::commands::Command::CommandSource::SOURCE_SDL == source) {
        return get_command_creator(id, message_type);
      }
      break;
    }
    case mobile_api::messageType::notification: {
      if (app_mngr::commands::Command::CommandSource::SOURCE_SDL == source) {
        return get_notification_creator(id);
      } else if (app_mngr::commands::Command::CommandSource::SOURCE_MOBILE ==
                 source) {
        return get_notification_from_mobile_creator(id);
      }
      break;
    }
    default: {}
  }
  CommandCreatorFactory factory(
      application_manager_, rpc_service_, hmi_capabilities_, policy_handler_);
  return factory.GetCreator<InvalidCommand>();
}

MobileCommandFactory::MobileCommandFactory(
    ApplicationManager& application_manager,
    rpc_service::RPCService& rpc_service,
    HMICapabilities& hmi_capabilities,
    policy::PolicyHandlerInterface& policy_handler)
    : application_manager_(application_manager)
    , rpc_service_(rpc_service)
    , hmi_capabilities_(hmi_capabilities)
    , policy_handler_(policy_handler) {}

bool MobileCommandFactory::IsAbleToProcess(
    const int32_t function_id,
    const application_manager::commands::Command::CommandSource message_source)
    const {
  auto id = static_cast<mobile_apis::FunctionID::eType>(function_id);
  return get_command_creator(id, mobile_apis::messageType::INVALID_ENUM)
             .CanBeCreated() ||
         get_notification_creator(id).CanBeCreated();
}

CommandSharedPtr MobileCommandFactory::CreateCommand(
    const app_mngr::commands::MessageSharedPtr& message,
    app_mngr::commands::Command::CommandSource source) {
  mobile_apis::messageType::eType message_type =
      static_cast<mobile_apis::messageType::eType>(
          (*message)[strings::params][strings::message_type].asInt());

  mobile_apis::FunctionID::eType function_id =
      static_cast<mobile_apis::FunctionID::eType>(
          (*message)[strings::params][strings::function_id].asInt());

  SDL_LOG_DEBUG(
      "MobileCommandFactory::CreateCommand function_id: " << function_id);

  return get_creator_factory(function_id, message_type, source).create(message);
}

}  // namespace sdl_rpc_plugin
