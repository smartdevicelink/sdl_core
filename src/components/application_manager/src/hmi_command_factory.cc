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

#include "application_manager/hmi_command_factory.h"
#include "application_manager/commands/mobile/add_command_request.h"
#include "application_manager/commands/mobile/add_command_response.h"
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
#include "application_manager/commands/mobile/delete_command_request.h"
#include "application_manager/commands/mobile/delete_command_response.h"
#include "application_manager/commands/mobile/delete_file_request.h"
#include "application_manager/commands/mobile/delete_file_response.h"
#include "application_manager/commands/mobile/delete_interaction_choice_set_request.h"
#include "application_manager/commands/mobile/delete_interaction_choice_set_response.h"
#include "application_manager/commands/mobile/delete_sub_menu_request.h"
#include "application_manager/commands/mobile/delete_sub_menu_response.h"
#include "application_manager/commands/mobile/dial_number_request.h"
#include "application_manager/commands/mobile/encoded_sync_pdata_request.h"
#include "application_manager/commands/mobile/encoded_sync_pdata_response.h"
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
#include "application_manager/commands/mobile/on_encoded_sync_pdata_notification.h"
#include "application_manager/commands/mobile/on_hmi_status_notification.h"
#include "application_manager/commands/mobile/on_language_change_notification.h"
#include "application_manager/commands/mobile/on_menu_entry_notification.h"
#include "application_manager/commands/mobile/on_permissions_change_notification.h"
#include "application_manager/commands/mobile/on_tbt_client_state_notification.h"
#include "application_manager/commands/mobile/on_vehicle_data_notification.h"
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
#include "application_manager/commands/mobile/set_icon_request.h"
#include "application_manager/commands/mobile/set_icon_response.h"
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



#include "interfaces/v4_protocol_v2_0_revT.h"

namespace application_manager {

CommandSharedPtr HMICommandFactory::CreateCommand(
  const MessageSharedPtr& message) {
  CommandSharedPtr command(NULL);

  return command;
}

}  // namespace application_manager
