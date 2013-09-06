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

#include <string>
#include "application_manager/commands/mobile/add_command_request.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/application_impl.h"
#include "application_manager/message_helper.h"
#include "interfaces/MOBILE_API.h"
#include "interfaces/HMI_API.h"
#include "utils/file_system.h"

namespace application_manager {

namespace commands {

AddCommandRequest::AddCommandRequest(const MessageSharedPtr& message)
    : CommandRequestImpl(message) {
}

AddCommandRequest::~AddCommandRequest() {
}

void AddCommandRequest::Run() {
  LOG4CXX_INFO(logger_, "AddCommandRequest::Run");

  Application* app = ApplicationManagerImpl::instance()->application(
      (*message_)[strings::params][strings::connection_key]);

  if (NULL == app) {
    LOG4CXX_ERROR_EXT(logger_, "No application associated with session key");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return;
  }

  mobile_apis::Result::eType verification_result =
      MessageHelper::VerifyImageFiles((*message_)[strings::msg_params], app);

  if (mobile_apis::Result::SUCCESS != verification_result) {
    LOG4CXX_ERROR_EXT(
        logger_,
        "MessageHelper::VerifyImageFiles return " << verification_result);
    SendResponse(false, verification_result);
    return;
  }

  if (!((*message_)[strings::msg_params].keyExists(strings::cmd_id))) {
    LOG4CXX_ERROR_EXT(logger_, "INVALID_DATA");
    SendResponse(false, mobile_apis::Result::INVALID_DATA);
    return;
  }

  if (app->FindCommand(
      (*message_)[strings::msg_params][strings::cmd_id].asUInt())) {
    LOG4CXX_ERROR_EXT(logger_, "INVALID_ID");
    SendResponse(false, mobile_apis::Result::INVALID_ID);
    return;
  }

  // we should specify amount of required responses in the 1st request
  unsigned int chaining_counter = 0;
  if ((*message_)[strings::msg_params].keyExists(strings::menu_params)) {
    if (!CheckCommandName(app)) {
      SendResponse(false, mobile_apis::Result::DUPLICATE_NAME);
      return;
    }
    if ((*message_)[strings::msg_params][strings::menu_params].keyExists(
        hmi_request::parent_id)) {
      if (!CheckCommandParentId(app)) {
        SendResponse(false, mobile_apis::Result::INVALID_ID,
                     "Parent ID doesn't exist");
        return;
      }
    }
    ++chaining_counter;
  }

  if (((*message_)[strings::msg_params].keyExists(strings::vr_commands))
      && ((*message_)[strings::msg_params][strings::vr_commands].length() > 0)) {
    if (!CheckCommandVRSynonym(app)) {
      SendResponse(false, mobile_apis::Result::DUPLICATE_NAME);
      return;
    }
    ++chaining_counter;
  }

  if (!chaining_counter) {
    LOG4CXX_ERROR_EXT(logger_, "INVALID_DATA");
    SendResponse(false, mobile_apis::Result::INVALID_DATA);
    return;
  }

  if ((*message_)[strings::msg_params].keyExists(strings::menu_params)) {
    smart_objects::SmartObject msg_params = smart_objects::SmartObject(
        smart_objects::SmartType_Map);
    msg_params[strings::cmd_id] =
        (*message_)[strings::msg_params][strings::cmd_id];
    msg_params[strings::menu_params] =
        (*message_)[strings::msg_params][strings::menu_params];

    msg_params[strings::app_id] = app->app_id();

    if (((*message_)[strings::msg_params][strings::cmd_icon].keyExists(
        strings::value))
        && (0
            < (*message_)[strings::msg_params][strings::cmd_icon][strings::value]
                .length())) {
      msg_params[strings::cmd_icon] =
          (*message_)[strings::msg_params][strings::cmd_icon];
    }

    CreateHMIRequest(hmi_apis::FunctionID::UI_AddCommand, msg_params, true,
                     chaining_counter);
  }

  if ((*message_)[strings::msg_params].keyExists(strings::vr_commands)) {
    smart_objects::SmartObject msg_params = smart_objects::SmartObject(
        smart_objects::SmartType_Map);
    msg_params[strings::cmd_id] =
        (*message_)[strings::msg_params][strings::cmd_id];
    msg_params[strings::vr_commands] =
        (*message_)[strings::msg_params][strings::vr_commands];
    msg_params[strings::app_id] = app->app_id();

    CreateHMIRequest(hmi_apis::FunctionID::VR_AddCommand, msg_params, true);
  }
}

bool AddCommandRequest::CheckCommandName(const Application* app) {
  if (NULL == app) {
    return false;
  }

  const CommandsMap& commands = app->commands_map();
  CommandsMap::const_iterator i = commands.begin();

  for (; commands.end() != i; ++i) {
    if ((*i->second)[strings::menu_params][strings::menu_name].asString()
        == (*message_)[strings::msg_params][strings::menu_params][strings::menu_name]
            .asString()) {
      LOG4CXX_INFO(logger_, "AddCommandRequest::CheckCommandName received"
                   " command name already exist");
      return false;
    }
  }
  return true;
}

bool AddCommandRequest::CheckCommandVRSynonym(const Application* app) {
  if (NULL == app) {
    return false;
  }

  const CommandsMap& commands = app->commands_map();
  CommandsMap::const_iterator it = commands.begin();

  for (; commands.end() != it; ++it) {
    for (size_t i = 0; i < (*it->second)[strings::vr_commands].length(); ++i) {
      for (size_t j = 0;
          j < (*message_)[strings::msg_params][strings::vr_commands].length();
          ++j) {
        std::string vr_cmd_i =
            (*it->second)[strings::vr_commands][i].asString();
        std::string vr_cmd_j =
            (*message_)[strings::msg_params][strings::vr_commands][j].asString();

        if (0 == strcasecmp(vr_cmd_i.c_str(), vr_cmd_j.c_str())) {
          LOG4CXX_INFO(logger_, "AddCommandRequest::CheckCommandVRSynonym"
                       " received command vr synonym already exist");
          return false;
        }
      }
    }
  }
  return true;
}

bool AddCommandRequest::CheckCommandParentId(const Application* app) {
  if (NULL == app) {
    return false;
  }

  const int parent_id =
      (*message_)[strings::msg_params][strings::menu_params][hmi_request::parent_id]
          .asInt();
  smart_objects::SmartObject* parent = app->FindSubMenu(parent_id);

  if (!parent) {
    LOG4CXX_INFO(logger_, "AddCommandRequest::CheckCommandParentId received"
                 " submenu doesn't exist");
    return false;
  }
  return true;
}

}  // namespace commands

}  // namespace application_manager
