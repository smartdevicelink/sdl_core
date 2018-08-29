#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_TCP_NETWORK_INTERFACE_LISTENER_H_
#define SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_TCP_NETWORK_INTERFACE_LISTENER_H_

namespace transport_manager {
namespace transport_adapter {

/**
 * @brief Listener to detect various events on network interfaces
 */
class NetworkInterfaceListener {
 public:
  /**
   * @brief Destructor
   */
  virtual ~NetworkInterfaceListener() {}

  /**
   * @brief Initialize this listener
   */
  virtual bool Init() = 0;

  /**
   * @brief Deinitialize this listener
   */
  virtual void Deinit() = 0;

  /**
   * @brief Start this listener
   */
  virtual bool Start() = 0;

  /**
   * @brief Stop this listener
   */
  virtual bool Stop() = 0;
};

}  // namespace transport_adapter
}  // namespace transport_manager

#endif  // SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_TCP_NETWORK_INTERFACE_LISTENER_H_
