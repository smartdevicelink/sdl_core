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

CommandSharedPtr HMICommandFactory::CreateCommand(
  const MessageSharedPtr& message) {
  CommandSharedPtr command(NULL);

  return command;
}

}  // namespace application_manager
