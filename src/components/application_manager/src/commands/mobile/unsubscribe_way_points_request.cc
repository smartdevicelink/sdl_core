#include "application_manager/application_manager.h"
#include "application_manager/commands/mobile/unsubscribe_way_points_request.h"

namespace application_manager {

namespace commands {

UnSubscribeWayPointsRequest::UnSubscribeWayPointsRequest(
    const MessageSharedPtr& message, ApplicationManager& application_manager)
    : CommandRequestImpl(message, application_manager) {}

UnSubscribeWayPointsRequest::~UnSubscribeWayPointsRequest() {}

void UnSubscribeWayPointsRequest::Run() {
  LOG4CXX_AUTO_TRACE(logger_);

  ApplicationSharedPtr app = application_manager_.application(connection_key());

  if (!app) {
    LOG4CXX_ERROR(logger_,
                  "An application with connection key "
                      << connection_key() << " is not registered.");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return;
  }

  if (!application_manager_.IsAppSubscribedForWayPoints(app->app_id())) {
    SendResponse(false, mobile_apis::Result::IGNORED);
    return;
  }

  SendHMIRequest(
      hmi_apis::FunctionID::Navigation_UnsubscribeWayPoints, NULL, true);
}

void UnSubscribeWayPointsRequest::on_event(const event_engine::Event& event) {
  LOG4CXX_AUTO_TRACE(logger_);
  ApplicationSharedPtr app = application_manager_.application(connection_key());
  const smart_objects::SmartObject& message = event.smart_object();
  switch (event.id()) {
    case hmi_apis::FunctionID::Navigation_UnsubscribeWayPoints: {
      LOG4CXX_INFO(logger_, "Received Navigation_UnSubscribeWayPoints event");
      mobile_apis::Result::eType result_code =
          GetMobileResultCode(static_cast<hmi_apis::Common_Result::eType>(
              message[strings::params][hmi_response::code].asUInt()));
      bool result = mobile_apis::Result::SUCCESS == result_code;
      if (result) {
        application_manager_.UnsubscribeAppFromWayPoints(app->app_id());
      }
      SendResponse(result, result_code, NULL, &(message[strings::msg_params]));
      break;
    }
    default: {
      LOG4CXX_ERROR(logger_, "Received unknown event" << event.id());
      break;
    }
  }
}

}  // namespace commands

}  // namespace application_manager
