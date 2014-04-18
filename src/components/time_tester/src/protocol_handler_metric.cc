
#include "application_manager/smart_object_keys.h"
#include "protocol_handler_metric.h"

#include "json/json.h"


namespace time_tester {

std::string ProtocolHandlerMectic::GetStyledString() {
  Json::Value result;
  result["logger"] = "ProtocolHandler";
  result["begin"] = static_cast<uint32_t>(message_metric->begin);
  result["end"] = static_cast<uint32_t>(message_metric->end);
  result["message_id"] = message_metric->message_id;
  result["session_id"] = message_metric->session_id;
  return result.toStyledString();
}

}
