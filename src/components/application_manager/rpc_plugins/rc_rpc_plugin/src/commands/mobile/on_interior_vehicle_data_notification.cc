#include "rc_rpc_plugin/commands/mobile/on_interior_vehicle_data_notification.h"
#include "rc_rpc_plugin/rc_rpc_plugin.h"
#include "rc_rpc_plugin/rc_module_constants.h"
#include "smart_objects/enum_schema_item.h"
#include "utils/macro.h"

namespace rc_rpc_plugin {
namespace commands {

CREATE_LOGGERPTR_GLOBAL(logger_, "RemoteControlModule")

OnInteriorVehicleDataNotification::OnInteriorVehicleDataNotification(
    const app_mngr::commands::MessageSharedPtr& message,
    app_mngr::ApplicationManager& application_manager,
    app_mngr::rpc_service::RPCService& rpc_service,
    app_mngr::HMICapabilities& hmi_capabilities,
    policy::PolicyHandlerInterface& policy_handler,
    ResourceAllocationManager& resource_allocation_manager)
    : app_mngr::commands::CommandNotificationImpl(message,
                                                  application_manager,
                                                  rpc_service,
                                                  hmi_capabilities,
                                                  policy_handler) {
  UNUSED(resource_allocation_manager);
}

OnInteriorVehicleDataNotification::~OnInteriorVehicleDataNotification() {}

void OnInteriorVehicleDataNotification::Run() {
  LOG4CXX_AUTO_TRACE(logger_);

  const std::string module_type = ModuleType();

  typedef std::vector<application_manager::ApplicationSharedPtr> AppPtrs;
  AppPtrs apps = RCRPCPlugin::GetRCApplications(application_manager_);

  for (AppPtrs::iterator it = apps.begin(); it != apps.end(); ++it) {
    DCHECK(*it);
    application_manager::Application& app = **it;

    RCAppExtensionPtr extension =
        application_manager::AppExtensionPtr::static_pointer_cast<
            RCAppExtension>(app.QueryInterface(RCRPCPlugin::kRCPluginID));
    DCHECK(extension);
    LOG4CXX_TRACE(logger_,
                  "Check subscription for "
                      << app.app_id() << "and module type " << module_type);
    if (extension->IsSubscibedToInteriorVehicleData(module_type)) {
      (*message_)[app_mngr::strings::params]
                 [app_mngr::strings::connection_key] = app.app_id();
      SendNotification();
    }
  }
}

std::string OnInteriorVehicleDataNotification::ModuleType() {
  mobile_apis::ModuleType::eType module_type = static_cast<
      mobile_apis::ModuleType::eType>(
      (*message_)[app_mngr::strings::msg_params][message_params::kModuleType]
          .asUInt());
  const char* str;
  const bool ok = NsSmartDeviceLink::NsSmartObjects::EnumConversionHelper<
      mobile_apis::ModuleType::eType>::EnumToCString(module_type, &str);
  return ok ? str : "unknown";
}

}  // namespace commands
}  // namespace rc_rpc_plugin
