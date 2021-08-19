#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_BLUETOOTH_LE_BLE_SERVER_H_
#define SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_BLUETOOTH_LE_BLE_SERVER_H_
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netdb.h>

#include "transport_manager/transport_adapter/transport_adapter_controller.h"

namespace transport_manager {
namespace transport_adapter {

class BleServer
{
public:
    using MessageDelegate = std::function<void(const std::vector<uint8_t>&)>;

    static constexpr char WriterSocketName[]  = "./localBleWriter";
    static constexpr char ControlSocketName[] = "./localBleControl";

    BleServer(const std::string& socket_name, MessageDelegate&& callback);
    ~BleServer();
    void Init();
    void Run();
    void Stop();

private:
    int server_sock_;
    int client_sock_;
    bool connected_;
    std::atomic_bool stop_requested_;
    std::string socket_name_;
    MessageDelegate callback_;
};

}  // namespace transport_adapter
}  // namespace transport_manager

#endif  // SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_BLUETOOTH_LE_BLE_SERVER_H_