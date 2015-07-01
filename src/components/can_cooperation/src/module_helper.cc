/*
 * Copyright (c) 2013, Ford Motor Company
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

#include "can_cooperation/module_helper.h"
#include "can_cooperation/can_module.h"
#include "can_cooperation/can_module_constants.h"
#include "can_cooperation/can_app_extension.h"
#include "application_manager/message.h"
#include "interfaces/HMI_API.h"

namespace can_cooperation {

using functional_modules::ProcessResult;
using application_manager::AppExtensionPtr;

application_manager::MessagePtr ResponseToHMI(unsigned int id,
                              hmi_apis::Common_Result::eType result_code,
                              const std::string& method_name) {
  Json::Value msg;
  msg[json_keys::kId] = id;
  msg[json_keys::kJsonrpc] = "2.0";

  if (hmi_apis::Common_Result::eType::SUCCESS == result_code) {
    msg[json_keys::kResult] = Json::Value(Json::ValueType::objectValue);
    msg[json_keys::kResult][json_keys::kCode] = result_code;
    msg[json_keys::kResult][json_keys::kMethod] = method_name;
  } else {
    msg[json_keys::kError] = Json::Value(Json::ValueType::objectValue);
    msg[json_keys::kError][json_keys::kCode] = result_code;
    msg[json_keys::kError][json_keys::kData] = Json::Value(
      Json::ValueType::objectValue);
    msg[json_keys::kError][json_keys::kData][json_keys::kMethod] = method_name;
  }

  application_manager::MessagePtr message(
    new application_manager::Message(
      protocol_handler::MessagePriority::kDefault));
  message->set_protocol_version(
    application_manager::ProtocolVersion::kHMI);
  message->set_correlation_id(msg[json_keys::kId].asInt());
  Json::FastWriter writer;
  std::string json_msg = writer.write(msg);
  message->set_json_message(json_msg);
  message->set_message_type(
    application_manager::MessageType::kResponse);
  return message;
}

ProcessResult ProcessOnAppDeactivation(
  const Json::Value& value) {
  if (value.isMember(json_keys::kParams)
      && value[json_keys::kParams].isMember(message_params::kHMIAppID)) {
    uint32_t hmi_app_id =
      value[json_keys::kParams][message_params::kHMIAppID].asUInt();
    typedef std::vector<application_manager::ApplicationSharedPtr> AppList;
    AppList applications = CANModule::instance()->service()->GetApplications(
                             CANModule::instance()->GetModuleID());
    if (!applications.empty()) {
      application_manager::ApplicationSharedPtr current_app;
      application_manager::ApplicationSharedPtr active_app;

      for (size_t i = 0; i < applications.size(); ++i) {
        if (applications[i]->hmi_app_id() == hmi_app_id) {
          current_app = applications[i];
        }
        if (applications[i]->IsFullscreen()) {
          active_app = applications[i];
        }
      }

      if (current_app && current_app == active_app) {
        CANModule::instance()->service()->ChangeNotifyHMILevel(
          current_app, mobile_apis::HMILevel::eType::HMI_LIMITED);
        return ProcessResult::PROCESSED;
      }

      return ProcessResult::PARTIALLY_PROCESSED;
    }
  }
  return ProcessResult::CANNOT_PROCESS;
}

functional_modules::ProcessResult ProcessSDLActivateApp(
  const Json::Value& value) {
  if (value.isMember(json_keys::kParams)
      && value[json_keys::kParams].isMember(message_params::kHMIAppID)) {
    uint32_t hmi_app_id =
      value[json_keys::kParams][message_params::kHMIAppID].asUInt();
    typedef std::vector<application_manager::ApplicationSharedPtr> AppList;
    AppList applications = CANModule::instance()->service()->GetApplications(
                             CANModule::instance()->GetModuleID());
    if (!applications.empty()) {
      application_manager::ApplicationSharedPtr new_app;
      application_manager::ApplicationSharedPtr active_app;
      CANAppExtensionPtr new_app_ext;
      AppList limited_apps;
      for (size_t i = 0; i < applications.size(); ++i) {
        if (applications[i]->IsFullscreen()) {
          active_app = applications[i];
        }
        if (applications[i]->hmi_app_id() == hmi_app_id) {
          new_app = applications[i];
          new_app_ext = AppExtensionPtr::static_pointer_cast<CANAppExtension>(
            new_app->QueryInterface(CANModule::instance()->GetModuleID()));
          DCHECK(new_app_ext);
          if (!new_app_ext) {
            return ProcessResult::CANNOT_PROCESS;
          }
          if (!new_app_ext->is_on_driver_device()) {
            // Do not change HMI Level of app on passenger's device
            application_manager::MessagePtr msg = ResponseToHMI(
              value[json_keys::kId].asUInt(),
              hmi_apis::Common_Result::GENERIC_ERROR,
              functional_modules::hmi_api::sdl_activate_app);

            CANModule::instance()->service()->SendMessageToHMI(msg);

            return ProcessResult::PROCESSED;
          }
        } else {
          CANAppExtensionPtr app_ext =
            AppExtensionPtr::static_pointer_cast<CANAppExtension>(
              applications[i]->QueryInterface(
                CANModule::instance()->GetModuleID()));
          DCHECK(app_ext);
          if (!app_ext) {
            continue;
          }
          if (applications[i]->hmi_level() == mobile_apis::HMILevel::HMI_LIMITED
              && app_ext->is_on_driver_device()) {
            limited_apps.push_back(applications[i]);
          }
        }
      }
      if (active_app == new_app) {
        return ProcessResult::CANNOT_PROCESS;
      }

      if (new_app_ext->is_on_driver_device()) {
        for (size_t i = 0; i < limited_apps.size(); ++i) {
          CANModule::instance()->service()->ChangeNotifyHMILevel(
            limited_apps[i], mobile_apis::HMILevel::eType::HMI_BACKGROUND);
        }
      }
      return ProcessResult::PARTIALLY_PROCESSED;
    }
  }
  return ProcessResult::CANNOT_PROCESS;
}

}  //  namespace can_cooperation
