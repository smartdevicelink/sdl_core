#include "transport_manager/bluetooth_le/ble_control_protocol.h"

#include "utils/logger.h"
#include "utils/jsoncpp_reader_wrapper.h"
#include "utils/helpers.h"

#include <map>
#include <algorithm>

namespace transport_manager {
namespace transport_adapter {

SDL_CREATE_LOG_VARIABLE("TransportManager")

namespace {
    std::map<BleProtocolActions, std::string> mapActionNames = {
    {BleProtocolActions::ON_DEVICE_CONNECTED, "ON_DEVICE_CONNECTED"},
    {BleProtocolActions::ON_DEVICE_DISCONNECTED, "ON_DEVICE_DISCONNECTED"}};

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

BleProtocolActions BleControlProtocol::GetMessageActionType(const MsgData& message) {
    const auto json = GetParsedJson(message);
    return GetActionFromString(GetStrFromJson(json, cActionKey));
}

BleProtocolActions BleControlProtocol::GetActionFromString(const std::string& str) {
    BleProtocolActions result = BleProtocolActions::INVALID_ENUM;

    const auto findResult = std::find_if(mapActionNames.begin(), mapActionNames.end(),
        [&str](const std::pair<BleProtocolActions, std::string> &pair) {
        return pair.second == str;});

    if(findResult != mapActionNames.end()) {
        result = findResult->first;
    }    

    return result;
}

std::string BleControlProtocol::GetActionString(const BleProtocolActions action) {
    auto it = mapActionNames.find(action);
    return it != mapActionNames.end() ? it->second : "UNKNOWN";
}

std::string BleControlProtocol::GetAddress(const MsgData& message) {
    const auto json = GetParsedJson(message);
    return GetStrFromJson(json.get(cParamsKey, Json::Value::null), cAddressKey);
}

std::string BleControlProtocol::GetName(const MsgData& message) {
    const auto json = GetParsedJson(message);
    return GetStrFromJson(json.get(cParamsKey, Json::Value::null), cNameKey);
}

}
}
