#include "application_manager/application_manager.h"
#include "application_manager/rpc_service.h"
#include "sdl_rpc_plugin/commands/mobile/get_system_capability_response.h"

namespace application_manager {

namespace commands {

GetSystemCapabilityResponse::GetSystemCapabilityResponse(
    const MessageSharedPtr& message, ApplicationManager& application_manager)
    : CommandResponseImpl(message, application_manager) {}

GetSystemCapabilityResponse::~GetSystemCapabilityResponse() {}

void GetSystemCapabilityResponse::Run() {
  LOG4CXX_AUTO_TRACE(logger_);

  application_manager_.GetRPCService().SendMessageToMobile(message_);
}

}  // namespace commands

}  // namespace application_manager
