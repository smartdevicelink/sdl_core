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
#include "can_cooperation/message_helper.h"

namespace can_cooperation {

using functional_modules::ProcessResult;
using application_manager::AppExtensionPtr;

application_manager::MessagePtr ModuleHelper::ResponseToHMI(
    unsigned int id,
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
    msg[json_keys::kError][json_keys::kData] =
        Json::Value(Json::ValueType::objectValue);
    msg[json_keys::kError][json_keys::kData][json_keys::kMethod] = method_name;
  }

  application_manager::MessagePtr message(new application_manager::Message(
      protocol_handler::MessagePriority::kDefault));
  message->set_protocol_version(application_manager::ProtocolVersion::kHMI);
  message->set_correlation_id(msg[json_keys::kId].asInt());
  Json::FastWriter writer;
  std::string json_msg = writer.write(msg);
  message->set_json_message(json_msg);
  message->set_message_type(application_manager::MessageType::kResponse);
  return message;
}

// (TODO)VS: Replace this functions for separate OnReverseAppsDisallowed
// notiifcation
void ModuleHelper::ProccessOnReverseAppsDisallowed(
    CANModuleInterface& can_module) {
  std::vector<application_manager::ApplicationSharedPtr> applications =
      can_module.service()->GetApplications(can_module.GetModuleID());

  for (uint32_t i = 0; i < applications.size(); ++i) {
    application_manager::AppExtensionPtr app_extension =
        applications[i]->QueryInterface(can_module.GetModuleID());
    if (app_extension) {
      CANAppExtensionPtr can_app_extension =
          application_manager::AppExtensionPtr::static_pointer_cast<
              CANAppExtension>(app_extension);
      if (!can_app_extension->is_on_driver_device()) {
      }
    }
  }
}

}  //  namespace can_cooperation
