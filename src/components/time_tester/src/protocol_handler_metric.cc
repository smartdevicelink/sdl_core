
#include "protocol_handler_metric.h"
#include "json/json.h"
#include "json_keys.h"



namespace time_tester {

std::string ProtocolHandlerMectic::GetStyledString() {
  Json::Value result;
  result[strings::logger] = "ProtocolHandler";
  result[strings::begin] = static_cast<uint32_t>(message_metric->begin);
  result[strings::end] = static_cast<uint32_t>(message_metric->end);
  result[strings::message_id] = message_metric->message_id;
  result[strings::connection_key] = message_metric->connection_key;
  return result.toStyledString();
}

}
