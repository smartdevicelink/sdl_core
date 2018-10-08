/*
 Copyright (c) 2018, Ford Motor Company
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following
 disclaimer in the documentation and/or other materials provided with the
 distribution.

 Neither the name of the copyright holders nor the names of their contributors
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

#include "rc_rpc_plugin/commands/mobile/on_interior_vehicle_data_notification.h"
#include "rc_rpc_plugin/rc_rpc_plugin.h"
#include "rc_rpc_plugin/rc_module_constants.h"
#include "rc_rpc_plugin/rc_helpers.h"
#include "smart_objects/enum_schema_item.h"
#include "utils/macro.h"

namespace rc_rpc_plugin {
namespace commands {

CREATE_LOGGERPTR_GLOBAL(logger_, "RemoteControlModule")

OnInteriorVehicleDataNotification::OnInteriorVehicleDataNotification(
    const app_mngr::commands::MessageSharedPtr& message,
    const RCCommandParams& params)
    : app_mngr::commands::CommandNotificationImpl(message,
                                                  params.application_manager_,
                                                  params.rpc_service_,
                                                  params.hmi_capabilities_,
                                                  params.policy_handler_)
    , interior_data_cache_(params.interior_data_cache_) {}

OnInteriorVehicleDataNotification::~OnInteriorVehicleDataNotification() {}

void OnInteriorVehicleDataNotification::AddDataToCache(
    const std::string& module_type) {
  const auto& data_mapping = RCHelpers::GetModuleTypeToDataMapping();
  const auto module_data =
      (*message_)[app_mngr::strings::msg_params][message_params::kModuleData]
                 [data_mapping(module_type)];
  interior_data_cache_.Add(module_type, module_data);
}

void OnInteriorVehicleDataNotification::Run() {
  LOG4CXX_AUTO_TRACE(logger_);

  const std::string module_type = ModuleType();
  auto apps_subscribed =
      RCHelpers::AppsSubscribedToModuleType(application_manager_, module_type);
  if (!apps_subscribed.empty()) {
    AddDataToCache(module_type);
  }
  typedef std::vector<application_manager::ApplicationSharedPtr> AppPtrs;
  AppPtrs apps = RCRPCPlugin::GetRCApplications(application_manager_);

  for (AppPtrs::iterator it = apps.begin(); it != apps.end(); ++it) {
    DCHECK(*it);
    application_manager::Application& app = **it;

    const auto extension = RCHelpers::GetRCExtension(app);
    DCHECK(extension);
    LOG4CXX_TRACE(logger_,
                  "Check subscription for "
                      << app.app_id() << "and module type " << module_type);
    if (extension->IsSubscibedToInteriorVehicleData(module_type)) {
      (*message_)[app_mngr::strings::params]
                 [app_mngr::strings::connection_key] = app.app_id();
      SendNotification();
    }
  }
}

std::string OnInteriorVehicleDataNotification::ModuleType() {
  mobile_apis::ModuleType::eType module_type =
      static_cast<mobile_apis::ModuleType::eType>(
          (*message_)[app_mngr::strings::msg_params]
                     [message_params::kModuleData][message_params::kModuleType]
                         .asUInt());
  const char* str;
  const bool ok = ns_smart_device_link::ns_smart_objects::EnumConversionHelper<
      mobile_apis::ModuleType::eType>::EnumToCString(module_type, &str);
  return ok ? str : "unknown";
}

}  // namespace commands
}  // namespace rc_rpc_plugin
