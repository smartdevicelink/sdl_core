#include "rc_rpc_plugin/commands/mobile/set_interior_vehicle_data_request.h"
#include "utils/macro.h"

namespace rc_rpc_plugin {
namespace commands {

SetInteriorVehicleDataRequest::SetInteriorVehicleDataRequest(
    ResourceAllocationManager& resource_allocation_manager,
    const app_mngr::commands::MessageSharedPtr& message,
    app_mngr::ApplicationManager& application_manager,
    app_mngr::rpc_service::RPCService& rpc_service,
    app_mngr::HMICapabilities& hmi_capabilities,
    policy::PolicyHandlerInterface& policy_handle)
    : RCCommandRequest(resource_allocation_manager,
                       message,
                       application_manager,
                       rpc_service,
                       hmi_capabilities,
                       policy_handle) {}

bool SetInteriorVehicleDataRequest::Init() {
  return true;
}
void SetInteriorVehicleDataRequest::Run() {}
void SetInteriorVehicleDataRequest::on_event(
    const application_manager::event_engine::Event& event) {}

}  // namespace commands
}  // namespace rc_rpc_plugin
