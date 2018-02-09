#include "rc_rpc_plugin/commands/mobile/get_interior_vehicle_data_response.h"
#include "utils/macro.h"

namespace rc_rpc_plugin {
namespace commands {

bool GetInteriorVehicleDataResponse::Init() {
    return true;
}
void GetInteriorVehicleDataResponse::Run() {}
void GetInteriorVehicleDataResponse::on_event(const application_manager::event_engine::Event& event) {}

}  // namespace commands
}  // namespace rc_rpc_plugin
