#include "rc_rpc_plugin/commands/hmi/rc_on_interior_vehicle_data_notification.h"
#include "utils/macro.h"

namespace rc_rpc_plugin {
namespace commands {

bool RCOnInteriorVehicleDataNotification::Init() {
  return true;
}
void RCOnInteriorVehicleDataNotification::Run() {}
void RCOnInteriorVehicleDataNotification::on_event(
    const application_manager::event_engine::Event& event) {}

}  // namespace commands
}  // namespace rc_rpc_plugin
