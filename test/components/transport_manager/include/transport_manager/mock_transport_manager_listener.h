/*
 * \file mock_device_adapter_listener.h
 * \brief 
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

#ifndef MOCK_TRANSPORT_MANAGER_LISTENER_H
#define MOCK_TRANSPORT_MANAGER_LISTENER_H

#include <gmock/gmock.h>
#include <transport_manager/common.h>
#include <transport_manager/device_adapter.h>
#include <transport_manager/device_handle_generator.h>
#include <transport_manager/transport_manager_listener_impl.h>

using namespace transport_manager;

namespace test  {
namespace components  {
namespace transport_manager {

class MockTransportManagerListener : public ::transport_manager::TransportManagerListenerImpl
{
  public:
  MOCK_METHOD2(onDeviceFound, void (const DeviceHandle device,
                                         const ApplicationList app_list));
  MOCK_METHOD0(onSearchDeviceDone, void ());
  MOCK_METHOD2(onSearchDeviceFailed, void (const DeviceAdapter* device_adapter,
                                           const SearchDeviceError& error));
  MOCK_METHOD2(onConnectDone, void(const DeviceAdapter* device_adapter,
                             const SessionID session_id));
  MOCK_METHOD3(onDataReceiveDone, void(const DeviceAdapter* device_adapter,
                                 const SessionID session_id,
                                 const RawMessageSptr data_container));
  MOCK_METHOD3(onDataSendDone, void(const DeviceAdapter* device_adapter,
                              const SessionID session_id,
                              const RawMessageSptr data_container));
  MOCK_METHOD3(onDataSendFailed, void(const DeviceAdapter* device_adapter,
                                const SessionID session_id,
                                const DataSendError& error));
  MOCK_METHOD3(onDataReceiveFailed, void (const DeviceAdapter* device_adapter,
                                   const SessionID session_id,
                                   const DataReceiveError& error));
  MOCK_METHOD2(onCommunicationError, void (const DeviceAdapter* device_adapter,
                                    const SessionID session_id));
  MOCK_METHOD2(onDisconnectDeviceDone, void(const DeviceAdapter* device_adapter,
                                      const SessionID session_id));
  MOCK_METHOD2(onDisconnectDone, void(const DeviceAdapter* device_adapter,
                                      const SessionID session_id));
  ~MockTransportManagerListener() { }

};

}}}


#endif /* MOCK_TRANSPORT_MANAGER_LISTENER_H */
