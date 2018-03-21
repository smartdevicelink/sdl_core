#include "application_manager/application_manager.h"
#include "application_manager/commands/mobile/unsubscribe_way_points_request.h"
#include "application_manager/message_helper.h"

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

  if (!application_manager_.IsAppSubscribedForWayPoints(app)) {
    SendResponse(false, mobile_apis::Result::IGNORED);
    return;
  }

  StartAwaitForInterface(HmiInterfaces::HMI_INTERFACE_Navigation);
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
      EndAwaitForInterface(HmiInterfaces::HMI_INTERFACE_Navigation);
      const hmi_apis::Common_Result::eType result_code =
          static_cast<hmi_apis::Common_Result::eType>(
              message[strings::params][hmi_response::code].asInt());
      std::string response_info;
      GetInfo(message, response_info);
      const bool result = PrepareResultForMobileResponse(
          result_code, HmiInterfaces::HMI_INTERFACE_Navigation);
      if (result) {
        application_manager_.UnsubscribeAppFromWayPoints(app);
      }
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

bool UnSubscribeWayPointsRequest::Init() {
  hash_update_mode_ = HashUpdateMode::kDoHashUpdate;
  return true;
}

}  // namespace commands

}  // namespace application_manager
