#include "rc_rpc_plugin/commands/mobile/on_interior_vehicle_data_notification.h"
#include "utils/macro.h"

namespace rc_rpc_plugin {
namespace commands {

OnInteriorVehicleDataNotification::OnInteriorVehicleDataNotification(
    ResourceAllocationManager& resource_allocation_manager,
    const app_mngr::commands::MessageSharedPtr& message,
    app_mngr::ApplicationManager& application_manager,
    app_mngr::rpc_service::RPCService& rpc_service,
    app_mngr::HMICapabilities& hmi_capabilities,
    policy::PolicyHandlerInterface& policy_handle)
    : application_manager::commands::CommandNotificationImpl(
          message,
          application_manager,
          rpc_service,
          hmi_capabilities,
          policy_handle) {}
bool OnInteriorVehicleDataNotification::Init() {
  return true;
}
void OnInteriorVehicleDataNotification::Run() {}
void OnInteriorVehicleDataNotification::on_event(
    const application_manager::event_engine::Event& event) {}

}  // namespace commands
}  // namespace rc_rpc_plugin
