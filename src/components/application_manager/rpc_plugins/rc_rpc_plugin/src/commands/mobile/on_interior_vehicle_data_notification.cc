#include "rc_rpc_plugin/commands/mobile/on_interior_vehicle_data_notification.h"
#include "rc_rpc_plugin/rc_rpc_plugin.h"
#include "rc_rpc_plugin/rc_module_constants.h"
#include "utils/macro.h"

namespace rc_rpc_plugin {
namespace commands {

CREATE_LOGGERPTR_GLOBAL(logger_, "RemoteControlModule")

OnInteriorVehicleDataNotification::OnInteriorVehicleDataNotification(
    const app_mngr::commands::MessageSharedPtr& message,
    app_mngr::ApplicationManager& application_manager,
    app_mngr::rpc_service::RPCService& rpc_service,
    app_mngr::HMICapabilities& hmi_capabilities,
    policy::PolicyHandlerInterface& policy_handler)
    : app_mngr::commands::CommandNotificationImpl(message,
                                                  application_manager,
                                                  rpc_service,
                                                  hmi_capabilities,
                                                  policy_handler) {}

OnInteriorVehicleDataNotification::~OnInteriorVehicleDataNotification() {}

void OnInteriorVehicleDataNotification::Run() {
    LOG4CXX_AUTO_TRACE(logger_);

    const std::string module_type =
        (*message_)[app_mngr::strings::msg_params][message_params::kModuleType].asString();

    typedef std::vector<application_manager::ApplicationSharedPtr> AppPtrs;
    AppPtrs apps = RCRPCPlugin::GetRCApplications(application_manager_);

    for (AppPtrs::iterator it = apps.begin(); it != apps.end(); ++it) {
      DCHECK(*it);
      application_manager::Application& app = **it;

      RCAppExtensionPtr extension =
          application_manager::AppExtensionPtr::static_pointer_cast<
              RCAppExtension>(app.QueryInterface(RCRPCPlugin::kRCPluginID));
      DCHECK(extension);
      LOG4CXX_TRACE(logger_, "Check subscription for " << app.app_id());
      if (extension->IsSubscibedToInteriorVehicleData(module_type)) {
        SendNotification();
      }
  }
}

}  // namespace commands
}  // namespace rc_rpc_plugin
