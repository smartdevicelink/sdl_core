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

#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_TRANSPORT_ADAPTER_TRANSPORT_ADAPTER_LISTENER_H_
#define SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_TRANSPORT_ADAPTER_TRANSPORT_ADAPTER_LISTENER_H_

#include "transport_manager/common.h"
#include "transport_manager/error.h"
#include "protocol/common.h"

namespace transport_manager {
namespace transport_adapter {

class TransportAdapter;

/**
 * @brief Abstract class for device adapter listener.
 */
class TransportAdapterListener {
 public:
  /**
   * @brief Destructor.
   */
  virtual ~TransportAdapterListener() {
  }

  /**
   * @brief Search specified device adapter in the container of shared pointers to device adapters to be sure it is available,
   * launch event ON_SEARCH_DONE in transport manager.
   *
   * @param transport_adapter Pointer to the device adapter.
   */
  virtual void OnSearchDeviceDone(
      const TransportAdapter* transport_adapter) = 0;

  /**
   * @brief Search specified device adapter in the container of shared pointers to device adapters to be sure it is available, create search device error,
   * launch event ON_SEARCH_FAIL in transport manager.
   *
   * @param transport_adapter Pointer to the device adapter.
   * @param error Error class with information about possible reason of search device failure.
   */
  virtual void OnSearchDeviceFailed(const TransportAdapter* transport_adapter,
                                    const SearchDeviceError& error) = 0;

  /**
   * @brief Notification by transport adapter that list of known devices has
   * changed. Updated list can be obtained using
   * TransportAdapter::GetDeviceList()
   *
   * @param transport_adapter Transport adapter that sent notifcation
   */
  virtual void OnDeviceListUpdated(
      const TransportAdapter* transport_adapter) = 0;

  /**
   * @brief Reaction to "Find new applications" request
   * @param adapter Current transport adapter
   */
  virtual void OnFindNewApplicationsRequest(const TransportAdapter* adapter) = 0;

  /**
   * @brief Search specified device adapter in the container of shared pointers to device adapters to be sure it is available,
   * launch event ON_CONNECT_DONE in transport manager.
   *
   * @param device_adater Pointer to the device adapter.
   * @param device_handle Device unique identifier.
   * @param app_id Handle of application.
   */
  virtual void OnConnectDone(const TransportAdapter* transport_adapter,
                             const DeviceUID& device_handle,
                             const ApplicationHandle& app_handle) = 0;

  /**
   * @brief Search specified device adapter in the container of shared pointers to device adapters to be sure it is available,
   * launch event ON_CONNECT_FAIL in transport manager.
   *
   * @param transport_adapter Pointer to the device adapter.
   * @param device Device unique identifier.
   * @param app_id Handle of application.
   * @param error Error class with information about possible reason of connect failure.
   */
  virtual void OnConnectFailed(const TransportAdapter* transport_adapter,
                               const DeviceUID& device_handle,
                               const ApplicationHandle& app_handle,
                               const ConnectError& error) = 0;

  /**
   * @brief
   *
   * @param transport_adapter Pointer to the device adapter.
   * @param device_handle Device unique identifier.
   * @param app_handle Handle of application.
   */
  virtual void OnConnectRequested(const TransportAdapter* transport_adapter,
                                  const DeviceUID& device_handle,
                                  const ApplicationHandle& app_handle) = 0;

  /**
   * @brief Search specified device adapter in the container of shared pointers to device adapters to be sure it is available,
   * launch event ON_UNEXPECTED_DISCONNECT in transport manager.
   *
   * @param transport_adapter Pointer to the device adapter.
   * @param device Device unique identifier.
   * @param app_id Handle of application.
   * @param error Error class with information about possible reason of unexpected Disconnect.
   */
  virtual void OnUnexpectedDisconnect(const TransportAdapter* transport_adapter,
                                      const DeviceUID& device_handle,
                                      const ApplicationHandle& app_handle,
                                      const CommunicationError& error) = 0;
  /**
   * @brief Search specified device adapter in the container of shared pointers to device adapters to be sure it is available,
   * launch event ON_DISCONNECT_DONE in transport manager.
   *
   * @param transport_adapter Pointer to the device adapter.
   * @param devcie_id Device unique identifier.
   * @param app_id Handle of application.
   */
  virtual void OnDisconnectDone(const TransportAdapter* transport_adapter,
                                const DeviceUID& device_handle,
                                const ApplicationHandle& app_handle) = 0;

  /**
   * @brief Search specified device adapter in the container of shared pointers to device adapters to be sure it is available, create Disconnect error,
   * launch event ON_DISCONNECT_FAIL in transport manager.
   *
   * @param transport_adapter Pointer to the device adapter.
   * @param device Device unique identifier.
   * @param app_id Handle of application.
   * @param error Error class with information about possible reason of Disconnect failure.
   */
  virtual void OnDisconnectFailed(const TransportAdapter* transport_adapter,
                                  const DeviceUID& device_handle,
                                  const ApplicationHandle& app_handle,
                                  const DisconnectError& error) = 0;

  /**
   * @brief
   *
   * @param transport_adapter Pointer to the device adapter.
   * @param devic Device unique identifier.
   */
  virtual void OnDisconnectDeviceDone(const TransportAdapter* transport_adapter,
                                      const DeviceUID& device_handle) = 0;

  /**
   * @brief
   *
   * @param transport_adapter Pointer to the device adapter.
   * @param device Device unique identifier.
   * @param error Error class with information about possible reason of Disconnect from device failure.
   */
  virtual void OnDisconnectDeviceFailed(
      const TransportAdapter* transport_adapter, const DeviceUID& device_handle,
      const DisconnectDeviceError& error) = 0;

  /**
   * @brief Search specified device adapter in the container of shared pointers to device adapters to be sure it is available, create error,
   * launch event ON_RECEIVED_DONE in transport manager.
   *
   * @param transport_adapter Pointer to the device adapter.
   * @param devcie Device unique identifier.
   * @param app_id Handle of application.
   * @param data_container Smart pointer to the raw message.
   */
  virtual void OnDataSendDone(const TransportAdapter* transport_adapter,
                              const DeviceUID& device_handle,
                              const ApplicationHandle& app_handle,
                              const ::protocol_handler::RawMessagePtr data_container) = 0;

  /**
   * @brief Search specified device adapter in the container of shared pointers to device adapters to be sure it is available, create data receive error,
   * launch event ON_RECEIVED_DONE in transport manager.
   *
   * @param transport_adapter Pointer to the device adapter.
   * @param device Device unique identifier.
   * @param app_id Handle of application.
   * @param error Error class with information about possible reason of data receive failure.
   */
  virtual void OnDataSendFailed(const TransportAdapter* transport_adapter,
                                const DeviceUID& device_handle,
                                const ApplicationHandle& app_handle,
                                const ::protocol_handler::RawMessagePtr data_container,
                                const DataSendError& error) = 0;

  /**
   * @brief Search specified device adapter in the container of shared pointers to device adapters to be sure it is available, create error,
   * launch event ON_SEND_DONE in transport manager.
   *
   * @param transport_adapter Pointer to the device adapter.
   * @param device Device unique identifier.
   * @param app_id Handle of application.
   * @param data_container Smart pointer to the raw message.
   */
  virtual void OnDataReceiveDone(const TransportAdapter* transport_adapter,
                                 const DeviceUID& device_handle,
                                 const ApplicationHandle& app_handle,
                                 const ::protocol_handler::RawMessagePtr data_container) = 0;

  /**
   * @brief Search specified device adapter in the container of shared pointers to device adapters to be sure it is available, create data send error,
   * launch event ON_SEND_FAIL in transport manager.
   */
  virtual void OnDataReceiveFailed(const TransportAdapter* transport_adapter,
                                   const DeviceUID& device_handle,
                                   const ApplicationHandle& app_handle,
                                   const DataReceiveError& error) = 0;

  /**
   * @brief Search specified device adapter in the container of shared pointers to device adapters to be sure it is available, create error,
   * launch event ON_COMMUNICATION_ERROR in transport manager.
   *
   * @param transport_adapter Pointer to the device adapter.
   * @param device Device unique identifier.
   * @param app_id Handle of application.
   */
  virtual void OnCommunicationError(const TransportAdapter* transport_adapter,
                                    const DeviceUID& device_handle,
                                    const ApplicationHandle& app_handle) = 0;
};

}  // transport_adapter namespace
}  // transport_manager namespace
#endif  // SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_TRANSPORT_ADAPTER_TRANSPORT_ADAPTER_LISTENER_H_
