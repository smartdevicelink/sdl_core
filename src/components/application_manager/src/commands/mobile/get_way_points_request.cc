#include "application_manager/application_manager.h"
#include "application_manager/commands/mobile/get_way_points_request.h"
#include "application_manager/message_helper.h"

namespace application_manager {

namespace commands {

GetWayPointsRequest::GetWayPointsRequest(
    const MessageSharedPtr& message, ApplicationManager& application_manager)
    : CommandRequestImpl(message, application_manager) {}

GetWayPointsRequest::~GetWayPointsRequest() {}

void GetWayPointsRequest::Run() {
  LOG4CXX_AUTO_TRACE(logger_);

  ApplicationSharedPtr app = application_manager_.application(connection_key());

  if (!app) {
    LOG4CXX_ERROR(logger_,
                  "An application with connection key "
                      << connection_key() << " is not registered.");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return;
  }
  smart_objects::SmartObject msg_params =
      smart_objects::SmartObject(smart_objects::SmartType_Map);

  msg_params = (*message_)[strings::msg_params];
  msg_params[strings::app_id] = app->app_id();
  SendHMIRequest(hmi_apis::FunctionID::Navigation_GetWayPoints,
                 msg_params.empty() ? NULL : &msg_params,
                 true);
}

void GetWayPointsRequest::on_event(const event_engine::Event& event) {
  LOG4CXX_AUTO_TRACE(logger_);
  const smart_objects::SmartObject& message = event.smart_object();
  switch (event.id()) {
    case hmi_apis::FunctionID::Navigation_GetWayPoints: {
      LOG4CXX_INFO(logger_, "Received Navigation_GetWayPoints event");
      const hmi_apis::Common_Result::eType result_code =
          static_cast<hmi_apis::Common_Result::eType>(
              message[strings::params][hmi_response::code].asInt());
      std::string response_info;
      GetInfo(message, response_info);
      const bool result = PrepareResultForMobileResponse(
          result_code, HmiInterfaces::HMI_INTERFACE_Navigation);
      SendResponse(result,
                   MessageHelper::HMIToMobileResult(result_code),
                   response_info.empty() ? NULL : response_info.c_str(),
                   &(message[strings::msg_params]));
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
