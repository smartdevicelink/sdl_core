
#include "transport_manager_metric.h"
#include "json/json.h"
#include "json_keys.h"
#include "application_manager/smart_object_keys.h"




namespace time_tester {

std::string TransportManagerMectic::GetStyledString() {
    Json::Value result;
    result[strings::logger] = "TransportManager";
    result[strings::begin] = static_cast<uint32_t>(message_metric->begin);
    result[strings::end] = static_cast<uint32_t>(message_metric->end);
    result[strings::data_size] = message_metric->data_size;
    return result.toStyledString();
}

}
