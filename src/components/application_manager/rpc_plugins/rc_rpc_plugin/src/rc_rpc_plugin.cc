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

#include "rc_rpc_plugin/rc_rpc_plugin.h"
#include <memory>
#include "rc_rpc_plugin/rc_command_factory.h"
#include "rc_rpc_plugin/rc_app_extension.h"
#include "rc_rpc_plugin/resource_allocation_manager_impl.h"
#include "rc_rpc_plugin/interior_data_cache_impl.h"
#include "rc_rpc_plugin/interior_data_manager_impl.h"
#include "rc_rpc_plugin/rc_helpers.h"
#include "utils/helpers.h"
#include "application_manager/smart_object_keys.h"
#include "application_manager/message_helper.h"
#include "rc_rpc_plugin/rc_pending_resumption_handler.h"

namespace rc_rpc_plugin {
CREATE_LOGGERPTR_GLOBAL(logger_, "RemoteControlModule");

static const char* module_type_key = "moduleType";
static const char* intended_action_key = "subscribe";

namespace plugins = application_manager::plugin_manager;
namespace strings = application_manager::strings;

bool RCRPCPlugin::Init(
    application_manager::ApplicationManager& app_manager,
    application_manager::rpc_service::RPCService& rpc_service,
    application_manager::HMICapabilities& hmi_capabilities,
    policy::PolicyHandlerInterface& policy_handler) {
  interior_data_cache_.reset(new InteriorDataCacheImpl());
  interior_data_manager_.reset(new InteriorDataManagerImpl(
      *this, *interior_data_cache_, app_manager, rpc_service));

  resource_allocation_manager_.reset(
      new ResourceAllocationManagerImpl(app_manager, rpc_service));
  RCCommandParams params{app_manager,
                         rpc_service,
                         hmi_capabilities,
                         policy_handler,
                         *(resource_allocation_manager_.get()),
                         *(interior_data_cache_.get()),
                         *(interior_data_manager_.get())};
  command_factory_.reset(new rc_rpc_plugin::RCCommandFactory(params));
  rpc_service_ = &rpc_service;
  app_mngr_ = &app_manager;
  pending_resumption_handler_ = std::make_shared<RCPendingResumptionHandler>(
      app_manager, interior_data_cache_);
  return true;
}

bool RCRPCPlugin::IsAbleToProcess(
    const int32_t function_id,
    const application_manager::commands::Command::CommandSource
        message_source) {
  return command_factory_->IsAbleToProcess(function_id, message_source);
}

std::string RCRPCPlugin::PluginName() {
  return "RC RPC Plugin";
}

application_manager::CommandFactory& RCRPCPlugin::GetCommandFactory() {
  return *command_factory_;
}

void RCRPCPlugin::OnPolicyEvent(
    application_manager::plugin_manager::PolicyEvent event) {
  resource_allocation_manager_->OnPolicyEvent(event);
  interior_data_manager_->OnPolicyEvent(event);
}

void RCRPCPlugin::OnApplicationEvent(
    application_manager::plugin_manager::ApplicationEvent event,
    application_manager::ApplicationSharedPtr application) {
  if (!application->is_remote_control_supported()) {
    return;
  }
  switch (event) {
    case plugins::kApplicationRegistered: {
      application->AddExtension(std::shared_ptr<RCAppExtension>(
          new RCAppExtension(kRCPluginID, this, *application)));
      resource_allocation_manager_->SendOnRCStatusNotifications(
          NotificationTrigger::APP_REGISTRATION, application);
      break;
    }
    case plugins::kApplicationExit: {
      resource_allocation_manager_->OnApplicationEvent(event, application);
      interior_data_manager_->OnApplicationEvent(event, application);
      break;
    }
    case plugins::kApplicationUnregistered: {
      resource_allocation_manager_->OnApplicationEvent(event, application);
      interior_data_manager_->OnApplicationEvent(event, application);
      break;
    }
    default:
      break;
  }
}

void RCRPCPlugin::ProcessResumptionSubscription(
    application_manager::Application& app,
    RCAppExtension& ext,
    resumption::ResumptionHandlingCallbacks callbacks) {
  LOG4CXX_AUTO_TRACE(logger_);

  pending_resumption_handler_->HandleResumptionSubscriptionRequest(
      ext, app, callbacks);
}

void RCRPCPlugin::RevertResumption(
    application_manager::Application& app,
    const std::set<std::string>& list_of_subscriptions) {
  LOG4CXX_AUTO_TRACE(logger_);


  pending_resumption_handler_->ClearPendingResumptionRequests();

  const auto& mapping = RCHelpers::GetModuleTypeToEnumMapping();

  for (auto& module_type : list_of_subscriptions) {
    std::string module_type_str =
        mapping(static_cast<RCModuleTypeIDs>(std::stoi(module_type)));
    LOG4CXX_DEBUG(logger_, "Processing unsubscription of " << module_type_str);
    if (!IsSubscribedAppExist(module_type_str)) {
      LOG4CXX_DEBUG(logger_,
                    "Creating unsubscription request for " << module_type_str);
      interior_data_cache_->Remove(module_type_str);
      smart_objects::SmartObjectSPtr request =
          CreateUnsubscriptionRequest(module_type_str);
      app_mngr_->GetRPCService().ManageHMICommand(request);
    }
  }
}

bool RCRPCPlugin::IsSubscribedAppExist(const std::string& module_type) {
  LOG4CXX_AUTO_TRACE(logger_);
  auto applications = app_mngr_->applications();

  for (auto& app : applications.GetData()) {
    auto& ext = ExtractInteriorVehicleDataExtension(*app);
    if (ext.IsSubscibedToInteriorVehicleData(module_type)) {
      return true;
    }
  }
  return false;
}

RCAppExtension& RCRPCPlugin::ExtractInteriorVehicleDataExtension(
    application_manager::Application& app) {
  LOG4CXX_AUTO_TRACE(logger_);
  auto ext_ptr = app.QueryInterface(kRCPluginID);
  DCHECK(ext_ptr);
  DCHECK(dynamic_cast<RCAppExtension*>(ext_ptr.get()));
  auto rc_app_extension = std::static_pointer_cast<RCAppExtension>(ext_ptr);
  DCHECK(rc_app_extension);
  return *rc_app_extension;
}

smart_objects::SmartObjectSPtr RCRPCPlugin::CreateUnsubscriptionRequest(
    const std::string& module_type) {
  LOG4CXX_AUTO_TRACE(logger_);
  smart_objects::SmartObject msg_params =
      smart_objects::SmartObject(smart_objects::SmartType_Map);

  msg_params[module_type_key] = module_type;
  msg_params[intended_action_key] = false;
  smart_objects::SmartObjectSPtr request =
      application_manager::MessageHelper::CreateModuleInfoSO(
          hmi_apis::FunctionID::RC_GetInteriorVehicleData, *app_mngr_);
  (*request)[strings::msg_params] = msg_params;

  return request;
}

RCRPCPlugin::Apps RCRPCPlugin::GetRCApplications(
    application_manager::ApplicationManager& app_mngr) {
  using application_manager::ApplicationSharedPtr;
  using application_manager::ApplicationSet;
  ApplicationSet accessor = app_mngr.applications().GetData();

  std::vector<ApplicationSharedPtr> result;
  for (const auto& it : accessor) {
    if (it->is_remote_control_supported()) {
      result.push_back(it);
    }
  }
  return result;
}

}  // namespace rc_rpc_plugin

extern "C" application_manager::plugin_manager::RPCPlugin* Create() {
  return new rc_rpc_plugin::RCRPCPlugin();
}  // namespace rc_rpc_plugin
