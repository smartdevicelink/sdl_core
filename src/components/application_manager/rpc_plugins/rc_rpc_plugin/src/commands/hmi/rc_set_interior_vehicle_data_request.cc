#include "rc_rpc_plugin/commands/hmi/rc_set_interior_vehicle_data_request.h"
#include "utils/macro.h"

namespace rc_rpc_plugin {
namespace commands {

RCSetInteriorVehicleDataRequest::RCSetInteriorVehicleDataRequest(
    const app_mngr::commands::MessageSharedPtr& message,
    app_mngr::ApplicationManager& application_manager)
    :app_mngr::commands::RequestToHMI(message, application_manager) {}

bool RCSetInteriorVehicleDataRequest::Init() {
  return true;
}
void RCSetInteriorVehicleDataRequest::Run() {}
void RCSetInteriorVehicleDataRequest::on_event(
    const application_manager::event_engine::Event& event) {}

}  // namespace commands
}  // namespace rc_rpc_plugin
