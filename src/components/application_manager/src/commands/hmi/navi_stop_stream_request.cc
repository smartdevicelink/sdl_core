#include "application_manager/commands/hmi/navi_stop_stream_request.h"

namespace application_manager {

namespace commands {

NaviStopStreamRequest::NaviStopStreamRequest(
    const MessageSharedPtr& message)
    : RequestToHMI(message) {
}

NaviStopStreamRequest::~NaviStopStreamRequest() {
}

void NaviStopStreamRequest::Run() {
  LOG4CXX_AUTO_TRACE(logger_);

  SendRequest();
}

}  // namespace commands

}  // namespace application_manager
