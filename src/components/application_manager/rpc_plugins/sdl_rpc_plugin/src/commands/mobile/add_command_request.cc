/*

 Copyright (c) 2016, Ford Motor Company
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

#include <string>
#include "sdl_rpc_plugin/commands/mobile/add_command_request.h"

#include "application_manager/application.h"
#include "application_manager/message_helper.h"
#include "utils/file_system.h"
#include "utils/helpers.h"
#include "utils/custom_string.h"

namespace sdl_rpc_plugin {
using namespace application_manager;

namespace commands {

namespace custom_str = utils::custom_string;

AddCommandRequest::AddCommandRequest(
    const application_manager::commands::MessageSharedPtr& message,
    ApplicationManager& application_manager,
    rpc_service::RPCService& rpc_service,
    HMICapabilities& hmi_capabilities,
    policy::PolicyHandlerInterface& policy_handler)
    : CommandRequestImpl(message,
                         application_manager,
                         rpc_service,
                         hmi_capabilities,
                         policy_handler)
    , send_ui_(false)
    , send_vr_(false)
    , is_ui_received_(false)
    , is_vr_received_(false)
    , ui_result_(hmi_apis::Common_Result::INVALID_ENUM)
    , vr_result_(hmi_apis::Common_Result::INVALID_ENUM) {}

AddCommandRequest::~AddCommandRequest() {}

void AddCommandRequest::onTimeOut() {
  LOG4CXX_AUTO_TRACE(logger_);
  RemoveCommand();
  CommandRequestImpl::onTimeOut();
}

bool AddCommandRequest::Init() {
  hash_update_mode_ = HashUpdateMode::kDoHashUpdate;
  return true;
}

void AddCommandRequest::Run() {
  LOG4CXX_AUTO_TRACE(logger_);

  ApplicationSharedPtr app = application_manager_.application(
      (*message_)[strings::params][strings::connection_key].asUInt());

  if (!app) {
    LOG4CXX_ERROR(logger_, "No application associated with session key");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return;
  }

  if ((*message_)[strings::msg_params].keyExists(strings::cmd_icon)) {
    mobile_apis::Result::eType verification_result = MessageHelper::VerifyImage(
        (*message_)[strings::msg_params][strings::cmd_icon],
        app,
        application_manager_);

    if (mobile_apis::Result::SUCCESS != verification_result) {
      LOG4CXX_ERROR(
          logger_, "MessageHelper::VerifyImage return " << verification_result);
      SendResponse(false, verification_result);
      return;
    }
  }

  if (!((*message_)[strings::msg_params].keyExists(strings::cmd_id))) {
    LOG4CXX_ERROR(logger_, "INVALID_DATA");
    SendResponse(false, mobile_apis::Result::INVALID_DATA);
    return;
  }

  if (app->FindCommand(
          (*message_)[strings::msg_params][strings::cmd_id].asUInt())) {
    LOG4CXX_ERROR(logger_, "INVALID_ID");
    SendResponse(false, mobile_apis::Result::INVALID_ID);
    return;
  }

  bool data_exist = false;

  if ((*message_)[strings::msg_params].keyExists(strings::menu_params)) {
    if (!CheckCommandName(app)) {
      SendResponse(false, mobile_apis::Result::DUPLICATE_NAME);
      return;
    }
    if (((*message_)[strings::msg_params][strings::menu_params].keyExists(
            hmi_request::parent_id)) &&
        (0 !=
         (*message_)[strings::msg_params][strings::menu_params]
                    [hmi_request::parent_id].asUInt())) {
      if (!CheckCommandParentId(app)) {
        SendResponse(
            false, mobile_apis::Result::INVALID_ID, "Parent ID doesn't exist");
        return;
      }
    }
    data_exist = true;
  }

  if (((*message_)[strings::msg_params].keyExists(strings::vr_commands)) &&
      ((*message_)[strings::msg_params][strings::vr_commands].length() > 0)) {
    if (!CheckCommandVRSynonym(app)) {
      SendResponse(false, mobile_apis::Result::DUPLICATE_NAME);
      return;
    }

    data_exist = true;
  }

  if (!data_exist) {
    LOG4CXX_ERROR(logger_, "INVALID_DATA");
    SendResponse(false, mobile_apis::Result::INVALID_DATA);
    return;
  }

  if (IsWhiteSpaceExist()) {
    LOG4CXX_ERROR(logger_, "Incoming add command has contains \t\n \\t \\n");
    SendResponse(false, mobile_apis::Result::INVALID_DATA);
    return;
  }

  app->AddCommand((*message_)[strings::msg_params][strings::cmd_id].asUInt(),
                  (*message_)[strings::msg_params]);

  smart_objects::SmartObject ui_msg_params =
      smart_objects::SmartObject(smart_objects::SmartType_Map);
  if ((*message_)[strings::msg_params].keyExists(strings::menu_params)) {
    ui_msg_params[strings::cmd_id] =
        (*message_)[strings::msg_params][strings::cmd_id];
    ui_msg_params[strings::menu_params] =
        (*message_)[strings::msg_params][strings::menu_params];

    ui_msg_params[strings::app_id] = app->app_id();

    if (((*message_)[strings::msg_params].keyExists(strings::cmd_icon)) &&
        ((*message_)[strings::msg_params][strings::cmd_icon].keyExists(
            strings::value)) &&
        (0 < (*message_)[strings::msg_params][strings::cmd_icon][strings::value]
                 .length())) {
      ui_msg_params[strings::cmd_icon] =
          (*message_)[strings::msg_params][strings::cmd_icon];
    }

    send_ui_ = true;
  }

  smart_objects::SmartObject vr_msg_params =
      smart_objects::SmartObject(smart_objects::SmartType_Map);
  if ((*message_)[strings::msg_params].keyExists(strings::vr_commands)) {
    vr_msg_params[strings::cmd_id] =
        (*message_)[strings::msg_params][strings::cmd_id];
    vr_msg_params[strings::vr_commands] =
        (*message_)[strings::msg_params][strings::vr_commands];
    vr_msg_params[strings::app_id] = app->app_id();

    vr_msg_params[strings::type] = hmi_apis::Common_VRCommandType::Command;
    vr_msg_params[strings::grammar_id] = app->get_grammar_id();

    send_vr_ = true;
  }

  if (send_ui_) {
    StartAwaitForInterface(HmiInterfaces::HMI_INTERFACE_UI);
    SendHMIRequest(hmi_apis::FunctionID::UI_AddCommand, &ui_msg_params, true);
  }

  if (send_vr_) {
    StartAwaitForInterface(HmiInterfaces::HMI_INTERFACE_VR);
    SendHMIRequest(hmi_apis::FunctionID::VR_AddCommand, &vr_msg_params, true);
  }
}

bool AddCommandRequest::CheckCommandName(ApplicationConstSharedPtr app) {
  if (!app) {
    return false;
  }

  const DataAccessor<CommandsMap> accessor = app->commands_map();
  const CommandsMap& commands = accessor.GetData();
  CommandsMap::const_iterator i = commands.begin();
  uint32_t saved_parent_id = 0;
  uint32_t parent_id = 0;
  if ((*message_)[strings::msg_params][strings::menu_params].keyExists(
          hmi_request::parent_id)) {
    parent_id = (*message_)[strings::msg_params][strings::menu_params]
                           [hmi_request::parent_id].asUInt();
  }

  for (; commands.end() != i; ++i) {
    if (!(*i->second).keyExists(strings::menu_params)) {
      continue;
    }

    saved_parent_id = 0;
    if ((*i->second)[strings::menu_params].keyExists(hmi_request::parent_id)) {
      saved_parent_id =
          (*i->second)[strings::menu_params][hmi_request::parent_id].asUInt();
    }
    if (((*i->second)[strings::menu_params][strings::menu_name].asString() ==
         (*message_)[strings::msg_params][strings::menu_params]
                    [strings::menu_name].asString()) &&
        (saved_parent_id == parent_id)) {
      LOG4CXX_INFO(logger_,
                   "AddCommandRequest::CheckCommandName received"
                   " command name already exist in same level menu");
      return false;
    }
  }
  return true;
}

bool AddCommandRequest::CheckCommandVRSynonym(ApplicationConstSharedPtr app) {
  if (!app) {
    return false;
  }

  const DataAccessor<CommandsMap> accessor = app->commands_map();
  const CommandsMap& commands = accessor.GetData();
  CommandsMap::const_iterator it = commands.begin();

  for (; commands.end() != it; ++it) {
    if (!(*it->second).keyExists(strings::vr_commands)) {
      continue;
    }

    for (size_t i = 0; i < (*it->second)[strings::vr_commands].length(); ++i) {
      for (size_t j = 0;
           j < (*message_)[strings::msg_params][strings::vr_commands].length();
           ++j) {
        const custom_str::CustomString& vr_cmd_i =
            (*it->second)[strings::vr_commands][i].asCustomString();
        const custom_str::CustomString& vr_cmd_j =
            (*message_)[strings::msg_params][strings::vr_commands][j]
                .asCustomString();

        if (vr_cmd_i.CompareIgnoreCase(vr_cmd_j)) {
          LOG4CXX_INFO(logger_,
                       "AddCommandRequest::CheckCommandVRSynonym"
                       " received command vr synonym already exist");
          return false;
        }
      }
    }
  }
  return true;
}

bool AddCommandRequest::CheckCommandParentId(ApplicationConstSharedPtr app) {
  if (!app) {
    return false;
  }

  const int32_t parent_id =
      (*message_)[strings::msg_params][strings::menu_params]
                 [hmi_request::parent_id].asInt();
  smart_objects::SmartObject* parent = app->FindSubMenu(parent_id);

  if (!parent) {
    LOG4CXX_INFO(logger_,
                 "AddCommandRequest::CheckCommandParentId received"
                 " submenu doesn't exist");
    return false;
  }
  return true;
}

// TODO(AKUTSAN) APPLINK-26973: Refactor AddCommandRequest
void AddCommandRequest::on_event(const event_engine::Event& event) {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace helpers;

  const smart_objects::SmartObject& message = event.smart_object();

  ApplicationSharedPtr application =
      application_manager_.application(connection_key());

  if (!application) {
    LOG4CXX_ERROR(logger_, "NULL pointer");
    return;
  }

  smart_objects::SmartObject msg_param(smart_objects::SmartType_Map);
  msg_param[strings::cmd_id] =
      (*message_)[strings::msg_params][strings::cmd_id];
  msg_param[strings::app_id] = application->app_id();

  switch (event.id()) {
    case hmi_apis::FunctionID::UI_AddCommand: {
      LOG4CXX_INFO(logger_, "Received UI_AddCommand event");
      EndAwaitForInterface(HmiInterfaces::HMI_INTERFACE_UI);
      is_ui_received_ = true;
      ui_result_ = static_cast<hmi_apis::Common_Result::eType>(
          message[strings::params][hmi_response::code].asInt());
      GetInfo(message, ui_info_);
      if (hmi_apis::Common_Result::SUCCESS != ui_result_) {
        (*message_)[strings::msg_params].erase(strings::menu_params);
      }
      break;
    }
    case hmi_apis::FunctionID::VR_AddCommand: {
      LOG4CXX_INFO(logger_, "Received VR_AddCommand event");
      EndAwaitForInterface(HmiInterfaces::HMI_INTERFACE_VR);
      is_vr_received_ = true;
      vr_result_ = static_cast<hmi_apis::Common_Result::eType>(
          message[strings::params][hmi_response::code].asInt());
      GetInfo(message, vr_info_);
      if (hmi_apis::Common_Result::SUCCESS != vr_result_) {
        (*message_)[strings::msg_params].erase(strings::vr_commands);
      }
      break;
    }
    default: {
      LOG4CXX_ERROR(logger_, "Received unknown event" << event.id());
      return;
    }
  }

  if (IsPendingResponseExist()) {
    return;
  }

  smart_objects::SmartObject msg_params(smart_objects::SmartType_Map);
  msg_params[strings::cmd_id] =
      (*message_)[strings::msg_params][strings::cmd_id];
  msg_params[strings::app_id] = application->app_id();

  mobile_apis::Result::eType result_code = mobile_apis::Result::INVALID_ENUM;

  const bool is_vr_invalid_unsupported =
      Compare<hmi_apis::Common_Result::eType, EQ, ONE>(
          vr_result_,
          hmi_apis::Common_Result::INVALID_ENUM,
          hmi_apis::Common_Result::UNSUPPORTED_RESOURCE);

  const bool is_ui_invalid_unsupported =
      Compare<hmi_apis::Common_Result::eType, EQ, ONE>(
          ui_result_,
          hmi_apis::Common_Result::INVALID_ENUM,
          hmi_apis::Common_Result::UNSUPPORTED_RESOURCE);
  const bool is_vr_unsupported =
      vr_result_ == hmi_apis::Common_Result::UNSUPPORTED_RESOURCE;
  const bool is_ui_unsupported =
      ui_result_ == hmi_apis::Common_Result::UNSUPPORTED_RESOURCE;

  const bool is_no_ui_error = Compare<hmi_apis::Common_Result::eType, EQ, ONE>(
      ui_result_,
      hmi_apis::Common_Result::SUCCESS,
      hmi_apis::Common_Result::WARNINGS,
      hmi_apis::Common_Result::WRONG_LANGUAGE,
      hmi_apis::Common_Result::RETRY,
      hmi_apis::Common_Result::SAVED,
      hmi_apis::Common_Result::UNSUPPORTED_RESOURCE);

  const bool is_no_vr_error = Compare<hmi_apis::Common_Result::eType, EQ, ONE>(
      vr_result_,
      hmi_apis::Common_Result::SUCCESS,
      hmi_apis::Common_Result::WARNINGS,
      hmi_apis::Common_Result::WRONG_LANGUAGE,
      hmi_apis::Common_Result::RETRY,
      hmi_apis::Common_Result::SAVED,
      hmi_apis::Common_Result::UNSUPPORTED_RESOURCE);

  bool result = (is_no_ui_error && is_no_vr_error) ||
                (is_no_ui_error && is_vr_invalid_unsupported) ||
                (is_no_vr_error && is_ui_invalid_unsupported);

  LOG4CXX_DEBUG(logger_,
                "calculated result " << ui_result_ << " " << is_no_ui_error
                                     << " " << is_no_vr_error);
  const bool is_vr_or_ui_warning =
      Compare<hmi_apis::Common_Result::eType, EQ, ONE>(
          hmi_apis::Common_Result::WARNINGS, ui_result_, vr_result_);

  const bool is_vr_or_ui_unsupported =
      Compare<hmi_apis::Common_Result::eType, EQ, ONE>(
          hmi_apis::Common_Result::UNSUPPORTED_RESOURCE,
          ui_result_,
          vr_result_);

  const bool is_vr_and_ui_unsupported =
      Compare<hmi_apis::Common_Result::eType, EQ, ALL>(
          hmi_apis::Common_Result::UNSUPPORTED_RESOURCE,
          ui_result_,
          vr_result_);

  if (!result && hmi_apis::Common_Result::REJECTED == ui_result_) {
    result_code = MessageHelper::HMIToMobileResult(ui_result_);
  } else if (result && is_vr_or_ui_unsupported) {
    result_code = mobile_apis::Result::UNSUPPORTED_RESOURCE;
  } else if (is_vr_or_ui_warning) {
    result_code = mobile_apis::Result::WARNINGS;
  } else {
    result_code =
        MessageHelper::HMIToMobileResult(std::max(ui_result_, vr_result_));
    if (hmi_apis::Common_Result::UNSUPPORTED_RESOURCE == ui_result_) {
      result_code = MessageHelper::HMIToMobileResult(vr_result_);
    }
    if (hmi_apis::Common_Result::UNSUPPORTED_RESOURCE == vr_result_) {
      result_code = MessageHelper::HMIToMobileResult(ui_result_);
    }
    LOG4CXX_DEBUG(logger_, "HMIToMobileResult " << result_code);
  }

  if (BothSend() && hmi_apis::Common_Result::SUCCESS == vr_result_) {
    const bool is_ui_not_ok = Compare<hmi_apis::Common_Result::eType, NEQ, ALL>(
        ui_result_,
        hmi_apis::Common_Result::SUCCESS,
        hmi_apis::Common_Result::WARNINGS,
        hmi_apis::Common_Result::UNSUPPORTED_RESOURCE);

    if (is_ui_not_ok) {
      result_code = ui_result_ == hmi_apis::Common_Result::REJECTED
                        ? mobile_apis::Result::REJECTED
                        : mobile_apis::Result::GENERIC_ERROR;

      msg_params[strings::grammar_id] = application->get_grammar_id();
      msg_params[strings::type] = hmi_apis::Common_VRCommandType::Command;

      SendHMIRequest(hmi_apis::FunctionID::VR_DeleteCommand, &msg_params);
      application->RemoveCommand(
          (*message_)[strings::msg_params][strings::cmd_id].asUInt());
      result = false;
      LOG4CXX_DEBUG(logger_, "Result " << result);
    }
  }

  if (BothSend() && hmi_apis::Common_Result::SUCCESS == ui_result_ &&
      !is_no_vr_error &&
      hmi_apis::Common_Result::UNSUPPORTED_RESOURCE != vr_result_) {
    result_code = vr_result_ == hmi_apis::Common_Result::REJECTED
                      ? mobile_apis::Result::REJECTED
                      : mobile_apis::Result::GENERIC_ERROR;

    SendHMIRequest(hmi_apis::FunctionID::UI_DeleteCommand, &msg_params);

    application->RemoveCommand(
        (*message_)[strings::msg_params][strings::cmd_id].asUInt());
    result = false;
    LOG4CXX_DEBUG(logger_, "Result " << result);
  }

  HmiInterfaces::InterfaceState ui_interface_state =
      application_manager_.hmi_interfaces().GetInterfaceState(
          HmiInterfaces::HMI_INTERFACE_UI);
  HmiInterfaces::InterfaceState vr_interface_state =
      application_manager_.hmi_interfaces().GetInterfaceState(
          HmiInterfaces::HMI_INTERFACE_VR);

  if (!BothSend() &&
      ((is_vr_unsupported &&
        HmiInterfaces::STATE_NOT_AVAILABLE == vr_interface_state) ||
       (is_ui_unsupported &&
        HmiInterfaces::STATE_NOT_AVAILABLE == ui_interface_state))) {
    LOG4CXX_DEBUG(logger_, "!BothSend() && is_vr_or_ui_unsupported");
    result = false;
  }

  if (is_vr_and_ui_unsupported) {
    LOG4CXX_DEBUG(logger_, "UI and VR interface both unsupported");
    result = false;
  }

  if (!result) {
    RemoveCommand();
  }

  const std::string info = GenerateMobileResponseInfo();
  SendResponse(result,
               result_code,
               info.empty() ? NULL : info.c_str(),
               &(message[strings::msg_params]));
}

bool AddCommandRequest::IsPendingResponseExist() {
  return send_ui_ != is_ui_received_ || send_vr_ != is_vr_received_;
}

bool AddCommandRequest::IsWhiteSpaceExist() {
  LOG4CXX_AUTO_TRACE(logger_);
  const char* str = NULL;

  if ((*message_)[strings::msg_params].keyExists(strings::menu_params)) {
    str = (*message_)[strings::msg_params][strings::menu_params]
                     [strings::menu_name].asCharArray();
    if (!CheckSyntax(str)) {
      LOG4CXX_ERROR(logger_, "Invalid menu name syntax check failed.");
      return true;
    }
  }

  if ((*message_)[strings::msg_params].keyExists(strings::vr_commands)) {
    const size_t len =
        (*message_)[strings::msg_params][strings::vr_commands].length();

    for (size_t i = 0; i < len; ++i) {
      str = (*message_)[strings::msg_params][strings::vr_commands][i]
                .asCharArray();
      if (!CheckSyntax(str)) {
        LOG4CXX_ERROR(logger_, "Invalid vr_commands syntax check failed");
        return true;
      }
    }
  }

  if ((*message_)[strings::msg_params].keyExists(strings::cmd_icon)) {
    str = (*message_)[strings::msg_params][strings::cmd_icon][strings::value]
              .asCharArray();
    if (!CheckSyntax(str)) {
      LOG4CXX_ERROR(logger_, "Invalid cmd_icon value syntax check failed");
      return true;
    }
  }
  return false;
}

bool AddCommandRequest::BothSend() const {
  return send_vr_ && send_ui_;
}

const std::string AddCommandRequest::GenerateMobileResponseInfo() {
  // In case if vr_result_ is UNSUPPORTED_RESOURCE vr_info should be on the
  // first place
  // In case if ui_result_ is UNSUPPORTED_RESOURCE ui_info should be on the
  // first place
  // Other way order is doesn't matter

  HmiInterfaces& hmi_interfaces = application_manager_.hmi_interfaces();
  HmiInterfaces::InterfaceState ui_interface_state =
      hmi_interfaces.GetInterfaceState(HmiInterfaces::HMI_INTERFACE_UI);

  HmiInterfaces::InterfaceState vr_interface_state =
      hmi_interfaces.GetInterfaceState(HmiInterfaces::HMI_INTERFACE_VR);

  if ((ui_interface_state == HmiInterfaces::STATE_NOT_AVAILABLE) &&
      (vr_interface_state != HmiInterfaces::STATE_NOT_AVAILABLE) &&
      !vr_info_.empty()) {
    return vr_info_;
  }

  if ((vr_interface_state == HmiInterfaces::STATE_NOT_AVAILABLE) &&
      (ui_interface_state != HmiInterfaces::STATE_NOT_AVAILABLE) &&
      !ui_info_.empty()) {
    return ui_info_;
  }

  return app_mngr::commands::MergeInfos(ui_info_, vr_info_);
}

void AddCommandRequest::RemoveCommand() {
  LOG4CXX_AUTO_TRACE(logger_);
  ApplicationSharedPtr app = application_manager_.application(connection_key());
  if (!app.valid()) {
    LOG4CXX_ERROR(logger_, "No application associated with session key");
    return;
  }

  smart_objects::SmartObject msg_params(smart_objects::SmartType_Map);
  msg_params[strings::cmd_id] =
      (*message_)[strings::msg_params][strings::cmd_id];
  msg_params[strings::app_id] = app->app_id();

  app->RemoveCommand(
      (*message_)[strings::msg_params][strings::cmd_id].asUInt());

  if (BothSend() && !(is_vr_received_ || is_ui_received_)) {
    // in case we have send bth UI and VR and no one respond
    // we have nothing to remove from HMI so no DeleteCommand expected
    return;
  }

  if (BothSend() && !is_vr_received_) {
    SendHMIRequest(hmi_apis::FunctionID::UI_DeleteCommand, &msg_params);
  }

  if (BothSend() && !is_ui_received_) {
    msg_params[strings::grammar_id] = app->get_grammar_id();
    msg_params[strings::type] = hmi_apis::Common_VRCommandType::Command;
    SendHMIRequest(hmi_apis::FunctionID::VR_DeleteCommand, &msg_params);
  }
}

}  // namespace commands

}  // namespace application_manager
