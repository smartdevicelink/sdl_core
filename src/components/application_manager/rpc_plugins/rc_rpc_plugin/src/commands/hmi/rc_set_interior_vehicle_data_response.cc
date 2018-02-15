#include "rc_rpc_plugin/commands/hmi/rc_set_interior_vehicle_data_response.h"
#include "utils/macro.h"

namespace rc_rpc_plugin {
namespace commands {

RCSetInteriorVehicleDataResponse::RCSetInteriorVehicleDataResponse(
    const app_mngr::commands::MessageSharedPtr& message,
    app_mngr::ApplicationManager& application_manager,
    app_mngr::rpc_service::RPCService& rpc_service,
    app_mngr::HMICapabilities& hmi_capabilities,
    policy::PolicyHandlerInterface& policy_handle)
    : application_manager::commands::ResponseFromHMI(message,
                                                     application_manager,
                                                     rpc_service,
                                                     hmi_capabilities,
                                                     policy_handle) {}

void RCSetInteriorVehicleDataResponse::Run() {
    LOG4CXX_AUTO_TRACE(logger_);

    app_mngr::event_engine::Event event(hmi_apis::FunctionID::RC_SetInteriorVehicleData);
    event.set_smart_object(*message_);
    event.raise(application_manager_.event_dispatcher());
}

RCSetInteriorVehicleDataResponse::~RCSetInteriorVehicleDataResponse(){}

}  // namespace commands
}  // namespace rc_rpc_plugin
