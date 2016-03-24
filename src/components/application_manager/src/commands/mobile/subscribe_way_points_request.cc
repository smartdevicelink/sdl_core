#include "application_manager/application_manager_impl.h"
#include "application_manager/commands/mobile/subscribe_way_points_request.h"

namespace application_manager {

namespace commands {

SubscribeWayPointsRequest::SubscribeWayPointsRequest(
    const MessageSharedPtr &message)
    : CommandRequestImpl(message) {}

SubscribeWayPointsRequest::~SubscribeWayPointsRequest() {}

void SubscribeWayPointsRequest::Run() {
  LOG4CXX_AUTO_TRACE(logger_);

  ApplicationSharedPtr app =
      application_manager::ApplicationManagerImpl::instance()->application(
          connection_key());

  if (!app) {
    LOG4CXX_ERROR(logger_, "An application with connection key "
                               << connection_key() << " is not registered.");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return;
  }

  if (application_manager::ApplicationManagerImpl::instance()
          ->IsAppSubscribedForWayPoints(app->app_id())) {
    SendResponse(false, mobile_apis::Result::IGNORED);
    return;
  }

  if (application_manager::ApplicationManagerImpl::instance()
          ->IsAnyAppSubscribedForWayPoints()) {
    application_manager::ApplicationManagerImpl::instance()
        ->SubscribeAppForWayPoints(app->app_id());
    SendResponse(true, mobile_apis::Result::SUCCESS);
    return;
  }

  SendHMIRequest(hmi_apis::FunctionID::Navigation_SubscribeWayPoints, NULL,
                 true);
}

void SubscribeWayPointsRequest::on_event(const event_engine::Event &event) {
  LOG4CXX_AUTO_TRACE(logger_);
  ApplicationSharedPtr app =
      application_manager::ApplicationManagerImpl::instance()->application(
          connection_key());
  const smart_objects::SmartObject &message = event.smart_object();
  switch (event.id()) {
    case hmi_apis::FunctionID::Navigation_SubscribeWayPoints: {
      LOG4CXX_INFO(logger_, "Received Navigation_SubscribeWayPoints event");
      mobile_apis::Result::eType result_code =
          GetMobileResultCode(static_cast<hmi_apis::Common_Result::eType>(
              message[strings::params][hmi_response::code].asUInt()));
      bool result = mobile_apis::Result::SUCCESS == result_code;
      if (result) {
        application_manager::ApplicationManagerImpl::instance()
            ->SubscribeAppForWayPoints(app->app_id());
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
