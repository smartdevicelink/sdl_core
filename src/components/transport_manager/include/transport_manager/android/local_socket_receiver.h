#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_ANDROID_LOCAL_SOCKET_RECEIVER_H_
#define SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_ANDROID_LOCAL_SOCKET_RECEIVER_H_

#include <atomic>
#include <functional>
#include "transport_manager/android/ipc_receiver.h"

namespace transport_manager {
namespace transport_adapter {

class LocalSocketReceiver : public IpcReceiver {
 public:
  using MessageDelegate = std::function<void(const std::vector<uint8_t>&)>;
  using ChannelNameGetter = std::function<std::string()>;

  LocalSocketReceiver(MessageDelegate&& callback,
                      ChannelNameGetter&& channelNameGetter);
  ~LocalSocketReceiver();
  void Init(const std::string& socket_name) override;
  void Run() override;
  void Stop() override;

 private:
  int server_sock_;
  int client_sock_;
  bool connected_;
  std::atomic_bool stop_requested_;
  MessageDelegate callback_;
  ChannelNameGetter channelNameGetter_;
};

}  // namespace transport_adapter
}  // namespace transport_manager

#endif  // SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_ANDROID_LOCAL_SOCKET_RECEIVER_H_
