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

#include <pthread.h>
#include "transport_manager/device_adapter.h"
#include "transport_manager/device_adapter_listener.h"
#include "transport_manager/transport_manager_impl.h"

namespace transport_manager {

log4cxx::LoggerPtr DeviceAdapterListenerImpl::logger_ = log4cxx::LoggerPtr(
    log4cxx::Logger::getLogger("DeviceAdapterListener"));
DeviceAdapterListener::~DeviceAdapterListener() {
}

DeviceAdapterListenerImpl::~DeviceAdapterListenerImpl() {
}

DeviceAdapterListener::DeviceAdapterEvent::DeviceAdapterEvent(
    int type, int session_id, DeviceAdapter *adapter, const DeviceHandle &device, RawMessageSptr data,
    DeviceAdapterError *error)
    : event_type_(type),
      session_id_(session_id),
      device_adapter_(adapter),
      device_handle_(device),
      event_data_(data),
      event_error_(error) {

}

DeviceAdapterListener::DeviceAdapterEvent::~DeviceAdapterEvent() {

}

void DeviceAdapterListener::DeviceAdapterEvent::set_event_type(int type) {
  event_type_ = type;
}

void DeviceAdapterListener::DeviceAdapterEvent::set_session_id(
    ConnectionId id) {
  session_id_ = id;
}

void DeviceAdapterListener::DeviceAdapterEvent::set_device_adapter(
    DeviceAdapter *device_adapter) {
  device_adapter_ = device_adapter;
}
void DeviceAdapterListener::DeviceAdapterEvent::set_data(RawMessageSptr data) {
  event_data_ = data;
}

void DeviceAdapterListener::DeviceAdapterEvent::set_error(
    DeviceAdapterError *error) {
  //todo: add copy data
  event_error_ = error;
}

void DeviceAdapterListener::DeviceAdapterEvent::set_device_handle(const DeviceHandle &device) {
  device_handle_ = device;
}

int DeviceAdapterListener::DeviceAdapterEvent::event_type(void) const {
  return event_type_;
}

ConnectionId DeviceAdapterListener::DeviceAdapterEvent::session_id(
    void) const {
  return session_id_;
}

DeviceAdapter *DeviceAdapterListener::DeviceAdapterEvent::device_adapter(
    void) const {
  return device_adapter_;
}

RawMessageSptr DeviceAdapterListener::DeviceAdapterEvent::data(void) const {
  return event_data_;
}

const DeviceHandle &DeviceAdapterListener::DeviceAdapterEvent::device_handle() const {
  return device_handle_;
}

DeviceAdapterError *DeviceAdapterListener::DeviceAdapterEvent::event_error(
    void) const {
  return event_error_;
}

DeviceAdapterListenerImpl::DeviceAdapterListenerImpl(
    transport_manager::TransportManagerImpl *tm) {
  transport_manager_impl_ = tm;
}

void DeviceAdapterListenerImpl::onSearchDeviceDone(
    const DeviceAdapter *device_adapter) {
  DeviceAdapterListenerImpl::DeviceAdapterEvent event(
      DeviceAdapterListenerImpl::EventTypeEnum::ON_SEARCH_DONE, 0,
      const_cast<DeviceAdapter *>(device_adapter), "",
      RawMessageSptr(),
      new DeviceAdapterError());

  transport_manager_impl_->receiveEventFromDevice(event);

  pthread_cond_signal(transport_manager_impl_->getDeviceListenerThreadWakeup());
}

void DeviceAdapterListenerImpl::onSearchDeviceFailed(
    const DeviceAdapter* device_adapter, const SearchDeviceError& error) {
  SearchDeviceError *err = new SearchDeviceError(error);
  DeviceAdapterListenerImpl::DeviceAdapterEvent event(
      DeviceAdapterListenerImpl::EventTypeEnum::ON_SEARCH_FAIL, 0,
      const_cast<DeviceAdapter *>(device_adapter), "",
      RawMessageSptr(), err);

  transport_manager_impl_->receiveEventFromDevice(event);

  pthread_cond_signal(transport_manager_impl_->getDeviceListenerThreadWakeup());
}

void DeviceAdapterListenerImpl::onConnectDone(
    const DeviceAdapter* device_adapter,
    const DeviceHandle device,
    const ConnectionId session_id) {
  DeviceAdapterListenerImpl::DeviceAdapterEvent event(
      DeviceAdapterListenerImpl::EventTypeEnum::ON_CONNECT_DONE, session_id,
      const_cast<DeviceAdapter *>(device_adapter), device,
      RawMessageSptr(),
      new DeviceAdapterError());

  transport_manager_impl_->receiveEventFromDevice(event);

  pthread_cond_signal(transport_manager_impl_->getDeviceListenerThreadWakeup());
}

void DeviceAdapterListenerImpl::onConnectFailed(
    const DeviceAdapter* device_adapter,
    const DeviceHandle device,
    const ConnectionId session_id, const ConnectError& error) {
  ConnectError *err = new ConnectError(error);
  DeviceAdapterListenerImpl::DeviceAdapterEvent event(
      DeviceAdapterListenerImpl::EventTypeEnum::ON_CONNECT_FAIL, session_id,
      const_cast<DeviceAdapter *>(device_adapter), device,
      RawMessageSptr(), err);

  transport_manager_impl_->receiveEventFromDevice(event);

  pthread_cond_signal(transport_manager_impl_->getDeviceListenerThreadWakeup());
}

void DeviceAdapterListenerImpl::onDisconnectDone(
    const DeviceAdapter* device_adapter,
    const ConnectionId session_id) {
  DeviceAdapterListenerImpl::DeviceAdapterEvent event(
      DeviceAdapterListenerImpl::EventTypeEnum::ON_DISCONNECT_DONE, session_id,
      const_cast<DeviceAdapter *>(device_adapter), "", RawMessageSptr(),
      new DeviceAdapterError());

  transport_manager_impl_->receiveEventFromDevice(event);

  pthread_cond_signal(transport_manager_impl_->getDeviceListenerThreadWakeup());
}

void DeviceAdapterListenerImpl::onDisconnectFailed(
    const DeviceAdapter* device_adapter, const ConnectionId session_id,
    const DisconnectError& error) {
  DisconnectError *err = new DisconnectError(error);
  DeviceAdapterListenerImpl::DeviceAdapterEvent event(
      DeviceAdapterListenerImpl::EventTypeEnum::ON_DISCONNECT_FAIL, session_id,
      const_cast<DeviceAdapter *>(device_adapter), "", RawMessageSptr(), err);

  transport_manager_impl_->receiveEventFromDevice(event);

  pthread_cond_signal(transport_manager_impl_->getDeviceListenerThreadWakeup());
}

void DeviceAdapterListenerImpl::onDisconnectDeviceDone(
    const DeviceAdapter* device_adapter, const ConnectionId session_id) {

}

void DeviceAdapterListenerImpl::onDisconnectDeviceFailed(
    const DeviceAdapter* device_adapter, const ConnectionId session_id,
    const DisconnectDeviceError& error) {

}

void DeviceAdapterListenerImpl::onDataReceiveDone(
    const DeviceAdapter* device_adapter,
    const ConnectionId session_id,
    const RawMessageSptr data_container) {
  DeviceAdapterListenerImpl::DeviceAdapterEvent event(
      DeviceAdapterListenerImpl::EventTypeEnum::ON_RECEIVED_DONE, session_id,
      const_cast<DeviceAdapter *>(device_adapter), "", data_container,
      new DeviceAdapterError());

  transport_manager_impl_->receiveEventFromDevice(event);

  pthread_cond_signal(transport_manager_impl_->getDeviceListenerThreadWakeup());
}

void DeviceAdapterListenerImpl::onDataReceiveFailed(
    const DeviceAdapter* device_adapter,
    const ConnectionId session_id,
    const DataReceiveError& error) {
  DataReceiveError *err = new DataReceiveError(error);
  DeviceAdapterListenerImpl::DeviceAdapterEvent event(
      DeviceAdapterListenerImpl::EventTypeEnum::ON_RECEIVED_DONE, session_id,
      const_cast<DeviceAdapter *>(device_adapter), "", RawMessageSptr(), err);

  transport_manager_impl_->receiveEventFromDevice(event);

  pthread_cond_signal(transport_manager_impl_->getDeviceListenerThreadWakeup());
}

void DeviceAdapterListenerImpl::onDataSendDone(
    const DeviceAdapter* device_adapter,
    const ConnectionId session_id,
    const RawMessageSptr data_container) {
  DeviceAdapterListenerImpl::DeviceAdapterEvent event(
      DeviceAdapterListenerImpl::EventTypeEnum::ON_SEND_DONE, session_id,
      const_cast<DeviceAdapter *>(device_adapter), "", data_container,
      new DeviceAdapterError());

  transport_manager_impl_->receiveEventFromDevice(event);

  pthread_cond_signal(transport_manager_impl_->getDeviceListenerThreadWakeup());
}

void DeviceAdapterListenerImpl::onDataSendFailed(
    const DeviceAdapter* device_adapter,
    const ConnectionId session_id,
    const RawMessageSptr data_container, const DataSendError& error) {
  DataSendError *err = new DataSendError(error);
  DeviceAdapterListenerImpl::DeviceAdapterEvent event(
      DeviceAdapterListenerImpl::EventTypeEnum::ON_SEND_FAIL, session_id,
      const_cast<DeviceAdapter *>(device_adapter), "", RawMessageSptr(), err);

  transport_manager_impl_->receiveEventFromDevice(event);

  pthread_cond_signal(transport_manager_impl_->getDeviceListenerThreadWakeup());
}

void DeviceAdapterListenerImpl::onCommunicationError(
    const DeviceAdapter* device_adapter,
    const ConnectionId session_id) {
  DeviceAdapterListenerImpl::DeviceAdapterEvent event(
      DeviceAdapterListenerImpl::EventTypeEnum::ON_COMMUNICATION_ERROR,
      session_id, const_cast<DeviceAdapter *>(device_adapter), "", RawMessageSptr(),
      new DeviceAdapterError());

  transport_manager_impl_->receiveEventFromDevice(event);

  pthread_cond_signal(transport_manager_impl_->getDeviceListenerThreadWakeup());
}
bool DeviceAdapterListenerImpl::DeviceAdapterEvent::operator ==(
    const DeviceAdapterListenerImpl::DeviceAdapterEvent &other) {
  if (this->event_type_ == other.event_type_
      && this->session_id_ == other.session_id_
      && this->device_adapter_ == other.device_adapter_) {

    if (!this->event_data_.valid() && !other.event_data_.valid())
      return true;

    if (this->event_data_.valid() && other.event_data_.valid()
        && this->event_data_->serial_number()
            == other.event_data_->serial_number()) {
      return true;
    }
    return false;
  } else {
    return false;
  }
}

}  //namespace
