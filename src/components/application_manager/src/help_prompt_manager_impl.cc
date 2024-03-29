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

#include "application_manager/help_prompt_manager_impl.h"

#include <algorithm>
#include <iterator>

#include "application_manager/application.h"
#include "application_manager/application_manager.h"
#include "application_manager/commands/command_impl.h"
#include "application_manager/message_helper.h"
#include "application_manager/smart_object_keys.h"
#include "smart_objects/smart_object.h"
#include "utils/logger.h"

SDL_CREATE_LOG_VARIABLE("HelpPromptManagerImpl")

namespace {
const size_t kLimitCommand = 30;
}

namespace application_manager {

HelpPromptManagerImpl::HelpPromptManagerImpl(Application& app,
                                             ApplicationManager& app_manager)
    : app_(app)
    , app_manager_(app_manager)
    , sending_type_(SendingType::kSendBoth)
    , is_tts_send_(false)
    , is_ui_send_(false) {}

HelpPromptManagerImpl::~HelpPromptManagerImpl() {
  SDL_LOG_AUTO_TRACE();
}

bool HelpPromptManagerImpl::AddCommand(
    const uint32_t cmd_id, const smart_objects::SmartObject& command) {
  if (!command.keyExists(strings::vr_commands)) {
    SDL_LOG_DEBUG("vr_commands does`t present");
    return false;
  }

  const smart_objects::SmartObject& commands = command[strings::vr_commands];
  if (commands.empty()) {
    SDL_LOG_DEBUG("vr_commands array is empty");
    return false;
  }

  sync_primitives::AutoLock lock(vr_commands_lock_);
  auto it = std::find_if(
      vr_commands_.begin(),
      vr_commands_.end(),
      [cmd_id](const VRCommandPair& pair) { return pair.first == cmd_id; });

  if (vr_commands_.end() != it) {
    SDL_LOG_DEBUG("Command with id:" << cmd_id << " already exists");
    return false;
  }

  const bool limit_exceeded =
      kLimitCommand <= GetCommandsCount(vr_commands_.end());

  SDL_LOG_DEBUG("Will be added first command from array "
                << strings::vr_commands);

  smart_objects::SmartObjectSPtr vr_item =
      std::make_shared<smart_objects::SmartObject>(commands.asArray()->front());
  vr_commands_.push_back(std::make_pair(cmd_id, vr_item));

  SDL_LOG_DEBUG("VR command with id: " << cmd_id << " added for appID: "
                                       << app_.app_id() << ". Total "
                                       << vr_commands_.size() << " in cache");

  return !limit_exceeded;
}

bool HelpPromptManagerImpl::DeleteCommand(const uint32_t cmd_id) {
  SDL_LOG_AUTO_TRACE();

  sync_primitives::AutoLock lock(vr_commands_lock_);

  auto it = std::find_if(
      vr_commands_.begin(),
      vr_commands_.end(),
      [cmd_id](const VRCommandPair& pair) { return pair.first == cmd_id; });

  if (vr_commands_.end() == it) {
    SDL_LOG_WARN("VR command with id: " << cmd_id << " not found");
    return false;
  }

  const size_t commands_before_current = GetCommandsCount(it);
  vr_commands_.erase(it);
  SDL_LOG_DEBUG("VR command with id: "
                << cmd_id << " found after " << commands_before_current
                << " commands was deleted for appID: " << app_.app_id()
                << " Cache size after deleting: " << vr_commands_.size());

  return commands_before_current < kLimitCommand;
}

void HelpPromptManagerImpl::OnVrCommandAdded(
    const uint32_t cmd_id,
    const smart_objects::SmartObject& command,
    const bool is_resumption) {
  SDL_LOG_AUTO_TRACE();
  if (SendingType::kNoneSend == sending_type_) {
    SDL_LOG_DEBUG("SendingType::kNoneSend"
                  << " commands with id:" << cmd_id << " will not be added");
    return;
  }
  if (AddCommand(cmd_id, command) && !is_resumption) {
    SendRequests();
  }
}

void HelpPromptManagerImpl::OnVrCommandDeleted(const uint32_t cmd_id,
                                               const bool is_resumption) {
  SDL_LOG_AUTO_TRACE();
  if (SendingType::kNoneSend == sending_type_) {
    SDL_LOG_DEBUG("SendingType::kNoneSend"
                  << " commands with id:" << cmd_id << " will not be deleted");
    return;
  }
  if (DeleteCommand(cmd_id) && !is_resumption) {
    SendRequests();
  }
}

void HelpPromptManagerImpl::OnSetGlobalPropertiesReceived(
    const smart_objects::SmartObject& msg, const bool is_response) {
  SDL_LOG_AUTO_TRACE();
  if (SendingType::kNoneSend == sending_type_) {
    SDL_LOG_DEBUG(
        "SendingType::kNoneSend"
        " do not track SetGlobalProperties");
    return;
  }

  if (!is_response) {
    if (msg.keyExists(strings::help_prompt)) {
      is_tts_send_ = true;
    }
    if (msg.keyExists(strings::vr_help)) {
      is_ui_send_ = true;
    }

    SDL_LOG_DEBUG("is_tts_send_:" << is_tts_send_);
    SDL_LOG_DEBUG("is_ui_send_:" << is_ui_send_);
    return;
  }

  SetSendingType(msg);
}

void HelpPromptManagerImpl::OnResetGlobalPropertiesReceived(
    const smart_objects::SmartObject& msg) {
  SDL_LOG_AUTO_TRACE();
  auto& global_properties_ids = msg[strings::properties];

  for (size_t i = 0; i < global_properties_ids.length(); ++i) {
    mobile_apis::GlobalProperty::eType global_property =
        static_cast<mobile_apis::GlobalProperty::eType>(
            global_properties_ids[i].asInt());
    switch (global_property) {
      case mobile_apis::GlobalProperty::HELPPROMPT: {
        sending_type_ = (SendingType::kNoneSend == sending_type_ ||
                         SendingType::kSendHelpPrompt == sending_type_)
                            ? SendingType::kSendHelpPrompt
                            : SendingType::kSendBoth;
        break;
      }
      case mobile_apis::GlobalProperty::VRHELPTITLE:
      case mobile_apis::GlobalProperty::VRHELPITEMS: {
        sending_type_ = (SendingType::kNoneSend == sending_type_ ||
                         SendingType::kSendVRHelp == sending_type_)
                            ? SendingType::kSendVRHelp
                            : SendingType::kSendBoth;
        break;
      }
      default: {
        return;
      }
    }
  }

  SDL_LOG_DEBUG("Sending type set to:" << static_cast<uint32_t>(sending_type_));
}

HelpPromptManagerImpl::SendingType HelpPromptManagerImpl::GetSendingType()
    const {
  return sending_type_;
}

size_t HelpPromptManagerImpl::GetCommandsCount(
    VRCommandPairs::const_iterator end_element) const {
  return std::distance(vr_commands_.begin(), end_element);
}

void HelpPromptManagerImpl::GenerateVrItems(
    smart_objects::SmartObject& out_msg_params, const char* vr_key) const {
  out_msg_params[vr_key] =
      smart_objects::SmartObject(smart_objects::SmartType_Array);

  const size_t count_to_add = std::min(vr_commands_.size(), kLimitCommand);
  for (size_t i = 0; i < count_to_add; ++i) {
    const VRCommandPair& pair = vr_commands_[i];

    smart_objects::SmartObject item(smart_objects::SmartType_Map);
    item[strings::text] = pair.second->asString();
    if (strings::help_prompt == vr_key) {
      item[strings::type] = mobile_apis::SpeechCapabilities::SC_TEXT;
    } else {
      item[strings::position] = i + 1;
    }

    out_msg_params[vr_key][i] = item;
  }
}

void HelpPromptManagerImpl::SendTTSRequest() {
  SDL_LOG_AUTO_TRACE();
  SDL_LOG_DEBUG("TTS request for appID:" << app_.app_id());
  smart_objects::SmartObjectSPtr tts_global_properties =
      std::make_shared<smart_objects::SmartObject>(
          smart_objects::SmartType_Map);
  if (tts_global_properties) {
    smart_objects::SmartObject& ref = *tts_global_properties;

    ref[strings::params][strings::message_type] =
        hmi_apis::messageType::request;
    ref[strings::params][strings::protocol_version] =
        commands::CommandImpl::protocol_version_;
    ref[strings::params][strings::protocol_type] =
        commands::CommandImpl::hmi_protocol_type_;
    ref[strings::params][strings::correlation_id] =
        app_manager_.GetNextHMICorrelationID();

    smart_objects::SmartObject& so_to_send = *tts_global_properties;
    so_to_send[strings::params][strings::function_id] =
        hmi_apis::FunctionID::TTS_SetGlobalProperties;

    smart_objects::SmartObject msg_params =
        smart_objects::SmartObject(smart_objects::SmartType_Map);

    CreatePromptMsg(msg_params);

    msg_params[strings::app_id] = app_.app_id();
    so_to_send[strings::msg_params] = msg_params;
    app_manager_.GetRPCService().ManageHMICommand(tts_global_properties);
  }
  is_tts_send_ = false;
}

void HelpPromptManagerImpl::SendUIRequest() {
  SDL_LOG_AUTO_TRACE();
  SDL_LOG_DEBUG("UI request for appID:" << app_.app_id());
  smart_objects::SmartObjectSPtr ui_global_properties =
      std::make_shared<smart_objects::SmartObject>(
          smart_objects::SmartType_Map);
  if (ui_global_properties) {
    smart_objects::SmartObject& ref = *ui_global_properties;

    ref[strings::params][strings::message_type] =
        hmi_apis::messageType::request;
    ref[strings::params][strings::protocol_version] =
        commands::CommandImpl::protocol_version_;
    ref[strings::params][strings::protocol_type] =
        commands::CommandImpl::hmi_protocol_type_;
    ref[strings::params][strings::correlation_id] =
        app_manager_.GetNextHMICorrelationID();

    smart_objects::SmartObject& so_to_send = *ui_global_properties;
    so_to_send[strings::params][strings::function_id] =
        hmi_apis::FunctionID::UI_SetGlobalProperties;

    smart_objects::SmartObject msg_params =
        smart_objects::SmartObject(smart_objects::SmartType_Map);

    CreateVRMsg(msg_params);

    msg_params[strings::app_id] = app_.app_id();
    so_to_send[strings::msg_params] = msg_params;
    app_manager_.GetRPCService().ManageHMICommand(ui_global_properties);
  }
  is_ui_send_ = false;
}

void HelpPromptManagerImpl::SendBothRequests() {
  SDL_LOG_AUTO_TRACE();
  SendTTSRequest();
  SendUIRequest();
}

void HelpPromptManagerImpl::SendRequests() {
  SDL_LOG_AUTO_TRACE();

  sync_primitives::AutoLock lock(vr_commands_lock_);
  switch (sending_type_) {
    case SendingType::kSendHelpPrompt:
      SendTTSRequest();
      return;
    case SendingType::kSendVRHelp:
      SendUIRequest();
      return;
    case SendingType::kSendBoth:
      SendBothRequests();
      return;
    case SendingType::kNoneSend:
      break;
  }
  SDL_LOG_DEBUG("SendingType:" << static_cast<uint32_t>(sending_type_)
                               << " request not sending");
}

void HelpPromptManagerImpl::CreatePromptMsg(
    smart_objects::SmartObject& out_msg_params) {
  SDL_LOG_AUTO_TRACE();
  GenerateVrItems(out_msg_params, strings::help_prompt);
  app_.set_help_prompt(out_msg_params[strings::help_prompt]);
}

void HelpPromptManagerImpl::CreateVRMsg(
    smart_objects::SmartObject& out_msg_params) {
  SDL_LOG_AUTO_TRACE();
  if (false == out_msg_params.keyExists(strings::vr_help_title)) {
    if (app_.vr_help_title()) {
      out_msg_params[strings::vr_help_title] = (*app_.vr_help_title());
    } else {
      out_msg_params[strings::vr_help_title] = app_.name();
    }
  }

  GenerateVrItems(out_msg_params, strings::vr_help);

  if (out_msg_params[strings::vr_help].empty()) {
    int32_t index = 0;

    smart_objects::SmartObject so_default_vr_help(smart_objects::SmartType_Map);
    so_default_vr_help[strings::position] = index + 1;
    so_default_vr_help[strings::text] = app_.name();
    out_msg_params[strings::vr_help][index++] = so_default_vr_help;

    if (app_.vr_synonyms()) {
      smart_objects::SmartObject item(smart_objects::SmartType_Map);
      item[strings::text] = (*(app_.vr_synonyms())).getElement(0);
      item[strings::position] = index + 1;
      out_msg_params[strings::vr_help][index++] = item;
    }
  }
  app_.set_vr_help(out_msg_params[strings::vr_help]);
}  // namespace application_manager

void HelpPromptManagerImpl::SetSendingType(
    const smart_objects::SmartObject& msg) {
  SDL_LOG_AUTO_TRACE();

  hmi_apis::Common_Result::eType result =
      static_cast<hmi_apis::Common_Result::eType>(
          msg[strings::params][hmi_response::code].asInt());
  SDL_LOG_DEBUG("HMI response result:" << result);
  if (hmi_apis::Common_Result::eType::SUCCESS == result) {
    hmi_apis::FunctionID::eType function_id =
        static_cast<hmi_apis::FunctionID::eType>(
            msg[strings::params][strings::function_id].asUInt());
    SDL_LOG_DEBUG("Function id:" << function_id);
    switch (function_id) {
      case hmi_apis::FunctionID::TTS_SetGlobalProperties: {
        if (is_tts_send_) {
          is_tts_send_ = false;
          sending_type_ = SendingType::kSendHelpPrompt == sending_type_
                              ? SendingType::kNoneSend
                              : SendingType::kSendVRHelp;
        }
        break;
      }
      case hmi_apis::FunctionID::UI_SetGlobalProperties: {
        if (is_ui_send_) {
          is_ui_send_ = false;
          sending_type_ = SendingType::kSendVRHelp == sending_type_
                              ? SendingType::kNoneSend
                              : SendingType::kSendHelpPrompt;
        }
        break;
      }
      default: {
        break;
      }
    }
    SDL_LOG_DEBUG(
        "Sending type set to:" << static_cast<uint32_t>(sending_type_));
  }
}

}  //  namespace application_manager
