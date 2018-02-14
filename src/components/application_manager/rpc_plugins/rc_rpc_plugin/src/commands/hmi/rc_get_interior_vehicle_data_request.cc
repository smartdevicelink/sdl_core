#include "rc_rpc_plugin/commands/hmi/rc_get_interior_vehicle_data_request.h"
#include "utils/macro.h"

namespace rc_rpc_plugin {
namespace commands {

RCGetInteriorVehicleDataRequest::RCGetInteriorVehicleDataRequest(
    const app_mngr::commands::MessageSharedPtr& message,
    app_mngr::ApplicationManager& application_manager)
    : app_mngr::commands::RequestToHMI(message, application_manager) {}

RCGetInteriorVehicleDataRequest::~RCGetInteriorVehicleDataRequest(){}

void RCGetInteriorVehicleDataRequest::Run() {
    LOG4CXX_AUTO_TRACE(logger_);
    SendRequest();
}

}  // namespace commands
}  // namespace rc_rpc_plugin
