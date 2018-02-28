#include "rc_rpc_plugin/rc_rpc_plugin.h"
#include "rc_rpc_plugin/rc_command_factory.h"
#include "rc_rpc_plugin/rc_app_extension.h"
#include "rc_rpc_plugin/resource_allocation_manager_impl.h"
#include "utils/helpers.h"

namespace rc_rpc_plugin {
namespace plugins = application_manager::plugin_manager;

bool RCRPCPlugin::Init(
    application_manager::ApplicationManager& app_manager,
    application_manager::rpc_service::RPCService& rpc_service,
    application_manager::HMICapabilities& hmi_capabilities,
    policy::PolicyHandlerInterface& policy_handler) {
  resource_allocation_manager_.reset(
      new ResourceAllocationManagerImpl(app_manager, rpc_service));
  command_factory_.reset(
      new rc_rpc_plugin::RCCommandFactory(app_manager,
                                          rpc_service,
                                          hmi_capabilities,
                                          policy_handler,
                                          *resource_allocation_manager_));
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
    application_manager::plugin_manager::PolicyEvent event) {}

void RCRPCPlugin::OnApplicationEvent(
    application_manager::plugin_manager::ApplicationEvent event,
    application_manager::ApplicationSharedPtr application) {
  switch (event) {
    case plugins::kApplicationRegistered: {
      application->AddExtension(new RCAppExtension(kRCPluginID));
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

  auto predicate = [](const ApplicationSharedPtr& app) {
    auto uid = RCRPCPlugin::kRCPluginID;
    return app ? app->QueryInterface(uid).valid() : false;
  };

  auto it = std::find_if(accessor.begin(), accessor.end(), predicate);

  std::vector<ApplicationSharedPtr> result;
  while (it != accessor.end()) {
    result.push_back(*it);
    it = std::find_if(++it, accessor.end(), predicate);
  }
  return result;
}

uint32_t RCRPCPlugin::current_audio_source_ = 0u;

uint32_t RCRPCPlugin::get_current_audio_source() {
  return current_audio_source_;
}

void RCRPCPlugin::set_current_audio_source(uint32_t source) {
  current_audio_source_ = source;
}

}  // namespace rc_rpc_plugin

extern "C" application_manager::plugin_manager::RPCPlugin* Create() {
  return new rc_rpc_plugin::RCRPCPlugin();
}  // namespace rc_rpc_plugin
