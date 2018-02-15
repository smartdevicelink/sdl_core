#include "rc_rpc_plugin/commands/hmi/rc_set_interior_vehicle_data_response.h"
#include "utils/macro.h"

namespace rc_rpc_plugin {
namespace commands {

RCSetInteriorVehicleDataResponse::RCSetInteriorVehicleDataResponse(
    const app_mngr::commands::MessageSharedPtr& message,
    app_mngr::ApplicationManager& application_manager,
    app_mngr::rpc_service::RPCService& rpc_service,
    app_mngr::HMICapabilities& hmi_capabilities,
    policy::PolicyHandlerInterface& policy_handle)
    : application_manager::commands::ResponseFromHMI(message,
                                                     application_manager,
                                                     rpc_service,
                                                     hmi_capabilities,
                                                     policy_handle) {}

bool RCSetInteriorVehicleDataResponse::Init() {
  return true;
}
void RCSetInteriorVehicleDataResponse::Run() {}

void RCSetInteriorVehicleDataResponse::on_event(
    const application_manager::event_engine::Event& event) {}

}  // namespace commands
}  // namespace rc_rpc_plugin
