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
#include "application_manager/application.h"
#include "application_manager/application_manager.h"
#include "application_manager/commands/command_impl.h"
#include "application_manager/message_helper.h"
#include "application_manager/smart_object_keys.h"
#include "smart_objects/smart_object.h"
#include "utils/logger.h"
#include "utils/make_shared.h"

CREATE_LOGGERPTR_GLOBAL(logger_, "HelpPromptManagerImpl")

namespace {
const std::size_t kLimitCommand = 30;
}

namespace application_manager {

HelpPromptManagerImpl::HelpPromptManagerImpl(Application& app,
                                             ApplicationManager& app_manager)
    : app_(app)
    , app_manager_(app_manager)
    , sending_type_(SendingType::kSendBoth)
    , count_vr_commands_(0)
    , is_tts_send_(false)
    , is_ui_send_(false) {}

HelpPromptManagerImpl::~HelpPromptManagerImpl() {
  LOG4CXX_AUTO_TRACE(logger_);
}

bool HelpPromptManagerImpl::AddCommand(
    const uint32_t cmd_id, const smart_objects::SmartObject& command) {
  sync_primitives::AutoLock lock(vr_commands_lock_);
  auto it = vr_commands_.find(cmd_id);
  if (vr_commands_.end() != it) {
    LOG4CXX_DEBUG(logger_, "Commands with id:" << cmd_id << " already exists");
    return false;
  }

  if (false == command.keyExists(strings::vr_commands)) {
    LOG4CXX_DEBUG(logger_, "vr_commands does`t present");
    return false;
  }
  const smart_objects::SmartObject& commands = command[strings::vr_commands];
  const std::size_t count_new_commands = commands.length();

  LOG4CXX_DEBUG(logger_, "Commands to add: " << count_new_commands);
  if (count_vr_commands_ >= kLimitCommand) {
    LOG4CXX_DEBUG(logger_, "Commands limit is exceeded");
    return false;
  }

  const std::size_t available_count = kLimitCommand - count_vr_commands_;
  const std::size_t count_to_add = available_count > count_new_commands
                                       ? count_new_commands
                                       : available_count;
  LOG4CXX_DEBUG(logger_, "Will be added " << count_to_add << " commands");

  vr_commands_[cmd_id] = utils::MakeShared<smart_objects::SmartObject>(
      smart_objects::SmartType_Array);
  smart_objects::SmartArray& ar_vr_cmd = *(vr_commands_[cmd_id]->asArray());
  smart_objects::SmartArray& ar_cmd = *(commands.asArray());
  ar_vr_cmd.reserve(count_vr_commands_ + count_to_add);
  ar_vr_cmd.insert(
      ar_vr_cmd.end(), ar_cmd.begin(), ar_cmd.begin() + count_to_add);

  LOG4CXX_DEBUG(logger_,
                "VR commands with id: " << cmd_id << " added for appID: "
                                        << app_.app_id());
  count_vr_commands_ += count_to_add;
  return true;
}

bool HelpPromptManagerImpl::DeleteCommand(const uint32_t cmd_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock lock(vr_commands_lock_);

  auto it = vr_commands_.find(cmd_id);
  if (vr_commands_.end() == it) {
    LOG4CXX_WARN(logger_, "VR command with id: " << cmd_id << " not found");
    return false;
  }

  count_vr_commands_ -= it->second->length();

  vr_commands_.erase(it);
  LOG4CXX_DEBUG(
      logger_,
      "VR command with id: "
          << cmd_id << " deleted for appID: " << app_.app_id()
          << ". Remaining number of commands: " << count_vr_commands_);
  return true;
}

void HelpPromptManagerImpl::OnVrCommandAdded(
    const uint32_t cmd_id,
    const smart_objects::SmartObject& command,
    const bool is_resumption) {
  LOG4CXX_AUTO_TRACE(logger_);
  if (SendingType::kNoneSend == sending_type_) {
    LOG4CXX_DEBUG(logger_,
                  "SendingType::kNoneSend"
                      << " commands with id:" << cmd_id
                      << " will not be added");
    return;
  }
  if (AddCommand(cmd_id, command) && !is_resumption) {
    SendRequests();
  }
}

void HelpPromptManagerImpl::OnVrCommandDeleted(const uint32_t cmd_id,
                                               const bool is_resumption) {
  LOG4CXX_AUTO_TRACE(logger_);
  if (SendingType::kNoneSend == sending_type_) {
    LOG4CXX_DEBUG(logger_,
                  "SendingType::kNoneSend"
                      << " commands with id:" << cmd_id
                      << " will not be deleted");
    return;
  }
  if (DeleteCommand(cmd_id) && !is_resumption) {
    SendRequests();
  }
}

void HelpPromptManagerImpl::OnSetGlobalPropertiesReceived(
    const smart_objects::SmartObject& msg, const bool is_response) {
  LOG4CXX_AUTO_TRACE(logger_);
  if (SendingType::kNoneSend == sending_type_) {
    LOG4CXX_DEBUG(logger_,
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

    LOG4CXX_DEBUG(logger_, "is_tts_send_:" << is_tts_send_);
    LOG4CXX_DEBUG(logger_, "is_ui_send_:" << is_ui_send_);
    return;
  }

  SetSendingType(msg);
}

HelpPromptManagerImpl::SendingType HelpPromptManagerImpl::GetSendingType()
    const {
  if (is_tts_send_ && is_ui_send_) {
    return sending_type_;
  } else if (is_tts_send_) {
    return SendingType::kSendHelpPrompt == sending_type_
               ? SendingType::kNoneSend
               : SendingType::kSendVRHelp;
  } else if (is_ui_send_) {
    return SendingType::kSendVRHelp == sending_type_
               ? SendingType::kNoneSend
               : SendingType::kSendHelpPrompt;
  }

  return sending_type_;
}

void HelpPromptManagerImpl::SendTTSRequest() {
  LOG4CXX_AUTO_TRACE(logger_);
  if (vr_commands_.empty()) {
    LOG4CXX_DEBUG(logger_, "vr_commands_ is empty");
    return;
  }
  LOG4CXX_DEBUG(logger_, "TTS request for appID:" << app_.app_id());
  smart_objects::SmartObjectSPtr tts_global_properties =
      utils::MakeShared<smart_objects::SmartObject>(
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
  LOG4CXX_AUTO_TRACE(logger_);
  if (vr_commands_.empty()) {
    LOG4CXX_DEBUG(logger_, "vr_commands_ is empty");
    return;
  }
  LOG4CXX_DEBUG(logger_, "UI request for appID:" << app_.app_id());
  smart_objects::SmartObjectSPtr ui_global_properties =
      utils::MakeShared<smart_objects::SmartObject>(
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
  LOG4CXX_AUTO_TRACE(logger_);
  SendTTSRequest();
  SendUIRequest();
}

void HelpPromptManagerImpl::SendRequests() {
  LOG4CXX_AUTO_TRACE(logger_);

  sync_primitives::AutoLock lock(vr_commands_lock_);
  if (vr_commands_.empty()) {
    LOG4CXX_DEBUG(logger_, "vr_commands_ is empty");
    return;
  }

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
  LOG4CXX_DEBUG(logger_,
                "SendingType:" << static_cast<uint32_t>(sending_type_)
                               << " request not sending");
}

void HelpPromptManagerImpl::CreatePromptMsg(
    smart_objects::SmartObject& out_msg_params) {
  LOG4CXX_AUTO_TRACE(logger_);
  if (vr_commands_.empty()) {
    LOG4CXX_DEBUG(logger_, "vr_commands_ is empty");
    return;
  }
  out_msg_params[strings::help_prompt] =
      smart_objects::SmartObject(smart_objects::SmartType_Array);
  uint32_t index = 0;
  for (const auto& it : vr_commands_) {
    for (std::size_t i = 0; i < it.second->length(); ++i) {
      smart_objects::SmartObject item(smart_objects::SmartType_Map);

      item[strings::text] = it.second->getElement(i).asString();
      item[strings::type] = mobile_apis::SpeechCapabilities::SC_TEXT;

      out_msg_params[strings::help_prompt][index++] = item;
    }
  }
  app_.set_help_prompt(out_msg_params[strings::help_prompt]);
}

void HelpPromptManagerImpl::CreateVRMsg(
    smart_objects::SmartObject& out_msg_params) {
  LOG4CXX_AUTO_TRACE(logger_);
  if (vr_commands_.empty()) {
    LOG4CXX_DEBUG(logger_, "vr_commands_ is empty");
    return;
  }
  if (false == out_msg_params.keyExists(strings::vr_help_title)) {
    if (app_.vr_help_title()) {
      out_msg_params[strings::vr_help_title] = (*app_.vr_help_title());
    } else {
      out_msg_params[strings::vr_help_title] = app_.name();
    }
  }
  out_msg_params[strings::vr_help] =
      smart_objects::SmartObject(smart_objects::SmartType_Array);
  uint32_t index = 0;
  for (const auto& it : vr_commands_) {
    for (std::size_t i = 0; i < it.second->length(); ++i) {
      smart_objects::SmartObject item(smart_objects::SmartType_Map);

      item[strings::text] = it.second->getElement(i).asString();
      item[strings::position] = index + 1;

      out_msg_params[strings::vr_help][index++] = item;
    }
  }
  app_.set_vr_help(out_msg_params[strings::vr_help]);
}

void HelpPromptManagerImpl::SetSendingType(
    const smart_objects::SmartObject& msg) {
  LOG4CXX_AUTO_TRACE(logger_);

  hmi_apis::Common_Result::eType result =
      static_cast<hmi_apis::Common_Result::eType>(
          msg[strings::params][hmi_response::code].asInt());
  LOG4CXX_DEBUG(logger_, "HMI response result:" << result);
  if (hmi_apis::Common_Result::eType::SUCCESS == result) {
    hmi_apis::FunctionID::eType function_id =
        static_cast<hmi_apis::FunctionID::eType>(
            msg[strings::params][strings::function_id].asUInt());
    LOG4CXX_DEBUG(logger_, "Function id:" << function_id);
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
      default: { break; }
    }
    LOG4CXX_DEBUG(
        logger_,
        "Sending type set to:" << static_cast<uint32_t>(sending_type_));
  }
}

}  //  namespace application_manager
