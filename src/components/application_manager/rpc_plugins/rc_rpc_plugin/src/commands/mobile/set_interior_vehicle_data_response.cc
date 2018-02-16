#include "rc_rpc_plugin/commands/mobile/set_interior_vehicle_data_response.h"
#include "utils/macro.h"

namespace rc_rpc_plugin {
namespace commands {

SetInteriorVehicleDataResponse::SetInteriorVehicleDataResponse(
    const app_mngr::commands::MessageSharedPtr& message,
    app_mngr::ApplicationManager& application_manager,
    app_mngr::rpc_service::RPCService& rpc_service,
    app_mngr::HMICapabilities& hmi_capabilities,
    policy::PolicyHandlerInterface& policy_handle,
    ResourceAllocationManager& resource_allocation_manager)
    : application_manager::commands::CommandResponseImpl(message,
                                                         application_manager,
                                                         rpc_service,
                                                         hmi_capabilities,
                                                         policy_handle) {
  UNUSED(resource_allocation_manager);
}
SetInteriorVehicleDataResponse::~SetInteriorVehicleDataResponse() {}

void SetInteriorVehicleDataResponse::Run() {
  LOG4CXX_AUTO_TRACE(logger_);
  application_manager_.GetRPCService().SendMessageToMobile(message_);
}

}  // namespace commands
}  // namespace rc_rpc_plugin
