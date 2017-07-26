/*

 Copyright (c) 2017, Ford Motor Company
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

#include "application_manager/commands/mobile/add_sub_menu_request.h"

#include "application_manager/message_helper.h"
#include "application_manager/application.h"
#include "utils/helpers.h"
#include "utils/file_system.h"

namespace application_manager {

namespace commands {

AddSubMenuRequest::AddSubMenuRequest(const MessageSharedPtr& message,
                                     ApplicationManager& application_manager)
    : CommandRequestImpl(message, application_manager) {}

AddSubMenuRequest::~AddSubMenuRequest() {}

void AddSubMenuRequest::Run() {
  LOG4CXX_AUTO_TRACE(logger_);

  ApplicationSharedPtr app = application_manager_.application(connection_key());

  if (!app) {
    LOG4CXX_ERROR(logger_, "NULL pointer");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return;
  }

  const int32_t menu_id =
      (*message_)[strings::msg_params][strings::menu_id].asInt();
  if (app->FindSubMenu(menu_id)) {
    LOG4CXX_ERROR(logger_, "Menu with id " << menu_id << " is not found.");
    SendResponse(false, mobile_apis::Result::INVALID_ID);
    return;
  }

  const std::string& menu_name =
      (*message_)[strings::msg_params][strings::menu_name].asString();

  if (app->IsSubMenuNameAlreadyExist(menu_name)) {
    LOG4CXX_ERROR(logger_, "Menu name " << menu_name << " is duplicated.");
    SendResponse(false, mobile_apis::Result::DUPLICATE_NAME);
    return;
  }

  if (!CheckSubMenuName()) {
    LOG4CXX_ERROR(logger_, "Sub-menu name is not valid.");
    SendResponse(false, mobile_apis::Result::INVALID_DATA);
    return;
  }

  smart_objects::SmartObject msg_params =
      smart_objects::SmartObject(smart_objects::SmartType_Map);

  msg_params[strings::menu_id] =
      (*message_)[strings::msg_params][strings::menu_id];
  if ((*message_)[strings::msg_params].keyExists(strings::position)) {
    msg_params[strings::menu_params][strings::position] =
        (*message_)[strings::msg_params][strings::position];
  }
  msg_params[strings::menu_params][strings::menu_name] =
      (*message_)[strings::msg_params][strings::menu_name];
  msg_params[strings::app_id] = app->app_id();
  const bool is_key_icon_exist =
      ((*message_)[strings::msg_params].keyExists(strings::sub_menu_icon));
  const bool is_icon_path_valid = CheckSubMenuIcon();
  const uint32_t image_type =
      (*message_)[strings::msg_params][strings::sub_menu_icon]
                 [strings::image_type].asUInt();
  const mobile_apis::ImageType::eType type =
      static_cast<mobile_apis::ImageType::eType>(image_type);

  if (is_key_icon_exist && is_icon_path_valid) {
    msg_params[strings::sub_menu_icon] =
        (*message_)[strings::msg_params][strings::sub_menu_icon];
    const bool is_dynamic_image = mobile_apis::ImageType::DYNAMIC == type;
    if (is_dynamic_image) {
      msg_params[strings::sub_menu_icon][strings::value] =
          ImageFullPath((*message_)[strings::msg_params][strings::sub_menu_icon]
                                   [strings::value].asString(),
                        app,
                        application_manager_);
    }
  }

  if (is_key_icon_exist && !is_icon_path_valid) {
    LOG4CXX_ERROR(logger_, "Sub-menu icon is not valid.");
    SendResponse(false, mobile_apis::Result::INVALID_DATA);
  } else {  // no matter, exists key_icon or not
    SendHMIRequest(hmi_apis::FunctionID::UI_AddSubMenu, &msg_params, true);
  }
}

std::string AddSubMenuRequest::ImageFullPath(const std::string& file_name,
                                             ApplicationConstSharedPtr app,
                                             ApplicationManager& app_mngr) {
  std::string full_file_path;
  const std::string& app_storage_folder =
      app_mngr.get_settings().app_storage_folder() + "/";
  full_file_path = app_storage_folder;
  full_file_path += app->folder_name();
  full_file_path += "/";
  full_file_path += file_name;
  return full_file_path;
}

void AddSubMenuRequest::on_event(const event_engine::Event& event) {
  LOG4CXX_AUTO_TRACE(logger_);
  const smart_objects::SmartObject& message = event.smart_object();

  switch (event.id()) {
    case hmi_apis::FunctionID::UI_AddSubMenu: {
      hmi_apis::Common_Result::eType result_code =
          static_cast<hmi_apis::Common_Result::eType>(
              message[strings::params][hmi_response::code].asInt());
      std::string response_info;
      GetInfo(message, response_info);
      const bool result = PrepareResultForMobileResponse(
          result_code, HmiInterfaces::HMI_INTERFACE_UI);

      ApplicationSharedPtr application =
          application_manager_.application(connection_key());

      if (!application) {
        LOG4CXX_ERROR(logger_, "NULL pointer");
        return;
      }

      if (result) {
        application->AddSubMenu(
            (*message_)[strings::msg_params][strings::menu_id].asInt(),
            (*message_)[strings::msg_params]);
        if (!CheckMenuIconExistedInStorage()) {
          response_info = "Reference image(s) not found";
        }
      }
      SendResponse(result,
                   MessageHelper::HMIToMobileResult(result_code),
                   response_info.empty() ? NULL : response_info.c_str(),
                   &(message[strings::msg_params]));
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

bool AddSubMenuRequest::CheckSubMenuName() {
  LOG4CXX_AUTO_TRACE(logger_);
  const char* str = NULL;

  str = (*message_)[strings::msg_params][strings::menu_name].asCharArray();
  if (!CheckSyntax(str)) {
    LOG4CXX_INFO(logger_, "Invalid subMenu name.");
    return false;
  }
  return true;
}

bool AddSubMenuRequest::CheckSubMenuIcon() {
  if (!(*message_)[strings::msg_params].keyExists(strings::sub_menu_icon)) {
    return false;
  }
  const std::string str =
      (*message_)[strings::msg_params][strings::sub_menu_icon][strings::value]
          .asString();
  if (!CheckSyntax(str) || std::string::npos != str.find(" ")) {
    LOG4CXX_ERROR(logger_, "Invalid menu icon syntax check failed.");
    return false;
  }

  return true;
}

bool AddSubMenuRequest::CheckMenuIconExistedInStorage() {
  if (!(*message_)[strings::msg_params].keyExists(strings::sub_menu_icon)) {
    return true;
  }
  std::string full_file_path =
      application_manager_.get_settings().app_storage_folder() + "/";

  full_file_path +=
      (*message_)[strings::msg_params][strings::sub_menu_icon][strings::value]
          .asString();

  if (!file_system::FileExists(full_file_path)) {
    return false;
  }
  return true;
}

}  // namespace commands

}  // namespace application_manager
