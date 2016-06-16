#include "application_manager/application_manager.h"
#include "application_manager/commands/mobile/subscribe_way_points_request.h"

namespace application_manager {

namespace commands {

SubscribeWayPointsRequest::SubscribeWayPointsRequest(
    const MessageSharedPtr& message, ApplicationManager& application_manager)
    : CommandRequestImpl(message, application_manager) {}

SubscribeWayPointsRequest::~SubscribeWayPointsRequest() {}

void SubscribeWayPointsRequest::Run() {
  SDL_AUTO_TRACE();

  ApplicationSharedPtr app = application_manager_.application(connection_key());

  if (!app) {
    SDL_ERROR("An application with connection key " << connection_key()
                                                    << " is not registered.");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return;
  }

  if (application_manager_.IsAppSubscribedForWayPoints(app->app_id())) {
    SendResponse(false, mobile_apis::Result::IGNORED);
    return;
  }

  if (application_manager_.IsAnyAppSubscribedForWayPoints()) {
    application_manager_.SubscribeAppForWayPoints(app->app_id());
    SendResponse(true, mobile_apis::Result::SUCCESS);
    return;
  }

  SendHMIRequest(
      hmi_apis::FunctionID::Navigation_SubscribeWayPoints, NULL, true);
}

void SubscribeWayPointsRequest::on_event(const event_engine::Event& event) {
  SDL_AUTO_TRACE();
  ApplicationSharedPtr app = application_manager_.application(connection_key());
  const smart_objects::SmartObject& message = event.smart_object();
  switch (event.id()) {
    case hmi_apis::FunctionID::Navigation_SubscribeWayPoints: {
      SDL_INFO("Received Navigation_SubscribeWayPoints event");
      mobile_apis::Result::eType result_code =
          GetMobileResultCode(static_cast<hmi_apis::Common_Result::eType>(
              message[strings::params][hmi_response::code].asUInt()));
      bool result = mobile_apis::Result::SUCCESS == result_code;
      if (result) {
        application_manager_.SubscribeAppForWayPoints(app->app_id());
      }
      SendResponse(result, result_code, NULL, &(message[strings::msg_params]));
      break;
    }
    default: {
      SDL_ERROR("Received unknown event" << event.id());
      break;
    }
  }
}

}  // namespace commands

}  // namespace application_manager
