/*
 * Copyright (c) 2014, Ford Motor Company
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

#ifndef SRC_COMPONENTS_INCLUDE_TRANSPORT_MANAGER_TRANSPORT_MANAGER_LISTENER_H_
#define SRC_COMPONENTS_INCLUDE_TRANSPORT_MANAGER_TRANSPORT_MANAGER_LISTENER_H_

#include <vector>
#include "transport_manager/common.h"
#include "transport_manager/info.h"
#include "transport_manager/error.h"
#include "protocol/common.h"

namespace transport_manager {

class TransportManagerListener {
 public:
  /**
   * @brief Destructor.
   */
  virtual ~TransportManagerListener() {
  }

  /**
   * @brief Reaction to the event, when the list of devices is updated.
   *
   * @param Container that holds information about devices.
   */
  virtual void OnDeviceListUpdated(const std::vector<DeviceInfo>&) = 0;

  /**
  * @brief Reaction to "Find new applications" request
  */
  virtual void OnFindNewApplicationsRequest() = 0;

  /**
   * @brief Reaction to the event, when the device is found.
   *
   * @param device_info Variable that hold information about device.
   */
  virtual void OnDeviceFound(const DeviceInfo& device_info) = 0;
  virtual void OnDeviceAdded(const DeviceInfo& device_info) = 0;
  virtual void OnDeviceRemoved(const DeviceInfo& device_info) = 0;

  /**
   * @brief Reaction to the event, when scanning of devices is finished.
   */
  virtual void OnScanDevicesFinished() = 0;

  /**
   * @brief Reaction to the event, when scanning of devices is failed.
   *
   * @param error Error information about possible reason of scanning of devices failure.
   */
  virtual void OnScanDevicesFailed(const SearchDeviceError& error) = 0;

  /**
   * @brief Reaction to the event, when connection is established.
   *
   * @param devcie_info Variable that hold information about device.
   * @param connection_id connection unique identifier.
   */
  virtual void OnConnectionEstablished(const DeviceInfo& device_info,
                                       const ConnectionUID connection_id) = 0;

  /**
   * @brief Reaction to the event, when connection to the device is failed.
   *
   * @param device_info Variable that hold information about device.
   * @param error Error information about possible reason of connect failure.
   */
  virtual void OnConnectionFailed(const DeviceInfo& device_info,
                                  const ConnectError& error) = 0;

  /**
   * @brief Reaction to the event, when connection is closed.
   *
   * @param connection_id Connection unique identifier.
   */
  virtual void OnConnectionClosed(const ConnectionUID connection_id) = 0;

  /**
   * @brief Called when connection is closed unexpectedly, i.e. disconnect was not requested
   *
   * @param connection_id Connection ID.
   * @param error Error information.
   */
  virtual void OnUnexpectedDisconnect(const ConnectionUID connection_id,
                                      const CommunicationError& error) = 0;

  /**
   * @brief Reaction to the event, when connection close is failed.
   *
   * @param connection_id Connection unique identifier.
   * @param error Error information about possible reason of failure.
   */
  virtual void OnConnectionClosedFailure(const ConnectionUID connection_id,
                                         const DisconnectError& error) = 0;

  /**
   * \brief Inform about losing connection with device.
   * \param device Handle of device.
   * \param error Error information about possible reason of loosing connection.
   */
  virtual void OnDeviceConnectionLost(const DeviceHandle& device,
                                      const DisconnectDeviceError& error) = 0;

  /**
   * \brief Inform about failure during DisconnectDevice procedure of transport manager.
   * \param device Handle of device.
   * \param error Error information about possible reason of disconnecting failure.
   */
  virtual void OnDisconnectFailed(const DeviceHandle& device,
                                  const DisconnectDeviceError& error) = 0;
  /**
   * @brief Notifies about recieving message from TM.
   *
   * @param message Received message
   **/
  virtual void OnTMMessageReceived(const ::protocol_handler::RawMessagePtr message) = 0;

  /**
   * @brief Reaction to the event, when receiving of massage for transport manager is failed.
   *
   * @param connection_id connection unique identifier.
   * @param error Error information about possible reason of failure.
   */
  virtual void OnTMMessageReceiveFailed(const DataReceiveError& error) = 0;

  /**
   * @brief Reaction to the event, when transport manager sent a massage.
   */
  virtual void OnTMMessageSend(const ::protocol_handler::RawMessagePtr message) = 0;

  /**
   * @brief Reaction to the event, when sending of massage by transport manager is failed.
   *
   * @param error Error information about possible reason of failure.
   * @param message Smart pointer to the raw massage.
   */
  virtual void OnTMMessageSendFailed(const DataSendError& error,
                                     const ::protocol_handler::RawMessagePtr message) = 0;
};
}  //  namespace transport_manager
#endif  // SRC_COMPONENTS_INCLUDE_TRANSPORT_MANAGER_TRANSPORT_MANAGER_LISTENER_H_
