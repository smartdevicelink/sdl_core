#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_TCP_NETWORK_INTERFACE_LISTENER_IMPL_H_
#define SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_TCP_NETWORK_INTERFACE_LISTENER_IMPL_H_

#include <string>
#include <memory>

#include "utils/macro.h"
#include "transport_manager/tcp/network_interface_listener.h"

namespace transport_manager {
namespace transport_adapter {

class TcpClientListener;

/**
 * @brief Listener to detect various events on network interfaces
 */
class NetworkInterfaceListenerImpl : public NetworkInterfaceListener {
 public:
  /**
   * @brief Constructor
   *
   * @param tcp_client_listener  an instance of TcpClientListener which receives
   *                             status updates
   * @param designated_interface  if we want to listen only on a specific
   *                              network interface, specify its name
   */
  NetworkInterfaceListenerImpl(TcpClientListener* tcp_client_listener,
                               const std::string designated_interface);

  /**
   * @brief Destructor
   */
  virtual ~NetworkInterfaceListenerImpl();

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
  std::unique_ptr<NetworkInterfaceListener> platform_specific_impl_;
};

}  // namespace transport_adapter
}  // namespace transport_manager

#endif  // SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_TCP_NETWORK_INTERFACE_LISTENER_IMPL_H_
