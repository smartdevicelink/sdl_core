
#include "application_manager/smart_object_keys.h"
#include "transport_manager_metric.h"

#include "json/json.h"


namespace time_tester {

std::string TransportManagerMectic::GetStyledString() {
    Json::Value result;
    result["logger"] = "TransportManager";
    result["begin"] = static_cast<uint32_t>(message_metric->begin);
    result["end"] = static_cast<uint32_t>(message_metric->end);
    result["data_size"] = message_metric->data_size;
    return result.toStyledString();
}

}
