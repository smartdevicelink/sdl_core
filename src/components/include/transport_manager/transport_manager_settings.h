#ifndef TRANSPORT_MANAGER_SETTINGS
#define TRANSPORT_MANAGER_SETTINGS
#include "transport_manager/transport_manager_mme_settings.h"
namespace transport_manager {
class TransportManagerSettings : public TransportManagerMMESettings {
 public:
  /**
   * @brief Returns true if last state singleton is used
   */
  virtual bool use_last_state() const = 0;

  /**
   * @brief Timeout in transport manager before disconnect
   */
  virtual uint32_t transport_manager_disconnect_timeout() const = 0;

  /**
   * @brief Returns port for TCP transport adapter
   */
  virtual uint16_t transport_manager_tcp_adapter_port() const = 0;
};
}
#endif  // TRANSPORT_MANAGER_SETTINGS
