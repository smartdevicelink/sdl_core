#include "application_manager/application_manager.h"
#include "application_manager/commands/mobile/unsubscribe_way_points_response.h"

namespace application_manager {

namespace commands {

UnsubscribeWayPointsResponse::UnsubscribeWayPointsResponse(
    const MessageSharedPtr& message, ApplicationManager& application_manager)
    : CommandResponseImpl(message, application_manager) {}

UnsubscribeWayPointsResponse::~UnsubscribeWayPointsResponse() {}

void UnsubscribeWayPointsResponse::Run() {
  SDL_AUTO_TRACE();

  application_manager_.SendMessageToMobile(message_);
}

}  // namespace commands

}  // namespace application_manager
