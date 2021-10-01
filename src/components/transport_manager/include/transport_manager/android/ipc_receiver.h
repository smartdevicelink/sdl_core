#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_ANDROID_IPC_RECEIVER_H_
#define SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_ANDROID_IPC_RECEIVER_H_

#include "transport_manager/transport_adapter/transport_adapter_controller.h"

namespace transport_manager {
namespace transport_adapter {

class IpcReceiver
{
public:
    virtual ~IpcReceiver() {}

   /**
   * @brief Initialize receiver
   * @param ipc_name local socket name
   */
    virtual void Init(const std::string& ipc_name) = 0;

   /**
   * @brief Run receiving cycle
   */
    virtual void Run() = 0;

   /**
   * @brief Stop receiving
   */
    virtual void Stop() = 0;
};

}  // namespace transport_adapter
}  // namespace transport_manager

#endif  // SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_ANDROID_IPC_RECEIVER_H_
