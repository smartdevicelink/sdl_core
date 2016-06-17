#include "application_manager/application_manager.h"
#include "application_manager/commands/mobile/get_way_points_response.h"

namespace application_manager {

namespace commands {

GetWayPointsResponse::GetWayPointsResponse(
    const MessageSharedPtr& message, ApplicationManager& application_manager)
    : CommandResponseImpl(message, application_manager) {}

GetWayPointsResponse::~GetWayPointsResponse() {}

void GetWayPointsResponse::Run() {
  SDL_AUTO_TRACE();

  application_manager_.SendMessageToMobile(message_);
}

}  // namespace commands

}  // namespace application_manager
