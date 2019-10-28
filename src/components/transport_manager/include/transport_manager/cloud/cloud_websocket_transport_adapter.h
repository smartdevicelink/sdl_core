/*
 * \file cloud_websocket_transport_adapter.h
 * \brief Cloud Websocket Transport Adapterclass header file.
 *
 * Copyright (c) 2018, Livio
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

#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_CLOUD_CLOUD_WEBSOCKET_TRANSPORT_ADAPTER_H_
#define SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_CLOUD_CLOUD_WEBSOCKET_TRANSPORT_ADAPTER_H_

#include "transport_manager/transport_adapter/transport_adapter_impl.h"

namespace transport_manager {
namespace transport_adapter {

/**
 * @brief Cloud transport adapter that uses websockets.
 */
class CloudWebsocketTransportAdapter : public TransportAdapterImpl {
 public:
  /**
   * @brief Constructor.
   */
  explicit CloudWebsocketTransportAdapter(
      resumption::LastState& last_state,
      const TransportManagerSettings& settings);

  /**
   * @brief Destructor.
   */
  virtual ~CloudWebsocketTransportAdapter();

  /**
   * @brief Set CloudTransportConfig for specified app_id
   *
   * @param app_id app ID string
   * @param properties New cloud app properties for the app
   */
  void SetAppCloudTransportConfig(std::string app_id,
                                  CloudAppProperties properties);

  /**
   * @brief Get CloudTransportConfig for specified app_id
   *
   * @param app_id app ID string
   * @return CloudAppProperties for the app
   */
  const CloudAppProperties& GetAppCloudTransportConfig(std::string app_id);

 protected:
  /**
   * @brief Return type of device.
   *
   * @return String with device type.
   */
  virtual DeviceType GetDeviceType() const;

  /**
   * @brief Store adapter state in last state singleton
   */
  virtual void Store() const;

  /**
   * @brief Restore adapter state from last state singleton
   *
   * @return True on success false otherwise
   */
  virtual bool Restore();

  void CreateDevice(const std::string& uid) OVERRIDE;

 private:
  CloudAppTransportConfig transport_config_;
};

}  // namespace transport_adapter
}  // namespace transport_manager

#endif  // SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_TCP_TCP_TRANSPORT_ADAPTER_H_
