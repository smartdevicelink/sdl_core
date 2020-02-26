/*
 * \file websocket_transport_adapter.h
 * \brief WebSocketTransportAdapter class header file.
 *
 * Copyright (c) 2020, Ford Motor Company
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of the Ford Motor Company nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_WEBSOCKET_SERVER_WEBSOCKET_SERVER_TRANSPORT_ADAPTER_H_
#define SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_WEBSOCKET_SERVER_WEBSOCKET_SERVER_TRANSPORT_ADAPTER_H_

#include <string>
#include "transport_manager/transport_adapter/transport_adapter_impl.h"

namespace transport_manager {
namespace transport_adapter {

/**
 * @brief Transport adapter that uses WebSocket transport.
 */
class WebSocketServerTransportAdapter : public TransportAdapterImpl {
 public:
  /**
   * @brief Constructor.
   */
  WebSocketServerTransportAdapter(
      resumption::LastStateWrapperPtr last_state_wrapper,
      const TransportManagerSettings& settings);

  /**
   * @brief Destructor.
   */
  virtual ~WebSocketServerTransportAdapter();

  /**
   * @brief Websocket server transport adapter
   * specific Init() method
   * @note Perform additional actions required by WS transport adapter
   * then calls basic class Init()
   */
  TransportAdapter::Error Init() OVERRIDE;

  /**
   * @brief Adds webengine device to internal storage
   * of related WebSocket transport adapter
   * @param device webengine device to add
   * @note webengine device storage required
   * to be used in Low Voltage conditions
   */
  DeviceSptr AddDevice(DeviceSptr device) OVERRIDE;

  /**
   * @brief Notification that transport's configuration is updated
   *
   * @param new_config The new configuration of the transport
   */
  void TransportConfigUpdated(const TransportConfig& new_config) OVERRIDE;

  /**
   * @brief Returns the transport's configuration information
   */
  virtual TransportConfig GetTransportConfiguration() const OVERRIDE;

  /**
   * @brief Store adapter state in last state
   */
  void Store() const OVERRIDE;

  /**
   * @brief Restore adapter state from last state
   * @return True on success false otherwise
   */
  bool Restore() OVERRIDE;

  /**
   * @brief Get stored device ID
   * @return stored unique device id, or empty string if no stored device id
   * present in last saved state
   */
  std::string GetStoredDeviceID() const;

 protected:
  /**
   * @brief Return type of device.
   *
   * @return String with device type.
   */
  DeviceType GetDeviceType() const OVERRIDE;

 private:
  /**
   * @brief Keeps transport specific configuration
   *
   * TCP transport uses following information:
   * - "enabled": whether the transport is currently enabled or not. Value can
   *              be "true" or "false".
   * - "tcp_ip_address": string representation of IP address (either IPv4 or
   *                     IPv6)
   * - "tcp_port": string representation of TCP port number (e.g. "12345")
   */
  TransportConfig transport_config_;
  DeviceSptr webengine_device_;
  std::string webengine_device_id_;
};

}  // namespace transport_adapter
}  // namespace transport_manager

#endif  // SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_WEBSOCKET_SERVER_WEBSOCKET_SERVER_TRANSPORT_ADAPTER_H_
