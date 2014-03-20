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

#include "transport_manager/usb/qnx/usb_iap_connection.h"
#include "transport_manager/transport_adapter/transport_adapter_impl.h"

namespace transport_manager {
namespace transport_adapter {

const char* UsbIAPConnection::protocol = "com.qnx.eaf";  // TODO(nvaganov@luxoft.com) choose protocol name

UsbIAPConnection::UsbIAPConnection(const DeviceUID& device_uid,
  const ApplicationHandle& app_handle,
  TransportAdapterController* controller,
  const char* device_path) : device_uid_(device_uid),
  app_handle_(app_handle),
  controller_(controller),
  device_path_(device_path) {
}

bool UsbIAPConnection::Init() {
  LOG4CXX_TRACE(logger_, "Connecting to " << device_path_);
  ipod_hdl_ = ipod_connect(device_path_.c_str(), 0);
  if (ipod_hdl_ != 0) {
    LOG4CXX_DEBUG(logger_, "Connected to " << device_path_);
  }
  else {
    LOG4CXX_ERROR(logger_, "Could not connect to " << device_path_);
    return false;
  }

  LOG4CXX_TRACE(logger_, "Opening protocol " << protocol);
// ipod_eaf_addprotocol() accepts pointer to non-const char
  int protocol_id = ipod_eaf_addprotocol(ipod_hdl_, const_cast<char*>(protocol));
  ipod_reidentify(ipod_hdl_);
  ipod_eaf_session_accept(ipod_hdl_, protocol_id, 0);
  session_id_ = ipod_eaf_session_open(ipod_hdl_, protocol_id);
  if (session_id_ != -1) {
    LOG4CXX_DEBUG(logger_, "Protocol " << protocol << " opened");
  }
  else {
    LOG4CXX_ERROR(logger_, "Could not open protocol " << protocol);
    return false;
  }

  receiver_thread_ = new threads::Thread("USB iAP receiver",
    new ReceiverThreadDelegate(ipod_hdl_, session_id_, this));
  receiver_thread_->start();

  controller_->ConnectDone(device_uid_, app_handle_);
  return true;
}

TransportAdapter::Error UsbIAPConnection::SendData(RawMessageSptr message) {
  LOG4CXX_TRACE(logger_, "USB iAP: sending data");
  if (ipod_eaf_send(ipod_hdl_, session_id_, message->data(), message->data_size()) != -1) {
    LOG4CXX_INFO(logger_, "USB iAP: data sent successfully");
    controller_->DataSendDone(device_uid_, app_handle_, message);
    return TransportAdapter::OK;
  }
  else {
    LOG4CXX_WARN(logger_, "USB iAP: error occured while sending data");
    controller_->DataSendFailed(device_uid_, app_handle_, message, DataSendError());
    return TransportAdapter::FAIL;
  }
}

TransportAdapter::Error UsbIAPConnection::Disconnect() {
  TransportAdapter::Error error = TransportAdapter::OK;

  receiver_thread_->stop();

  LOG4CXX_TRACE(logger_, "Closing protocol " << protocol);
  if (ipod_eaf_session_free(ipod_hdl_, session_id_) != -1) {
    LOG4CXX_DEBUG(logger_, "Protocol " << protocol << " closed");
  }
  else {
    LOG4CXX_WARN(logger_, "Could not close protocol " << protocol);
    error = TransportAdapter::FAIL;
  }

  LOG4CXX_TRACE(logger_, "Disconnecting from " << device_path_);
  if (ipod_disconnect(ipod_hdl_) != -1) {
    LOG4CXX_DEBUG(logger_, "Disconnected from " << device_path_);
  }
  else {
    LOG4CXX_WARN(logger_, "Could not disconnect from " << device_path_);
    error = TransportAdapter::FAIL;
  }

  controller_->DisconnectDone(device_uid_, app_handle_);
  return error;
}

void UsbIAPConnection::OnDataReceived(RawMessageSptr message) {
  controller_->DataReceiveDone(device_uid_, app_handle_, message);
}

void UsbIAPConnection::OnReceiveFailed() {
  controller_->DataReceiveFailed(device_uid_, app_handle_, DataReceiveError());
}

UsbIAPConnection::ReceiverThreadDelegate::ReceiverThreadDelegate(
  ipod_hdl_t* ipod_hdl, int session_id, UsbIAPConnection* parent) :
  parent_(parent), ipod_hdl_(ipod_hdl), session_id_(session_id) {
}

bool UsbIAPConnection::ReceiverThreadDelegate::ArmEvent(struct sigevent* event) {
  LOG4CXX_TRACE(logger_, "Arming for USB iAP input notification");
  if (ipod_notify(ipod_hdl_, _NOTIFY_ACTION_POLLARM, _NOTIFY_COND_INPUT, event) != -1) {
    LOG4CXX_DEBUG(logger_, "Successfully armed for USB iAP input notification");
    return true;
  }
  else {
    LOG4CXX_WARN(logger_, "Could not arm for USB iAP input notification");
    return false;
  }
}

void UsbIAPConnection::ReceiverThreadDelegate::OnPulse() {
  receive();
}

void UsbIAPConnection::ReceiverThreadDelegate::receive() {
// this method can be invoked from the only thread
// thus it does not need any synchronization
  LOG4CXX_TRACE(logger_, "USB iAP: receiving data");
  int size = ipod_eaf_recv(ipod_hdl_, session_id_, buffer_, kBufferSize);
  if (size != -1) {
    LOG4CXX_INFO(logger_, "USB iAP: received " << size << " bytes");
    RawMessageSptr message(new protocol_handler::RawMessage(0, 0, buffer_, size));
    parent_->OnDataReceived(message);
  }
  else {
    LOG4CXX_WARN(logger_, "USB iAP: error occured while receiving data");
    parent_->OnReceiveFailed();
  }
}

}  // namespace transport_adapter
}  // namespace transport_manager
