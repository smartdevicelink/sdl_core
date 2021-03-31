/*
 Copyright (c) 2020, Ford Motor Company
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
#include "application_manager/plugin_manager/plugin_keys.h"
#include "rc_rpc_plugin/interior_data_cache_impl.h"
#include "rc_rpc_plugin/interior_data_manager_impl.h"
#include "rc_rpc_plugin/rc_app_extension.h"
#include "rc_rpc_plugin/rc_capabilities_manager_impl.h"
#include "rc_rpc_plugin/rc_command_factory.h"
#include "rc_rpc_plugin/rc_consent_manager_impl.h"
#include "rc_rpc_plugin/rc_helpers.h"
#include "rc_rpc_plugin/rc_pending_resumption_handler.h"
#include "rc_rpc_plugin/resource_allocation_manager_impl.h"
#include "utils/helpers.h"

namespace rc_rpc_plugin {
SDL_CREATE_LOG_VARIABLE("RemoteControlModule");

namespace plugins = application_manager::plugin_manager;

bool RCRPCPlugin::Init(
    application_manager::ApplicationManager& app_manager,
    application_manager::rpc_service::RPCService& rpc_service,
    application_manager::HMICapabilities& hmi_capabilities,
    policy::PolicyHandlerInterface& policy_handler,
    resumption::LastStateWrapperPtr last_state) {
  rc_consent_manager_.reset(new rc_rpc_plugin::RCConsentManagerImpl(
      last_state,
      app_manager,
      app_manager.get_settings().period_for_consent_expiration()));
  interior_data_cache_.reset(new InteriorDataCacheImpl());
  interior_data_manager_.reset(new InteriorDataManagerImpl(
      *this, *interior_data_cache_, app_manager, rpc_service));
  rc_capabilities_manager_.reset(
      new RCCapabilitiesManagerImpl(hmi_capabilities));
  resource_allocation_manager_.reset(new ResourceAllocationManagerImpl(
      app_manager, rpc_service, *(rc_capabilities_manager_.get())));
  RCCommandParams params{app_manager,
                         rpc_service,
                         hmi_capabilities,
                         policy_handler,
                         *(resource_allocation_manager_.get()),
                         *(interior_data_cache_.get()),
                         *(interior_data_manager_.get()),
                         *(rc_capabilities_manager_.get()),
                         *(rc_consent_manager_.get())};
  command_factory_.reset(new rc_rpc_plugin::RCCommandFactory(params));
  rpc_service_ = &rpc_service;
  app_mngr_ = &app_manager;
  pending_resumption_handler_ = std::make_shared<RCPendingResumptionHandler>(
      app_manager, *(interior_data_cache_.get()));

  // Check all saved consents and remove expired
  rc_consent_manager_->RemoveExpiredConsents();

  return true;
}

bool RCRPCPlugin::Init(application_manager::ApplicationManager&,
                       application_manager::rpc_service::RPCService&,
                       application_manager::HMICapabilities&,
                       policy::PolicyHandlerInterface&,
                       resumption::LastState&) {
  return false;
}

bool RCRPCPlugin::IsAbleToProcess(
    const int32_t function_id,
    const application_manager::commands::Command::CommandSource
        message_source) {
  return command_factory_->IsAbleToProcess(function_id, message_source);
}

std::string RCRPCPlugin::PluginName() {
  return plugins::plugin_names::rc_rpc_plugin;
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
  SDL_LOG_AUTO_TRACE();
  if (!application->is_remote_control_supported()) {
    SDL_LOG_DEBUG(
        "Remote control is not supported for application with app_id: "
        << application->app_id());
    return;
  }
  switch (event) {
    case plugins::kApplicationRegistered: {
      auto extension = std::shared_ptr<RCAppExtension>(
          new RCAppExtension(kRCPluginID, *this, *application));
      DCHECK_OR_RETURN_VOID(application->AddExtension(extension));
      const auto driver_location =
          rc_capabilities_manager_
              ->GetDriverLocationFromSeatLocationCapability();
      extension->SetUserLocation(driver_location);
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
    case plugins::kGlobalPropertiesUpdated: {
      const auto user_location = application->get_user_location();
      auto extension = RCHelpers::GetRCExtension(*application);
      extension->SetUserLocation(user_location);
      break;
    }
    case plugins::kRCStatusChanged: {
      resource_allocation_manager_->SendOnRCStatusNotifications(
          NotificationTrigger::APP_REGISTRATION, application);
      break;
    }
    default:
      break;
  }
}

void RCRPCPlugin::ProcessResumptionSubscription(
    application_manager::Application& app, RCAppExtension& ext) {
  SDL_LOG_AUTO_TRACE();

  pending_resumption_handler_->HandleResumptionSubscriptionRequest(ext, app);
}

void RCRPCPlugin::RevertResumption(const std::set<ModuleUid>& subscriptions) {
  SDL_LOG_AUTO_TRACE();

  interior_data_manager_->OnResumptionRevert(subscriptions);
  pending_resumption_handler_->OnResumptionRevert();
}

bool RCRPCPlugin::IsOtherAppsSubscribed(const rc_rpc_types::ModuleUid& module,
                                        const uint32_t app_id) {
  auto get_subscriptions = [](application_manager::ApplicationSharedPtr app) {
    std::set<ModuleUid> result;
    auto rc_app_extension = RCHelpers::GetRCExtension(*app);
    if (rc_app_extension) {
      result = rc_app_extension->InteriorVehicleDataSubscriptions();
    }
    return result;
  };

  auto another_app_subscribed =
      [app_id, module, &get_subscriptions](
          application_manager::ApplicationSharedPtr app) {
        if (app_id == app->app_id()) {
          return false;
        }
        auto subscriptions = get_subscriptions(app);
        auto it = subscriptions.find(module);
        return subscriptions.end() != it;
      };

  auto accessor = app_mngr_->applications();
  auto it = std::find_if(accessor.GetData().begin(),
                         accessor.GetData().end(),
                         another_app_subscribed);
  return accessor.GetData().end() != it;
}

RCRPCPlugin::Apps RCRPCPlugin::GetRCApplications(
    application_manager::ApplicationManager& app_mngr) {
  using application_manager::ApplicationSet;
  using application_manager::ApplicationSharedPtr;
  ApplicationSet accessor = app_mngr.applications().GetData();

  std::vector<ApplicationSharedPtr> result;

  std::copy_if(accessor.begin(),
               accessor.end(),
               std::back_inserter(result),
               [](const ApplicationSharedPtr& app) {
                 return app->is_remote_control_supported();
               });

  return result;
}

}  // namespace rc_rpc_plugin

extern "C" __attribute__((visibility("default")))
application_manager::plugin_manager::RPCPlugin*
Create(logger::Logger* logger_instance) {
  logger::Logger::instance(logger_instance);
  return new rc_rpc_plugin::RCRPCPlugin();
}  // namespace rc_rpc_plugin

extern "C" __attribute__((visibility("default"))) void Delete(
    application_manager::plugin_manager::RPCPlugin* data) {
  delete data;
}
