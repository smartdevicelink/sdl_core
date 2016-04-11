#include "application_manager/application_manager.h"
#include "application_manager/commands/mobile/get_way_points_request.h"

namespace application_manager {

namespace commands {

GetWayPointsRequest::GetWayPointsRequest(
    const MessageSharedPtr& message, ApplicationManager& application_manager)
    :CommandRequestImpl(message, application_manager){
}

GetWayPointsRequest::~GetWayPointsRequest() {}

void GetWayPointsRequest::Run() {
  LOG4CXX_AUTO_TRACE(logger_);

  ApplicationSharedPtr app =
        application_manager_.application(
            connection_key());

  if (!app) {
      LOG4CXX_ERROR(logger_, "An application with connection key "
                                 << connection_key() << " is not registered.");
      SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
      return;
  }

  SendHMIRequest(hmi_apis::FunctionID::Navigation_GetWayPoints, NULL, true);
}

void GetWayPointsRequest::on_event(const event_engine::Event& event) {
  LOG4CXX_AUTO_TRACE(logger_);
  ApplicationSharedPtr app =
        application_manager_.application(
            connection_key());
  const smart_objects::SmartObject &message = event.smart_object();
  switch(event.id()) {
    case hmi_apis::FunctionID::Navigation_GetWayPoints: {
      LOG4CXX_INFO(logger_, "Received Navigation_GetWayPoints event");
      mobile_apis::Result::eType result_code =
                GetMobileResultCode(static_cast<hmi_apis::Common_Result::eType>(
                    message[strings::params][hmi_response::code].asUInt()));
      bool result = mobile_apis::Result::SUCCESS == result_code;
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
