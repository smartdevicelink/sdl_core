#ifndef SRC_COMPONENTS_INCLUDE_PROTOCOL_HANDLER_PROTOCOL_HANDLER_SETTINGS_H_
#define SRC_COMPONENTS_INCLUDE_PROTOCOL_HANDLER_PROTOCOL_HANDLER_SETTINGS_H_

#include <cstdint>
#include <string>
#include <vector>

namespace protocol_handler {
/**
 * \class ProtocolHandlerSettings
 * \brief Interface for protocol handler component settings.
 */
class ProtocolHandlerSettings {
 public:
  virtual ~ProtocolHandlerSettings() {}

  virtual size_t maximum_payload_size() const = 0;
  virtual size_t message_frequency_count() const = 0;
  virtual size_t message_frequency_time() const = 0;
  virtual bool malformed_message_filtering() const = 0;
  virtual size_t malformed_frequency_count() const = 0;
  virtual size_t malformed_frequency_time() const = 0;
  /*
   * @brief Heartbeat timeout before closing connection
   */
  virtual uint32_t heart_beat_timeout() const = 0;

  virtual uint16_t max_supported_protocol_version() const = 0;
  /*
   * @brief Protocol version, from .ini file.
   */
  virtual bool enable_protocol_4() const = 0;

  virtual uint32_t multiframe_waiting_timeout() const = 0;
#ifdef ENABLE_SECURITY
  /**
   * @brief Returns force protected services
   */
  virtual const std::vector<int>& force_protected_service() const = 0;
  /**
   * @brief Returns force unprotected services
   */
  virtual const std::vector<int>& force_unprotected_service() const = 0;
#endif  // ENABLE_SECURITY
};
}  // namespace protocol_handler
#endif  // SRC_COMPONENTS_INCLUDE_PROTOCOL_HANDLER_PROTOCOL_HANDLER_SETTINGS_H_
