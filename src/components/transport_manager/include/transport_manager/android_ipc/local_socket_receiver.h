#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_ANDROID_IPC_LOCAL_SOCKET_RECEIVER_H_
#define SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_ANDROID_IPC_LOCAL_SOCKET_RECEIVER_H_
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
#include "transport_manager/android_ipc/ipc_receiver.h"

namespace transport_manager {
namespace transport_adapter {

class LocalSocketReceiver: public IpcReceiver
{
public:
    using MessageDelegate = std::function<void(const std::vector<uint8_t>&)>;

    static constexpr char WriterSocketName[]  = "./localBleWriter";
    static constexpr char ControlSocketName[] = "./localBleControl";

    LocalSocketReceiver(const std::string& socket_name, MessageDelegate&& callback);
    ~LocalSocketReceiver();
    void Init() override;
    void Run() override;
    void Stop() override;

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

#endif  // SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_ANDROID_IPC_LOCAL_SOCKET_RECEIVER_H_