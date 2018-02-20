#include "rc_rpc_plugin/commands/hmi/rc_on_interior_vehicle_data_notification.h"
#include "utils/macro.h"

namespace rc_rpc_plugin {
namespace commands {

RCOnInteriorVehicleDataNotification::RCOnInteriorVehicleDataNotification(
    const app_mngr::commands::MessageSharedPtr& message,
    app_mngr::ApplicationManager& application_manager,
    app_mngr::rpc_service::RPCService& rpc_service,
    app_mngr::HMICapabilities& hmi_capabilities,
    policy::PolicyHandlerInterface& policy_handle,
    ResourceAllocationManager& resource_allocation_manager)
    : application_manager::commands::NotificationFromHMI(message,
                                                         application_manager,
                                                         rpc_service,
                                                         hmi_capabilities,
                                                         policy_handle) {
  UNUSED(resource_allocation_manager);
}

RCOnInteriorVehicleDataNotification::~RCOnInteriorVehicleDataNotification() {}

void RCOnInteriorVehicleDataNotification::Run() {
  LOG4CXX_AUTO_TRACE(logger_);
  (*message_)[app_mngr::strings::params][app_mngr::strings::function_id] =
      static_cast<int>(mobile_apis::FunctionID::eType::OnInteriorVehicleDataID);
  SendNotificationToMobile(message_);
}

}  // namespace commands
}  // namespace rc_rpc_plugin
