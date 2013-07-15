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

#include <string>
#include <set>
#include "protocol_handler/raw_message.h"

namespace transport_manager {

struct DeviceInfo {
  std::string name;
  int handle;
};

// TODO(PV):
enum TMResult {
  OK = 0,
  FAIL,
  CONNECTION_CLOSED_BY_APP,
  SENDING_FAILED_WHO_KNOWS_WHY
};

typedef int ConnectionID;

class TransportManagerListener {
  public:
    virtual ~TransportManagerListener() = 0;

    // TODO(PV): can be used other container type.
    virtual void OnDeviceListUpdated(
      const std::set<DeviceInfo>& device_list) = 0;
    virtual void OnScanDevicesFinished(TMResult result) = 0;
    virtual void onConnectionEstablished(ConnectionID connection_id) = 0;
    virtual void onConnectionClosed(ConnectionID connection_id,
                                    TMResult reason) = 0;

    // TODO(PV): is it frame or whole message or just some number of bytes?
    virtual void onFrameReceived(
      const protocol_handler::RawMessage& message) = 0;

    // TODO(PV): probably we don't need to give message back,
    // maybe we should instead some messaged id/correlationID/serial_number?
    virtual void onFrameSend(
      const protocol_handler::RawMessage& message,
      TMResult& result) = 0;
};
}  //  namespace transport_manager
#endif  //  SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_TRANSPORT_MANAGER_LISTENER
