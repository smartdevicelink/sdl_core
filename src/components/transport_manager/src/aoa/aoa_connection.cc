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

#include "transport_manager/aoa/aoa_connection.h"

#include "utils/logger.h"
#include "transport_manager/transport_adapter/transport_adapter_controller.h"
#include "transport_manager/aoa/aoa_wrapper.h"

namespace transport_manager {
namespace transport_adapter {

CREATE_LOGGERPTR_GLOBAL(logger_, "TransportManager")

AOAConnection::AOAConnection(const DeviceUID& device_uid,
                             const ApplicationHandle& app_handle,
                             TransportAdapterController* controller,
                             AOAWrapper::AOAHandle aoa_handle)
    : wrapper_(new AOAWrapper(aoa_handle)),
      device_uid_(device_uid),
      app_handle_(app_handle),
      controller_(controller),
      observer_(new DeviceObserver(this)) {
  wrapper_->Subscribe(observer_);
}

AOAConnection::~AOAConnection() {
  wrapper_->Unsubscribe();
  delete observer_;
  delete wrapper_;
}

TransportAdapter::Error AOAConnection::SendData(RawMessagePtr message) {
  LOG4CXX_TRACE(logger_, "AOA: send data to " << device_uid_ << " " << app_handle_);
  if (wrapper_->SendMessage(message)) {
    controller_->DataSendDone(device_uid_, app_handle_, message);
    return TransportAdapter::OK;
  } else {
    controller_->DataSendFailed(device_uid_, app_handle_, message, DataSendError());
    return TransportAdapter::FAIL;
  }
}

TransportAdapter::Error AOAConnection::Disconnect() {
  LOG4CXX_TRACE(logger_, "AOA: disconnect " << device_uid_ << " " << app_handle_);
  controller_->DisconnectDone(device_uid_, app_handle_);
  return TransportAdapter::OK;
}

void AOAConnection::DeviceObserver::OnReceivedMessage(RawMessagePtr message) {
  //controller_->DataReceiveDone(message);
}

void AOAConnection::DeviceObserver::OnTransmittedMessage(
    AOAWrapper::AOAHandle handle, bool success) {
}

}  // namespace transport_adapter
}  // namespace transport_manager

