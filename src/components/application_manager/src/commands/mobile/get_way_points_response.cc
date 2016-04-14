#include "application_manager/application_manager_impl.h"
#include "application_manager/commands/mobile/get_way_points_response.h"

namespace application_manager {

namespace commands {

GetWayPointsResponse::GetWayPointsResponse(
    const MessageSharedPtr &message)
    : CommandResponseImpl(message) {
}

GetWayPointsResponse::~GetWayPointsResponse() {
}

void GetWayPointsResponse::Run() {
  LOG4CXX_AUTO_TRACE(logger_);

  ApplicationManagerImpl::instance()->SendMessageToMobile(message_);
}

}  // namespace commands

}  // namespace application_manager
