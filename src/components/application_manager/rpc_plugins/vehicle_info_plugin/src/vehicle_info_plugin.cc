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

#include "vehicle_info_plugin/vehicle_info_plugin.h"
#include "application_manager/message_helper.h"
#include "application_manager/plugin_manager/plugin_keys.h"
#include "application_manager/rpc_handler.h"
#include "application_manager/smart_object_keys.h"
#include "vehicle_info_plugin/custom_vehicle_data_manager_impl.h"
#include "vehicle_info_plugin/vehicle_info_app_extension.h"
#include "vehicle_info_plugin/vehicle_info_command_factory.h"

namespace vehicle_info_plugin {
CREATE_LOGGERPTR_GLOBAL(logger_, "VehicleInfoPlugin")

namespace strings = application_manager::strings;
namespace plugins = application_manager::plugin_manager;
namespace commands = application_manager::commands;

VehicleInfoPlugin::VehicleInfoPlugin() : application_manager_(nullptr) {}

bool VehicleInfoPlugin::Init(
    application_manager::ApplicationManager& app_manager,
    application_manager::rpc_service::RPCService& rpc_service,
    application_manager::HMICapabilities& hmi_capabilities,
    policy::PolicyHandlerInterface& policy_handler,
    resumption::LastState& last_state) {
  UNUSED(last_state);
  application_manager_ = &app_manager;
  custom_vehicle_data_manager_.reset(
      new CustomVehicleDataManagerImpl(policy_handler, rpc_service));
  command_factory_.reset(new vehicle_info_plugin::VehicleInfoCommandFactory(
      app_manager,
      rpc_service,
      hmi_capabilities,
      policy_handler,
      *(custom_vehicle_data_manager_.get())));
  return true;
}

bool VehicleInfoPlugin::IsAbleToProcess(
    const int32_t function_id, const commands::Command::CommandSource source) {
  return command_factory_->IsAbleToProcess(function_id, source);
}

std::string VehicleInfoPlugin::PluginName() {
  return plugins::plugin_names::vehicle_info_rpc_plugin;
}

app_mngr::CommandFactory& VehicleInfoPlugin::GetCommandFactory() {
  return *command_factory_;
}

void VehicleInfoPlugin::OnPolicyEvent(plugins::PolicyEvent event) {
  custom_vehicle_data_manager_->OnPolicyEvent(event);
}

void VehicleInfoPlugin::OnApplicationEvent(
    plugins::ApplicationEvent event,
    app_mngr::ApplicationSharedPtr application) {
  if (plugins::ApplicationEvent::kApplicationRegistered == event) {
    application->AddExtension(
        std::make_shared<VehicleInfoAppExtension>(*this, *application));
  } else if ((plugins::ApplicationEvent::kDeleteApplicationData == event) ||
             (plugins::ApplicationEvent::kApplicationUnregistered == event)) {
    DeleteSubscriptions(application);
  }
}

void VehicleInfoPlugin::ProcessResumptionSubscription(
    application_manager::Application& app, VehicleInfoAppExtension& ext) {
  LOG4CXX_AUTO_TRACE(logger_);
  smart_objects::SmartObject msg_params =
      smart_objects::SmartObject(smart_objects::SmartType_Map);
  msg_params[strings::app_id] = app.app_id();
  const auto& subscriptions = ext.Subscriptions();
  for (const auto& item : subscriptions) {
    msg_params[item] = true;
  }
  smart_objects::SmartObjectSPtr request =
      application_manager::MessageHelper::CreateModuleInfoSO(
          hmi_apis::FunctionID::VehicleInfo_SubscribeVehicleData,
          *application_manager_);
  (*request)[strings::msg_params] = msg_params;
  application_manager_->GetRPCService().ManageHMICommand(request);
}

application_manager::ApplicationSharedPtr FindAppSubscribedToIVI(
    const std::string& ivi_name,
    application_manager::ApplicationManager& app_mngr) {
  auto applications = app_mngr.applications();

  for (auto& app : applications.GetData()) {
    auto& ext = VehicleInfoAppExtension::ExtractVIExtension(*app);
    if (ext.isSubscribedToVehicleInfo(ivi_name)) {
      return app;
    }
  }
  return application_manager::ApplicationSharedPtr();
}

smart_objects::SmartObjectSPtr VehicleInfoPlugin::GetUnsubscribeIVIRequest(
    const std::vector<std::string>& ivi_names) {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace smart_objects;

  auto msg_params = smart_objects::SmartObject(smart_objects::SmartType_Map);

  auto find_ivi_name = [](const std::string& ivi_name) {
    for (auto item : application_manager::MessageHelper::vehicle_data()) {
      if (ivi_name == item.first) {
        return item.first;
      }
    }
    return std::string();
  };

  for (const auto& ivi_name : ivi_names) {
    // try to find the name in vehicle data types
    std::string key_name = find_ivi_name(ivi_name);

    if (key_name.empty()) {
      // the name hasn't been found in vehicle data types
      if (custom_vehicle_data_manager_->IsValidCustomVehicleDataName(
              ivi_name)) {
        key_name = ivi_name;
      }
    }

    DCHECK_OR_RETURN(!key_name.empty(), smart_objects::SmartObjectSPtr());
    msg_params[key_name] = true;
  }

  auto message = application_manager::MessageHelper::CreateMessageForHMI(
      hmi_apis::messageType::request,
      application_manager_->GetNextHMICorrelationID());
  DCHECK(message);

  SmartObject& object = *message;
  object[strings::params][strings::function_id] =
      hmi_apis::FunctionID::VehicleInfo_UnsubscribeVehicleData;

  object[strings::msg_params] = msg_params;
  return message;
}

void VehicleInfoPlugin::DeleteSubscriptions(
    application_manager::ApplicationSharedPtr app) {
  auto& ext = VehicleInfoAppExtension::ExtractVIExtension(*app);
  auto subscriptions = ext.Subscriptions();
  std::vector<std::string> ivi_to_unsubscribe;
  for (auto& ivi : subscriptions) {
    ext.unsubscribeFromVehicleInfo(ivi);
    auto still_subscribed_app =
        FindAppSubscribedToIVI(ivi, *application_manager_);
    if (!still_subscribed_app) {
      ivi_to_unsubscribe.push_back(ivi);
    }
  }

  if (!ivi_to_unsubscribe.empty()) {
    auto message = GetUnsubscribeIVIRequest(ivi_to_unsubscribe);
    application_manager_->GetRPCService().ManageHMICommand(message);
  }
}
}  // namespace vehicle_info_plugin

extern "C" __attribute__((visibility("default")))
application_manager::plugin_manager::RPCPlugin*
Create() {
  return new vehicle_info_plugin::VehicleInfoPlugin();
}

extern "C" __attribute__((visibility("default"))) void Delete(
    application_manager::plugin_manager::RPCPlugin* data) {
  delete data;
  DELETE_THREAD_LOGGER(vehicle_info_plugin::logger_);
}
