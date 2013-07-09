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

Device::Device(const char* name)
    : name_(name),
      unique_device_id_() {
}

Device::~Device(void) {
}

Connection::Connection(const DeviceHandle device_handle,
                       const ApplicationHandle app_handle, const int session_id)
    : device_handle_(device_handle),
      app_handle_(app_handle),
      session_id_(session_id),
      frames_to_send_(),
      frames_to_send_mutex_() {
  pthread_mutex_init(&frames_to_send_mutex_, 0);
}

Connection::~Connection() {
  pthread_mutex_destroy(&frames_to_send_mutex_);
}

void Connection::pushFrame(RawMessageSptr frame) {
  pthread_mutex_lock(&frames_to_send_mutex_);
  frames_to_send_.push(frame);
  pthread_mutex_unlock(&frames_to_send_mutex_);
}

RawMessageSptr Connection::popFrame() {
  pthread_mutex_lock(&frames_to_send_mutex_);
  RawMessageSptr frame = 0;
  if (!frames_to_send_.empty()) {
    frame = frames_to_send_.front();
    frames_to_send_.pop();
  }
  pthread_mutex_unlock(&frames_to_send_mutex_);
  return frame;
}

DeviceAdapterImpl::DeviceAdapterImpl()
    : listener_(0),
      handle_generator_(0),
      initialised_(0),
      devices_(),
      devices_mutex_(),
      connected_connections_(),
      finished_connections_(),
      connections_mutex_(),
      device_scanner_(0),
      data_transmitter_(0),
      server_connection_processor_(0),
      client_connection_listener_(0),
      disconnector_(0),
      functionals_() {
  pthread_mutex_init(&devices_mutex_, 0);
  pthread_mutex_init(&connections_mutex_, 0);
}

DeviceAdapterImpl::~DeviceAdapterImpl() {
  pthread_mutex_destroy(&connections_mutex_);
  pthread_mutex_destroy(&devices_mutex_);
}

Error DeviceAdapterImpl::init(DeviceAdapterListener* listener,
                              DeviceHandleGenerator* handle_generator,
                              Configuration* configuration) {
  if (listener == 0)
    return BAD_PARAM;
  if (handle_generator == 0)
    return BAD_PARAM;

  LOG4CXX_INFO(logger_, "Initializing device adapter");

  listener_ = listener;
  handle_generator_ = handle_generator;

  for (DeviceAdapterFunctionals::iterator it = functionals_.begin();
      it != functionals_.end(); ++it) {
    DeviceAdapterFunctional* functional = *it;
    Error error = functional->init();
    if (error != OK) {
      return error;
    }
  }

  initialised_ = true;
  return OK;
}

Error DeviceAdapterImpl::searchDevices() {
  if (!initialised_)
    return BAD_STATE;
  if (device_scanner_.get() == 0)
    return NOT_SUPPORTED;
  device_scanner_->scan();
  return OK;
}

Error DeviceAdapterImpl::connect(const DeviceHandle device_handle,
                                 const ApplicationHandle app_handle,
                                 const SessionID session_id) {
  if (!initialised_)
    return BAD_STATE;
  if (server_connection_processor_.get() == 0)
    return NOT_SUPPORTED;
  server_connection_processor_->createConnection(device_handle, app_handle,
                                                 session_id);
  return OK;
}

Error DeviceAdapterImpl::disconnect(const SessionID session_id) {
  if (!initialised_)
    return BAD_STATE;
  if (disconnector_.get() == 0)
    return NOT_SUPPORTED;

  Error error = OK;
  ConnectionSptr connection = findConnection(session_id);

  if (connection.get() != 0) {
    endConnection(connection);
    return OK;
  } else {
    return BAD_PARAM;
  }
}

Error DeviceAdapterImpl::disconnectDevice(const DeviceHandle device_handle) {
  if (!initialised_)
    return BAD_STATE;
  if (disconnector_.get() == 0)
    return NOT_SUPPORTED;

  pthread_mutex_lock(&connections_mutex_);
  for (ConnectionMap::iterator it = connected_connections_.begin();
      it != connected_connections_.end(); ++it) {
    ConnectionSptr connection = it->second;
    if (connection->device_handle() == device_handle) {
      disconnector_->disconnect(connection);
    }
  }
  pthread_mutex_unlock(&connections_mutex_);

  return OK;
}

Error DeviceAdapterImpl::sendData(const int session_id,
                                  const RawMessageSptr data) {
  if (!initialised_)
    return BAD_STATE;
  if (data_transmitter_.get() == 0)
    return NOT_SUPPORTED;

  ConnectionSptr connection = findConnection(session_id);
  if (connection.get() != 0) {
    connection->pushFrame(data);
    data_transmitter_->notifyDataAvailable(connection);
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

DeviceAdapterListener* DeviceAdapterImpl::getListener() {
  return listener_;
}

DeviceHandleGenerator* DeviceAdapterImpl::getDeviceHandleGenerator() {
  return handle_generator_;
}

void DeviceAdapterImpl::addDevice(DeviceSptr device) {
  DeviceHandle handle;

  bool same_device_found = false;
  pthread_mutex_lock(&devices_mutex_);
  for (DeviceMap::const_iterator i = devices_.begin(); i != devices_.end();
      ++i) {
    const DeviceSptr other_device = i->second;
    if (device->isSameAs(other_device.get())) {
      same_device_found = true;
      break;
    }
  }
  if (!same_device_found) {
    DeviceHandle handle = handle_generator_->generate();
    devices_[handle] = device;
  }
  pthread_mutex_unlock(&devices_mutex_);
}

void DeviceAdapterImpl::setDevices(const DeviceVector& devices) {
  DeviceMap new_devices;
  for (DeviceVector::const_iterator it = devices.begin(); it != devices.end(); ++it) {
    DeviceSptr device = *it;
    DeviceHandle device_handle;
    bool device_found = false;

    pthread_mutex_lock(&devices_mutex_);
    for (DeviceMap::iterator it = devices_.begin(); it != devices_.end();
        ++it) {
      DeviceSptr existing_device = it->second;
      if (device->isSameAs(existing_device.get())) {
        device_handle = it->first;
        device_found = true;
        break;
      }
    }
    pthread_mutex_unlock(&devices_mutex_);

    if (!device_found) {
      device_handle = handle_generator_->generate();
      LOG4CXX_INFO(
          logger_,
          "Adding new device " << device_handle << " (\"" << device->name() << "\")");
    }

    new_devices[device_handle] = device;
  }

  pthread_mutex_lock(&connections_mutex_);
  std::set<DeviceHandle> connected_devices;
  for (ConnectionMap::const_iterator it = connected_connections_.begin();
      it != connected_connections_.end(); ++it) {
    const ConnectionSptr connection = it->second;
    connected_devices.insert(connection->device_handle());
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
}

void DeviceAdapterImpl::addConnection(ConnectionSptr connection) {
  pthread_mutex_lock(&connections_mutex_);
  connected_connections_[connection->session_id()] = connection;
  pthread_mutex_unlock(&connections_mutex_);
  if (data_transmitter_.get() != 0) {
    data_transmitter_->registerConnection(connection);
  }
}

void DeviceAdapterImpl::endConnection(ConnectionSptr connection) {
  pthread_mutex_lock(&connections_mutex_);
  connected_connections_.erase(connection->session_id());
  finished_connections_[connection->session_id()] = connection;
  pthread_mutex_unlock(&connections_mutex_);
  if (data_transmitter_.get() != 0) {
    data_transmitter_->unregisterConnection(connection);
  }
  if (disconnector_.get() != 0) {
    disconnector_->disconnect(connection);
  }
}

void DeviceAdapterImpl::removeConnection(ConnectionSptr connection) {
  pthread_mutex_lock(&connections_mutex_);
  finished_connections_.erase(connection->session_id());
  pthread_mutex_unlock(&connections_mutex_);
}

void DeviceAdapterImpl::setDeviceScanner(DeviceScanner* device_scanner) {
  functionals_.erase(device_scanner);
  device_scanner_.reset(device_scanner);
  functionals_.insert(device_scanner);
}

void DeviceAdapterImpl::setDataTransmitter(
    DataTransmitter* data_transmitter) {
  functionals_.erase(data_transmitter);
  data_transmitter_.reset(data_transmitter);
  functionals_.insert(data_transmitter);
}

void DeviceAdapterImpl::setServerConnectionProcessor(
    ServerConnectionProcessor* server_connection_processor) {
  functionals_.erase(server_connection_processor);
  server_connection_processor_.reset(server_connection_processor);
  functionals_.insert(server_connection_processor);
}

void DeviceAdapterImpl::setClientConnectionListener(
    ClientConnectionListener* client_connection_listener) {
  functionals_.erase(client_connection_listener);
  client_connection_listener_.reset(client_connection_listener);
  functionals_.insert(client_connection_listener);
}

void DeviceAdapterImpl::setDisconnector(Disconnector* disconnector) {
  functionals_.erase(disconnector);
  disconnector_.reset(disconnector);
  functionals_.insert(disconnector);
}

bool DeviceAdapterImpl::isSearchDevicesSupported() const {
  return device_scanner_.get() != 0;
}

bool DeviceAdapterImpl::isServerOriginatedConnectSupported() const {
  return server_connection_processor_.get() != 0;
}

bool DeviceAdapterImpl::isClientOriginatedConnectSupported() const {
  return client_connection_listener_.get() != 0;
}

ConnectionSptr DeviceAdapterImpl::findConnection(const SessionID session_id) {
  ConnectionSptr connection;
  pthread_mutex_lock(&connections_mutex_);
  ConnectionMap::iterator it = connected_connections_.find(session_id);
  if (it != connected_connections_.end()) {
    connection = it->second;
  }
  pthread_mutex_unlock(&connections_mutex_);
  return connection;
}

}  // namespace device_adapter

}  // namespace transport_manager
