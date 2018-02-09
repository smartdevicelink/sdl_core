#include "rc_rpc_plugin/commands/mobile/on_interior_vehicle_data_notification.h"
#include "utils/macro.h"

namespace rc_rpc_plugin {
namespace commands {

OnInteriorVehicleDataNotification::OnInteriorVehicleDataNotification(
    const app_mngr::commands::MessageSharedPtr& message,
    app_mngr::ApplicationManager& application_manager)
    : app_mngr::commands::CommandNotificationImpl(message,
                                                  application_manager) {}

bool OnInteriorVehicleDataNotification::Init() {
  return true;
}
void OnInteriorVehicleDataNotification::Run() {}
void OnInteriorVehicleDataNotification::on_event(
    const application_manager::event_engine::Event& event) {}

}  // namespace commands
}  // namespace rc_rpc_plugin
