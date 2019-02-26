#include "application_manager/application_manager.h"
#include "application_manager/rpc_service.h"
#include "sdl_rpc_plugin/commands/mobile/get_cloud_app_properties_response.h"

namespace sdl_rpc_plugin {
using namespace application_manager;

namespace commands {

GetCloudAppPropertiesResponse::GetCloudAppPropertiesResponse(
    const app_mngr::commands::MessageSharedPtr& message,
    app_mngr::ApplicationManager& application_manager,
    app_mngr::rpc_service::RPCService& rpc_service,
    app_mngr::HMICapabilities& hmi_capabilities,
    policy::PolicyHandlerInterface& policy_handler)
    : CommandResponseImpl(message,
                            application_manager,
                            rpc_service,
                            hmi_capabilities,
                            policy_handler) {}

GetCloudAppPropertiesResponse::~GetCloudAppPropertiesResponse() {}

void GetCloudAppPropertiesResponse::Run() {
    LOG4CXX_AUTO_TRACE(logger_);

    rpc_service_.SendMessageToMobile(message_);
}

}  // namespace commands
}  // namespace sdl_rpc_plugins