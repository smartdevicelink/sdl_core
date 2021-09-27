#include "transport_manager/android/android_ipc_control_protocol.h"

#include "utils/logger.h"
#include "utils/jsoncpp_reader_wrapper.h"
#include "utils/helpers.h"

#include <map>
#include <algorithm>

namespace transport_manager {
namespace transport_adapter {

SDL_CREATE_LOG_VARIABLE("TransportManager")

namespace {
    std::map<AndroidIpcProtocolActions, std::string> mapActionNames = {
    {AndroidIpcProtocolActions::ON_DEVICE_CONNECTED, "ON_DEVICE_CONNECTED"},
    {AndroidIpcProtocolActions::ON_DEVICE_DISCONNECTED, "ON_DEVICE_DISCONNECTED"}};

    const char cActionKey[] = "action";
    const char cNameKey[] = "name";
    const char cAddressKey[] = "address";
    const char cParamsKey[] = "params";

    Json::Value GetParsedJson(const MsgData& message) {
        utils::JsonReader reader_;
        Json::Value value;
        if (reader_.parse((char*)(message.data()), &value)) {
            return value;
        }
        return Json::Value::null;
    }
    
    std::string GetStrFromJson(const Json::Value& value, const std::string& key) {
        if (value == Json::Value::null) {
            return std::string();
        }
        auto params = value.get(key, Json::Value::null);
        if (params == Json::Value::null) {
            return std::string();
        }
        return params.asString();
    }

}

AndroidIpcProtocolActions AndroidIpcControlProtocol::GetMessageActionType(const MsgData& message) {
    const auto json = GetParsedJson(message);
    return GetActionFromString(GetStrFromJson(json, cActionKey));
}

AndroidIpcProtocolActions AndroidIpcControlProtocol::GetActionFromString(const std::string& str) {
    AndroidIpcProtocolActions result = AndroidIpcProtocolActions::INVALID_ENUM;

    const auto findResult = std::find_if(mapActionNames.begin(), mapActionNames.end(),
        [&str](const std::pair<AndroidIpcProtocolActions, std::string> &pair) {
        return pair.second == str;});

    if(findResult != mapActionNames.end()) {
        result = findResult->first;
    }    

    return result;
}

std::string AndroidIpcControlProtocol::GetActionString(const AndroidIpcProtocolActions action) {
    auto it = mapActionNames.find(action);
    return it != mapActionNames.end() ? it->second : "UNKNOWN";
}

std::string AndroidIpcControlProtocol::GetAddress(const MsgData& message) {
    const auto json = GetParsedJson(message);
    return GetStrFromJson(json.get(cParamsKey, Json::Value::null), cAddressKey);
}

std::string AndroidIpcControlProtocol::GetName(const MsgData& message) {
    const auto json = GetParsedJson(message);
    return GetStrFromJson(json.get(cParamsKey, Json::Value::null), cNameKey);
}

}
}
