/**
 * \file transport_adapter_listener_impl.cc
 * \brief Listener class source file.
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

#include "transport_manager/transport_adapter/transport_adapter_listener_impl.h"

#include <algorithm>

#include "transport_manager/transport_manager_impl.h"
#include "transport_manager/common.h"

using transport_manager::transport_adapter::TransportAdapter;

namespace transport_manager {

log4cxx::LoggerPtr TransportAdapterListenerImpl::logger_ = log4cxx::LoggerPtr(
    log4cxx::Logger::getLogger("TransportAdapterListener"));

TransportAdapterListenerImpl::~TransportAdapterListenerImpl() {
}

TransportAdapterListenerImpl::TransportAdapterListenerImpl(
    transport_manager::TransportManagerImpl *tm) {
  transport_manager_impl_ = tm;
}

void TransportAdapterListenerImpl::OnSearchDeviceDone(
    const transport_adapter::TransportAdapter *transport_adapter) {
  AdapterIterator it;
  if (!FindSharedPtr(transport_adapter, it)) {
    LOG4CXX_ERROR(logger_, "Can't find device adapter " << transport_adapter);
    return;
  }

  TransportAdapterEvent event(
      TransportAdapterListenerImpl::EventTypeEnum::ON_SEARCH_DONE,
      *it, "", 0, RawMessageSptr(), new BaseError());

  transport_manager_impl_->ReceiveEventFromDevice(event);
}

void TransportAdapterListenerImpl::OnSearchDeviceFailed(
    const transport_adapter::TransportAdapter* transport_adapter,
    const SearchDeviceError& error) {
  AdapterIterator it;
  if (!FindSharedPtr(transport_adapter, it)) {
    LOG4CXX_ERROR(logger_, "Can't find device adapter " << transport_adapter);
    return;
  }

  SearchDeviceError *err = new SearchDeviceError(error);
  TransportAdapterEvent event(
      TransportAdapterListenerImpl::EventTypeEnum::ON_SEARCH_FAIL,
      *it, "", 0, RawMessageSptr(),
      err);

  transport_manager_impl_->ReceiveEventFromDevice(event);
}

void TransportAdapterListenerImpl::OnConnectDone(
    const TransportAdapter* transport_adapter, const DeviceUID& device,
    const ApplicationHandle& application_id) {
  AdapterIterator it;
  if (!FindSharedPtr(transport_adapter, it)) {
    LOG4CXX_ERROR(logger_, "Can't find device adapter " << transport_adapter);
    return;
  }

  TransportAdapterEvent event(
      TransportAdapterListenerImpl::EventTypeEnum::ON_CONNECT_DONE,
      *it, device, application_id,
      RawMessageSptr(), new BaseError());

  transport_manager_impl_->ReceiveEventFromDevice(event);
}

void TransportAdapterListenerImpl::OnConnectFailed(
    const TransportAdapter* transport_adapter, const DeviceUID& device,
    const ApplicationHandle& app_id, const ConnectError& error) {
  AdapterIterator it;
  if (!FindSharedPtr(transport_adapter, it)) {
    LOG4CXX_ERROR(logger_, "Can't find device adapter " << transport_adapter);
    return;
  }

  ConnectError *err = new ConnectError(error);
  TransportAdapterEvent event(
      TransportAdapterListenerImpl::EventTypeEnum::ON_CONNECT_FAIL,
      *it, device, app_id, RawMessageSptr(),
      err);

  transport_manager_impl_->ReceiveEventFromDevice(event);
}

void TransportAdapterListenerImpl::OnDisconnectDone(
    const TransportAdapter* transport_adapter, const DeviceUID& device,
    const ApplicationHandle& app_id) {
  AdapterIterator it;
  if (!FindSharedPtr(transport_adapter, it)) {
    LOG4CXX_ERROR(logger_, "Can't find device adapter " << transport_adapter);
    return;
  }

  TransportAdapterEvent event(
      TransportAdapterListenerImpl::EventTypeEnum::ON_DISCONNECT_DONE,
      *it, device, app_id, RawMessageSptr(),
      new BaseError());

  transport_manager_impl_->ReceiveEventFromDevice(event);
}

void TransportAdapterListenerImpl::OnDisconnectFailed(
    const TransportAdapter* transport_adapter, const DeviceUID& device,
    const ApplicationHandle& app_id, const DisconnectError& error) {
  AdapterIterator it;
  if (!FindSharedPtr(transport_adapter, it)) {
    LOG4CXX_ERROR(logger_, "Can't find device adapter " << transport_adapter);
    return;
  }

  DisconnectError *err = new DisconnectError(error);
  TransportAdapterEvent event(
      TransportAdapterListenerImpl::EventTypeEnum::ON_DISCONNECT_FAIL,
      *it, device, app_id, RawMessageSptr(),
      err);

  transport_manager_impl_->ReceiveEventFromDevice(event);
}

void TransportAdapterListenerImpl::OnDisconnectDeviceDone(
    const TransportAdapter* transport_adapter, const DeviceUID& device) {

}

void TransportAdapterListenerImpl::OnDisconnectDeviceFailed(
    const TransportAdapter* transport_adapter, const DeviceUID& device,
    const DisconnectDeviceError& error) {

}

void TransportAdapterListenerImpl::OnDataReceiveDone(
    const TransportAdapter* transport_adapter, const DeviceUID& device,
    const ApplicationHandle& app_id, const RawMessageSptr data_container) {
  AdapterIterator it;
  if (!FindSharedPtr(transport_adapter, it)) {
    LOG4CXX_ERROR(logger_, "Can't find device adapter " << transport_adapter);
    return;
  }

  TransportAdapterEvent event(
      TransportAdapterListenerImpl::EventTypeEnum::ON_RECEIVED_DONE,
      *it, device, app_id, data_container,
      new BaseError());

  transport_manager_impl_->ReceiveEventFromDevice(event);
}

void TransportAdapterListenerImpl::OnDataReceiveFailed(
    const TransportAdapter* transport_adapter, const DeviceUID& device,
    const ApplicationHandle& app_id,

    const DataReceiveError& error) {
  AdapterIterator it;
  if (!FindSharedPtr(transport_adapter, it)) {
    LOG4CXX_ERROR(logger_, "Can't find device adapter " << transport_adapter);
    return;
  }

  DataReceiveError *err = new DataReceiveError(error);
  TransportAdapterEvent event(
      TransportAdapterListenerImpl::EventTypeEnum::ON_RECEIVED_DONE,
      *it, device, app_id, RawMessageSptr(),
      err);

  transport_manager_impl_->ReceiveEventFromDevice(event);
}

void TransportAdapterListenerImpl::OnDataSendDone(
    const TransportAdapter* transport_adapter, const DeviceUID& device,
    const ApplicationHandle& app_id, const RawMessageSptr data_container) {
  AdapterIterator it;
  if (!FindSharedPtr(transport_adapter, it)) {
    LOG4CXX_ERROR(logger_, "Can't find device adapter " << transport_adapter);
    return;
  }

  TransportAdapterEvent event(
      TransportAdapterListenerImpl::EventTypeEnum::ON_SEND_DONE,
      *it, device, app_id, data_container,
      new BaseError());
  transport_manager_impl_->ReceiveEventFromDevice(event);
}

void TransportAdapterListenerImpl::OnDataSendFailed(
    const TransportAdapter* transport_adapter, const DeviceUID& device,
    const ApplicationHandle& app_id, const RawMessageSptr data_container,
    const DataSendError& error) {
  AdapterIterator it;
  if (!FindSharedPtr(transport_adapter, it)) {
    LOG4CXX_ERROR(logger_, "Can't find device adapter " << transport_adapter);
    return;
  }

  DataSendError *err = new DataSendError(error);
  TransportAdapterEvent event(
      TransportAdapterListenerImpl::EventTypeEnum::ON_SEND_FAIL,
      *it, device, app_id, data_container,
      err);

  transport_manager_impl_->ReceiveEventFromDevice(event);
}

void TransportAdapterListenerImpl::OnConnectRequested(
    const TransportAdapter* transport_adapter, const DeviceUID& device_handle,
    const ApplicationHandle& app_handle) {
}

void TransportAdapterListenerImpl::OnUnexpectedDisconnect(
    const transport_adapter::TransportAdapter* transport_adapter,
    const DeviceUID& device, const ApplicationHandle& application,
    const CommunicationError& error) {
  AdapterIterator it;
  if (!FindSharedPtr(transport_adapter, it)) {
    LOG4CXX_ERROR(logger_, "Can't find device adapter " << transport_adapter);
    return;
  }

  TransportAdapterEvent event(
      TransportAdapterListenerImpl::EventTypeEnum::ON_UNEXPECTED_DISCONNECT,
      *it, device, application, RawMessageSptr(),
      nullptr);

  transport_manager_impl_->ReceiveEventFromDevice(event);
}

void TransportAdapterListenerImpl::OnCommunicationError(
    const TransportAdapter* transport_adapter, const DeviceUID& device,
    const ApplicationHandle& app_id) {
  AdapterIterator it;
  if (!FindSharedPtr(transport_adapter, it)) {
    LOG4CXX_ERROR(logger_, "Can't find device adapter " << transport_adapter);
    return;
  }

  TransportAdapterEvent event(
      TransportAdapterListenerImpl::EventTypeEnum::ON_COMMUNICATION_ERROR,
      *it, device, app_id, RawMessageSptr(),
      new BaseError());

  transport_manager_impl_->ReceiveEventFromDevice(event);
}

bool TransportAdapterListenerImpl::FindSharedPtr(
    const TransportAdapter* transport_adapter, AdapterIterator &it) {
  it = std::find(transport_manager_impl_->transport_adapters_.begin(),
                 transport_manager_impl_->transport_adapters_.end(),
                 transport_adapter);

  return it != transport_manager_impl_->transport_adapters_.end();
}

}  //namespace transport_manager

