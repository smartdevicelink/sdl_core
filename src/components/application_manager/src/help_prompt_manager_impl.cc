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

CREATE_LOGGERPTR_GLOBAL(logger_, "HelpPromptManagerImpl")

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
  LOG4CXX_AUTO_TRACE(logger_);
}

bool HelpPromptManagerImpl::AddCommand(
    const uint32_t cmd_id, const smart_objects::SmartObject& command) {
  if (!command.keyExists(strings::vr_commands)) {
    LOG4CXX_DEBUG(logger_, "vr_commands does`t present");
    return false;
  }

  const smart_objects::SmartObject& commands = command[strings::vr_commands];
  if (commands.empty()) {
    LOG4CXX_DEBUG(logger_, "vr_commands array is empty");
    return false;
  }

  sync_primitives::AutoLock lock(vr_commands_lock_);
  auto it = std::find_if(
      vr_commands_.begin(),
      vr_commands_.end(),
      [cmd_id](const VRCommandPair& pair) { return pair.first == cmd_id; });

  if (vr_commands_.end() != it) {
    LOG4CXX_DEBUG(logger_, "Command with id:" << cmd_id << " already exists");
    return false;
  }

  const bool limit_exceeded =
      kLimitCommand <= GetCommandsCount(vr_commands_.end());

  LOG4CXX_DEBUG(logger_,
                "Will be added first command from array "
                    << strings::vr_commands);

  smart_objects::SmartObjectSPtr vr_item =
      std::make_shared<smart_objects::SmartObject>(commands.asArray()->front());
  vr_commands_.push_back(std::make_pair(cmd_id, vr_item));

  LOG4CXX_DEBUG(logger_,
                "VR command with id: " << cmd_id << " added for appID: "
                                       << app_.app_id() << ". Total "
                                       << vr_commands_.size() << " in cache");

  return !limit_exceeded;
}

bool HelpPromptManagerImpl::DeleteCommand(const uint32_t cmd_id) {
  LOG4CXX_AUTO_TRACE(logger_);

  sync_primitives::AutoLock lock(vr_commands_lock_);

  auto it = std::find_if(
      vr_commands_.begin(),
      vr_commands_.end(),
      [cmd_id](const VRCommandPair& pair) { return pair.first == cmd_id; });

  if (vr_commands_.end() == it) {
    LOG4CXX_WARN(logger_, "VR command with id: " << cmd_id << " not found");
    return false;
  }

  const size_t commands_before_current = GetCommandsCount(it);
  vr_commands_.erase(it);
  LOG4CXX_DEBUG(logger_,
                "VR command with id: "
                    << cmd_id << " found after " << commands_before_current
                    << " commands was deleted for appID: " << app_.app_id()
                    << " Cache size after deleting: " << vr_commands_.size());

  return commands_before_current < kLimitCommand;
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
  LOG4CXX_AUTO_TRACE(logger_);
  LOG4CXX_DEBUG(logger_, "TTS request for appID:" << app_.app_id());
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
  LOG4CXX_AUTO_TRACE(logger_);
  LOG4CXX_DEBUG(logger_, "UI request for appID:" << app_.app_id());
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
  LOG4CXX_AUTO_TRACE(logger_);
  SendTTSRequest();
  SendUIRequest();
}

void HelpPromptManagerImpl::SendRequests() {
  LOG4CXX_AUTO_TRACE(logger_);

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
  LOG4CXX_DEBUG(logger_,
                "SendingType:" << static_cast<uint32_t>(sending_type_)
                               << " request not sending");
}

void HelpPromptManagerImpl::CreatePromptMsg(
    smart_objects::SmartObject& out_msg_params) {
  LOG4CXX_AUTO_TRACE(logger_);
  GenerateVrItems(out_msg_params, strings::help_prompt);
  app_.set_help_prompt(out_msg_params[strings::help_prompt]);
}

void HelpPromptManagerImpl::CreateVRMsg(
    smart_objects::SmartObject& out_msg_params) {
  LOG4CXX_AUTO_TRACE(logger_);
  if (false == out_msg_params.keyExists(strings::vr_help_title)) {
    if (app_.vr_help_title()) {
      out_msg_params[strings::vr_help_title] = (*app_.vr_help_title());
    } else {
      out_msg_params[strings::vr_help_title] = app_.name();
    }
  }

  GenerateVrItems(out_msg_params, strings::vr_help);

  if (out_msg_params[strings::vr_help].empty()) {
    out_msg_params.erase(strings::vr_help);
    app_.reset_vr_help();
  } else {
    app_.set_vr_help(out_msg_params[strings::vr_help]);
  }
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
