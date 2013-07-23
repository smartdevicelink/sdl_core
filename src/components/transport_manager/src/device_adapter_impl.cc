/**
 * \file device_adapter_impl.cpp
 * \brief Class DeviceAdapterImpl.
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

#include "transport_manager/device_adapter_impl.h"
#include "transport_manager/device_adapter_listener.h"
#include "transport_manager/device_handle_generator.h"

namespace transport_manager {

namespace device_adapter {

log4cxx::LoggerPtr logger_ = log4cxx::LoggerPtr(
    log4cxx::Logger::getLogger("TransportManager"));

Device::Device(const std::string& name, const DeviceHandle& unique_device_id)
    : name_(name),
      unique_device_id_(unique_device_id) {
}

Device::~Device() {
}

Connection::Connection() {
}

Connection::~Connection() {
}

DeviceAdapterImpl::DeviceAdapterImpl(
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
      client_connection_listener_(client_connection_listener) {
  pthread_mutex_init(&devices_mutex_, 0);
  pthread_mutex_init(&connections_mutex_, 0);
}

DeviceAdapterImpl::~DeviceAdapterImpl() {
  if (device_scanner_) {
    device_scanner_->terminate();
    delete device_scanner_;
  }
  if (server_connection_factory_) {
    server_connection_factory_->terminate();
    delete server_connection_factory_;
  }
  if (client_connection_listener_) {
    client_connection_listener_->terminate();
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

DeviceAdapter::Error DeviceAdapterImpl::init(Configuration* configuration) {
  LOG4CXX_INFO(logger_, "Initializing device adapter");

  Error error = OK;

  if ((error == OK) && device_scanner_)
    error = device_scanner_->init();
  if ((error == OK) && server_connection_factory_)
    error = server_connection_factory_->init();
  if ((error == OK) && client_connection_listener_)
    error = client_connection_listener_->init();

  initialised_ = true;
  return error;
}

DeviceAdapter::Error DeviceAdapterImpl::searchDevices() {
  if (device_scanner_ == 0)
    return NOT_SUPPORTED;
  if (!device_scanner_->isInitialised())
    return BAD_STATE;
  return device_scanner_->scan();
}

DeviceAdapter::Error DeviceAdapterImpl::connect(
    const DeviceHandle device_handle, const ApplicationHandle app_handle) {
  if (server_connection_factory_ == 0)
    return NOT_SUPPORTED;
  if (!server_connection_factory_->isInitialised())
    return BAD_STATE;

  return server_connection_factory_->createConnection(device_handle, app_handle);
}

DeviceAdapter::Error DeviceAdapterImpl::disconnect(
    const DeviceHandle device_handle, const ApplicationHandle app_handle) {
  if (!initialised_)
    return BAD_STATE;
  ConnectionSptr connection = findEstablishedConnection(device_handle,
                                                        app_handle);
  if (connection.get() != 0) {
    return connection->disconnect();
  } else {
    return BAD_PARAM;
  }
}

DeviceAdapter::Error DeviceAdapterImpl::disconnectDevice(
    const DeviceHandle device_handle) {
  if (!initialised_)
    return BAD_STATE;

  Error error = OK;
  pthread_mutex_lock(&connections_mutex_);
  for (ConnectionMap::iterator it = connections_.begin();
      it != connections_.end(); ++it) {
    ConnectionInfo& info = it->second;
    if (info.device_handle == device_handle
        && info.state != ConnectionInfo::FINALISING) {
      if (OK != info.connection->disconnect()) {
        error = FAIL;
      }
    }
  }
  pthread_mutex_unlock(&connections_mutex_);

  return OK;
}

DeviceAdapter::Error DeviceAdapterImpl::sendData(
    const DeviceHandle device_handle, const ApplicationHandle app_handle,
    const RawMessageSptr data) {
  if (!initialised_)
    return BAD_STATE;

  ConnectionSptr connection = findEstablishedConnection(device_handle,
                                                        app_handle);
  if (connection.get() != 0) {
    connection->sendData(data);
    return OK;
  } else {
    return BAD_PARAM;
  }
}

DeviceList DeviceAdapterImpl::getDeviceList() const {
  DeviceList devices;
  pthread_mutex_lock(&devices_mutex_);
  for (DeviceMap::const_iterator it = devices_.begin(); it != devices_.end();
      ++it) {
    devices.push_back(it->first);
  }
  pthread_mutex_unlock(&devices_mutex_);
  return devices;
}

DeviceSptr DeviceAdapterImpl::addDevice(DeviceSptr device) {
  DeviceSptr existing_device;
  bool same_device_found = false;
  pthread_mutex_lock(&devices_mutex_);
  for (DeviceMap::const_iterator i = devices_.begin(); i != devices_.end();
      ++i) {
    existing_device = i->second;
    if (device->isSameAs(existing_device.get())) {
      same_device_found = true;
      break;
    }
  }
  if (!same_device_found) {
    devices_[device->unique_device_id()] = device;
  }
  pthread_mutex_unlock(&devices_mutex_);
  if (same_device_found)
    return existing_device;
  else
    return device;
}

void DeviceAdapterImpl::searchDeviceDone(const DeviceVector& devices) {
  DeviceMap new_devices;
  for (DeviceVector::const_iterator it = devices.begin(); it != devices.end();
      ++it) {
    DeviceSptr device = *it;
    bool device_found = false;

    pthread_mutex_lock(&devices_mutex_);
    for (DeviceMap::iterator it = devices_.begin(); it != devices_.end();
        ++it) {
      DeviceSptr existing_device = it->second;
      if (device->isSameAs(existing_device.get())) {
        device_found = true;
        break;
      }
    }
    pthread_mutex_unlock(&devices_mutex_);

    if (!device_found) {
      LOG4CXX_INFO(
          logger_,
          "Adding new device " << device->unique_device_id() << " (\"" << device->name() << "\")");
    }

    new_devices[device->unique_device_id()] = device;
  }

  pthread_mutex_lock(&connections_mutex_);
  std::set<DeviceHandle> connected_devices;
  for (ConnectionMap::const_iterator it = connections_.begin();
      it != connections_.end(); ++it) {
    const ConnectionInfo& info = it->second;
    if (info.state != ConnectionInfo::FINALISING)
      connected_devices.insert(info.device_handle);
  }
  pthread_mutex_unlock(&connections_mutex_);

  pthread_mutex_lock(&devices_mutex_);
  for (DeviceMap::iterator it = devices_.begin(); it != devices_.end(); ++it) {
    DeviceSptr existing_device = it->second;

    if (new_devices.end() == new_devices.find(it->first)) {
      if (connected_devices.end() != connected_devices.find(it->first)) {
        new_devices[it->first] = existing_device;
      }
    }
  }
  devices_ = new_devices;
  pthread_mutex_unlock(&devices_mutex_);

  for (DeviceAdapterListenerList::iterator it = listeners_.begin();
      it != listeners_.end(); ++it)
    (*it)->onSearchDeviceDone(this);
}

void DeviceAdapterImpl::searchDeviceFailed(const SearchDeviceError& error) {
  for (DeviceAdapterListenerList::iterator it = listeners_.begin();
      it != listeners_.end(); ++it)
    (*it)->onSearchDeviceFailed(this, error);
}

bool DeviceAdapterImpl::isSearchDevicesSupported() const {
  return device_scanner_ != 0;
}

bool DeviceAdapterImpl::isServerOriginatedConnectSupported() const {
  return server_connection_factory_ != 0;
}

bool DeviceAdapterImpl::isClientOriginatedConnectSupported() const {
  return client_connection_listener_ != 0;
}

void DeviceAdapterImpl::connectionCreated(ConnectionSptr connection,
                                          const DeviceHandle device_handle,
                                          const ApplicationHandle app_handle) {
  pthread_mutex_lock(&connections_mutex_);
  ConnectionInfo& info = connections_[std::make_pair(device_handle, app_handle)];
  info.app_handle = app_handle;
  info.device_handle = device_handle;
  info.connection = connection;
  info.state = ConnectionInfo::NEW;
  pthread_mutex_unlock(&connections_mutex_);
}

void DeviceAdapterImpl::disconnectDone(const DeviceHandle device_handle,
                                       const ApplicationHandle app_handle) {
  bool device_disconnected = true;
  pthread_mutex_lock(&connections_mutex_);
  connections_.erase(std::make_pair(device_handle, app_handle));
  for(ConnectionMap::const_iterator it = connections_.begin(); it != connections_.end(); ++it) {
    if(it->first.first == device_handle) {
      device_disconnected = false;
      break;
    }
  }
  pthread_mutex_unlock(&connections_mutex_);
  for (DeviceAdapterListenerList::iterator it = listeners_.begin();
      it != listeners_.end(); ++it) {
    DeviceAdapterListener* listener = *it;
    listener->onDisconnectDone(this, device_handle, app_handle);
    if(device_disconnected)
      listener->onDisconnectDeviceDone(this, device_handle);
  }
}

void DeviceAdapterImpl::dataReceiveDone(const DeviceHandle device_handle,
                                        const ApplicationHandle app_handle,
                                        RawMessageSptr message) {
  for (DeviceAdapterListenerList::iterator it = listeners_.begin();
      it != listeners_.end(); ++it)
    (*it)->onDataReceiveDone(this, device_handle, app_handle, message);
}

void DeviceAdapterImpl::dataReceiveFailed(const DeviceHandle device_handle,
                                          const ApplicationHandle app_handle,
                                          const DataReceiveError& error) {
  for (DeviceAdapterListenerList::iterator it = listeners_.begin();
      it != listeners_.end(); ++it)
    (*it)->onDataReceiveFailed(this, device_handle, app_handle, error);
}

void DeviceAdapterImpl::dataSendDone(const DeviceHandle device_handle,
                                     const ApplicationHandle app_handle,
                                     RawMessageSptr message) {
  for (DeviceAdapterListenerList::iterator it = listeners_.begin();
      it != listeners_.end(); ++it)
    (*it)->onDataSendDone(this, device_handle, app_handle, message);
}

void DeviceAdapterImpl::dataSendFailed(const DeviceHandle device_handle,
                                       const ApplicationHandle app_handle,
                                       RawMessageSptr message,
                                       const DataSendError& error) {
  for (DeviceAdapterListenerList::iterator it = listeners_.begin();
      it != listeners_.end(); ++it)
    (*it)->onDataSendFailed(this, device_handle, app_handle, message, error);
}

DeviceSptr DeviceAdapterImpl::findDevice(DeviceHandle device_handle) const {
  DeviceSptr ret;
  pthread_mutex_lock(&devices_mutex_);
  DeviceMap::const_iterator it = devices_.find(device_handle);
  if (it != devices_.end())
    ret = it->second;
  pthread_mutex_unlock(&devices_mutex_);
  return ret;
}

void DeviceAdapterImpl::connectDone(const DeviceHandle device_handle,
                                    const ApplicationHandle app_handle) {
  pthread_mutex_lock(&connections_mutex_);
  ConnectionMap::iterator it = connections_.find(
      std::make_pair(device_handle, app_handle));
  if (it != connections_.end()) {
    ConnectionInfo& info = it->second;
    info.state = ConnectionInfo::ESTABLISHED;
  }
  pthread_mutex_unlock(&connections_mutex_);

  for (DeviceAdapterListenerList::iterator it = listeners_.begin();
      it != listeners_.end(); ++it)
    (*it)->onConnectDone(this, device_handle, app_handle);
}

void DeviceAdapterImpl::connectFailed(const DeviceHandle device_handle,
                                      const ApplicationHandle app_handle,
                                      const ConnectError& error) {
  pthread_mutex_lock(&connections_mutex_);
  connections_.erase(std::make_pair(device_handle, app_handle));
  pthread_mutex_unlock(&connections_mutex_);
  for (DeviceAdapterListenerList::iterator it = listeners_.begin();
      it != listeners_.end(); ++it)
    (*it)->onConnectFailed(this, device_handle, app_handle, error);
}

void DeviceAdapterImpl::addListener(DeviceAdapterListener* listener) {
  listeners_.push_back(listener);
}

ApplicationList DeviceAdapterImpl::getApplicationList(
    const DeviceHandle device_handle) const {
  DeviceSptr device = findDevice(device_handle);
  if (device.get())
    return device->getApplicationList();
  else
    return ApplicationList();
}

void DeviceAdapterImpl::connectionFinished(const DeviceHandle device_handle,
                                           const ApplicationHandle app_handle) {
  pthread_mutex_lock(&connections_mutex_);
  ConnectionMap::iterator it = connections_.find(
      std::make_pair(device_handle, app_handle));
  if (it != connections_.end()) {
    ConnectionInfo& info = it->second;
    info.state = ConnectionInfo::FINALISING;
  }
  pthread_mutex_unlock(&connections_mutex_);
}

void DeviceAdapterImpl::connectionAborted(const DeviceHandle device_handle,
                                          const ApplicationHandle app_handle,
                                          const CommunicationError& error) {
  connectionFinished(device_handle, app_handle);
  for (DeviceAdapterListenerList::iterator it = listeners_.begin();
      it != listeners_.end(); ++it)
    (*it)->onUnexpectedDisconnect(this, device_handle, app_handle, error);
}

bool DeviceAdapterImpl::isInitialised() const {
  if (!initialised_)
    return false;
  if (device_scanner_ && !device_scanner_->isInitialised())
    return false;
  if (server_connection_factory_
      && !server_connection_factory_->isInitialised())
    return false;
  if (client_connection_listener_
      && !client_connection_listener_->isInitialised())
    return false;
  return true;
}

ConnectionSptr DeviceAdapterImpl::findEstablishedConnection(
    const DeviceHandle device_handle, const ApplicationHandle app_handle) {
  ConnectionSptr connection;
  pthread_mutex_lock(&connections_mutex_);
  ConnectionMap::iterator it = connections_.find(
      std::make_pair(device_handle, app_handle));
  if (it != connections_.end()) {
    ConnectionInfo& info = it->second;
    if (info.state == ConnectionInfo::ESTABLISHED)
      connection = info.connection;
  }
  pthread_mutex_unlock(&connections_mutex_);
  return connection;
}

}  // namespace device_adapter

}  // namespace transport_manager

