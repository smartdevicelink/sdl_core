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

#include "can_cooperation/commands/on_interior_vehicle_data_notification.h"
#include <algorithm>
#include <vector>
#include "json/json.h"
#include "utils/make_shared.h"
#include "can_cooperation/can_module_constants.h"
#include "can_cooperation/message_helper.h"
#include "can_cooperation/can_module.h"
#include "can_cooperation/can_app_extension.h"

namespace can_cooperation {

namespace commands {

CREATE_LOGGERPTR_GLOBAL(logger_, "OnInteriorVehicleDataNotification")

OnInteriorVehicleDataNotification::OnInteriorVehicleDataNotification(
    const application_manager::MessagePtr& message,
    CANModuleInterface& can_module)
    : BaseCommandNotification(message, can_module) {}

OnInteriorVehicleDataNotification::~OnInteriorVehicleDataNotification() {}

void OnInteriorVehicleDataNotification::Execute() {
  LOG4CXX_AUTO_TRACE(logger_);

  Json::Value json;

  application_manager::MessagePtr msg = message();

  json = MessageHelper::StringToValue(msg->json_message());

  Json::Value module_type = ModuleType(json);

  typedef std::vector<application_manager::ApplicationSharedPtr> AppPtrs;
  AppPtrs apps = service_->GetApplications(can_module_.GetModuleID());
  LOG4CXX_TRACE(logger_, "Process notification for " << module_type.asString());

  for (AppPtrs::iterator it = apps.begin(); it != apps.end(); ++it) {
    DCHECK(*it);
    application_manager::Application& app = **it;

    CANAppExtensionPtr extension =
        application_manager::AppExtensionPtr::static_pointer_cast<
            CANAppExtension>(app.QueryInterface(can_module_.GetModuleID()));
    DCHECK(extension);
    LOG4CXX_TRACE(logger_, "Check subscription for " << app.app_id());
    if (extension->IsSubscibedToInteriorVehicleData(module_type)) {
      application_manager::MessagePtr message =
          utils::MakeShared<application_manager::Message>(*msg);
      message->set_message_type(
          application_manager::MessageType::kNotification);
      message->set_protocol_version(application_manager::kV3);
      message->set_function_id(functional_modules::ON_INTERIOR_VEHICLE_DATA);
      message->set_function_name(MessageHelper::GetMobileAPIName(
          functional_modules::ON_INTERIOR_VEHICLE_DATA));
      message->set_connection_key(app.app_id());
      NotifyOneApplication(message);
    }
  }
}

std::string OnInteriorVehicleDataNotification::ModuleType(
    const Json::Value& message) {
  const Json::Value& module_data =
      message.get(message_params::kModuleData, Json::Value(Json::objectValue));
  return module_data.get(message_params::kModuleType, "").asString();
}

}  // namespace commands

}  // namespace can_cooperation
