#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_TCP_PLATFORM_SPECIFIC_QNX_PLATFORM_SPECIFIC_NETWORK_INTERFACE_LISTENER_H_
#define SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_TCP_PLATFORM_SPECIFIC_QNX_PLATFORM_SPECIFIC_NETWORK_INTERFACE_LISTENER_H_

#include <map>
#include <string>
#include <vector>

#include <netinet/in.h>
#include "transport_manager/tcp/network_interface_listener.h"
#include "utils/macro.h"
#include "utils/threads/thread_delegate.h"

class Thread;
struct ifaddrmsg;

namespace transport_manager {
namespace transport_adapter {

class TcpClientListener;

/**
 * @brief Listener to detect various events on network interfaces
 */
class PlatformSpecificNetworkInterfaceListener
    : public NetworkInterfaceListener {
 public:
  /**
   * @brief Constructor
   *
   * @param tcp_client_listener  an instance of TcpClientListener which receives
   *                             status updates
   * @param designated_interface  if we want to listen only on a specific
   *                              network interface, specify its name
   */
  PlatformSpecificNetworkInterfaceListener(
      TcpClientListener* tcp_client_listener,
      const std::string designated_interface = "");

  /**
   * @brief Destructor
   */
  virtual ~PlatformSpecificNetworkInterfaceListener();

  /**
   * @brief Initialize this listener
   */
  bool Init() OVERRIDE;

  /**
   * @brief Deinitialize this listener
   */
  void Deinit() OVERRIDE;

  /**
   * @brief Start this listener
   */
  bool Start() OVERRIDE;

  /**
   * @brief Stop this listener
   */
  bool Stop() OVERRIDE;

 private:
};

}  // namespace transport_adapter
}  // namespace transport_manager

#endif  // SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_TCP_PLATFORM_SPECIFIC_QNX_PLATFORM_SPECIFIC_NETWORK_INTERFACE_LISTENER_H_
