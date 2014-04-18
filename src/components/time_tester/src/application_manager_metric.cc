
#include "application_manager/smart_object_keys.h"
#include "application_manager_metric.h"
#include "json_keys.h"


namespace time_tester {

std::string ApplicationManagerMectic::GetStyledString() {
    Json::Value result;
    result[strings::logger] = "ApplicationManager";
    result[strings::begin] = static_cast<uint32_t>(message_metric->begin);
    result[strings::end] = static_cast<uint32_t>(message_metric->end);
    int32_t mid = message_metric->message->getElement(application_manager::strings::params)[application_manager::strings::correlation_id].asInt();
    int32_t connection_key = message_metric->message->getElement(application_manager::strings::params)[application_manager::strings::connection_key].asInt();
    result[strings::correlation_id] = mid;
    result[strings::connection_key] = connection_key;
    return result.toStyledString();
}

}
