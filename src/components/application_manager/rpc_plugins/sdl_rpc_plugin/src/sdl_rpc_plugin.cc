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

#include "sdl_rpc_plugin/sdl_rpc_plugin.h"
#include "application_manager/message_helper.h"
#include "application_manager/plugin_manager/plugin_keys.h"
#include "sdl_rpc_plugin/extensions/system_capability_app_extension.h"
#include "sdl_rpc_plugin/sdl_app_extension.h"
#include "sdl_rpc_plugin/sdl_command_factory.h"
#include "sdl_rpc_plugin/sdl_pending_resumption_handler.h"

namespace sdl_rpc_plugin {
namespace app_mngr = application_manager;
namespace plugins = application_manager::plugin_manager;

CREATE_LOGGERPTR_GLOBAL(logger_, "SdlRPCPlugin")

SDLRPCPlugin::SDLRPCPlugin()
    : application_manager_(nullptr), pending_resumption_handler_(nullptr) {}

bool SDLRPCPlugin::Init(app_mngr::ApplicationManager& app_manager,
                        app_mngr::rpc_service::RPCService& rpc_service,
                        app_mngr::HMICapabilities& hmi_capabilities,
                        policy::PolicyHandlerInterface& policy_handler,
                        resumption::LastStateWrapperPtr last_state) {
  UNUSED(last_state);
  application_manager_ = &app_manager;
  pending_resumption_handler_ =
      std::make_shared<SDLPendingResumptionHandler>(app_manager);
  command_factory_.reset(new sdl_rpc_plugin::SDLCommandFactory(
      app_manager, rpc_service, hmi_capabilities, policy_handler));
  return true;
}

bool SDLRPCPlugin::Init(
    application_manager::ApplicationManager& app_manager,
    application_manager::rpc_service::RPCService& rpc_service,
    application_manager::HMICapabilities& hmi_capabilities,
    policy::PolicyHandlerInterface& policy_handler,
    resumption::LastState& last_state) {
  UNUSED(last_state);
  command_factory_.reset(new sdl_rpc_plugin::SDLCommandFactory(
      app_manager, rpc_service, hmi_capabilities, policy_handler));
  return true;
}

bool SDLRPCPlugin::IsAbleToProcess(
    const int32_t function_id,
    const app_mngr::commands::Command::CommandSource message_source) {
  return command_factory_->IsAbleToProcess(function_id, message_source);
}

std::string SDLRPCPlugin::PluginName() {
  return plugins::plugin_names::sdl_rpc_plugin;
}

app_mngr::CommandFactory& SDLRPCPlugin::GetCommandFactory() {
  return *command_factory_;
}

void SDLRPCPlugin::OnPolicyEvent(plugins::PolicyEvent event) {}

void SDLRPCPlugin::OnApplicationEvent(
    plugins::ApplicationEvent event,
    app_mngr::ApplicationSharedPtr application) {
  LOG4CXX_AUTO_TRACE(logger_);
  if (plugins::ApplicationEvent::kApplicationRegistered == event) {
    application->AddExtension(
        std::make_shared<SDLAppExtension>(*this, *application));

    auto sys_cap_ext_ptr =
        std::make_shared<SystemCapabilityAppExtension>(*this, *application);
    application->AddExtension(sys_cap_ext_ptr);
    // Processing automatic subscription to SystemCapabilities for DISPLAY type
    const auto capability_type =
        mobile_apis::SystemCapabilityType::eType::DISPLAYS;
    LOG4CXX_DEBUG(logger_, "Subscription to DISPLAYS capability is enabled");
    sys_cap_ext_ptr->SubscribeTo(capability_type);

  } else if (plugins::ApplicationEvent::kDeleteApplicationData == event) {
    ClearSubscriptions(application);
  }
}

void SDLRPCPlugin::ProcessResumptionSubscription(
    application_manager::Application& app,
    SDLAppExtension& ext,
    resumption::Subscriber subscriber) {
  LOG4CXX_AUTO_TRACE(logger_);
  application_manager::ApplicationSharedPtr application =
      application_manager_->application(app.app_id());
  std::set<uint32_t> apps =
      application_manager_->GetAppsSubscribedForWayPoints();
  application_manager_->SubscribeAppForWayPoints(application);
  pending_resumption_handler_->HandleResumptionSubscriptionRequest(
      ext, subscriber, app);
}

void SDLRPCPlugin::SaveResumptionData(
    application_manager::Application& app,
    smart_objects::SmartObject& resumption_data) {
  resumption_data[application_manager::strings::subscribed_for_way_points] =
      application_manager_->IsAppSubscribedForWayPoints(app);
}

void SDLRPCPlugin::RevertResumption(application_manager::Application& app,
                                    bool is_way_point_request_successful) {
  LOG4CXX_AUTO_TRACE(logger_);

  pending_resumption_handler_->ClearPendingResumptionRequests();

  if (application_manager_->IsAppSubscribedForWayPoints(app)) {
    application_manager_->UnsubscribeAppFromWayPoints(app.app_id());
    if (!application_manager_->IsAnyAppSubscribedForWayPoints() &&
        is_way_point_request_successful) {
      LOG4CXX_DEBUG(logger_, "Send UnsubscribeWayPoints");
      auto request =
          application_manager::MessageHelper::CreateUnsubscribeWayPointsRequest(
              application_manager_->GetNextHMICorrelationID());
      application_manager_->GetRPCService().ManageHMICommand(request);
    }
  }
}

void SDLRPCPlugin::ClearSubscriptions(app_mngr::ApplicationSharedPtr app) {
  auto& ext = SystemCapabilityAppExtension::ExtractExtension(*app);
  ext.UnsubscribeFromAll();
}

}  // namespace sdl_rpc_plugin

extern "C" __attribute__((visibility("default")))
application_manager::plugin_manager::RPCPlugin*
Create() {
  return new sdl_rpc_plugin::SDLRPCPlugin();
}

extern "C" __attribute__((visibility("default"))) void Delete(
    application_manager::plugin_manager::RPCPlugin* data) {
  delete data;
  DELETE_THREAD_LOGGER(sdl_rpc_plugin::logger_);
}
