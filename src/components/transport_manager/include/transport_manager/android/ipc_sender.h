#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_ANDROID_IPC_SENDER_H_
#define SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_ANDROID_IPC_SENDER_H_

#include "protocol/raw_message.h"

namespace transport_manager {
namespace transport_adapter {

class IpcSender
{
public:

    virtual ~IpcSender() {}

    /**
    * @brief Initialize sender
    * @param socket_name local socket name
    */
    virtual void Init(const std::string& ipc_name) = 0;

    /**
    * @brief Run sending cycle
    */
    virtual void Run() = 0;
    
    /**
    * @brief Run sending
    */
    virtual void Stop() = 0;
    
    /**
    * @brief Send message
    * @param message message to send
    */
    virtual void Send(::protocol_handler::RawMessagePtr message) = 0;

};

}  // namespace transport_adapter
}  // namespace transport_manager

#endif  // SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_ANDROID_IPC_SENDER_H_