/**
 * \file device_adapter_listener_impl.h
 * \brief DeviceAdapterListenerImpl class header file.
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

#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_DEVICE_ADAPTER_LISTENER_IMPL
#define SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_DEVICE_ADAPTER_LISTENER_IMPL

#include "transport_manager/device_adapter/device_adapter_listener.h"
#include "transport_manager/device_adapter/device_adapter.h"
#include "utils/logger.h"

using transport_manager::device_adapter::DeviceAdapter;
using transport_manager::device_adapter::DeviceAdapterSptr;

namespace transport_manager {

typedef std::vector<DeviceAdapterSptr>::const_iterator AdapterIterator;

class TransportManagerImpl;

class DeviceAdapterListenerImpl : public device_adapter::DeviceAdapterListener {
 public:
  enum EventTypeEnum {
    ON_SEARCH_DONE = 0,
    ON_SEARCH_FAIL,
    ON_CONNECT_DONE,
    ON_CONNECT_FAIL,
    ON_DISCONNECT_DONE,
    ON_DISCONNECT_FAIL,
    ON_SEND_DONE,
    ON_SEND_FAIL,
    ON_RECEIVED_DONE,
    ON_RECEIVED_FAIL,
    ON_COMMUNICATION_ERROR,
    ON_UNEXPECTED_DISCONNECT
  };

  explicit DeviceAdapterListenerImpl(transport_manager::TransportManagerImpl *tm);
  virtual ~DeviceAdapterListenerImpl();

  virtual void onSearchDeviceDone(const DeviceAdapter* device_adapter);
  virtual void onSearchDeviceFailed(const DeviceAdapter* device_adapter,
                                    const SearchDeviceError& error);

  virtual void onConnectDone(const DeviceAdapter* device_adapter,
                             const DeviceUID& device_handle,
                             const ApplicationHandle& app_id);
  virtual void onConnectFailed(const DeviceAdapter* device_adapter,
                               const DeviceUID& device,
                               const ApplicationHandle& app_id,
                               const ConnectError& error);

  virtual void onConnectRequested(const DeviceAdapter* device_adapter,
                                  const DeviceUID& device_handle,
                                  const ApplicationHandle& app_handle);

  virtual void onUnexpectedDisconnect(const DeviceAdapter* device_adapter,
                                      const DeviceUID& device,
                                      const ApplicationHandle& app_id,
                                      const CommunicationError& error);
  virtual void onDisconnectDone(const DeviceAdapter* device_adapter,
                                const DeviceUID& device_id,
                                const ApplicationHandle& app_id);
  virtual void onDisconnectFailed(const DeviceAdapter* device_adapter,
                                  const DeviceUID& device,
                                  const ApplicationHandle& app_id,
                                  const DisconnectError& error);

  virtual void onDisconnectDeviceDone(
      const device_adapter::DeviceAdapter* device_adapter,
      const DeviceUID& devic);
  virtual void onDisconnectDeviceFailed(
      const device_adapter::DeviceAdapter* device_adapter,
      const DeviceUID& device, const DisconnectDeviceError& error);

  virtual void onDataReceiveDone(
      const device_adapter::DeviceAdapter* device_adapter,
      const DeviceUID& device,
      const ApplicationHandle& app_id, const RawMessageSptr data_container);
  virtual void onDataReceiveFailed(
      const device_adapter::DeviceAdapter* device_adapter,
      const DeviceUID& device,
      const ApplicationHandle& app_id, const DataReceiveError& error);

  virtual void onDataSendDone(const DeviceAdapter* device_adapter,
                              const DeviceUID& device,
                              const ApplicationHandle& app_id,
                              const RawMessageSptr data_container);
  virtual void onDataSendFailed(const DeviceAdapter* device_adapter,
                                const DeviceUID& device,
                                const ApplicationHandle& app_id,
                                const RawMessageSptr data_container,
                                const DataSendError& error);

  virtual void onCommunicationError(
      const device_adapter::DeviceAdapter* device_adapter,
      const DeviceUID& device,
      const ApplicationHandle& app_id);

  inline bool FindSharedPtr(const DeviceAdapter* device_adapter,
                            AdapterIterator &it);

 private:
  /**
   * \brief For logging.
   */
  static log4cxx::LoggerPtr logger_;
  TransportManagerImpl *transport_manager_impl_;

};
}  // namespace transport_manager

#endif // SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_DEVICE_ADAPTER_LISTENER_IMPL
