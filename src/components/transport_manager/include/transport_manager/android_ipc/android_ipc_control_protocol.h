#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_ANDROID_IPC_ANDROID_IPC_CONTROL_PROTOCOL_H_
#define SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_ANDROID_IPC_ANDROID_IPC_CONTROL_PROTOCOL_H_

#include <vector>
#include <stdint.h>
#include <string>

namespace transport_manager {
namespace transport_adapter {

enum class AndroidIpcProtocolActions {
    INVALID_ENUM = -1,
    ON_DEVICE_CONNECTED,
    ON_DEVICE_DISCONNECTED,
};

using MsgData = std::vector<uint8_t>;

struct AndroidIpcControlProtocol {

    AndroidIpcControlProtocol() = delete;

    static AndroidIpcProtocolActions GetMessageActionType(const MsgData& message);
    static std::string GetAddress(const MsgData& message);
    static std::string GetName(const MsgData& message);

    static AndroidIpcProtocolActions GetActionFromString(const std::string& str);
    static std::string GetActionString(const AndroidIpcProtocolActions action);

};

}
}

#endif  // SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_ANDROID_IPC_ANDROID_IPC_CONTROL_PROTOCOL_H_
