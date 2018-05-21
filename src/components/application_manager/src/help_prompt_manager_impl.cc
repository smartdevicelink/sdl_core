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
#include "application_manager/message_helper.h"
#include "application_manager/smart_object_keys.h"
#include "smart_objects/smart_object.h"
#include "application_manager/application_manager.h"
#include "application_manager/commands/command_impl.h"
#include "utils/logger.h"
#include "utils/timer_task_impl.h"
#include "utils/make_shared.h"

CREATE_LOGGERPTR_GLOBAL(logger_, "HelpPromptManagerImpl")

namespace {
const std::size_t kLimitCommand = 30;
const uint32_t kBufferingTimeout = 10000;
}

namespace application_manager {

HelpPromptManagerImpl::HelpPromptManagerImpl(Application& app,
                                             ApplicationManager& app_manager)
    : app_(app)
    , app_manager_(app_manager)
    , internal_timer_("HelpPromtManagerTimer",
                      new ::timer::TimerTaskImpl<HelpPromptManagerImpl>(
                          this, &HelpPromptManagerImpl::OnTimerExpired))
    , sending_type_(SendingType::kNoneSend)
    , count_requests_commands_(0)
    , is_tts_send_(false)
    , is_ui_send_(false) {}

HelpPromptManagerImpl::~HelpPromptManagerImpl() {
  LOG4CXX_AUTO_TRACE(logger_);
  StopTimer();
}

void HelpPromptManagerImpl::OnVrCommandAdded(
    const uint32_t cmd_id, const smart_objects::SmartObject& command) {
  LOG4CXX_AUTO_TRACE(logger_);
  if (SendingType::kNoneSend == sending_type_) {
    LOG4CXX_DEBUG(logger_,
                  "SendingType::kNoneSend"
                      << " commands with id:" << cmd_id
                      << " will not be added");
    return;
  }
  auto it = vr_commands_.find(cmd_id);
  if (vr_commands_.end() != it) {
    LOG4CXX_DEBUG(logger_, "Commands with id:" << cmd_id << " alreday exists");
    return;
  }

  if (false == command.keyExists(strings::vr_commands)) {
    LOG4CXX_DEBUG(logger_, "vr_commands does`t present");
    return;
  }
  const smart_objects::SmartObject& commands = command[strings::vr_commands];
  /**
   * The remaining number of commands for adding
   * without taking into account the commands
   * added during the timer operation
   */
  std::size_t limit = kLimitCommand - count_requests_commands_;
  LOG4CXX_DEBUG(logger_, "Remaining number of commands" << limit);
  if (0 == limit || limit > kLimitCommand) {
    LOG4CXX_DEBUG(logger_, "Commands limit is exceeded");
    return;
  }

  if (internal_timer_.is_running() || commands.length() < limit) {
    limit = commands.length();
  }

  LOG4CXX_DEBUG(logger_,
                "Internal timer is running: " << internal_timer_.is_running()
                                              << " Will be added " << limit
                                              << " commands");

  vr_commands_[cmd_id] = utils::MakeShared<smart_objects::SmartObject>(
      smart_objects::SmartType_Array);
  smart_objects::SmartArray& ar_vr_cmd = *(vr_commands_[cmd_id]->asArray());
  smart_objects::SmartArray& ar_cmd = *(commands.asArray());
  ar_vr_cmd.reserve(limit);
  ar_vr_cmd.insert(ar_vr_cmd.end(), ar_cmd.begin(), ar_cmd.begin() + limit);
  LOG4CXX_DEBUG(logger_,
                "VR commands with id: " << cmd_id << " added for appID: "
                                        << app_.app_id());
  if (false == internal_timer_.is_running()) {
    count_requests_commands_ += limit;
    SendRequests();
  }
}

void HelpPromptManagerImpl::OnVrCommandDeleted(const uint32_t cmd_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  if (SendingType::kNoneSend == sending_type_) {
    LOG4CXX_DEBUG(logger_,
                  "SendingType::kNoneSend"
                      << " commands with id:" << cmd_id
                      << " will not be deleted");
    return;
  }
  auto it = vr_commands_.find(cmd_id);
  if (vr_commands_.end() != it) {
    count_requests_commands_ -=
        kLimitCommand == count_requests_commands_ ? 0 : it->second->length();

    vr_commands_.erase(it);
    LOG4CXX_DEBUG(logger_,
                  "VR command with id: " << cmd_id << " deleted for appID: "
                                         << app_.app_id());
    if (false == internal_timer_.is_running()) {
      SendRequests();
    }
  }
}

void HelpPromptManagerImpl::OnTimeoutExpired() {
  LOG4CXX_AUTO_TRACE(logger_);
  SendRequests();
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
  sending_type_ = GetSendingType(msg, is_response);
  if (SendingType::kNoneSend == sending_type_) {
    StopTimer();
  }
  LOG4CXX_DEBUG(logger_, "Set sending type to:" << unsigned(sending_type_));
}

void HelpPromptManagerImpl::OnAppActivated(const bool is_restore) {
  LOG4CXX_AUTO_TRACE(logger_);
  LOG4CXX_DEBUG(logger_, "Activated for appID:" << app_.app_id());
  if (is_restore) {
    LOG4CXX_DEBUG(logger_, "restoring");
    is_tts_send_ = (app_.help_prompt() && app_.help_prompt()->length());
    is_ui_send_ = (app_.vr_help() && app_.vr_help()->length());
    LOG4CXX_DEBUG(logger_, "is_tts_send_:" << is_tts_send_);
    LOG4CXX_DEBUG(logger_, "is_ui_send_:" << is_ui_send_);
    if (is_tts_send_ && is_ui_send_) {
      LOG4CXX_DEBUG(logger_,
                    "SetGlobalProperties RPC"
                    " with the vrHelp and helpPrompt"
                    " has been sent");
      return;
    }
  }
  StartTimer(is_restore);
}

void HelpPromptManagerImpl::OnAppUnregistered() {
  LOG4CXX_AUTO_TRACE(logger_);
  LOG4CXX_DEBUG(logger_, "Unregisted for appID:" << app_.app_id());
  StopTimer();
}

void HelpPromptManagerImpl::StartTimer(const bool is_restore) {
  LOG4CXX_AUTO_TRACE(logger_);
  LOG4CXX_DEBUG(logger_, "Timer for appID:" << app_.app_id());
  StopTimer();
  sending_type_ = SendingType::kSendBoth;
  internal_timer_.Start(kBufferingTimeout, timer::kSingleShot);
  if (is_restore) {
    sending_type_ = is_tts_send_ ? SendingType::kSendVRHelp : sending_type_;
    sending_type_ = is_ui_send_ ? SendingType::kSendHelpPromt : sending_type_;
    is_tts_send_ = false;
    is_ui_send_ = false;
    const DataAccessor<CommandsMap> accessor = app_.commands_map();
    const CommandsMap& commands = accessor.GetData();
    for (auto& command : commands) {
      OnVrCommandAdded(command.first, *command.second);
    }
  }
}

void HelpPromptManagerImpl::StopTimer() {
  LOG4CXX_AUTO_TRACE(logger_);
  sending_type_ = SendingType::kNoneSend;
  if (internal_timer_.is_running()) {
    internal_timer_.Stop();
  }
  vr_commands_.clear();
}

void HelpPromptManagerImpl::OnTimerExpired() {
  LOG4CXX_AUTO_TRACE(logger_);
  OnTimeoutExpired();
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

    CreatePromtMsg(msg_params);

    msg_params[strings::app_id] = app_.app_id();
    so_to_send[strings::msg_params] = msg_params;
    app_manager_.ManageHMICommand(tts_global_properties);
  }
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
    app_manager_.ManageHMICommand(ui_global_properties);
  }
}

void HelpPromptManagerImpl::SendBothRequests() {
  LOG4CXX_AUTO_TRACE(logger_);
  SendTTSRequest();
  SendUIRequest();
}

void HelpPromptManagerImpl::SendRequests() {
  LOG4CXX_AUTO_TRACE(logger_);
  if (vr_commands_.empty()) {
    LOG4CXX_DEBUG(logger_, "vr_commands_ is empty");
    return;
  }

  switch (sending_type_) {
    case SendingType::kSendHelpPromt:
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
                "SendingType:" << (unsigned)sending_type_
                               << " request not sending");
}

void HelpPromptManagerImpl::CreatePromtMsg(
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

HelpPromptManagerImpl::SendingType HelpPromptManagerImpl::GetSendingType(
    const smart_objects::SmartObject& msg, const bool is_response) {
  LOG4CXX_AUTO_TRACE(logger_);
  if (false == is_response) {
    if (msg.keyExists(strings::help_prompt)) {
      is_tts_send_ = true;
    }
    if (msg.keyExists(strings::vr_help)) {
      is_ui_send_ = true;
    }
    LOG4CXX_DEBUG(logger_, "is_tts_send_:" << is_tts_send_);
    LOG4CXX_DEBUG(logger_, "is_ui_send_:" << is_ui_send_);
    return sending_type_;
  }
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
      case hmi_apis::FunctionID::TTS_SetGlobalProperties:
        if (is_tts_send_) {
          is_tts_send_ = false;
          return (SendingType::kSendHelpPromt == sending_type_)
                     ? SendingType::kNoneSend
                     : SendingType::kSendVRHelp;
        }
        break;
      case hmi_apis::FunctionID::UI_SetGlobalProperties:
        if (is_ui_send_) {
          is_ui_send_ = false;
          return (SendingType::kSendVRHelp == sending_type_)
                     ? SendingType::kNoneSend
                     : SendingType::kSendHelpPromt;
        }
        break;
      default:
        return sending_type_;
    }
  }
  return sending_type_;
}

}  //  namespace application_manager
