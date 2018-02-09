#include "rc_rpc_plugin/commands/mobile/on_interior_vehicle_data_notification.h"
#include "utils/macro.h"

namespace rc_rpc_plugin {
namespace commands {

bool OnInteriorVehicleDataNotification::Init() {
    return true;
}
void OnInteriorVehicleDataNotification::Run() {}
void OnInteriorVehicleDataNotification::on_event(const application_manager::event_engine::Event& event) {}

}  // namespace commands
}  // namespace rc_rpc_plugin
