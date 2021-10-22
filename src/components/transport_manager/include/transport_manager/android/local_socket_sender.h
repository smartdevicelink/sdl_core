#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_ANDROID_LOCAL_SOCKET_SENDER_H_
#define SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_ANDROID_LOCAL_SOCKET_SENDER_H_

#include <sys/socket.h>
#include <sys/un.h>

#include <utils/threads/message_loop_thread.h>
#include "protocol/raw_message.h"
#include "transport_manager/android/ipc_sender.h"
#include "transport_manager/transport_adapter/transport_adapter_controller.h"

namespace transport_manager {
namespace transport_adapter {

typedef threads::MessageQueue<protocol_handler::RawMessagePtr>
    RawMessageLoopQueue;

class LocalSocketSender : public IpcSender {
 public:
  using OnDataSentCallback =
      std::function<void(protocol_handler::RawMessagePtr)>;
  using OnConnectedCallback = std::function<void(const bool)>;
  using ChannelNameGetter = std::function<std::string()>;

  LocalSocketSender(OnDataSentCallback&& sent_callback,
                    OnConnectedCallback&& connected_callback,
                    ChannelNameGetter&& channelNameGetter);
  ~LocalSocketSender();

  void Init(const std::string& socket_name) override;
  void Run() override;
  void Stop() override;
  void Send(::protocol_handler::RawMessagePtr message) override;

 private:
  int socket_id_;
  bool connected_;
  RawMessageLoopQueue message_queue_;
  OnConnectedCallback connected_callback_;
  OnDataSentCallback sent_callback_;
  ChannelNameGetter channelNameGetter_;

  bool TryToConnect(sockaddr_un& addr, socklen_t len);
};

}  // namespace transport_adapter
}  // namespace transport_manager

#endif  // SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_ANDROID_LOCAL_SOCKET_SENDER_H_
