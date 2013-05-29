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

#include "application_manager/basic_command_factory.h"
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
#include "application_manager/commands/create_interaction_choice_set_command.h"
#include "application_manager/commands/create_interaction_choice_set_command_response.h"
#include "application_manager/commands/perform_interaction_command.h"
#include "application_manager/commands/perform_interaction_response_command.h"

// TODO(AK): Include the directory when naming .h files
#include "v4_protocol_v2_0_revT.h"

namespace application_manager {

CommandSharedPtr BasicCommandFactory::CreateCommand(
    const MessageSharedPtr& message) {
  CommandSharedPtr command(NULL);

  switch (static_cast<int>((*message)[strings::params][strings::function_id])) {
    case NsSmartDeviceLinkRPC::V2::FunctionID::eType::RegisterAppInterfaceID: {
      if ((*message)[strings::params][strings::message_type] ==
          MessageType::kRequest)
        command.reset(new commands::RegisterAppInterfaceCommand(message));
      else
        command.reset(
            new commands::RegisterAppInterfaceResponseCommand(message));
      break;
    }
    case NsSmartDeviceLinkRPC::V2::FunctionID::eType::UnregisterAppInterfaceID: {
      if ((*message)[strings::params][strings::message_type] ==
          MessageType::kRequest)
        command.reset(new commands::UnregisterAppInterfaceCommand(message));
      else
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
    default: {
      command.reset(new commands::GenericResponseCommand(message));
      break;
    }
  }

  return command;
}

}  // namespace application_manager
