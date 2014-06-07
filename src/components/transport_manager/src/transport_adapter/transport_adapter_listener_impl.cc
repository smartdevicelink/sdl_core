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

#include <algorithm>

#include "utils/logger.h"

#include "transport_manager/transport_adapter/transport_adapter_listener_impl.h"
#include "transport_manager/transport_manager_impl.h"
#include "transport_manager/transport_adapter/transport_adapter_event.h"

namespace transport_manager {

CREATE_LOGGERPTR_GLOBAL(logger_, "TransportAdapterImpl")

TransportAdapterListenerImpl::~TransportAdapterListenerImpl() {}

TransportAdapterListenerImpl::TransportAdapterListenerImpl(
    TransportManagerImpl* manager, TransportAdapter* adapter)
    : transport_manager_impl_(manager), transport_adapter_(adapter) {}

void TransportAdapterListenerImpl::OnSearchDeviceDone(
    const TransportAdapter* adapter) {
  TransportAdapterEvent event(
      TransportAdapterListenerImpl::EventTypeEnum::ON_SEARCH_DONE,
      transport_adapter_, "", 0, RawMessageSptr(), new BaseError());

  if (transport_manager::E_SUCCESS !=
      transport_manager_impl_->ReceiveEventFromDevice(event)) {
    LOG4CXX_WARN(logger_, "Failed to receive event from device");
  }
}

void TransportAdapterListenerImpl::OnSearchDeviceFailed(
    const TransportAdapter* adapter,
    const SearchDeviceError& error) {
  SearchDeviceError* err = new SearchDeviceError(error);
  TransportAdapterEvent event(
      TransportAdapterListenerImpl::EventTypeEnum::ON_SEARCH_FAIL,
      transport_adapter_, "", 0, RawMessageSptr(), err);

  if (transport_manager::E_SUCCESS !=
      transport_manager_impl_->ReceiveEventFromDevice(event)) {
    LOG4CXX_WARN(logger_, "Failed to receive event from device");
  }
}

void TransportAdapterListenerImpl::OnDeviceListUpdated(
    const TransportAdapter* adapter) {
  TransportAdapterEvent event(
      TransportAdapterListenerImpl::EventTypeEnum::ON_DEVICE_LIST_UPDATED,
      transport_adapter_, "", 0, RawMessageSptr(), NULL);

  if (transport_manager::E_SUCCESS !=
      transport_manager_impl_->ReceiveEventFromDevice(event)) {
    LOG4CXX_WARN(logger_, "Failed to receive event from device");
  }
}

void TransportAdapterListenerImpl::OnFindNewApplicationsRequest(const TransportAdapter* adapter) {
  TransportAdapterEvent event(
    TransportAdapterListenerImpl::ON_FIND_NEW_APPLICATIONS_REQUEST,
    transport_adapter_,
    0,
    0,
    RawMessageSptr(),
    NULL
  );

  if (transport_manager::E_SUCCESS !=
      transport_manager_impl_->ReceiveEventFromDevice(event)) {
    LOG4CXX_WARN(logger_, "Failed to receive event from device");
  }
}

void TransportAdapterListenerImpl::OnConnectDone(
    const TransportAdapter* adapter, const DeviceUID& device,
    const ApplicationHandle& application_id) {
  TransportAdapterEvent event(
      TransportAdapterListenerImpl::EventTypeEnum::ON_CONNECT_DONE,
      transport_adapter_, device, application_id, RawMessageSptr(),
      new BaseError());

  if (transport_manager::E_SUCCESS !=
      transport_manager_impl_->ReceiveEventFromDevice(event)) {
    LOG4CXX_WARN(logger_, "Failed to receive event from device");
  }
}

void TransportAdapterListenerImpl::OnConnectFailed(
    const TransportAdapter* adapter, const DeviceUID& device,
    const ApplicationHandle& app_id, const ConnectError& error) {
  ConnectError* err = new ConnectError(error);
  TransportAdapterEvent event(
      TransportAdapterListenerImpl::EventTypeEnum::ON_CONNECT_FAIL,
      transport_adapter_, device, app_id, RawMessageSptr(), err);

  if (transport_manager::E_SUCCESS !=
      transport_manager_impl_->ReceiveEventFromDevice(event)) {
    LOG4CXX_WARN(logger_, "Failed to receive event from device");
  }
}

void TransportAdapterListenerImpl::OnDisconnectDone(
    const TransportAdapter* adapter, const DeviceUID& device,
    const ApplicationHandle& app_id) {
  TransportAdapterEvent event(
      TransportAdapterListenerImpl::EventTypeEnum::ON_DISCONNECT_DONE,
      transport_adapter_, device, app_id, RawMessageSptr(), new BaseError());

  if (transport_manager::E_SUCCESS !=
      transport_manager_impl_->ReceiveEventFromDevice(event)) {
    LOG4CXX_WARN(logger_, "Failed to receive event from device");
  }
}

void TransportAdapterListenerImpl::OnDisconnectFailed(
    const TransportAdapter* adapter, const DeviceUID& device,
    const ApplicationHandle& app_id, const DisconnectError& error) {
  DisconnectError* err = new DisconnectError(error);
  TransportAdapterEvent event(
      TransportAdapterListenerImpl::EventTypeEnum::ON_DISCONNECT_FAIL,
      transport_adapter_, device, app_id, RawMessageSptr(), err);

  if (transport_manager::E_SUCCESS !=
      transport_manager_impl_->ReceiveEventFromDevice(event)) {
    LOG4CXX_WARN(logger_, "Failed to receive event from device");
  }
}

void TransportAdapterListenerImpl::OnDisconnectDeviceDone(
    const TransportAdapter* adapter, const DeviceUID& device) {}

void TransportAdapterListenerImpl::OnDisconnectDeviceFailed(
    const TransportAdapter* adapter, const DeviceUID& device,
    const DisconnectDeviceError& error) {}

void TransportAdapterListenerImpl::OnDataReceiveDone(
    const TransportAdapter* adapter, const DeviceUID& device,
    const ApplicationHandle& app_id, const RawMessageSptr data_container) {
  TransportAdapterEvent event(
      TransportAdapterListenerImpl::EventTypeEnum::ON_RECEIVED_DONE,
      transport_adapter_, device, app_id, data_container, new BaseError());

  if (transport_manager::E_SUCCESS !=
      transport_manager_impl_->ReceiveEventFromDevice(event)) {
    LOG4CXX_WARN(logger_, "Failed to receive event from device");
  }
}

void TransportAdapterListenerImpl::OnDataReceiveFailed(
    const TransportAdapter* adapter, const DeviceUID& device,
    const ApplicationHandle& app_id, const DataReceiveError& error) {
  DataReceiveError* err = new DataReceiveError(error);
  TransportAdapterEvent event(
      TransportAdapterListenerImpl::EventTypeEnum::ON_RECEIVED_FAIL,
      transport_adapter_, device, app_id, RawMessageSptr(), err);

  if (transport_manager::E_SUCCESS !=
      transport_manager_impl_->ReceiveEventFromDevice(event)) {
    LOG4CXX_WARN(logger_, "Failed to receive event from device");
  }
}

void TransportAdapterListenerImpl::OnDataSendDone(
    const TransportAdapter* adapter, const DeviceUID& device,
    const ApplicationHandle& app_id, const RawMessageSptr data_container) {
  TransportAdapterEvent event(
      TransportAdapterListenerImpl::EventTypeEnum::ON_SEND_DONE,
      transport_adapter_, device, app_id, data_container, new BaseError());

  if (transport_manager::E_SUCCESS !=
      transport_manager_impl_->ReceiveEventFromDevice(event)) {
    LOG4CXX_WARN(logger_, "Failed to receive event from device");
  }
}

void TransportAdapterListenerImpl::OnDataSendFailed(
    const TransportAdapter* adapter, const DeviceUID& device,
    const ApplicationHandle& app_id, const RawMessageSptr data_container,
    const DataSendError& error) {
  DataSendError* err = new DataSendError(error);
  TransportAdapterEvent event(
      TransportAdapterListenerImpl::EventTypeEnum::ON_SEND_FAIL,
      transport_adapter_, device, app_id, data_container, err);

  if (transport_manager::E_SUCCESS !=
      transport_manager_impl_->ReceiveEventFromDevice(event)) {
    LOG4CXX_WARN(logger_, "Failed to receive event from device");
  }
}

void TransportAdapterListenerImpl::OnConnectRequested(
    const TransportAdapter* adapter, const DeviceUID& device_handle,
    const ApplicationHandle& app_handle) {}

void TransportAdapterListenerImpl::OnUnexpectedDisconnect(
    const TransportAdapter* adapter,
    const DeviceUID& device, const ApplicationHandle& application,
    const CommunicationError& error) {
  CommunicationError* err = new CommunicationError(error);
  TransportAdapterEvent event(
      TransportAdapterListenerImpl::EventTypeEnum::ON_UNEXPECTED_DISCONNECT,
      transport_adapter_, device, application, RawMessageSptr(), err);

  if (transport_manager::E_SUCCESS !=
      transport_manager_impl_->ReceiveEventFromDevice(event)) {
    LOG4CXX_WARN(logger_, "Failed to receive event from device");
  }
}

void TransportAdapterListenerImpl::OnCommunicationError(
    const TransportAdapter* adapter, const DeviceUID& device,
    const ApplicationHandle& app_id) {
  TransportAdapterEvent event(
      TransportAdapterListenerImpl::EventTypeEnum::ON_COMMUNICATION_ERROR,
      transport_adapter_, device, app_id, RawMessageSptr(), new BaseError());

  if (transport_manager::E_SUCCESS !=
      transport_manager_impl_->ReceiveEventFromDevice(event)) {
    LOG4CXX_WARN(logger_, "Failed to receive event from device");
  }
}

}  // namespace transport_manager
