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
#include "sdl_rpc_plugin/sdl_command_factory.h"
#include "sdl_rpc_plugin/sdl_app_extension.h"
#include "sdl_rpc_plugin/sdl_pending_resumption_handler.h"
#include "application_manager/message_helper.h"

namespace sdl_rpc_plugin {
CREATE_LOGGERPTR_GLOBAL(logger_, "SDLRPCPlugin")
namespace plugins = application_manager::plugin_manager;

SDLRPCPlugin::SDLRPCPlugin()
    : application_manager_(nullptr), pending_resumption_handler_(nullptr) {}

bool SDLRPCPlugin::Init(
    application_manager::ApplicationManager& app_manager,
    application_manager::rpc_service::RPCService& rpc_service,
    application_manager::HMICapabilities& hmi_capabilities,
    policy::PolicyHandlerInterface& policy_handler) {
  application_manager_ = &app_manager;
  pending_resumption_handler_ =
      std::make_shared<SDLPendingResumptionHandler>(app_manager);
  command_factory_.reset(new sdl_rpc_plugin::SDLCommandFactory(
      app_manager, rpc_service, hmi_capabilities, policy_handler));
  return true;
}

bool SDLRPCPlugin::IsAbleToProcess(
    const int32_t function_id,
    const application_manager::commands::Command::CommandSource
        message_source) {
  return command_factory_->IsAbleToProcess(function_id, message_source);
}

std::string SDLRPCPlugin::PluginName() {
  return "SDL RPC Plugin";
}

application_manager::CommandFactory& SDLRPCPlugin::GetCommandFactory() {
  return *command_factory_;
}

void SDLRPCPlugin::OnPolicyEvent(
    application_manager::plugin_manager::PolicyEvent event) {}

void SDLRPCPlugin::OnApplicationEvent(
    application_manager::plugin_manager::ApplicationEvent event,
    application_manager::ApplicationSharedPtr application) {
  LOG4CXX_AUTO_TRACE(logger_);
  if (plugins::ApplicationEvent::kApplicationRegistered == event) {
    application->AddExtension(
        std::make_shared<SDLAppExtension>(*this, *application));
  }
}

void SDLRPCPlugin::ProcessResumptionSubscription(
    application_manager::Application& app,
    SDLAppExtension& ext,
    resumption::Subscriber subscriber) {
  LOG4CXX_AUTO_TRACE(logger_);
  application_manager::ApplicationSharedPtr application =
      application_manager_->application(app.app_id());
  std::set<int32_t> apps =
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

void SDLRPCPlugin::RevertResumption(application_manager::Application& app) {
  application_manager::ApplicationSharedPtr application =
      application_manager_->application(app.app_id());
  pending_resumption_handler_->ClearPendingResumptionRequests();
  std::set<int32_t> apps =
      application_manager_->GetAppsSubscribedForWayPoints();
  if (1 == apps.size() &&
      application_manager_->IsAppSubscribedForWayPoints(*application)) {
    auto subscribe_waypoints_msg =
        application_manager::MessageHelper::CreateMessageForHMI(
            hmi_apis::FunctionID::Navigation_UnsubscribeWayPoints,
            application_manager_->GetNextHMICorrelationID());
    (*subscribe_waypoints_msg)[application_manager::strings::params]
                              [application_manager::strings::message_type] =
                                  hmi_apis::messageType::request;
    (*subscribe_waypoints_msg)[application_manager::strings::msg_params]
                              [application_manager::strings::app_id] =
                                  app.app_id();

    application_manager_->GetRPCService().ManageHMICommand(
        subscribe_waypoints_msg);
  }
  application_manager_->UnsubscribeAppFromWayPoints(application);
}

}  // namespace sdl_rpc_plugin

extern "C" application_manager::plugin_manager::RPCPlugin* Create() {
  return new sdl_rpc_plugin::SDLRPCPlugin();
}
