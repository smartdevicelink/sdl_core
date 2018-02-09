#include "rc_rpc_plugin/commands/mobile/get_interior_vehicle_data_request.h"
#include "utils/macro.h"

namespace rc_rpc_plugin {
namespace commands {

bool GetInteriorVehicleDataRequest::Init() {
    return true;
}
void GetInteriorVehicleDataRequest::Run() {}
void GetInteriorVehicleDataRequest::on_event(const application_manager::event_engine::Event& event) {}

}  // namespace commands
}  // namespace rc_rpc_plugin
