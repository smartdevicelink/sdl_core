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

#include "config_profile/profile.h"

#include "utils/logger.h"

#include "transport_manager/transport_adapter/transport_adapter_impl.h"
#include "transport_manager/transport_adapter/transport_adapter_listener.h"
#include "transport_manager/transport_adapter/device_scanner.h"
#include "transport_manager/transport_adapter/server_connection_factory.h"
#include "transport_manager/transport_adapter/client_connection_listener.h"

namespace transport_manager {
namespace transport_adapter {

CREATE_LOGGERPTR_GLOBAL(logger_, "TransportAdapterImpl")

TransportAdapterImpl::TransportAdapterImpl(
    DeviceScanner* device_scanner,
    ServerConnectionFactory* server_connection_factory,
    ClientConnectionListener* client_connection_listener)
    : listeners_(),
      initialised_(0),
      devices_(),
      devices_mutex_(),
      connections_(),
      connections_mutex_(),
      device_scanner_(device_scanner),
      server_connection_factory_(server_connection_factory),
      client_connection_listener_(client_connection_listener),
      metric_observer_(NULL) {
  pthread_mutex_init(&devices_mutex_, 0);
  pthread_mutex_init(&connections_mutex_, 0);
}

TransportAdapterImpl::~TransportAdapterImpl() {
  if (device_scanner_) {
    device_scanner_->Terminate();
    delete device_scanner_;
  }
  if (server_connection_factory_) {
    server_connection_factory_->Terminate();
    delete server_connection_factory_;
  }
  if (client_connection_listener_) {
    client_connection_listener_->Terminate();
    delete client_connection_listener_;
  }

  pthread_mutex_lock(&connections_mutex_);
  ConnectionMap connections;
  std::swap(connections, connections_);
  pthread_mutex_unlock(&connections_mutex_);
  connections.clear();

  pthread_mutex_lock(&devices_mutex_);
  DeviceMap devices;
  std::swap(devices, devices_);
  pthread_mutex_unlock(&devices_mutex_);
  devices.clear();

  pthread_mutex_destroy(&connections_mutex_);
  pthread_mutex_destroy(&devices_mutex_);
}

TransportAdapter::Error TransportAdapterImpl::Init() {
  LOG4CXX_INFO(logger_, "Initializing device adapter");

  Error error = OK;

  if ((error == OK) && device_scanner_) error = device_scanner_->Init();
  if ((error == OK) && server_connection_factory_)
    error = server_connection_factory_->Init();
  if ((error == OK) && client_connection_listener_)
    error = client_connection_listener_->Init();

  initialised_ = (error == OK);

  if (profile::Profile::instance()->use_last_state()) {
    if (!Restore()) {
      LOG4CXX_WARN(logger_, "could not restore transport adapter state");
      error = FAIL;
    }
  }

  return error;
}

TransportAdapter::Error TransportAdapterImpl::SearchDevices() {
  LOG4CXX_INFO(logger_, "enter");
  if (device_scanner_ == NULL) {
    LOG4CXX_INFO(logger_, "not supported");
    return NOT_SUPPORTED;
  } else if (!device_scanner_->IsInitialised()) {
    LOG4CXX_INFO(logger_, "bad state");
    return BAD_STATE;
  }
  LOG4CXX_INFO(logger_, "exit");
  return device_scanner_->Scan();
}

TransportAdapter::Error TransportAdapterImpl::Connect(
    const DeviceUID& device_id, const ApplicationHandle& app_handle) {
  LOG4CXX_INFO(logger_,
               "enter TransportAdapter::Error TransportAdapterImpl::Connect");
  if (server_connection_factory_ == 0) {
    LOG4CXX_ERROR(logger_, "feature is NOT_SUPPORTED");
    return NOT_SUPPORTED;
  }
  if (!server_connection_factory_->IsInitialised()) {
    LOG4CXX_ERROR(logger_, "BAD_STATE");
    return BAD_STATE;
  }

  pthread_mutex_lock(&connections_mutex_);
  const bool already_exists =
      connections_.end() !=
      connections_.find(std::make_pair(device_id, app_handle));
  if (!already_exists) {
    ConnectionInfo& info = connections_[std::make_pair(device_id, app_handle)];
    info.app_handle = app_handle;
    info.device_id = device_id;
    info.state = ConnectionInfo::NEW;
  }
  pthread_mutex_unlock(&connections_mutex_);
  if (already_exists) {
    LOG4CXX_ERROR(logger_, "Connection for device " << device_id << ", channel "
                                                    << app_handle
                                                    << " already exists");
    return ALREADY_EXISTS;
  }

  const TransportAdapter::Error err =
      server_connection_factory_->CreateConnection(device_id, app_handle);
  if (TransportAdapter::OK != err) {
    pthread_mutex_lock(&connections_mutex_);
    connections_.erase(std::make_pair(device_id, app_handle));
    pthread_mutex_unlock(&connections_mutex_);
  }
  return err;
}

TransportAdapter::Error TransportAdapterImpl::ConnectDevice(
    const DeviceUID& device_handle) {
  DeviceSptr device = FindDevice(device_handle);
  if (device) {
    return ConnectDevice(device);
  }
  else {
    return BAD_PARAM;
  }
}

TransportAdapter::Error TransportAdapterImpl::Disconnect(
    const DeviceUID& device_id, const ApplicationHandle& app_handle) {
  if (!initialised_) return BAD_STATE;
  ConnectionSptr connection = FindEstablishedConnection(device_id, app_handle);
  if (connection.valid()) {
    return connection->Disconnect();
  } else {
    return BAD_PARAM;
  }
}

TransportAdapter::Error TransportAdapterImpl::DisconnectDevice(
    const DeviceUID& device_id) {
  if (!initialised_) return BAD_STATE;

  Error error = OK;
  pthread_mutex_lock(&connections_mutex_);
  for (ConnectionMap::iterator it = connections_.begin();
       it != connections_.end(); ++it) {

    ConnectionInfo& info = it->second;
    if (info.device_id == device_id &&
        info.state != ConnectionInfo::FINALISING) {
      if (OK != info.connection->Disconnect()) {
        error = FAIL;
      }
    }
  }
  pthread_mutex_unlock(&connections_mutex_);

  return OK;
}

TransportAdapter::Error TransportAdapterImpl::SendData(
    const DeviceUID& device_id, const ApplicationHandle& app_handle,
    const RawMessageSptr data) {
  if (!initialised_) return BAD_STATE;

  ConnectionSptr connection = FindEstablishedConnection(device_id, app_handle);
  if (connection.get() != 0) {
    return connection->SendData(data);
  } else {
    return BAD_PARAM;
  }
}

TransportAdapter::Error TransportAdapterImpl::StartClientListening() {
  if (client_connection_listener_ == 0) return NOT_SUPPORTED;
  if (!client_connection_listener_->IsInitialised()) return BAD_STATE;
  return client_connection_listener_->StartListening();
}

TransportAdapter::Error TransportAdapterImpl::StopClientListening() {
  if (client_connection_listener_ == 0) return NOT_SUPPORTED;
  if (!client_connection_listener_->IsInitialised()) return BAD_STATE;
  return client_connection_listener_->StopListening();
}

DeviceList TransportAdapterImpl::GetDeviceList() const {
  DeviceList devices;
  pthread_mutex_lock(&devices_mutex_);
  for (DeviceMap::const_iterator it = devices_.begin(); it != devices_.end();
       ++it) {
    devices.push_back(it->first);
  }
  pthread_mutex_unlock(&devices_mutex_);
  return devices;
}

DeviceSptr TransportAdapterImpl::AddDevice(DeviceSptr device) {
  DeviceSptr existing_device;
  bool same_device_found = false;
  pthread_mutex_lock(&devices_mutex_);
  for (DeviceMap::const_iterator i = devices_.begin(); i != devices_.end();
       ++i) {
    existing_device = i->second;
    if (device->IsSameAs(existing_device.get())) {
      same_device_found = true;
      break;
    }
  }
  if (!same_device_found) {
    devices_[device->unique_device_id()] = device;
  }
  pthread_mutex_unlock(&devices_mutex_);
  if (same_device_found) {
    return existing_device;
  } else {
    for (TransportAdapterListenerList::iterator it = listeners_.begin();
         it != listeners_.end(); ++it) {
      (*it)->OnDeviceListUpdated(this);
    }
    if (ToBeAutoConnected(device)) {
      ConnectDevice(device);
    }
    return device;
  }
}

void TransportAdapterImpl::SearchDeviceDone(const DeviceVector& devices) {
  DeviceMap new_devices;
  for (DeviceVector::const_iterator it = devices.begin(); it != devices.end();
       ++it) {
    DeviceSptr device = *it;
    bool device_found = false;

    pthread_mutex_lock(&devices_mutex_);
    for (DeviceMap::iterator it = devices_.begin(); it != devices_.end();
         ++it) {
      DeviceSptr existing_device = it->second;
      if (device->IsSameAs(existing_device.get())) {
        existing_device->set_keep_on_disconnect(true);
        device_found = true;
        break;
      }
    }
    pthread_mutex_unlock(&devices_mutex_);

    if (!device_found) {
      LOG4CXX_INFO(logger_, "Adding new device " << device->unique_device_id()
                                                 << " (\"" << device->name()
                                                 << "\")");
    }

    device->set_keep_on_disconnect(true);
    new_devices[device->unique_device_id()] = device;
  }

  pthread_mutex_lock(&connections_mutex_);
  std::set<DeviceUID> connected_devices;
  for (ConnectionMap::const_iterator it = connections_.begin();
       it != connections_.end(); ++it) {
    const ConnectionInfo& info = it->second;
    if (info.state != ConnectionInfo::FINALISING)
      connected_devices.insert(info.device_id);
  }
  pthread_mutex_unlock(&connections_mutex_);

  DeviceMap all_devices = new_devices;
  pthread_mutex_lock(&devices_mutex_);
  for (DeviceMap::iterator it = devices_.begin(); it != devices_.end(); ++it) {
    DeviceSptr existing_device = it->second;

    if (all_devices.end() == all_devices.find(it->first)) {
      if (connected_devices.end() != connected_devices.find(it->first)) {
        existing_device->set_keep_on_disconnect(false);
        all_devices[it->first] = existing_device;
      }
    }
  }
  devices_ = all_devices;
  pthread_mutex_unlock(&devices_mutex_);

  for (TransportAdapterListenerList::iterator it = listeners_.begin();
       it != listeners_.end(); ++it) {
    (*it)->OnDeviceListUpdated(this);
    (*it)->OnSearchDeviceDone(this);
  }

  for (DeviceMap::iterator it = new_devices.begin(); it != new_devices.end(); ++it) {
    DeviceSptr device = it->second;
    if (ToBeAutoConnected(device)) {
      ConnectDevice(device);
    }
  }
}

void TransportAdapterImpl::SearchDeviceFailed(const SearchDeviceError& error) {
  for (TransportAdapterListenerList::iterator it = listeners_.begin();
       it != listeners_.end(); ++it)
    (*it)->OnSearchDeviceFailed(this, error);
}

bool TransportAdapterImpl::IsSearchDevicesSupported() const {
  return device_scanner_ != 0;
}

bool TransportAdapterImpl::IsServerOriginatedConnectSupported() const {
  return server_connection_factory_ != 0;
}

bool TransportAdapterImpl::IsClientOriginatedConnectSupported() const {
  return client_connection_listener_ != 0;
}

void TransportAdapterImpl::ConnectionCreated(
    ConnectionSptr connection, const DeviceUID& device_id,
    const ApplicationHandle& app_handle) {
  pthread_mutex_lock(&connections_mutex_);
  ConnectionInfo& info = connections_[std::make_pair(device_id, app_handle)];
  info.app_handle = app_handle;
  info.device_id = device_id;
  info.connection = connection;
  info.state = ConnectionInfo::NEW;
  pthread_mutex_unlock(&connections_mutex_);
}

void TransportAdapterImpl::DeviceDisconnected(const DeviceUID& device_handle,
                                              const DisconnectDeviceError& error) {

  ApplicationList app_list = GetApplicationList(device_handle);
  for (ApplicationList::const_iterator i = app_list.begin(); i != app_list.end(); ++i) {
    ApplicationHandle app_handle = *i;
    for (TransportAdapterListenerList::iterator it = listeners_.begin(); it != listeners_.end(); ++it) {
      TransportAdapterListener* listener = *it;
      listener->OnUnexpectedDisconnect(this, device_handle, app_handle, CommunicationError());
    }
  }

  pthread_mutex_lock(&connections_mutex_);
  for (TransportAdapterListenerList::iterator it = listeners_.begin(); it != listeners_.end(); ++it) {
    TransportAdapterListener* listener = *it;
    listener->OnDisconnectDeviceDone(this, device_handle);
  }
  for (ApplicationList::const_iterator i = app_list.begin(); i != app_list.end(); ++i) {
    ApplicationHandle app_handle = *i;
    connections_.erase(std::make_pair(device_handle, app_handle));
  }
  pthread_mutex_unlock(&connections_mutex_);

  RemoveDevice(device_handle);
}

void TransportAdapterImpl::DisconnectDone(const DeviceUID& device_id,
                                          const ApplicationHandle& app_handle) {
  bool device_disconnected = true;
  pthread_mutex_lock(&connections_mutex_);
  for (ConnectionMap::const_iterator it = connections_.begin();
       it != connections_.end(); ++it) {
    const DeviceUID& current_device_id = it->first.first;
    const ApplicationHandle& current_app_handle = it->first.second;
    if (current_device_id == device_id && current_app_handle != app_handle) {
      device_disconnected = false;
      break;
    }
  }
  pthread_mutex_unlock(&connections_mutex_);
  for (TransportAdapterListenerList::iterator it = listeners_.begin();
       it != listeners_.end(); ++it) {
    TransportAdapterListener* listener = *it;
    listener->OnDisconnectDone(this, device_id, app_handle);
    if (device_disconnected) listener->OnDisconnectDeviceDone(this, device_id);
  }
  pthread_mutex_lock(&connections_mutex_);
  connections_.erase(std::make_pair(device_id, app_handle));
  pthread_mutex_unlock(&connections_mutex_);

  if (device_disconnected) {
    RemoveDevice(device_id);
  }

  Store();
}

void TransportAdapterImpl::DataReceiveDone(const DeviceUID& device_id,
                                           const ApplicationHandle& app_handle,
                                           RawMessageSptr message) {
  if (metric_observer_) {
    metric_observer_->StartRawMsg(message.get());
  }
  for (TransportAdapterListenerList::iterator it = listeners_.begin();
       it != listeners_.end(); ++it)
    (*it)->OnDataReceiveDone(this, device_id, app_handle, message);
}

void TransportAdapterImpl::DataReceiveFailed(
    const DeviceUID& device_id, const ApplicationHandle& app_handle,
    const DataReceiveError& error) {
  for (TransportAdapterListenerList::iterator it = listeners_.begin();
       it != listeners_.end(); ++it)
    (*it)->OnDataReceiveFailed(this, device_id, app_handle, error);
}

void TransportAdapterImpl::DataSendDone(const DeviceUID& device_id,
                                        const ApplicationHandle& app_handle,
                                        RawMessageSptr message) {
  for (TransportAdapterListenerList::iterator it = listeners_.begin();
       it != listeners_.end(); ++it)
    (*it)->OnDataSendDone(this, device_id, app_handle, message);
}

void TransportAdapterImpl::DataSendFailed(const DeviceUID& device_id,
                                          const ApplicationHandle& app_handle,
                                          RawMessageSptr message,
                                          const DataSendError& error) {
  for (TransportAdapterListenerList::iterator it = listeners_.begin();
       it != listeners_.end(); ++it)
    (*it)->OnDataSendFailed(this, device_id, app_handle, message, error);
}

DeviceSptr TransportAdapterImpl::FindDevice(const DeviceUID& device_id) const {
  DeviceSptr ret;
  pthread_mutex_lock(&devices_mutex_);
  LOG4CXX_INFO(logger_,
               "DeviceSptr TransportAdapterImpl::FindDevice(const DeviceUID& "
               "device_id) enter");
  DeviceMap::const_iterator it = devices_.find(device_id);
  LOG4CXX_INFO(logger_, "devices_.size() = " << devices_.size());
  if (it != devices_.end()) ret = it->second;
  pthread_mutex_unlock(&devices_mutex_);
  LOG4CXX_INFO(logger_,
               "DeviceSptr TransportAdapterImpl::FindDevice(const DeviceUID& "
               "device_id) exit");
  return ret;
}

void TransportAdapterImpl::ConnectDone(const DeviceUID& device_id,
                                       const ApplicationHandle& app_handle) {
  pthread_mutex_lock(&connections_mutex_);
  ConnectionMap::iterator it_conn =
      connections_.find(std::make_pair(device_id, app_handle));
  if (it_conn != connections_.end()) {
    ConnectionInfo& info = it_conn->second;
    info.state = ConnectionInfo::ESTABLISHED;
  }
  pthread_mutex_unlock(&connections_mutex_);

  for (TransportAdapterListenerList::iterator it = listeners_.begin();
       it != listeners_.end(); ++it)
    (*it)->OnConnectDone(this, device_id, app_handle);

  Store();
}

void TransportAdapterImpl::ConnectFailed(const DeviceUID& device_id,
                                         const ApplicationHandle& app_handle,
                                         const ConnectError& error) {
  pthread_mutex_lock(&connections_mutex_);
  connections_.erase(std::make_pair(device_id, app_handle));
  pthread_mutex_unlock(&connections_mutex_);
  for (TransportAdapterListenerList::iterator it = listeners_.begin();
       it != listeners_.end(); ++it)
    (*it)->OnConnectFailed(this, device_id, app_handle, error);
}

void TransportAdapterImpl::AddListener(TransportAdapterListener* listener) {
  listeners_.push_back(listener);
}

ApplicationList TransportAdapterImpl::GetApplicationList(
    const DeviceUID& device_id) const {
  LOG4CXX_INFO(
      logger_,
      "ApplicationList TransportAdapterImpl::GetApplicationList enter");
  DeviceSptr device = FindDevice(device_id);
  if (device.valid()) {
    LOG4CXX_INFO(logger_, "device is valid");
    return device->GetApplicationList();
  }

  LOG4CXX_INFO(logger_, "device is NOT valid. Return empty list");
  return ApplicationList();
}

void TransportAdapterImpl::ConnectionFinished(
    const DeviceUID& device_id, const ApplicationHandle& app_handle) {
  pthread_mutex_lock(&connections_mutex_);
  ConnectionMap::iterator it =
      connections_.find(std::make_pair(device_id, app_handle));
  if (it != connections_.end()) {
    ConnectionInfo& info = it->second;
    info.state = ConnectionInfo::FINALISING;
  }
  pthread_mutex_unlock(&connections_mutex_);
}

void TransportAdapterImpl::ConnectionAborted(
    const DeviceUID& device_id, const ApplicationHandle& app_handle,
    const CommunicationError& error) {
  ConnectionFinished(device_id, app_handle);
  for (TransportAdapterListenerList::iterator it = listeners_.begin();
       it != listeners_.end(); ++it)
    (*it)->OnUnexpectedDisconnect(this, device_id, app_handle, error);
}

bool TransportAdapterImpl::IsInitialised() const {
  if (!initialised_) return false;
  if (device_scanner_ && !device_scanner_->IsInitialised()) return false;
  if (server_connection_factory_ &&
      !server_connection_factory_->IsInitialised())
    return false;
  if (client_connection_listener_ &&
      !client_connection_listener_->IsInitialised())
    return false;
  return true;
}

std::string TransportAdapterImpl::DeviceName(const DeviceUID& device_id) const {
  DeviceSptr device = FindDevice(device_id);
  if (device.valid()) {
    return device->name();
  } else {
    return "";
  }
}

void TransportAdapterImpl::SetTimeMetricObserver(TMMetricObserver* observer) {
  metric_observer_ = observer;
}

TMMetricObserver* TransportAdapterImpl::GetTimeMetricObserver() {
  return metric_observer_;
}

void TransportAdapterImpl::Store() const {
}

bool TransportAdapterImpl::Restore() {
  return true;
}

bool TransportAdapterImpl::ToBeAutoConnected(DeviceSptr device) const {
  return false;
}

ConnectionSptr TransportAdapterImpl::FindEstablishedConnection(
    const DeviceUID& device_id, const ApplicationHandle& app_handle) const {
  ConnectionSptr connection;
  pthread_mutex_lock(&connections_mutex_);
  ConnectionMap::const_iterator it =
      connections_.find(std::make_pair(device_id, app_handle));
  if (it != connections_.end()) {
    const ConnectionInfo& info = it->second;
    if (info.state == ConnectionInfo::ESTABLISHED) connection = info.connection;
  }
  pthread_mutex_unlock(&connections_mutex_);
  return connection;
}

TransportAdapter::Error TransportAdapterImpl::ConnectDevice(DeviceSptr device) {
  DeviceUID device_id = device->unique_device_id();
  ApplicationList app_list = device->GetApplicationList();
  bool errors_occured = false;
  for (ApplicationList::iterator it = app_list.begin(); it != app_list.end(); ++it) {
    ApplicationHandle app_handle = *it;
    LOG4CXX_INFO(logger_, "Attempt to connect device " << device_id <<
                          ", channel " << app_handle);
    Error error = Connect(device_id, app_handle);
    switch (error) {
      case OK:
        LOG4CXX_DEBUG(logger_, "OK");
        break;
      case ALREADY_EXISTS:
        LOG4CXX_INFO(logger_, "Already connected");
        break;
      default:
        LOG4CXX_ERROR(logger_, "Connect to device " << device_id <<
                               ", channel " << app_handle <<
                               " failed with error " << error);
        errors_occured = true;
        break;
    }
  }
  return errors_occured ? OK : FAIL;
}

void TransportAdapterImpl::RemoveDevice(const DeviceUID& device_handle) {
  pthread_mutex_lock(&devices_mutex_);
  DeviceMap::iterator i = devices_.find(device_handle); //ykazakov: there is no erase for const iterator on QNX
  if (i != devices_.end()) {
    DeviceSptr device = i->second;
    if (!device->keep_on_disconnect()) {
      devices_.erase(i);
      for (TransportAdapterListenerList::iterator it = listeners_.begin(); it != listeners_.end(); ++it) {
        TransportAdapterListener* listener = *it;
        listener->OnDeviceListUpdated(this);
      }
    }
  }
  pthread_mutex_unlock(&devices_mutex_);
}

}  // namespace transport_adapter

}  // namespace transport_manager
