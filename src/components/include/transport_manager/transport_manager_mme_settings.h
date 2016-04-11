#ifndef TRANSPORT_MANAGER_MME_SETTINGS_H
#define TRANSPORT_MANAGER_MME_SETTINGS_H
#include <stdint.h>
#include <string>
#include <vector>

namespace transport_manager {
class TransportManagerMMESettings {
 public:
  virtual const std::string& event_mq_name() const = 0;

  virtual const std::string& ack_mq_name() const = 0;

  virtual uint32_t iap2_hub_connect_attempts() const = 0;

  virtual uint32_t default_hub_protocol_index() const = 0;

  virtual const std::string& iap_legacy_protocol_mask() const = 0;

  virtual const std::string& iap_hub_protocol_mask() const = 0;

  virtual const std::string& iap_pool_protocol_mask() const = 0;

  virtual const std::string& iap_system_config() const = 0;

  virtual const std::string& iap2_system_config() const = 0;

  virtual uint32_t iap_hub_connection_wait_timeout() const = 0;
};
}
#endif  // TRANSPORT_MANAGER_MME_SETTINGS_H
