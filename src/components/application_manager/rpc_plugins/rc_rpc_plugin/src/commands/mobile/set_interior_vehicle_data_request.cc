#include "rc_rpc_plugin/commands/mobile/set_interior_vehicle_data_request.h"
#include "utils/macro.h"

namespace rc_rpc_plugin {
namespace commands {

bool SetInteriorVehicleDataRequest::Init() {
    return true;
}
void SetInteriorVehicleDataRequest::Run() {}
void SetInteriorVehicleDataRequest::on_event(const application_manager::event_engine::Event& event) {}

}  // namespace commands
}  // namespace rc_rpc_plugin
