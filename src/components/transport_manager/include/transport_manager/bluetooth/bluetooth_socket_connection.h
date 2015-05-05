/*
 * \file bluetooth_socket_connection.h
 * \brief BluetoothSocketConnection class header file.
 *
 * Copyright (c) 2013, Ford Motor Company
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

#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_BLUETOOTH_BLUETOOTH_SOCKET_CONNECTION_H_
#define SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_BLUETOOTH_BLUETOOTH_SOCKET_CONNECTION_H_

#include "transport_manager/transport_adapter/threaded_socket_connection.h"

namespace transport_manager {
namespace transport_adapter {

class TransportAdapterController;

/**
 * @brief Class responsible for communication over bluetooth sockets.
 */
class BluetoothSocketConnection : public ThreadedSocketConnection {
 public:

  /**
   * @brief Constructor.
   *
   * @param device_uid Device unique identifier.
   * @param app_handle Handle of device.
   * @param controller Pointer to the device adapter controller.
   */
  BluetoothSocketConnection(const DeviceUID& device_uid,
                            const ApplicationHandle& app_handle,
                            TransportAdapterController* controller);

  /**
   * @brief Destructor.
   */
  virtual ~BluetoothSocketConnection();
 protected:

  /**
   * @brief Establish connection.
   *
   * @param error Connection error.
   *
   * @return True - connection established.
   * false - connection not established.
   */
  virtual bool Establish(ConnectError** error);
};

}  // namespace transport_adapter
}  // namespace transport_manager

#endif /* BLUETOOTH_SOCKET_CONNECTION_H_ */
