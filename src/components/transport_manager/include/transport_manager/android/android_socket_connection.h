/*
 * \file android_socket_connection.h
 * \brief AndroidSocketConnection class header file.
 *
 * Copyright (c) 2021, Ford Motor Company
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

#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_ANDROID_ANDROID_SOCKET_CONNECTION_H_
#define SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_ANDROID_ANDROID_SOCKET_CONNECTION_H_

#include "transport_manager/transport_adapter/connection.h"
#include "transport_manager/android/local_socket_sender.h"
#include "transport_manager/android/local_socket_receiver.h"
#include "transport_manager/android/android_transport_adapter.h"

namespace transport_manager {
namespace transport_adapter {

class TransportAdapterController;

/**
 * @brief Class responsible for communication over bluetooth LE sockets.
 */
class AndroidSocketConnection : public Connection {
 public:
  /**
   * @brief Constructor.
   *
   * @param device_uid Device unique identifier.
   * @param app_handle Handle of device.
   * @param controller Pointer to the device adapter controller.
   */
  AndroidSocketConnection(const DeviceUID& device_uid,
                            const ApplicationHandle& app_handle,
                            AndroidTransportAdapter* controller);

  ~AndroidSocketConnection() override;

  TransportAdapter::Error SendData(
            ::protocol_handler::RawMessagePtr message) override;

  TransportAdapter::Error Disconnect() override;

  /**
   * @brief Starts connection send/receive threads
   * @returns OK if connection threads are started, otherwise returns false
   */
  TransportAdapter::Error Start();

 private:
    void ProcessMessage(const std::vector<uint8_t>& data);
    void OnMessageSent(protocol_handler::RawMessagePtr message);
    void OnClientConnectionDone(const bool is_connected);

    DeviceUID device_uid_;
    ApplicationHandle app_handle_;
    AndroidTransportAdapter* controller_;

    std::unique_ptr<IpcSender> sender_;
    std::unique_ptr<IpcReceiver> receiver_;

    std::thread receiver_thread_;
    std::thread sender_thread_;

};

}  // namespace transport_adapter
}  // namespace transport_manager

#endif  // SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_ANDROID_ANDROID_SOCKET_CONNECTION_H_
