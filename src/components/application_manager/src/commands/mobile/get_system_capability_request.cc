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
  mobile_apis::SystemCapabilityType::eType response_type =
      static_cast<mobile_apis::SystemCapabilityType::eType>(
          (*message_)[strings::msg_params][strings::system_capability_type]
              .asInt());
  response_params[strings::system_capability][strings::system_capability_type] =
      response_type;

  const DataAccessor<HMICapabilities> hmi_capabilities_accessor = application_manager_.const_hmi_capabilities();
  const HMICapabilities& hmi_capabilities = hmi_capabilities_accessor.GetData();

  switch (response_type) {
    case mobile_apis::SystemCapabilityType::NAVIGATION: {
      if (hmi_capabilities.navigation_capability()) {
        response_params[strings::system_capability]
                       [strings::navigation_capability] = 
                           *hmi_capabilities.navigation_capability();
      } else {
        SendResponse(false, mobile_apis::Result::DATA_NOT_AVAILABLE);
        return;
      }
      break;
    }
    case mobile_apis::SystemCapabilityType::PHONE_CALL: {
      if (hmi_capabilities.phone_capability()) {
        response_params[strings::system_capability][strings::phone_capability] =
            *hmi_capabilities.phone_capability();
      } else {
        SendResponse(false, mobile_apis::Result::DATA_NOT_AVAILABLE);
        return;
      }
      break;
    }
    case mobile_apis::SystemCapabilityType::REMOTE_CONTROL: {
      if (hmi_capabilities.rc_capability()) {
        response_params[strings::system_capability][strings::rc_capability] =
            *hmi_capabilities.rc_capability();
      } else {
        SendResponse(false, mobile_apis::Result::DATA_NOT_AVAILABLE);
        return;
      }
      break;
    }
    case mobile_apis::SystemCapabilityType::VIDEO_STREAMING:
      if (hmi_capabilities.video_streaming_capability()) {
        response_params[strings::system_capability]
                       [strings::video_streaming_capability] =
                           *hmi_capabilities.video_streaming_capability();
      } else {
        SendResponse(false, mobile_apis::Result::DATA_NOT_AVAILABLE);
        return;
      }
      break;
    default:  // Return unsupported resource
      SendResponse(false, mobile_apis::Result::UNSUPPORTED_RESOURCE);
      return;
  }
  SendResponse(true, mobile_apis::Result::SUCCESS, NULL, &response_params);
}

void GetSystemCapabilityRequest::on_event(const event_engine::Event& event) {
  LOG4CXX_INFO(logger_, "GetSystemCapabilityRequest on_event");
}

}  // namespace commands

}  // namespace application_manager
