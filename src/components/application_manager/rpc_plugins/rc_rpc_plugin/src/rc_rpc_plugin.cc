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
#include "rc_rpc_plugin/rc_command_factory.h"
#include "rc_rpc_plugin/rc_app_extension.h"
#include "rc_rpc_plugin/resource_allocation_manager_impl.h"
#include "rc_rpc_plugin/interior_data_cache_impl.h"
#include "rc_rpc_plugin/rc_helpers.h"
#include "utils/helpers.h"
#include "utils/date_time.h"

namespace rc_rpc_plugin {
CREATE_LOGGERPTR_GLOBAL(logger_, "RemoteControlModule");

namespace plugins = application_manager::plugin_manager;

bool RCRPCPlugin::Init(
    application_manager::ApplicationManager& app_manager,
    application_manager::rpc_service::RPCService& rpc_service,
    application_manager::HMICapabilities& hmi_capabilities,
    policy::PolicyHandlerInterface& policy_handler) {
  const auto& settings = app_manager.get_settings();
  const auto frequency = settings.get_interior_vehicle_data_frequency();
  interior_data_cache_.reset(new InteriorDataCacheImpl(
      frequency.second * date_time::DateTime::MILLISECONDS_IN_SECOND));

  resource_allocation_manager_.reset(
      new ResourceAllocationManagerImpl(app_manager, rpc_service));
  command_factory_.reset(
      new rc_rpc_plugin::RCCommandFactory(app_manager,
                                          rpc_service,
                                          hmi_capabilities,
                                          policy_handler,
                                          *resource_allocation_manager_,
                                          *interior_data_cache_));
  rpc_service_ = &rpc_service;
  app_mngr_ = &app_manager;
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
  switch (event) {
    case plugins::kApplicationPolicyUpdated: {
      resource_allocation_manager_->OnPolicyEvent(event);
      break;
    }
    default:
      break;
  }
}

void RCRPCPlugin::OnApplicationEvent(
    application_manager::plugin_manager::ApplicationEvent event,
    application_manager::ApplicationSharedPtr application) {
  if (!application->is_remote_control_supported()) {
    return;
  }
  switch (event) {
    case plugins::kApplicationRegistered: {
      application->AddExtension(new RCAppExtension(kRCPluginID));
      break;
    }
    case plugins::kApplicationExit: {
      UpdateHMISubscriptionsOnAppUnregistered(*application);
      resource_allocation_manager_->OnApplicationEvent(event, application);
      break;
    }
    case plugins::kApplicationUnregistered: {
      UpdateHMISubscriptionsOnAppUnregistered(*application);
      resource_allocation_manager_->OnApplicationEvent(event, application);
      break;
    }
    default:
      break;
  }
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

void RCRPCPlugin::UpdateHMISubscriptionsOnAppUnregistered(
    application_manager::Application& app) {
  LOG4CXX_AUTO_TRACE(logger_);

  auto unsubscribe_from_interior_data = [this](const std::string& module_type) {
    auto unsubscribe_request = RCHelpers::CreateUnsubscribeRequestToHMI(
        module_type, app_mngr_->GetNextHMICorrelationID());
    DCHECK_OR_RETURN_VOID(rpc_service_);
    LOG4CXX_DEBUG(logger_, "Send Unsubscribe from " << module_type);
    rpc_service_->ManageHMICommand(unsubscribe_request);
  };

  auto rc_extension = RCHelpers::GetRCExtension(app);
  auto subscribed_data = rc_extension->InteriorVehicleDataSubscriptions();
  for (auto& data : subscribed_data) {
    auto apps_subscribed = RCHelpers::AppsSubscribedTo(*app_mngr_, data);
    if (apps_subscribed.empty()) {
      unsubscribe_from_interior_data(data);
    }
    if (apps_subscribed.size() == 1 &&
        apps_subscribed.front()->hmi_app_id() == app.hmi_app_id()) {
      unsubscribe_from_interior_data(data);
    }
  }
}

}  // namespace rc_rpc_plugin

extern "C" application_manager::plugin_manager::RPCPlugin* Create() {
  return new rc_rpc_plugin::RCRPCPlugin();
}
