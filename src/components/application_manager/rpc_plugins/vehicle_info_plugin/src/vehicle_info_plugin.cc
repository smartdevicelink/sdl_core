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
#include "vehicle_info_plugin/vehicle_info_command_factory.h"
#include "vehicle_info_plugin/vehicle_info_app_extension.h"
#include "application_manager/smart_object_keys.h"
#include "application_manager/message_helper.h"
#include "application_manager/message_helper.h"

namespace vehicle_info_plugin {
CREATE_LOGGERPTR_GLOBAL(logger_, "VehicleInfoPlugin")

namespace strings = application_manager::strings;

VehicleInfoPlugin::VehicleInfoPlugin() : application_manager_(nullptr) {}

bool VehicleInfoPlugin::Init(
    application_manager::ApplicationManager& app_manager,
    application_manager::rpc_service::RPCService& rpc_service,
    application_manager::HMICapabilities& hmi_capabilities,
    policy::PolicyHandlerInterface& policy_handler) {
  application_manager_ = &app_manager;
  command_factory_.reset(new vehicle_info_plugin::VehicleInfoCommandFactory(
      app_manager, rpc_service, hmi_capabilities, policy_handler));
  return true;
}

bool VehicleInfoPlugin::IsAbleToProcess(
    const int32_t function_id, const commands::Command::CommandSource source) {
  return command_factory_->IsAbleToProcess(function_id, source);
}

std::string VehicleInfoPlugin::PluginName() {
  return "Vehicle Info Plugin";
}

app_mngr::CommandFactory& VehicleInfoPlugin::GetCommandFactory() {
  return *command_factory_;
}

void VehicleInfoPlugin::OnPolicyEvent(plugins::PolicyEvent event) {}

void VehicleInfoPlugin::OnApplicationEvent(
    plugins::ApplicationEvent event,
    app_mngr::ApplicationSharedPtr application) {
  if (plugins::ApplicationEvent::kApplicationRegistered == event) {
    application->AddExtension(
        std::make_shared<VehicleInfoAppExtension>(*this, *application));
  }
}

void VehicleInfoPlugin::ProcessResumptionSubscription(
    application_manager::Application& app, VehicleInfoAppExtension& ext) {
  LOG4CXX_AUTO_TRACE(logger_);
  smart_objects::SmartObject msg_params =
      smart_objects::SmartObject(smart_objects::SmartType_Map);
  msg_params[strings::app_id] = app.app_id();
  const auto& subscriptions = ext.Subscriptions();
  for (auto& ivi_data : application_manager::MessageHelper::vehicle_data()) {
    mobile_apis::VehicleDataType::eType type_id = ivi_data.second;
    if (subscriptions.end() != subscriptions.find(type_id)) {
      std::string key_name = ivi_data.first;
      msg_params[key_name] = true;
    }
  }
  smart_objects::SmartObjectSPtr request =
      application_manager::MessageHelper::CreateModuleInfoSO(
          hmi_apis::FunctionID::VehicleInfo_SubscribeVehicleData,
          *application_manager_);
  (*request)[strings::msg_params] = msg_params;
  application_manager_->GetRPCService().ManageHMICommand(request);
}

application_manager::ApplicationSharedPtr FindAppSubscribedToIVI(
    mobile_apis::VehicleDataType::eType ivi_data,
    application_manager::ApplicationManager& app_mngr) {
  auto applications = app_mngr.applications();

  for (auto& app : applications.GetData()) {
    auto& ext = VehicleInfoAppExtension::ExtractVIExtension(*app);
    if (ext.isSubscribedToVehicleInfo(ivi_data)) {
      return app;
    }
  }
  return application_manager::ApplicationSharedPtr();
}

smart_objects::SmartObjectSPtr GetUnsubscribeIVIRequest(
    int32_t ivi_id, application_manager::ApplicationManager& app_mngr) {
  using namespace smart_objects;

  auto find_ivi_name = [ivi_id]() {
    for (auto item : application_manager::MessageHelper::vehicle_data()) {
      if (ivi_id == item.second) {
        return item.first;
      }
    }
    return std::string();
  };
  std::string key_name = find_ivi_name();
  DCHECK_OR_RETURN(!key_name.empty(), smart_objects::SmartObjectSPtr());
  auto msg_params = smart_objects::SmartObject(smart_objects::SmartType_Map);
  msg_params[key_name] = true;

  auto message = application_manager::MessageHelper::CreateMessageForHMI(
      hmi_apis::messageType::request, app_mngr.GetNextHMICorrelationID());
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
  for (auto& ivi : subscriptions) {
    ext.unsubscribeFromVehicleInfo(ivi);
    auto still_subscribed_app =
        FindAppSubscribedToIVI(ivi, *application_manager_);
    if (!still_subscribed_app) {
      auto message = GetUnsubscribeIVIRequest(ivi, *application_manager_);
      application_manager_->GetRPCService().ManageHMICommand(message);
    }
  }
}
}

extern "C" application_manager::plugin_manager::RPCPlugin* Create() {
  return new vehicle_info_plugin::VehicleInfoPlugin();
}
