#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_BLUETOOTH_LE_BLE_CLIENT_H_
#define SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_BLUETOOTH_LE_BLE_CLIENT_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netdb.h>
#include <atomic>

#include "protocol/raw_message.h"
#include "transport_manager/transport_adapter/transport_adapter_controller.h"
#include <utils/threads/message_loop_thread.h>

namespace transport_manager {
namespace transport_adapter {

typedef threads::MessageQueue<protocol_handler::RawMessagePtr> RawMessageLoopQueue;

class BleClient
{
public:
    using OnDataSentCallback = std::function<void(protocol_handler::RawMessagePtr)>;
    using OnConnectedCallback = std::function<void(const bool)>;

    BleClient(OnDataSentCallback&& sent_callback, OnConnectedCallback&& connected_callback);
    ~BleClient();

    void Init();
    void Run();
    void Stop();
    void Send(::protocol_handler::RawMessagePtr message);

private:
    int socket_id_;
    bool connected_;
    RawMessageLoopQueue message_queue_;
    OnConnectedCallback connected_callback_;
    OnDataSentCallback sent_callback_;

    bool TryToConnect(sockaddr_un &addr, socklen_t len);
};

}  // namespace transport_adapter
}  // namespace transport_manager

#endif  // SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_BLUETOOTH_LE_BLE_CLIENT_H_