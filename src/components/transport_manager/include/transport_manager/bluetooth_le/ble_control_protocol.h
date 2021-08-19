#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_BLE_CONTROL_PROTOCOL_H_
#define SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_BLE_CONTROL_PROTOCOL_H_

#include <vector>
#include <stdint.h>
#include <string>

namespace transport_manager {
namespace transport_adapter {

enum class BleProtocolActions {
    INVALID_ENUM = -1,
    ON_DEVICE_CONNECTED,
    ON_DEVICE_DISCONNECTED,
};

using MsgData = std::vector<uint8_t>;

struct BleControlProtocol {

    BleControlProtocol() = delete;

    static BleProtocolActions GetMessageActionType(const MsgData& message);
    static std::string GetAddress(const MsgData& message);
    static std::string GetName(const MsgData& message);

    static BleProtocolActions GetActionFromString(const std::string& str);
    static std::string GetActionString(const BleProtocolActions action);

};

}
}

#endif  // SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_BLE_CONTROL_PROTOCOL_H_
