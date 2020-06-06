#include "rc_rpc_plugin/commands/mobile/set_interior_vehicle_data_response.h"
#include "rc_rpc_plugin/interior_data_cache.h"
#include "utils/macro.h"

namespace rc_rpc_plugin {
namespace commands {

SetInteriorVehicleDataResponse::SetInteriorVehicleDataResponse(
    const app_mngr::commands::MessageSharedPtr& message,
    const RCCommandParams& params)
    : application_manager::commands::CommandResponseImpl(
          message,
          params.application_manager_,
          params.rpc_service_,
          params.hmi_capabilities_,
          params.policy_handler_) {}
SetInteriorVehicleDataResponse::~SetInteriorVehicleDataResponse() {}

void SetInteriorVehicleDataResponse::Run() {
  LOG4CXX_AUTO_TRACE(logger_);
  application_manager_.GetRPCService().SendMessageToMobile(message_);
}

}  // namespace commands
}  // namespace rc_rpc_plugin
