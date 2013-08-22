/**
 * \file device_adapter_controller.h
 * \brief Device adapter controller header file.
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

#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_DEVICE_ADAPTER_DEVICE_DEVICE_ADAPTER_CONTROLLER_H_
#define SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_DEVICE_ADAPTER_DEVICE_DEVICE_ADAPTER_CONTROLLER_H_

#include "transport_manager/device_adapter/device.h"
#include "transport_manager/device_adapter/connection.h"

/**
 * @brief Transport manager namespace.
 */
namespace transport_manager {

/**
 * @brief Device adapter namespace, part of transport manager namespace.
 */
namespace device_adapter {


class DeviceAdapterController {
 public:

  virtual ~DeviceAdapterController() {
  }

  /**
   * @brief Add device to the container(map), if container doesn't hold it yet.
   *
   * @param device smart pointer to the device.
   *
   * @return smart pointer to the device.
   */
  virtual DeviceSptr addDevice(DeviceSptr device) = 0;

  /**
   * @brief ????
   */
  virtual void searchDeviceDone(const DeviceVector& devices) = 0;

  /**
   * @brief ????
   */
  virtual void searchDeviceFailed(const SearchDeviceError& error) = 0;

  /**
   * @brief Find device in the internal container(map).
   *
   * @param device_handle device unique identifier.
   *
   * @return smart pointer to the device.
   */
  virtual DeviceSptr findDevice(const DeviceUID& device_handle) const = 0;

  /**
   * @brief Create connection and fill its parameters.
   *
   * @param connection  smart pointer to the connection.
   * @param device_handle device unique identifier.
   * @param app_handle handle of application.
   */
  virtual void connectionCreated(ConnectionSptr connection,
                                 const DeviceUID& device_handle,
                                 const ApplicationHandle& app_handle) = 0;

  /**
   * @brief Make state of specified connection - ESTABLISHED and launch onConnectDone event in device adapter listener.
   *
   * @param devcie_handle device unique identifier.
   * @param app_handle handle of application.
   */
  virtual void connectDone(const DeviceUID& device_handle,
                           const ApplicationHandle& app_handle) = 0;

  /**
   * @brief Delete connection from the container of connections and launch onConnectFailed event in the device adapter listener.
   *
   * @param device_handle device unique identifier.
   * @param app_handle handle of application.
   */
  virtual void connectFailed(const DeviceUID& device_handle,
                             const ApplicationHandle& app_handle,
                             const ConnectError& error) = 0;

  /**
   * @brief Make state of specified connection - FINILIZING.
   *
   * @param device_handle device unique identifier.
   * @param app_handle handle of application.
   */
  virtual void connectionFinished(const DeviceUID& device_handle,
                                  const ApplicationHandle& app_handle) = 0;

  /**
   * @brief Set specified connection state to FINILIZING and launch onUnexpectedDisconnect event in the device adapter listener.
   *
   * @param device_handle device unique identifier.
   * @param app_handle handle of application.
   * @param error error class that contains details of this error situation.
   */
  virtual void connectionAborted(const DeviceUID& device_handle,
                                 const ApplicationHandle& app_handle,
                                 const CommunicationError& error) = 0;

  /**
   * @brief Delete specified connection from the container(map) of connections and launch event in the device adapter listener.
   *
   * @param device_handle device unique identifier.
   * @param app_handle handle of application.
   */
  virtual void disconnectDone(const DeviceUID& device_handle,
                              const ApplicationHandle& app_handle) = 0;

   /**
   * @brief Launch onDataReceiveDone event in the device adapter listener.
   *
   * @param device_handle device unique identifier.
   * @param app_handle handle of application.
   * @param message smart pointer to the raw message.
   */
  virtual void dataReceiveDone(const DeviceUID& device_handle,
                               const ApplicationHandle& app_handle,
                               RawMessageSptr message) = 0;

  /**
   * @brief Launch onDataReceiveFailed event in the device adapter listener.
   *
   * @param device_handle device unique identifier.
   * @param app_handle handle of application.
   * @param error class that contains details of this error situation.
   */
  virtual void dataReceiveFailed(const DeviceUID& device_handle,
                                 const ApplicationHandle& app_handle,
                                 const DataReceiveError&) = 0;

  /**
   * @brief Launch onDataSendDone event in the device adapter listener.
   *
   * @param device_handle device unique identifier.
   * @param app_handle handle of application.
   * @param message smart pointer to raw message.
   */
  virtual void dataSendDone(const DeviceUID& device_handle,
                            const ApplicationHandle& app_handle,
                            RawMessageSptr message) = 0;

  /**
   * @brief Launch onDataSendFailed event in the device adapter listener.
   *
   * @param device_handle device unique identifier.
   * @param app_handle handle of application.
   * @param message smart pointer to raw message.
   * @param error class that contains details of this error situation.
   */
  virtual void dataSendFailed(const DeviceUID& device_handle,
                              const ApplicationHandle& app_handle,
                              RawMessageSptr message, const DataSendError&) = 0;
};

}  // namespace device_adapter
}  // namespace transport_manager

#endif /* DEVICE_ADAPTER_CONTROLLER_H_ */
