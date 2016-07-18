#include "application_manager/application_manager.h"
#include "application_manager/commands/mobile/get_way_points_request.h"

namespace application_manager {

namespace commands {

namespace {
using NsSmartDeviceLink::NsSmartObjects::SmartType_Map;
using NsSmartDeviceLink::NsSmartObjects::SmartType_String;
using NsSmartDeviceLink::NsSmartObjects::SmartType_Array;
typedef std::set<std::string>::const_iterator ConstSetIter;
}

GetWayPointsRequest::GetWayPointsRequest(
    const MessageSharedPtr& message, ApplicationManager& application_manager)
    : CommandRequestImpl(message, application_manager) {}

GetWayPointsRequest::~GetWayPointsRequest() {}

void GetWayPointsRequest::Run() {
  SDL_AUTO_TRACE();

  ApplicationSharedPtr app = application_manager_.application(connection_key());

  if (!app) {
    SDL_ERROR("An application with connection key " << connection_key()
                                                    << " is not registered.");
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

bool GetWayPointsRequest::CheckResponseSyntax(
    const smart_objects::SmartObject& obj) {
  const char* str = obj.asCharArray();
  const bool is_str_empty = str && !str[0];
  return is_str_empty || CheckSyntax(str);
}

bool GetWayPointsRequest::ValidateSmartMapResponse(
    const smart_objects::SmartObject& obj, ConstSetIter key) {
  const smart_objects::SmartObject& value = obj[*key];
  switch (value.getType()) {
    case SmartType_Array:
    case SmartType_Map: {
      return ValidateResponseFromHMI(value);
    }
    case SmartType_String: {
      return CheckResponseSyntax(value);
    }
    default:
      // Catch SmartType_Integer or SmartType_Double
      break;
  }
  return true;
}

bool GetWayPointsRequest::ValidateResponseFromHMI(
    const smart_objects::SmartObject& obj) {
  switch (obj.getType()) {
    case SmartType_Array: {
      for (uint32_t i = 0; i < obj.length(); ++i) {
        if (!ValidateResponseFromHMI(obj[i])) {
          return false;
        }
      }
      break;
    }
    case SmartType_Map: {
      const std::set<std::string> keys = obj.enumerate();
      for (ConstSetIter key = keys.begin(); key != keys.end(); key++) {
        if (!ValidateSmartMapResponse(obj, key)) {
          return false;
        }
      }
      break;
    }
    case SmartType_String: {
      return CheckResponseSyntax(obj);
    }
    default:
      // Catch SmartType_Integer or SmartType_Double
      break;
  }
  return true;
}

void GetWayPointsRequest::onTimeOut() {
  SDL_AUTO_TRACE();
  SendResponse(false,
               mobile_apis::Result::GENERIC_ERROR,
               "Invalid response from system");
}

void GetWayPointsRequest::on_event(const event_engine::Event& event) {
  SDL_AUTO_TRACE();
  smart_objects::SmartObject message = event.smart_object();
  switch (event.id()) {
    case hmi_apis::FunctionID::Navigation_GetWayPoints: {
      SDL_INFO("Received Navigation_GetWayPoints event");
      const bool all_checks_passed = ValidateResponseFromHMI(
          message[strings::msg_params][strings::way_points]);
      const char* response_info =
          message[strings::params][hmi_response::message].asCharArray();
      mobile_apis::Result::eType result_code =
          GetMobileResultCode(static_cast<hmi_apis::Common_Result::eType>(
              message[strings::params][hmi_response::code].asUInt()));
      const bool result = mobile_apis::Result::SUCCESS == result_code;

      if(!result) {
        SendResponse(false,
                     result_code,
                     "Invalid response from system");
        return;
      }
      if (!CheckSyntax(response_info)) {
        response_info = NULL;
      }

      message[strings::msg_params].erase(strings::info);
      if (all_checks_passed) {
        SendResponse(result,
                     result_code,
                     response_info,
                     &(message[strings::msg_params]));
      } else {
        SendResponse(false,
                     mobile_apis::Result::GENERIC_ERROR,
                     "Invalid response from system");
      }
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
