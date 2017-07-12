#include "application_manager/commands/mobile/get_system_capability_request.h"

namespace application_manager {

namespace commands {

GetSystemCapabilityRequest::GetSystemCapabilityRequest(
    const MessageSharedPtr& message, ApplicationManager& application_manager)
    : CommandRequestImpl(message, application_manager) {}

GetSystemCapabilityRequest::~GetSystemCapabilityRequest() {}

void GetSystemCapabilityRequest::Run() {
  LOG4CXX_AUTO_TRACE(logger_);

  ApplicationSharedPtr app = application_manager_.application(connection_key());

  if (!app) {
    LOG4CXX_ERROR(logger_, "Application is not registered");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return;
  }

  if ((*message_)[strings::msg_params].empty()) {
    LOG4CXX_ERROR(logger_, strings::msg_params << " is empty.");
    SendResponse(false, mobile_apis::Result::INVALID_DATA);
    return;
  }
  smart_objects::SmartObject response_params(smart_objects::SmartType_Map);

  response_params[strings::system_capability][strings::system_capability_type] = mobile_apis::SystemCapabilityType::PHONE_CALL;
  LOG4CXX_INFO(logger_ ,"Sanity " << response_params[strings::system_capability_type].asInt());
  SendResponse(true, mobile_apis::Result::SUCCESS, NULL, &response_params);
}

void GetSystemCapabilityRequest::on_event(const event_engine::Event& event){
  LOG4CXX_INFO(logger_, "GetSystemCapabilityRequest onEVENTs");
}

} // namespace commands

} // namespace application_manager