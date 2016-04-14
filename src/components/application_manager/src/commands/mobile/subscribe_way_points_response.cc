#include "application_manager/application_manager_impl.h"
#include "application_manager/commands/mobile/subscribe_way_points_response.h"

namespace application_manager {

namespace commands {

SubscribeWayPointsResponse::SubscribeWayPointsResponse(
    const MessageSharedPtr &message)
    : CommandResponseImpl(message) {
}

SubscribeWayPointsResponse::~SubscribeWayPointsResponse() {
}

void SubscribeWayPointsResponse::Run() {
  LOG4CXX_AUTO_TRACE(logger_);

  ApplicationManagerImpl::instance()->SendMessageToMobile(message_);
}

}  // namespace commands

}  // namespace application_manager
