/*
 Copyright (c) 2019, Ford Motor Company, Livio
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following
 disclaimer in the documentation and/or other materials provided with the
 distribution.

 Neither the name of the the copyright holders nor the names of their
 contributors may be used to endorse or promote products derived from this
 software without specific prior written permission.

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

#include "app_service_rpc_plugin/app_service_rpc_plugin.h"
#include "app_service_rpc_plugin/app_service_app_extension.h"
#include "app_service_rpc_plugin/app_service_command_factory.h"
#include "application_manager/message_helper.h"
#include "application_manager/plugin_manager/plugin_keys.h"
#include "application_manager/smart_object_keys.h"

namespace app_service_rpc_plugin {
SDL_CREATE_LOG_VARIABLE("AppServiceRpcPlugin")

namespace strings = application_manager::strings;
namespace plugins = application_manager::plugin_manager;

AppServiceRpcPlugin::AppServiceRpcPlugin() : application_manager_(nullptr) {}

bool AppServiceRpcPlugin::Init(
    application_manager::ApplicationManager& app_manager,
    application_manager::rpc_service::RPCService& rpc_service,
    application_manager::HMICapabilities& hmi_capabilities,
    policy::PolicyHandlerInterface& policy_handler,
    resumption::LastStateWrapperPtr last_state) {
  UNUSED(last_state);
  application_manager_ = &app_manager;
  command_factory_.reset(new app_service_rpc_plugin::AppServiceCommandFactory(
      app_manager, rpc_service, hmi_capabilities, policy_handler));
  return true;
}

bool AppServiceRpcPlugin::IsAbleToProcess(
    const int32_t function_id, const commands::Command::CommandSource source) {
  return command_factory_->IsAbleToProcess(function_id, source);
}

std::string AppServiceRpcPlugin::PluginName() {
  return plugins::plugin_names::app_service_rpc_plugin;
}

app_mngr::CommandFactory& AppServiceRpcPlugin::GetCommandFactory() {
  return *command_factory_;
}

void AppServiceRpcPlugin::OnPolicyEvent(plugins::PolicyEvent event) {}

void AppServiceRpcPlugin::OnApplicationEvent(
    plugins::ApplicationEvent event,
    app_mngr::ApplicationSharedPtr application) {
  switch (event) {
    case plugins::ApplicationEvent::kApplicationRegistered: {
      application->AddExtension(
          std::make_shared<AppServiceAppExtension>(*this, *application));
      break;
    }

    case plugins::ApplicationEvent::kApplicationUnregistered:
    case plugins::ApplicationEvent::kDeleteApplicationData: {
      DeleteSubscriptions(application);
      break;
    }

    default: {
      break;
    }
  }
}

void AppServiceRpcPlugin::DeleteSubscriptions(
    application_manager::ApplicationSharedPtr app) {
  auto& ext = AppServiceAppExtension::ExtractASExtension(*app);
  auto subscriptions = ext.Subscriptions();
  for (auto& service_type : subscriptions) {
    ext.UnsubscribeFromAppService(service_type);
  }
}

}  // namespace app_service_rpc_plugin

using RPCPlugin = application_manager::plugin_manager::RPCPlugin;
// cppcheck-suppress unusedFunction
extern "C" __attribute__((visibility("default"))) RPCPlugin* Create(
    logger::Logger* logger_instance) {
  logger::Logger::instance(logger_instance);
  return new app_service_rpc_plugin::AppServiceRpcPlugin();
}

// cppcheck-suppress unusedFunction
extern "C" __attribute__((visibility("default"))) void Delete(RPCPlugin* data) {
  delete data;
}
