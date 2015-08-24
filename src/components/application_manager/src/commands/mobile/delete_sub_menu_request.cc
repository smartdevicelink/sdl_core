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

#include "application_manager/commands/mobile/delete_sub_menu_request.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/application_impl.h"
#include "interfaces/HMI_API.h"
#include "utils/helpers.h"

namespace application_manager {

namespace commands {

DeleteSubMenuRequest::DeleteSubMenuRequest(const MessageSharedPtr& message)
    : CommandRequestImpl(message) {
}

DeleteSubMenuRequest::~DeleteSubMenuRequest() {
}

void DeleteSubMenuRequest::Run() {
  LOG4CXX_AUTO_TRACE(logger_);

  ApplicationSharedPtr app =
      ApplicationManagerImpl::instance()->application(connection_key());

  if (!app) {
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    LOG4CXX_ERROR(logger_, "Application is not registered");
    return;
  }

  const int32_t menu_id =
      (*message_)[strings::msg_params][strings::menu_id].asInt();

  if (!app->FindSubMenu(menu_id)) {
    LOG4CXX_ERROR(logger_, "Menu with id " << menu_id << " is not found.");
    SendResponse(false, mobile_apis::Result::INVALID_ID);
    return;
  }

  smart_objects::SmartObject msg_params = smart_objects::SmartObject(
      smart_objects::SmartType_Map);

  msg_params[strings::menu_id] =
      (*message_)[strings::msg_params][strings::menu_id];
  msg_params[strings::app_id] = app->app_id();

  SendHMIRequest(hmi_apis::FunctionID::UI_DeleteSubMenu, &msg_params, true);
}

void DeleteSubMenuRequest::DeleteSubMenuVRCommands(
    ApplicationConstSharedPtr app) {
  LOG4CXX_AUTO_TRACE(logger_);

  const DataAccessor<CommandsMap> accessor = app->commands_map();
  const CommandsMap& commands = accessor.GetData();
  CommandsMap::const_iterator it = commands.begin();

  for (; commands.end() != it; ++it) {

    if (!(*it->second).keyExists(strings::vr_commands)) {
      continue;
    }

    if ((*message_)[strings::msg_params][strings::menu_id].asInt()
        == (*it->second)[strings::menu_params]
                         [hmi_request::parent_id].asInt()) {
      smart_objects::SmartObject msg_params = smart_objects::SmartObject(
          smart_objects::SmartType_Map);
      msg_params[strings::cmd_id] = (*it->second)[strings::cmd_id].asInt();
      msg_params[strings::app_id] = app->app_id();
      msg_params[strings::grammar_id] = app->get_grammar_id();
      msg_params[strings::type] = hmi_apis::Common_VRCommandType::Command;

      SendHMIRequest(hmi_apis::FunctionID::VR_DeleteCommand, &msg_params);
    }
  }
}

void DeleteSubMenuRequest::DeleteSubMenuUICommands(
    ApplicationSharedPtr const app) {
  LOG4CXX_AUTO_TRACE(logger_);

  const DataAccessor<CommandsMap> accessor(app->commands_map());
  const CommandsMap& commands = accessor.GetData();
  CommandsMap::const_iterator it = commands.begin();

  while (commands.end() != it) {
    if (!(*it->second).keyExists(strings::menu_params)) {
      LOG4CXX_ERROR(logger_, "menu_params not exist");
      ++it;
      continue;
    }

    if ((*message_)[strings::msg_params][strings::menu_id].asInt()
        == (*it->second)[strings::menu_params]
                         [hmi_request::parent_id].asInt()) {
      smart_objects::SmartObject msg_params = smart_objects::SmartObject(
          smart_objects::SmartType_Map);
      msg_params[strings::app_id] = app->app_id();
      msg_params[strings::cmd_id] = (*it->second)[strings::cmd_id].asInt();
      app->RemoveCommand((*it->second)[strings::cmd_id].asInt());
      it = commands.begin();  // Can not relay on
                              // iterators after erase was called

      SendHMIRequest(hmi_apis::FunctionID::UI_DeleteCommand, &msg_params);
    } else {
      ++it;
    }
  }
}

void DeleteSubMenuRequest::on_event(const event_engine::Event& event) {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace helpers;
  const smart_objects::SmartObject& message = event.smart_object();

  switch (event.id()) {
    case hmi_apis::FunctionID::UI_DeleteSubMenu: {
      mobile_apis::Result::eType result_code =
          static_cast<mobile_apis::Result::eType>(
              message[strings::params][hmi_response::code].asInt());

      const bool result =
          Compare<mobile_api::Result::eType, EQ, ONE>(
            result_code,
            mobile_api::Result::SUCCESS,
            mobile_api::Result::WARNINGS);

      ApplicationSharedPtr application =
             ApplicationManagerImpl::instance()->application(connection_key());

      if (!application) {
        LOG4CXX_ERROR(logger_, "NULL pointer");
        return;
      }

      if (result) {
        // delete sub menu items from SDL and HMI
        DeleteSubMenuVRCommands(application);
        DeleteSubMenuUICommands(application);
        application->RemoveSubMenu(
            (*message_)[strings::msg_params][strings::menu_id].asInt());
       }

      SendResponse(result, result_code, NULL, &(message[strings::msg_params]));
      if (result) {
        application->UpdateHash();
      }
      break;
    }
    default: {
      LOG4CXX_ERROR(logger_, "Received unknown event" << event.id());
      return;
    }
  }
}


}  // namespace commands

}  // namespace application_manager
