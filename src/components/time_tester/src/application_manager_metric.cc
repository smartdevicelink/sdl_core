
#include "application_manager/smart_object_keys.h"
#include "application_manager_metric.h"



namespace time_tester {

std::string ApplicationManagerMectic::GetStyledString() {
    Json::Value result;
    result["logger"] = "ApplicationManager";
    result["begin"] = static_cast<uint32_t>(message_metric->begin);
    result["end"] = static_cast<uint32_t>(message_metric->end);
    int32_t mid = message_metric->message->getElement(application_manager::strings::params)[application_manager::strings::correlation_id].asInt();
    result["MessageID"] = mid;
    return result.toStyledString();
}

}
