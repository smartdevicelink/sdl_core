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

#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_TRANSPORT_ADAPTER_TRANSPORT_ADAPTER_CONTROLLER_H_
#define SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_TRANSPORT_ADAPTER_TRANSPORT_ADAPTER_CONTROLLER_H_

#include "transport_manager/transport_adapter/device.h"
#include "transport_manager/transport_adapter/connection.h"
#include "protocol/common.h"

namespace transport_manager {
namespace transport_adapter {

class TransportAdapterController {
 public:
  virtual ~TransportAdapterController() {
  }

  /**
   * @brief Add device to the container(map), if container doesn't hold it yet.
   *
   * @param device Smart pointer to the device.
   *
   * @return Smart pointer to the device.
   */
  virtual DeviceSptr AddDevice(DeviceSptr device) = 0;

  /**
   * @brief Search for device in container of devices, if it is not there - adds it.
   *
   * @param devices Container(vector) of smart pointers to devices.
   */
  virtual void SearchDeviceDone(const DeviceVector& devices) = 0;

  /**
   * @brief application list update notification from device
   * @param device_handle device id
   */
  virtual void ApplicationListUpdated(const DeviceUID& device_handle) = 0;

  /**
   * @brief "Find new applications" request
   */
  virtual void FindNewApplicationsRequest() = 0;

  /**
   * @brief Launch OnSearchDeviceFailed event in device adapter listener.
   *
   * @param error Error class that contains details of this error situation.
   */
  virtual void SearchDeviceFailed(const SearchDeviceError& error) = 0;

  /**
   * @brief Find device in the internal container(map).
   *
   * @param device_handle Device unique identifier.
   *
   * @return Smart pointer to the device.
   */
  virtual DeviceSptr FindDevice(const DeviceUID& device_handle) const = 0;

  /**
   * @brief Create connection and fill its parameters.
   *
   * @param connection  Smart pointer to the connection.
   * @param device_handle Device unique identifier.
   * @param app_handle Handle of application.
   */
  virtual void ConnectionCreated(ConnectionSPtr connection,
                                 const DeviceUID& device_handle,
                                 const ApplicationHandle& app_handle) = 0;

  /**
   * @brief Make state of specified connection - ESTABLISHED and launch OnConnectDone event in device adapter listener.
   *
   * @param devcie_handle Device unique identifier.
   * @param app_handle Handle of application.
   */
  virtual void ConnectDone(const DeviceUID& device_handle,
                           const ApplicationHandle& app_handle) = 0;

  /**
   * @brief Delete connection from the container of connections and launch OnConnectFailed event in the device adapter listener.
   *
   * @param device_handle Device unique identifier.
   * @param app_handle Handle of application.
   */
  virtual void ConnectFailed(const DeviceUID& device_handle,
                             const ApplicationHandle& app_handle,
                             const ConnectError& error) = 0;

  /**
   * @brief Make state of specified connection - FINILIZING.
   *
   * @param device_handle Device unique identifier.
   * @param app_handle Handle of application.
   */
  virtual void ConnectionFinished(const DeviceUID& device_handle,
                                  const ApplicationHandle& app_handle) = 0;

  /**
   * @brief Set specified connection state to FINALISING and launch OnUnexpectedDisconnect event in the device adapter listener.
   *
   * @param device_handle Device unique identifier.
   * @param app_handle Handle of application.
   * @param error Error class that contains details of this error situation.
   */
  virtual void ConnectionAborted(const DeviceUID& device_handle,
                                 const ApplicationHandle& app_handle,
                                 const CommunicationError& error) = 0;

  /**
   * @brief Remove specified device and all its connections
   * @param device_handle Device unique identifier.
   * @param error Error class that contains details of this error situation.
   */
  virtual void DeviceDisconnected(const DeviceUID& device_handle,
                                  const DisconnectDeviceError& error) = 0;

  /**
   * @brief Delete specified connection from the container(map) of connections and launch event in the device adapter listener.
   *
   * @param device_handle Device unique identifier.
   * @param app_handle Handle of application.
   */
  virtual void DisconnectDone(const DeviceUID& device_handle,
                              const ApplicationHandle& app_handle) = 0;

   /**
   * @brief Launch OnDataReceiveDone event in the device adapter listener.
   *
   * @param device_handle Device unique identifier.
   * @param app_handle Handle of application.
   * @param message Smart pointer to the raw message.
   */
  virtual void DataReceiveDone(const DeviceUID& device_handle,
                               const ApplicationHandle& app_handle,
                               ::protocol_handler::RawMessagePtr message) = 0;

  /**
   * @brief Launch OnDataReceiveFailed event in the device adapter listener.
   *
   * @param device_handle Device unique identifier.
   * @param app_handle Handle of application.
   * @param error Class that contains details of this error situation.
   */
  virtual void DataReceiveFailed(const DeviceUID& device_handle,
                                 const ApplicationHandle& app_handle,
                                 const DataReceiveError&) = 0;

  /**
   * @brief Launch OnDataSendDone event in the device adapter listener.
   *
   * @param device_handle Device unique identifier.
   * @param app_handle Handle of application.
   * @param message Smart pointer to raw message.
   */
  virtual void DataSendDone(const DeviceUID& device_handle,
                            const ApplicationHandle& app_handle,
                            ::protocol_handler::RawMessagePtr message) = 0;

  /**
   * @brief Launch OnDataSendFailed event in the device adapter listener.
   *
   * @param device_handle Device unique identifier.
   * @param app_handle Handle of application.
   * @param message Smart pointer to raw message.
   * @param error Class that contains details of this error situation.
   */
  virtual void DataSendFailed(const DeviceUID& device_handle,
                              const ApplicationHandle& app_handle,
                              ::protocol_handler::RawMessagePtr message, const DataSendError&) = 0;
};

}  // namespace transport_adapter
}  // namespace transport_manager
#endif  // SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_TRANSPORT_ADAPTER_TRANSPORT_ADAPTER_CONTROLLER_H_
