#include "application_manager/application_manager.h"
#include "sdl_rpc_plugin/commands/mobile/unsubscribe_way_points_response.h"
#include "application_manager/rpc_service.h"

namespace sdl_rpc_plugin {
using namespace application_manager;

namespace commands {

UnsubscribeWayPointsResponse::UnsubscribeWayPointsResponse(
    const application_manager::commands::MessageSharedPtr& message,
    ApplicationManager& application_manager)
    : CommandResponseImpl(message, application_manager) {}

UnsubscribeWayPointsResponse::~UnsubscribeWayPointsResponse() {}

void UnsubscribeWayPointsResponse::Run() {
  LOG4CXX_AUTO_TRACE(logger_);

  application_manager_.GetRPCService().SendMessageToMobile(message_);
}

}  // namespace commands

}  // namespace application_manager
