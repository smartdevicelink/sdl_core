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

#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_TRANSPORT_MANAGER_LISTENER_IMPL
#define SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_TRANSPORT_MANAGER_LISTENER_IMPL

#include "transport_manager/transport_manager_listener.h"

namespace transport_manager {

class TransportManagerListenerImpl : public TransportManagerListener {
 public:
  virtual ~TransportManagerListenerImpl(){};

  virtual void OnDeviceFound(const DeviceHandle& device, const ApplicationList &app_list) {}
  virtual void OnAccessRequested(const DeviceDesc& device) {
  }
  virtual void OnScanDevicesFinished() {
  }
  virtual void OnScanDevicesFailed(const SearchDeviceError& error) {
  }
  virtual void OnConnectionEstablished(const DeviceHandle& device,
                                       const ApplicationHandle &app_id,
                                       ConnectionUID connection_id) {}
  virtual void OnConnectionFailed(const DeviceHandle& device,
                                  const ApplicationHandle &app_id,
                                  const ConnectError& error) {}
  virtual void OnConnectionClosed(ConnectionUID connection_id) {
  }
  virtual void OnConnectionClosedFailure(ConnectionUID connection_id,
                                         const DisconnectError& error) {
  }
  virtual void OnDeviceConnectionLost(const DeviceHandle& device,
                                      const DisconnectDeviceError& error){}
  virtual void OnDisconnectFailed(const DeviceHandle& device,
                                  const DisconnectDeviceError& error) {}
  virtual void OnTMMessageReceived(const RawMessageSptr message) {}
  virtual void OnTMMessageReceiveFailed(ConnectionUID connection_id,
                                        const DataReceiveError& error){}

  virtual void OnTMMessageSend() {
  }
  virtual void OnTMMessageSendFailed(const DataSendError& error,
                                     const RawMessageSptr& message) {
  }
};
}  // namespace transport_manager
#endif  //  SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_TRANSPORT_MANAGER_LISTENER_IMPL
