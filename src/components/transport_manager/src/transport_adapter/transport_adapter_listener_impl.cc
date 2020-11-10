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

#include "transport_manager/transport_adapter/transport_adapter_event.h"
#include "transport_manager/transport_adapter/transport_adapter_listener_impl.h"
#include "transport_manager/transport_manager_impl.h"

namespace transport_manager {
SDL_CREATE_LOG_VARIABLE("TransportManager")

TransportAdapterListenerImpl::TransportAdapterListenerImpl(
    TransportManager* manager, TransportAdapter* adapter)
    : transport_manager_(manager), transport_adapter_(adapter) {}

void TransportAdapterListenerImpl::OnSearchDeviceDone(
    const TransportAdapter* adapter) {
  SDL_LOG_TRACE("enter. adapter* " << adapter);
  const TransportAdapterEvent event(EventTypeEnum::ON_SEARCH_DONE,
                                    transport_adapter_,
                                    "",
                                    0,
                                    ::protocol_handler::RawMessagePtr(),
                                    BaseErrorPtr());
  if (transport_manager_ != NULL &&
      transport_manager::E_SUCCESS !=
          transport_manager_->ReceiveEventFromDevice(event)) {
    SDL_LOG_WARN("Failed to receive event from device");
  }
  SDL_LOG_TRACE("exit");
}

void TransportAdapterListenerImpl::OnSearchDeviceFailed(
    const TransportAdapter* adapter, const SearchDeviceError& error) {
  SDL_LOG_TRACE("enter. adapter: " << adapter << ", error: " << &error);
  SearchDeviceError* err = new SearchDeviceError(error);
  const TransportAdapterEvent event(EventTypeEnum::ON_SEARCH_FAIL,
                                    transport_adapter_,
                                    "",
                                    0,
                                    ::protocol_handler::RawMessagePtr(),
                                    BaseErrorPtr(err));
  if (transport_manager_ != NULL &&
      transport_manager::E_SUCCESS !=
          transport_manager_->ReceiveEventFromDevice(event)) {
    SDL_LOG_WARN("Failed to receive event from device");
  }
  SDL_LOG_TRACE("exit");
}

void TransportAdapterListenerImpl::OnDeviceListUpdated(
    const TransportAdapter* adapter) {
  SDL_LOG_TRACE("enter. adapter* " << adapter);
  const TransportAdapterEvent event(EventTypeEnum::ON_DEVICE_LIST_UPDATED,
                                    transport_adapter_,
                                    "",
                                    0,
                                    ::protocol_handler::RawMessagePtr(),
                                    BaseErrorPtr());
  if (transport_manager_ != NULL &&
      transport_manager::E_SUCCESS !=
          transport_manager_->ReceiveEventFromDevice(event)) {
    SDL_LOG_WARN("Failed to receive event from device");
  }
  SDL_LOG_TRACE("exit");
}

void TransportAdapterListenerImpl::OnFindNewApplicationsRequest(
    const TransportAdapter* adapter) {
  SDL_LOG_TRACE("enter. adapter* " << adapter);
  const TransportAdapterEvent event(
      EventTypeEnum::ON_FIND_NEW_APPLICATIONS_REQUEST,
      transport_adapter_,
      "",
      0,
      ::protocol_handler::RawMessagePtr(),
      BaseErrorPtr());
  if (transport_manager_ != NULL &&
      transport_manager::E_SUCCESS !=
          transport_manager_->ReceiveEventFromDevice(event)) {
    SDL_LOG_WARN("Failed to receive event from device");
  }
  SDL_LOG_TRACE("exit");
}

void TransportAdapterListenerImpl::OnConnectionStatusUpdated(
    const TransportAdapter* adapter) {
  SDL_LOG_TRACE("enter. adapter* " << adapter);
  const TransportAdapterEvent event(EventTypeEnum::ON_CONNECTION_STATUS_UPDATED,
                                    transport_adapter_,
                                    "",
                                    0,
                                    ::protocol_handler::RawMessagePtr(),
                                    BaseErrorPtr(new BaseError()));
  if (transport_manager_ != NULL &&
      transport_manager::E_SUCCESS !=
          transport_manager_->ReceiveEventFromDevice(event)) {
    SDL_LOG_WARN("Failed to receive event from device");
  }
  SDL_LOG_TRACE("exit");
}

void TransportAdapterListenerImpl::OnConnectPending(
    const TransportAdapter* adapter,
    const DeviceUID& device,
    const ApplicationHandle& application_id) {
  SDL_LOG_TRACE("enter adapter*: " << adapter << ", device: " << &device
                                   << ", application_id: " << &application_id);
  const TransportAdapterEvent event(EventTypeEnum::ON_CONNECT_PENDING,
                                    transport_adapter_,
                                    device,
                                    application_id,
                                    ::protocol_handler::RawMessagePtr(),
                                    BaseErrorPtr(new BaseError()));
  if (transport_manager_ != NULL &&
      transport_manager::E_SUCCESS !=
          transport_manager_->ReceiveEventFromDevice(event)) {
    SDL_LOG_WARN("Failed to receive event from device");
  }
  SDL_LOG_TRACE("exit");
}

void TransportAdapterListenerImpl::OnConnectDone(
    const TransportAdapter* adapter,
    const DeviceUID& device,
    const ApplicationHandle& application_id) {
  SDL_LOG_TRACE("enter adapter*: " << adapter << ", device: " << &device
                                   << ", application_id: " << &application_id);
  const TransportAdapterEvent event(EventTypeEnum::ON_CONNECT_DONE,
                                    transport_adapter_,
                                    device,
                                    application_id,
                                    ::protocol_handler::RawMessagePtr(),
                                    BaseErrorPtr(new BaseError()));
  if (transport_manager_ != NULL &&
      transport_manager::E_SUCCESS !=
          transport_manager_->ReceiveEventFromDevice(event)) {
    SDL_LOG_WARN("Failed to receive event from device");
  }
  SDL_LOG_TRACE("exit");
}

void TransportAdapterListenerImpl::OnConnectFailed(
    const TransportAdapter* adapter,
    const DeviceUID& device,
    const ApplicationHandle& app_id,
    const ConnectError& error) {
  SDL_LOG_TRACE("enter. adapter: " << adapter << ", device: " << &device
                                   << ", application_id: " << &app_id
                                   << ", error: " << &error);
  ConnectError* err = new ConnectError(error);
  const TransportAdapterEvent event(EventTypeEnum::ON_CONNECT_FAIL,
                                    transport_adapter_,
                                    device,
                                    app_id,
                                    ::protocol_handler::RawMessagePtr(),
                                    BaseErrorPtr(err));
  if (transport_manager_ != NULL &&
      transport_manager::E_SUCCESS !=
          transport_manager_->ReceiveEventFromDevice(event)) {
    SDL_LOG_WARN("Failed to receive event from device");
  }
  SDL_LOG_TRACE("exit");
}

void TransportAdapterListenerImpl::OnDisconnectDone(
    const TransportAdapter* adapter,
    const DeviceUID& device,
    const ApplicationHandle& app_id) {
  SDL_LOG_AUTO_TRACE();
  SDL_LOG_TRACE("enter. adapter: " << adapter << ", device: " << &device
                                   << ", application_id: " << &app_id);
  const TransportAdapterEvent event(EventTypeEnum::ON_DISCONNECT_DONE,
                                    transport_adapter_,
                                    device,
                                    app_id,
                                    ::protocol_handler::RawMessagePtr(),
                                    BaseErrorPtr(new BaseError()));
  if (transport_manager_ != NULL &&
      transport_manager::E_SUCCESS !=
          transport_manager_->ReceiveEventFromDevice(event)) {
    SDL_LOG_WARN("Failed to receive event from device");
  }
  SDL_LOG_TRACE("exit");
}

void TransportAdapterListenerImpl::OnDisconnectFailed(
    const TransportAdapter* adapter,
    const DeviceUID& device,
    const ApplicationHandle& app_id,
    const DisconnectError& error) {
  SDL_LOG_TRACE("enter. adapter: " << adapter << ", device: " << &device
                                   << ", application_id: " << &app_id
                                   << ", error: " << &error);
  DisconnectError* err = new DisconnectError(error);
  const TransportAdapterEvent event(EventTypeEnum::ON_DISCONNECT_FAIL,
                                    transport_adapter_,
                                    device,
                                    app_id,
                                    ::protocol_handler::RawMessagePtr(),
                                    BaseErrorPtr(err));
  if (transport_manager_ != NULL &&
      transport_manager::E_SUCCESS !=
          transport_manager_->ReceiveEventFromDevice(event)) {
    SDL_LOG_WARN("Failed to receive event from device");
  }
  SDL_LOG_TRACE("exit");
}

void TransportAdapterListenerImpl::OnDisconnectDeviceDone(
    const TransportAdapter* adapter, const DeviceUID& device) {
  SDL_LOG_AUTO_TRACE();
}

void TransportAdapterListenerImpl::OnDisconnectDeviceFailed(
    const TransportAdapter* adapter,
    const DeviceUID& device,
    const DisconnectDeviceError& error) {}

void TransportAdapterListenerImpl::OnDataReceiveDone(
    const TransportAdapter* adapter,
    const DeviceUID& device,
    const ApplicationHandle& app_id,
    const ::protocol_handler::RawMessagePtr data_container) {
  SDL_LOG_TRACE("enter. adapter: " << adapter << ", device: " << &device
                                   << ", application_id: " << &app_id
                                   << ", data_container: " << data_container);
  const TransportAdapterEvent event(EventTypeEnum::ON_RECEIVED_DONE,
                                    transport_adapter_,
                                    device,
                                    app_id,
                                    data_container,
                                    BaseErrorPtr(new BaseError()));
  if (transport_manager_ != NULL &&
      transport_manager::E_SUCCESS !=
          transport_manager_->ReceiveEventFromDevice(event)) {
    SDL_LOG_WARN("Failed to receive event from device");
  }
  SDL_LOG_TRACE("exit");
}

void TransportAdapterListenerImpl::OnDataReceiveFailed(
    const TransportAdapter* adapter,
    const DeviceUID& device,
    const ApplicationHandle& app_id,
    const DataReceiveError& error) {
  SDL_LOG_TRACE("enter. adapter: " << adapter << ", device: " << &device
                                   << ", application_id: " << &app_id
                                   << ", error: " << &error);
  DataReceiveError* err = new DataReceiveError(error);
  const TransportAdapterEvent event(EventTypeEnum::ON_RECEIVED_FAIL,
                                    transport_adapter_,
                                    device,
                                    app_id,
                                    ::protocol_handler::RawMessagePtr(),
                                    BaseErrorPtr(err));
  if (transport_manager_ != NULL &&
      transport_manager::E_SUCCESS !=
          transport_manager_->ReceiveEventFromDevice(event)) {
    SDL_LOG_WARN("Failed to receive event from device");
  }
  SDL_LOG_TRACE("exit");
}

void TransportAdapterListenerImpl::OnDataSendDone(
    const TransportAdapter* adapter,
    const DeviceUID& device,
    const ApplicationHandle& app_id,
    const ::protocol_handler::RawMessagePtr data_container) {
  SDL_LOG_TRACE("enter. adapter: " << adapter << ", device: " << &device
                                   << ", application_id: " << &app_id
                                   << ", data_container: " << data_container);
  const TransportAdapterEvent event(EventTypeEnum::ON_SEND_DONE,
                                    transport_adapter_,
                                    device,
                                    app_id,
                                    data_container,
                                    std::make_shared<BaseError>());
  if (transport_manager_ != NULL &&
      transport_manager::E_SUCCESS !=
          transport_manager_->ReceiveEventFromDevice(event)) {
    SDL_LOG_WARN("Failed to receive event from device");
  }
  SDL_LOG_TRACE("exit");
}

void TransportAdapterListenerImpl::OnDataSendFailed(
    const TransportAdapter* adapter,
    const DeviceUID& device,
    const ApplicationHandle& app_id,
    const ::protocol_handler::RawMessagePtr data_container,
    const DataSendError& error) {
  SDL_LOG_TRACE("enter. adapter: " << adapter << ", device: " << &device
                                   << ", application_id: " << &app_id
                                   << ", data_container: " << data_container
                                   << ", error: " << &error);
  DataSendError* err = new DataSendError(error);
  const TransportAdapterEvent event(EventTypeEnum::ON_SEND_FAIL,
                                    transport_adapter_,
                                    device,
                                    app_id,
                                    data_container,
                                    BaseErrorPtr(err));
  if (transport_manager_ != NULL &&
      transport_manager::E_SUCCESS !=
          transport_manager_->ReceiveEventFromDevice(event)) {
    SDL_LOG_WARN("Failed to receive event from device");
  }
  SDL_LOG_TRACE("exit");
}

void TransportAdapterListenerImpl::OnConnectRequested(
    const TransportAdapter* adapter,
    const DeviceUID& device_handle,
    const ApplicationHandle& app_handle) {}

void TransportAdapterListenerImpl::OnUnexpectedDisconnect(
    const TransportAdapter* adapter,
    const DeviceUID& device,
    const ApplicationHandle& application,
    const CommunicationError& error) {
  SDL_LOG_TRACE("enter. adapter: " << adapter << ", device: " << &device
                                   << ", application: " << &application
                                   << ", error: " << &error);
  CommunicationError* err = new CommunicationError(error);
  const TransportAdapterEvent event(EventTypeEnum::ON_UNEXPECTED_DISCONNECT,
                                    transport_adapter_,
                                    device,
                                    application,
                                    ::protocol_handler::RawMessagePtr(),
                                    BaseErrorPtr(err));
  if (transport_manager_ != NULL &&
      transport_manager::E_SUCCESS !=
          transport_manager_->ReceiveEventFromDevice(event)) {
    SDL_LOG_WARN("Failed to receive event from device");
  }
  SDL_LOG_TRACE("exit");
}

void TransportAdapterListenerImpl::OnCommunicationError(
    const TransportAdapter* adapter,
    const DeviceUID& device,
    const ApplicationHandle& app_id) {
  SDL_LOG_TRACE("enter. adapter: " << adapter << ", device: " << &device
                                   << ", application_id: " << &app_id);
  const TransportAdapterEvent event(EventTypeEnum::ON_COMMUNICATION_ERROR,
                                    transport_adapter_,
                                    device,
                                    app_id,
                                    ::protocol_handler::RawMessagePtr(),
                                    BaseErrorPtr(new BaseError()));
  if (transport_manager_ != NULL &&
      transport_manager::E_SUCCESS !=
          transport_manager_->ReceiveEventFromDevice(event)) {
    SDL_LOG_WARN("Failed to receive event from device");
  }
  SDL_LOG_TRACE("exit");
}

void TransportAdapterListenerImpl::OnTransportSwitchRequested(
    const transport_adapter::TransportAdapter* adapter) {
  SDL_LOG_AUTO_TRACE();
  const TransportAdapterEvent event(
      EventTypeEnum::ON_TRANSPORT_SWITCH_REQUESTED,
      transport_adapter_,
      "",
      0,
      ::protocol_handler::RawMessagePtr(),
      BaseErrorPtr());
  if (transport_manager_ != NULL &&
      transport_manager::E_SUCCESS !=
          transport_manager_->ReceiveEventFromDevice(event)) {
    SDL_LOG_WARN("Failed to receive event from device");
  }
}

void TransportAdapterListenerImpl::OnTransportConfigUpdated(
    const transport_adapter::TransportAdapter* adapter) {
  SDL_LOG_AUTO_TRACE();

  const TransportAdapterEvent event(EventTypeEnum::ON_TRANSPORT_CONFIG_UPDATED,
                                    transport_adapter_,
                                    "",
                                    0,
                                    ::protocol_handler::RawMessagePtr(),
                                    BaseErrorPtr());

  if (transport_manager_ != NULL &&
      transport_manager::E_SUCCESS !=
          transport_manager_->ReceiveEventFromDevice(event)) {
    SDL_LOG_WARN("Failed to receive event from device");
  }
}

}  // namespace transport_manager
