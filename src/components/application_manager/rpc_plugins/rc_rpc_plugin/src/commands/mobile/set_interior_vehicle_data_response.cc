#include "rc_rpc_plugin/commands/mobile/set_interior_vehicle_data_response.h"
#include "utils/macro.h"

namespace rc_rpc_plugin {
namespace commands {

bool SetInteriorVehicleDataResponse::Init() {
  return true;
}
void SetInteriorVehicleDataResponse::Run() {}
void SetInteriorVehicleDataResponse::on_event(
    const application_manager::event_engine::Event& event) {}

}  // namespace commands
}  // namespace rc_rpc_plugin
