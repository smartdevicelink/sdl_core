/**
 * \file transport_manager_listener.h
 * \brief interface for TransportManagerListener header file.
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

#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_TRANSPORT_MANAGER_LISTENER
#define SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_TRANSPORT_MANAGER_LISTENER

#include "transport_manager/common.h"
#include "transport_manager/info.h"
#include "transport_manager/error.h"

namespace transport_manager {

class TransportManagerListener {
 public:
  virtual ~TransportManagerListener() {
  }

  virtual void OnDeviceListUpdated(const std::vector<DeviceInfo>&) = 0;

  virtual void OnDeviceFound(const DeviceInfo &device_info) = 0;

  virtual void OnNoDeviceFound() = 0;
  virtual void OnScanDevicesFinished() = 0;
  virtual void OnScanDevicesFailed(const SearchDeviceError& error) = 0;
  virtual void OnConnectionEstablished(const DeviceInfo &device_info,
                                       const ConnectionUID &connection_id) = 0;
  virtual void OnConnectionFailed(const DeviceInfo &device_info,
                                  const ConnectError& error) = 0;
  virtual void OnConnectionClosed(ConnectionUID connection_id) = 0;
  virtual void OnConnectionClosedFailure(ConnectionUID connection_id,
                                         const DisconnectError& error) = 0;

  /**
   * \brief Informs about losing connection with device
   * \param device Information about disconnected device
   * \param error Information about possible reason of loosing connection
   */
  virtual void OnDeviceConnectionLost(const DeviceHandle& device,
                                      const DisconnectDeviceError& error) = 0;

  /**
   * \brief Informs about failure during DisconnectDevice procedure of TM
   * \param device Information about disconnected device
   * \param error Information about possible reason of loosing connection
   */
  virtual void OnDisconnectFailed(const DeviceHandle& device,
                                  const DisconnectDeviceError& error) = 0;
  /**
   * @brief Notifies about recieving message from TM.
   *
   * @param message Recieved message
   **/
  virtual void OnTMMessageReceived(const RawMessageSptr message) = 0;

  /**
   * @brief Notifies about error on receiving message from TM.
   *
   * @param error Occured error
   **/
  virtual void OnTMMessageReceiveFailed(ConnectionUID connection_id,
                                        const DataReceiveError& error) = 0;

  /**
   * @brief Notifies about successfully sending message.
   *
   **/
  virtual void OnTMMessageSend() = 0;

  /**
   * @brief Notifies about error occured during
   * sending message.
   *
   * @param error Describes occured error.
   * @param message Message during sending which error occured.
   **/
  virtual void OnTMMessageSendFailed(const DataSendError& error,
                                     const RawMessageSptr message) = 0;
};
}  //  namespace transport_manager
#endif  //  SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_TRANSPORT_MANAGER_LISTENER
