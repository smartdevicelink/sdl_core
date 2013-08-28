/**
 * \file device_adapter_listener_impl.cc
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

#include "transport_manager/device_adapter/device_adapter_listener_impl.h"

#include <algorithm>

#include "transport_manager/transport_manager_impl.h"
#include "transport_manager/common.h"

using transport_manager::device_adapter::DeviceAdapter;

namespace transport_manager {

log4cxx::LoggerPtr DeviceAdapterListenerImpl::logger_ = log4cxx::LoggerPtr(
    log4cxx::Logger::getLogger("DeviceAdapterListener"));

DeviceAdapterListenerImpl::~DeviceAdapterListenerImpl() {
}

DeviceAdapterListenerImpl::DeviceAdapterListenerImpl(
    transport_manager::TransportManagerImpl *tm) {
  transport_manager_impl_ = tm;
}

void DeviceAdapterListenerImpl::OnSearchDeviceDone(
    const device_adapter::DeviceAdapter *device_adapter) {
  AdapterIterator it;
  if (!FindSharedPtr(device_adapter, it)) {
    LOG4CXX_ERROR(logger_, "Can't find device adapter " << device_adapter);
    return;
  }

  DeviceAdapterEvent event(
      DeviceAdapterListenerImpl::EventTypeEnum::ON_SEARCH_DONE,
      *it, "", 0, RawMessageSptr(), new BaseError());

  transport_manager_impl_->ReceiveEventFromDevice(event);
}

void DeviceAdapterListenerImpl::OnSearchDeviceFailed(
    const device_adapter::DeviceAdapter* device_adapter,
    const SearchDeviceError& error) {
  AdapterIterator it;
  if (!FindSharedPtr(device_adapter, it)) {
    LOG4CXX_ERROR(logger_, "Can't find device adapter " << device_adapter);
    return;
  }

  SearchDeviceError *err = new SearchDeviceError(error);
  DeviceAdapterEvent event(
      DeviceAdapterListenerImpl::EventTypeEnum::ON_SEARCH_FAIL,
      *it, "", 0, RawMessageSptr(),
      err);

  transport_manager_impl_->ReceiveEventFromDevice(event);
}

void DeviceAdapterListenerImpl::OnConnectDone(
    const DeviceAdapter* device_adapter, const DeviceUID& device,
    const ApplicationHandle& application_id) {
  AdapterIterator it;
  if (!FindSharedPtr(device_adapter, it)) {
    LOG4CXX_ERROR(logger_, "Can't find device adapter " << device_adapter);
    return;
  }

  DeviceAdapterEvent event(
      DeviceAdapterListenerImpl::EventTypeEnum::ON_CONNECT_DONE,
      *it, device, application_id,
      RawMessageSptr(), new BaseError());

  transport_manager_impl_->ReceiveEventFromDevice(event);
}

void DeviceAdapterListenerImpl::OnConnectFailed(
    const DeviceAdapter* device_adapter, const DeviceUID& device,
    const ApplicationHandle& app_id, const ConnectError& error) {
  AdapterIterator it;
  if (!FindSharedPtr(device_adapter, it)) {
    LOG4CXX_ERROR(logger_, "Can't find device adapter " << device_adapter);
    return;
  }

  ConnectError *err = new ConnectError(error);
  DeviceAdapterEvent event(
      DeviceAdapterListenerImpl::EventTypeEnum::ON_CONNECT_FAIL,
      *it, device, app_id, RawMessageSptr(),
      err);

  transport_manager_impl_->ReceiveEventFromDevice(event);
}

void DeviceAdapterListenerImpl::OnDisconnectDone(
    const DeviceAdapter* device_adapter, const DeviceUID& device,
    const ApplicationHandle& app_id) {
  AdapterIterator it;
  if (!FindSharedPtr(device_adapter, it)) {
    LOG4CXX_ERROR(logger_, "Can't find device adapter " << device_adapter);
    return;
  }

  DeviceAdapterEvent event(
      DeviceAdapterListenerImpl::EventTypeEnum::ON_DISCONNECT_DONE,
      *it, device, app_id, RawMessageSptr(),
      new BaseError());

  transport_manager_impl_->ReceiveEventFromDevice(event);
}

void DeviceAdapterListenerImpl::OnDisconnectFailed(
    const DeviceAdapter* device_adapter, const DeviceUID& device,
    const ApplicationHandle& app_id, const DisconnectError& error) {
  AdapterIterator it;
  if (!FindSharedPtr(device_adapter, it)) {
    LOG4CXX_ERROR(logger_, "Can't find device adapter " << device_adapter);
    return;
  }

  DisconnectError *err = new DisconnectError(error);
  DeviceAdapterEvent event(
      DeviceAdapterListenerImpl::EventTypeEnum::ON_DISCONNECT_FAIL,
      *it, device, app_id, RawMessageSptr(),
      err);

  transport_manager_impl_->ReceiveEventFromDevice(event);
}

void DeviceAdapterListenerImpl::OnDisconnectDeviceDone(
    const DeviceAdapter* device_adapter, const DeviceUID& device) {

}

void DeviceAdapterListenerImpl::OnDisconnectDeviceFailed(
    const DeviceAdapter* device_adapter, const DeviceUID& device,
    const DisconnectDeviceError& error) {

}

void DeviceAdapterListenerImpl::OnDataReceiveDone(
    const DeviceAdapter* device_adapter, const DeviceUID& device,
    const ApplicationHandle& app_id, const RawMessageSptr data_container) {
  AdapterIterator it;
  if (!FindSharedPtr(device_adapter, it)) {
    LOG4CXX_ERROR(logger_, "Can't find device adapter " << device_adapter);
    return;
  }

  DeviceAdapterEvent event(
      DeviceAdapterListenerImpl::EventTypeEnum::ON_RECEIVED_DONE,
      *it, device, app_id, data_container,
      new BaseError());

  transport_manager_impl_->ReceiveEventFromDevice(event);
}

void DeviceAdapterListenerImpl::OnDataReceiveFailed(
    const DeviceAdapter* device_adapter, const DeviceUID& device,
    const ApplicationHandle& app_id,

    const DataReceiveError& error) {
  AdapterIterator it;
  if (!FindSharedPtr(device_adapter, it)) {
    LOG4CXX_ERROR(logger_, "Can't find device adapter " << device_adapter);
    return;
  }

  DataReceiveError *err = new DataReceiveError(error);
  DeviceAdapterEvent event(
      DeviceAdapterListenerImpl::EventTypeEnum::ON_RECEIVED_DONE,
      *it, device, app_id, RawMessageSptr(),
      err);

  transport_manager_impl_->ReceiveEventFromDevice(event);
}

void DeviceAdapterListenerImpl::OnDataSendDone(
    const DeviceAdapter* device_adapter, const DeviceUID& device,
    const ApplicationHandle& app_id, const RawMessageSptr data_container) {
  AdapterIterator it;
  if (!FindSharedPtr(device_adapter, it)) {
    LOG4CXX_ERROR(logger_, "Can't find device adapter " << device_adapter);
    return;
  }

  DeviceAdapterEvent event(
      DeviceAdapterListenerImpl::EventTypeEnum::ON_SEND_DONE,
      *it, device, app_id, data_container,
      new BaseError());
  transport_manager_impl_->ReceiveEventFromDevice(event);
}

void DeviceAdapterListenerImpl::OnDataSendFailed(
    const DeviceAdapter* device_adapter, const DeviceUID& device,
    const ApplicationHandle& app_id, const RawMessageSptr data_container,
    const DataSendError& error) {
  AdapterIterator it;
  if (!FindSharedPtr(device_adapter, it)) {
    LOG4CXX_ERROR(logger_, "Can't find device adapter " << device_adapter);
    return;
  }

  DataSendError *err = new DataSendError(error);
  DeviceAdapterEvent event(
      DeviceAdapterListenerImpl::EventTypeEnum::ON_SEND_FAIL,
      *it, device, app_id, data_container,
      err);

  transport_manager_impl_->ReceiveEventFromDevice(event);
}

void DeviceAdapterListenerImpl::OnConnectRequested(
    const DeviceAdapter* device_adapter, const DeviceUID& device_handle,
    const ApplicationHandle& app_handle) {
}

void DeviceAdapterListenerImpl::OnUnexpectedDisconnect(
    const device_adapter::DeviceAdapter* device_adapter,
    const DeviceUID& device, const ApplicationHandle& application,
    const CommunicationError& error) {
  AdapterIterator it;
  if (!FindSharedPtr(device_adapter, it)) {
    LOG4CXX_ERROR(logger_, "Can't find device adapter " << device_adapter);
    return;
  }

  DeviceAdapterEvent event(
      DeviceAdapterListenerImpl::EventTypeEnum::ON_UNEXPECTED_DISCONNECT,
      *it, device, application, RawMessageSptr(),
      nullptr);

  transport_manager_impl_->ReceiveEventFromDevice(event);
}

void DeviceAdapterListenerImpl::OnCommunicationError(
    const DeviceAdapter* device_adapter, const DeviceUID& device,
    const ApplicationHandle& app_id) {
  AdapterIterator it;
  if (!FindSharedPtr(device_adapter, it)) {
    LOG4CXX_ERROR(logger_, "Can't find device adapter " << device_adapter);
    return;
  }

  DeviceAdapterEvent event(
      DeviceAdapterListenerImpl::EventTypeEnum::ON_COMMUNICATION_ERROR,
      *it, device, app_id, RawMessageSptr(),
      new BaseError());

  transport_manager_impl_->ReceiveEventFromDevice(event);
}

bool DeviceAdapterListenerImpl::FindSharedPtr(
    const DeviceAdapter* device_adapter, AdapterIterator &it) {
  it = std::find(transport_manager_impl_->device_adapters_.begin(),
                 transport_manager_impl_->device_adapters_.end(),
                 device_adapter);

  return it != transport_manager_impl_->device_adapters_.end();
}

}  //namespace transport_manager

