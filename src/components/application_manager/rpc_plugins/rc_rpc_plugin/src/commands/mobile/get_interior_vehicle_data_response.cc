#include "rc_rpc_plugin/commands/mobile/get_interior_vehicle_data_response.h"
#include "utils/macro.h"

namespace rc_rpc_plugin {
namespace commands {

GetInteriorVehicleDataResponse::GetInteriorVehicleDataResponse(
    const app_mngr::commands::MessageSharedPtr& message,
    app_mngr::ApplicationManager& application_manager)
    : app_mngr::commands::CommandResponseImpl(message, application_manager) {}

GetInteriorVehicleDataResponse::~GetInteriorVehicleDataResponse(){}

void GetInteriorVehicleDataResponse::Run() {
    LOG4CXX_AUTO_TRACE(logger_);
    application_manager_.GetRPCService().SendMessageToMobile(message_);
}

}  // namespace commands
}  // namespace rc_rpc_plugin
