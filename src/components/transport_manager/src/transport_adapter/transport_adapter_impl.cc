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
#include "utils/helpers.h"

#include "transport_manager/transport_adapter/transport_adapter_impl.h"
#include "transport_manager/transport_adapter/transport_adapter_listener.h"
#include "transport_manager/transport_adapter/device_scanner.h"
#include "transport_manager/transport_adapter/server_connection_factory.h"
#include "transport_manager/transport_adapter/client_connection_listener.h"

namespace transport_manager {
namespace transport_adapter {

SDL_CREATE_LOGGER("TransportManager")
namespace {
#ifdef SDL_CPP11
DeviceTypes devices_type = {
    std::make_pair(AOA, std::string("USB_AOA")),
    std::make_pair(PASA_AOA, std::string("USB_AOA")),
    std::make_pair(MME, std::string("USB_IOS")),
    std::make_pair(BLUETOOTH, std::string("BLUETOOTH")),
    std::make_pair(PASA_BLUETOOTH, std::string("BLUETOOTH")),
    std::make_pair(TCP, std::string("WIFI"))};
#else
DeviceTypes create_map() {
  DeviceTypes devices_type;
  devices_type.insert(std::make_pair(AOA, std::string("USB_AOA")));
  devices_type.insert(std::make_pair(PASA_AOA, std::string("USB_AOA")));
  devices_type.insert(std::make_pair(MME, std::string("USB_IOS")));
  devices_type.insert(std::make_pair(BLUETOOTH, std::string("BLUETOOTH")));
  devices_type.insert(std::make_pair(PASA_BLUETOOTH, std::string("BLUETOOTH")));
  devices_type.insert(std::make_pair(TCP, std::string("WIFI")));
  return devices_type;
}
DeviceTypes devices_type = create_map();
#endif  // SDL_CPP11
}

TransportAdapterImpl::TransportAdapterImpl(
    DeviceScanner* device_scanner,
    ServerConnectionFactory* server_connection_factory,
    ClientConnectionListener* client_connection_listener,
    resumption::LastState& last_state,
    const TransportManagerSettings& settings)
    : listeners_()
    , initialised_(0)
    , devices_()
    , devices_mutex_()
    , connections_()
    , connections_lock_()
    ,
#ifdef TELEMETRY_MONITOR
    metric_observer_(NULL)
    ,
#endif  // TELEMETRY_MONITOR
    device_scanner_(device_scanner)
    , server_connection_factory_(server_connection_factory)
    , client_connection_listener_(client_connection_listener)
    , last_state_(last_state)
    , settings_(settings) {
}

TransportAdapterImpl::~TransportAdapterImpl() {
  Terminate();

  if (device_scanner_) {
    SDL_DEBUG("Deleting device_scanner_ " << device_scanner_);
    delete device_scanner_;
    SDL_DEBUG("device_scanner_ deleted.");
  }
  if (server_connection_factory_) {
    SDL_DEBUG("Deleting server_connection_factory "
              << server_connection_factory_);
    delete server_connection_factory_;
    SDL_DEBUG("server_connection_factory deleted.");
  }
  if (client_connection_listener_) {
    SDL_DEBUG("Deleting client_connection_listener_ "
              << client_connection_listener_);
    delete client_connection_listener_;
    SDL_DEBUG("client_connection_listener_ deleted.");
  }
}

void TransportAdapterImpl::Terminate() {
  if (device_scanner_) {
    device_scanner_->Terminate();
    SDL_DEBUG("device_scanner_ " << device_scanner_ << " terminated.");
  }
  if (server_connection_factory_) {
    server_connection_factory_->Terminate();
    SDL_DEBUG("server_connection_factory " << server_connection_factory_
                                           << " terminated.");
  }
  if (client_connection_listener_) {
    client_connection_listener_->Terminate();
    SDL_DEBUG("client_connection_listener_ " << client_connection_listener_
                                             << " terminated.");
  }

  ConnectionMap connections;
  {
    sync_primitives::AutoWriteLock lock(connections_lock_);
    std::swap(connections, connections_);
  }
  connections.clear();

  SDL_DEBUG("Connections deleted");

  DeviceMap devices;
  devices_mutex_.Acquire();
  std::swap(devices, devices_);
  devices_mutex_.Release();
  devices.clear();

  SDL_DEBUG("Devices deleted");
}

TransportAdapter::Error TransportAdapterImpl::Init() {
  SDL_TRACE("enter");

  Error error = OK;

  if ((error == OK) && device_scanner_) {
    error = device_scanner_->Init();
  }
  if ((error == OK) && server_connection_factory_) {
    error = server_connection_factory_->Init();
  }
  if ((error == OK) && client_connection_listener_) {
    error = client_connection_listener_->Init();
  }

  initialised_ = (error == OK);

  if (get_settings().use_last_state()) {
    if (!Restore()) {
      SDL_WARN("could not restore transport adapter state");
      error = FAIL;
    }
  }
  SDL_TRACE("exit with error: " << error);
  return error;
}

TransportAdapter::Error TransportAdapterImpl::SearchDevices() {
  SDL_TRACE("enter");
  if (device_scanner_ == NULL) {
    SDL_TRACE("exit with NOT_SUPPORTED");
    return NOT_SUPPORTED;
  } else if (!device_scanner_->IsInitialised()) {
    SDL_TRACE("exit with BAD_STATE");
    return BAD_STATE;
  }
  TransportAdapter::Error er = device_scanner_->Scan();
  SDL_TRACE("exit with error: " << er);
  return er;
}

TransportAdapter::Error TransportAdapterImpl::Connect(
    const DeviceUID& device_id, const ApplicationHandle& app_handle) {
  SDL_TRACE("enter. DeviceUID " << device_id << " ApplicationHandle "
                                << app_handle);
  if (server_connection_factory_ == 0) {
    SDL_TRACE("exit with NOT_SUPPORTED");
    return NOT_SUPPORTED;
  }
  if (!server_connection_factory_->IsInitialised()) {
    SDL_TRACE("exit with BAD_STATE");
    return BAD_STATE;
  }

  {
    sync_primitives::AutoWriteLock lock(connections_lock_);
    const bool already_exists =
        connections_.end() !=
        connections_.find(std::make_pair(device_id, app_handle));
    if (!already_exists) {
      ConnectionInfo& info =
          connections_[std::make_pair(device_id, app_handle)];
      info.app_handle = app_handle;
      info.device_id = device_id;
      info.state = ConnectionInfo::NEW;
    } else {
      SDL_TRACE("exit with ALREADY_EXISTS");
      return ALREADY_EXISTS;
    }
  }

  const TransportAdapter::Error err =
      server_connection_factory_->CreateConnection(device_id, app_handle);
  if (TransportAdapter::OK != err) {
    sync_primitives::AutoWriteLock lock(connections_lock_);
    connections_.erase(std::make_pair(device_id, app_handle));
  }
  SDL_TRACE("exit with error: " << err);
  return err;
}

TransportAdapter::Error TransportAdapterImpl::ConnectDevice(
    const DeviceUID& device_handle) {
  SDL_TRACE("enter with device_handle: " << &device_handle);
  DeviceSptr device = FindDevice(device_handle);
  if (device) {
    TransportAdapter::Error err = ConnectDevice(device);
    SDL_TRACE("exit with error: " << err);
    return err;
  } else {
    SDL_TRACE("exit with BAD_PARAM");
    return BAD_PARAM;
  }
}

TransportAdapter::Error TransportAdapterImpl::Disconnect(
    const DeviceUID& device_id, const ApplicationHandle& app_handle) {
  SDL_TRACE("enter. device_id: " << &device_id
                                 << ", device_id: " << &device_id);
  if (!initialised_) {
    SDL_TRACE("exit with BAD_STATE");
    return BAD_STATE;
  }
  ConnectionSPtr connection = FindEstablishedConnection(device_id, app_handle);
  if (connection) {
    TransportAdapter::Error err = connection->Disconnect();
    SDL_TRACE("exit with error: " << err);
    return err;
  } else {
    SDL_TRACE("exit with BAD_PARAM");
    return BAD_PARAM;
  }
}

TransportAdapter::Error TransportAdapterImpl::DisconnectDevice(
    const DeviceUID& device_id) {
  SDL_TRACE("enter. device_id: " << &device_id);
  if (!initialised_) {
    SDL_TRACE("exit with BAD_STATE");
    return BAD_STATE;
  }

  Error error = OK;

  std::vector<ConnectionInfo> to_disconnect;
  {
    sync_primitives::AutoReadLock lock(connections_lock_);
    for (ConnectionMap::const_iterator i = connections_.begin();
         i != connections_.end();
         ++i) {
      ConnectionInfo info = i->second;
      if (info.device_id == device_id &&
          info.state != ConnectionInfo::FINALISING) {
        to_disconnect.push_back(info);
      }
    }
  }

  for (std::vector<ConnectionInfo>::const_iterator j = to_disconnect.begin();
       j != to_disconnect.end();
       ++j) {
    ConnectionInfo info = *j;
    if (OK != info.connection->Disconnect()) {
      error = FAIL;
      SDL_ERROR("Error on disconnect " << error);
    }
  }

  return error;
}

TransportAdapter::Error TransportAdapterImpl::SendData(
    const DeviceUID& device_id,
    const ApplicationHandle& app_handle,
    const ::protocol_handler::RawMessagePtr data) {
  SDL_TRACE("enter. device_id: " << &device_id << ", app_handle: "
                                 << &app_handle << ", data: " << data);
  if (!initialised_) {
    SDL_TRACE("exit with BAD_STATE");
    return BAD_STATE;
  }

  ConnectionSPtr connection = FindEstablishedConnection(device_id, app_handle);
  if (connection) {
    TransportAdapter::Error err = connection->SendData(data);
    SDL_TRACE("exit with error: " << err);
    return err;
  } else {
    SDL_TRACE("exit with BAD_PARAM");
    return BAD_PARAM;
  }
}

TransportAdapter::Error TransportAdapterImpl::StartClientListening() {
  SDL_TRACE("enter");
  if (client_connection_listener_ == 0) {
    SDL_TRACE("exit with NOT_SUPPORTED");
    return NOT_SUPPORTED;
  }
  if (!client_connection_listener_->IsInitialised()) {
    SDL_TRACE("exit with BAD_STATE");
    return BAD_STATE;
  }
  TransportAdapter::Error err = client_connection_listener_->StartListening();
  SDL_TRACE("exit with error: " << err);
  return err;
}

TransportAdapter::Error TransportAdapterImpl::StopClientListening() {
  SDL_TRACE("enter");
  if (client_connection_listener_ == 0) {
    SDL_TRACE("exit with NOT_SUPPORTED");
    return NOT_SUPPORTED;
  }
  if (!client_connection_listener_->IsInitialised()) {
    SDL_TRACE("exit with BAD_STATE");
    return BAD_STATE;
  }
  TransportAdapter::Error err = client_connection_listener_->StopListening();
  sync_primitives::AutoLock locker(devices_mutex_);
  for (DeviceMap::iterator it = devices_.begin(); it != devices_.end(); ++it) {
    it->second->Stop();
  }
  SDL_TRACE("exit with error: " << err);
  return err;
}

DeviceList TransportAdapterImpl::GetDeviceList() const {
  SDL_AUTO_TRACE();
  DeviceList devices;
  sync_primitives::AutoLock locker(devices_mutex_);
  for (DeviceMap::const_iterator it = devices_.begin(); it != devices_.end();
       ++it) {
    devices.push_back(it->first);
  }
  SDL_TRACE("exit with DeviceList. It's' size = " << devices.size());
  return devices;
}

DeviceSptr TransportAdapterImpl::AddDevice(DeviceSptr device) {
  SDL_TRACE("enter. device: " << device);
  DeviceSptr existing_device;
  bool same_device_found = false;
  devices_mutex_.Acquire();
  for (DeviceMap::const_iterator i = devices_.begin(); i != devices_.end();
       ++i) {
    existing_device = i->second;
    if (device->IsSameAs(existing_device.get())) {
      same_device_found = true;
      SDL_DEBUG("device " << device << "already exists");
      break;
    }
  }
  if (!same_device_found) {
    devices_[device->unique_device_id()] = device;
  }
  devices_mutex_.Release();
  if (same_device_found) {
    SDL_TRACE("exit with TRUE. Condition: same_device_found");
    return existing_device;
  } else {
    for (TransportAdapterListenerList::iterator it = listeners_.begin();
         it != listeners_.end();
         ++it) {
      (*it)->OnDeviceListUpdated(this);
    }
    if (ToBeAutoConnected(device)) {
      ConnectDevice(device);
    }
    SDL_TRACE("exit with DeviceSptr " << device);
    return device;
  }
}

void TransportAdapterImpl::SearchDeviceDone(const DeviceVector& devices) {
  SDL_TRACE("enter. devices: " << &devices);
  DeviceMap new_devices;
  for (DeviceVector::const_iterator it = devices.begin(); it != devices.end();
       ++it) {
    DeviceSptr device = *it;
    bool device_found = false;

    devices_mutex_.Acquire();
    for (DeviceMap::iterator it = devices_.begin(); it != devices_.end();
         ++it) {
      DeviceSptr existing_device = it->second;
      if (device->IsSameAs(existing_device.get())) {
        existing_device->set_keep_on_disconnect(true);
        device_found = true;
        SDL_DEBUG("device found. DeviceSptr" << it->second);
        break;
      }
    }
    devices_mutex_.Release();

    if (!device_found) {
      SDL_INFO("Adding new device " << device->unique_device_id() << " (\""
                                    << device->name() << "\")");
    }

    device->set_keep_on_disconnect(true);
    new_devices[device->unique_device_id()] = device;
  }

  std::set<DeviceUID> connected_devices;
  {
    sync_primitives::AutoReadLock lock(connections_lock_);
    for (ConnectionMap::const_iterator it = connections_.begin();
         it != connections_.end();
         ++it) {
      const ConnectionInfo& info = it->second;
      if (info.state != ConnectionInfo::FINALISING) {
        connected_devices.insert(info.device_id);
      }
    }
  }

  DeviceMap all_devices = new_devices;
  devices_mutex_.Acquire();
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
  devices_mutex_.Release();

  for (TransportAdapterListenerList::iterator it = listeners_.begin();
       it != listeners_.end();
       ++it) {
    (*it)->OnDeviceListUpdated(this);
    (*it)->OnSearchDeviceDone(this);
  }

  for (DeviceMap::iterator it = new_devices.begin(); it != new_devices.end();
       ++it) {
    DeviceSptr device = it->second;
    if (ToBeAutoConnected(device)) {
      ConnectDevice(device);
    }
  }
  SDL_TRACE("exit");
}

void TransportAdapterImpl::ApplicationListUpdated(
    const DeviceUID& device_handle) {
  // default implementation does nothing
  // and is reimplemented in MME transport adapter only
}

void TransportAdapterImpl::FindNewApplicationsRequest() {
  SDL_TRACE("enter");
  for (TransportAdapterListenerList::iterator i = listeners_.begin();
       i != listeners_.end();
       ++i) {
    TransportAdapterListener* listener = *i;
    listener->OnFindNewApplicationsRequest(this);
  }
  SDL_TRACE("exit");
}

void TransportAdapterImpl::SearchDeviceFailed(const SearchDeviceError& error) {
  SDL_TRACE("enter");
  for (TransportAdapterListenerList::iterator it = listeners_.begin();
       it != listeners_.end();
       ++it) {
    (*it)->OnSearchDeviceFailed(this, error);
  }
  SDL_TRACE("exit");
}

bool TransportAdapterImpl::IsSearchDevicesSupported() const {
  SDL_TRACE("enter");
  return device_scanner_ != 0;
  SDL_TRACE("exit");
}

bool TransportAdapterImpl::IsServerOriginatedConnectSupported() const {
  SDL_TRACE("enter");
  return server_connection_factory_ != 0;
  SDL_TRACE("exit");
}

bool TransportAdapterImpl::IsClientOriginatedConnectSupported() const {
  SDL_AUTO_TRACE();
  return client_connection_listener_ != 0;
}

void TransportAdapterImpl::ConnectionCreated(
    ConnectionSPtr connection,
    const DeviceUID& device_id,
    const ApplicationHandle& app_handle) {
  SDL_TRACE("enter connection:" << connection << ", device_id: " << &device_id
                                << ", app_handle: " << &app_handle);
  sync_primitives::AutoReadLock lock(connections_lock_);
  ConnectionInfo& info = connections_[std::make_pair(device_id, app_handle)];
  info.app_handle = app_handle;
  info.device_id = device_id;
  info.connection = connection;
  info.state = ConnectionInfo::NEW;
}

void TransportAdapterImpl::DeviceDisconnected(
    const DeviceUID& device_handle, const DisconnectDeviceError& error) {
  const DeviceUID device_uid = device_handle;
  SDL_TRACE("enter. device_handle: " << &device_uid << ", error: " << &error);
  ApplicationList app_list = GetApplicationList(device_uid);
  for (ApplicationList::const_iterator i = app_list.begin();
       i != app_list.end();
       ++i) {
    ApplicationHandle app_handle = *i;
    for (TransportAdapterListenerList::iterator it = listeners_.begin();
         it != listeners_.end();
         ++it) {
      TransportAdapterListener* listener = *it;
      listener->OnUnexpectedDisconnect(
          this, device_uid, app_handle, CommunicationError());
    }
  }

  for (TransportAdapterListenerList::iterator it = listeners_.begin();
       it != listeners_.end();
       ++it) {
    TransportAdapterListener* listener = *it;
    listener->OnDisconnectDeviceDone(this, device_uid);
  }

  {
    sync_primitives::AutoWriteLock lock(connections_lock_);
    for (ApplicationList::const_iterator i = app_list.begin();
         i != app_list.end();
         ++i) {
      ApplicationHandle app_handle = *i;
      connections_.erase(std::make_pair(device_uid, app_handle));
    }
  }

  RemoveDevice(device_uid);
  SDL_TRACE("exit");
}

bool TransportAdapterImpl::IsSingleApplication(
    const DeviceUID& device_uid, const ApplicationHandle& app_uid) {
  sync_primitives::AutoReadLock locker(connections_lock_);
  for (ConnectionMap::const_iterator it = connections_.begin();
       it != connections_.end();
       ++it) {
    const DeviceUID& current_device_id = it->first.first;
    const ApplicationHandle& current_app_handle = it->first.second;
    if (current_device_id == device_uid && current_app_handle != app_uid) {
      SDL_DEBUG(
          "break. Condition: current_device_id == device_id && "
          "current_app_handle != app_handle");

      return false;
    }
  }
  return true;
}

void TransportAdapterImpl::DisconnectDone(const DeviceUID& device_handle,
                                          const ApplicationHandle& app_handle) {
  const DeviceUID device_uid = device_handle;
  const ApplicationHandle app_uid = app_handle;
  SDL_TRACE("enter. device_id: " << &device_uid
                                 << ", app_handle: " << &app_uid);
  DeviceSptr device = FindDevice(device_handle);
  if (!device) {
    SDL_WARN("Device: uid " << &device_uid << " not found");
    return;
  }

  bool device_disconnected =
      ToBeAutoDisconnected(device) && IsSingleApplication(device_uid, app_uid);

  for (TransportAdapterListenerList::iterator it = listeners_.begin();
       it != listeners_.end();
       ++it) {
    TransportAdapterListener* listener = *it;
    listener->OnDisconnectDone(this, device_uid, app_uid);
    if (device_disconnected) {
      listener->OnDisconnectDeviceDone(this, device_uid);
    }
  }
  {
    sync_primitives::AutoWriteLock lock(connections_lock_);
    connections_.erase(std::make_pair(device_uid, app_uid));
  }

  if (device_disconnected) {
    RemoveDevice(device_uid);
  }

  Store();
  SDL_TRACE("exit");
}

void TransportAdapterImpl::DataReceiveDone(
    const DeviceUID& device_id,
    const ApplicationHandle& app_handle,
    ::protocol_handler::RawMessagePtr message) {
  SDL_TRACE("enter. device_id: " << &device_id << ", app_handle: "
                                 << &app_handle << ", message: " << message);

#ifdef TELEMETRY_MONITOR
  if (metric_observer_) {
    metric_observer_->StartRawMsg(message.get());
  }
#endif  // TELEMETRY_MONITOR

  for (TransportAdapterListenerList::iterator it = listeners_.begin();
       it != listeners_.end();
       ++it) {
    (*it)->OnDataReceiveDone(this, device_id, app_handle, message);
  }
  SDL_TRACE("exit");
}

void TransportAdapterImpl::DataReceiveFailed(
    const DeviceUID& device_id,
    const ApplicationHandle& app_handle,
    const DataReceiveError& error) {
  SDL_TRACE("enter");
  for (TransportAdapterListenerList::iterator it = listeners_.begin();
       it != listeners_.end();
       ++it) {
    (*it)->OnDataReceiveFailed(this, device_id, app_handle, error);
  }
  SDL_TRACE("exit");
}

void TransportAdapterImpl::DataSendDone(
    const DeviceUID& device_id,
    const ApplicationHandle& app_handle,
    ::protocol_handler::RawMessagePtr message) {
  SDL_TRACE("enter");
  for (TransportAdapterListenerList::iterator it = listeners_.begin();
       it != listeners_.end();
       ++it) {
    (*it)->OnDataSendDone(this, device_id, app_handle, message);
  }
  SDL_TRACE("exit");
}

void TransportAdapterImpl::DataSendFailed(
    const DeviceUID& device_id,
    const ApplicationHandle& app_handle,
    ::protocol_handler::RawMessagePtr message,
    const DataSendError& error) {
  SDL_TRACE("enter");
  for (TransportAdapterListenerList::iterator it = listeners_.begin();
       it != listeners_.end();
       ++it) {
    (*it)->OnDataSendFailed(this, device_id, app_handle, message, error);
  }
  SDL_TRACE("exit");
}

DeviceSptr TransportAdapterImpl::FindDevice(const DeviceUID& device_id) const {
  SDL_TRACE("enter. device_id: " << &device_id);
  DeviceSptr ret;
  sync_primitives::AutoLock locker(devices_mutex_);
  SDL_DEBUG("devices_.size() = " << devices_.size());
  DeviceMap::const_iterator it = devices_.find(device_id);
  if (it != devices_.end()) {
    ret = it->second;
  } else {
    SDL_WARN("Device " << device_id << " not found.");
  }
  SDL_TRACE("exit with DeviceSptr: " << ret);
  return ret;
}

void TransportAdapterImpl::ConnectDone(const DeviceUID& device_id,
                                       const ApplicationHandle& app_handle) {
  SDL_TRACE("enter. device_id: " << &device_id
                                 << ", app_handle: " << &app_handle);
  {
    sync_primitives::AutoReadLock lock(connections_lock_);
    ConnectionMap::iterator it_conn =
        connections_.find(std::make_pair(device_id, app_handle));
    if (it_conn != connections_.end()) {
      ConnectionInfo& info = it_conn->second;
      info.state = ConnectionInfo::ESTABLISHED;
    }
  }

  for (TransportAdapterListenerList::iterator it = listeners_.begin();
       it != listeners_.end();
       ++it) {
    (*it)->OnConnectDone(this, device_id, app_handle);
  }

  Store();
  SDL_TRACE("exit");
}

void TransportAdapterImpl::ConnectFailed(const DeviceUID& device_handle,
                                         const ApplicationHandle& app_handle,
                                         const ConnectError& error) {
  const DeviceUID device_uid = device_handle;
  const ApplicationHandle app_uid = app_handle;
  SDL_TRACE("enter. device_id: " << &device_uid << ", app_handle: " << &app_uid
                                 << ", error: " << &error);
  {
    sync_primitives::AutoWriteLock lock(connections_lock_);
    connections_.erase(std::make_pair(device_uid, app_uid));
  }
  for (TransportAdapterListenerList::iterator it = listeners_.begin();
       it != listeners_.end();
       ++it) {
    (*it)->OnConnectFailed(this, device_uid, app_uid, error);
  }
  SDL_TRACE("exit");
}

void TransportAdapterImpl::RemoveFinalizedConnection(
    const DeviceUID& device_handle, const ApplicationHandle& app_handle) {
  const DeviceUID device_uid = device_handle;
  const ApplicationHandle app_uid = app_handle;
  UNUSED(app_uid);
  SDL_AUTO_TRACE();
  sync_primitives::AutoWriteLock lock(connections_lock_);
  ConnectionMap::iterator it_conn =
      connections_.find(std::make_pair(device_uid, app_handle));
  if (it_conn == connections_.end()) {
    SDL_WARN("Device_id: " << &device_uid << ", app_handle: " << &app_uid
                           << " connection not found");
    return;
  }
  const ConnectionInfo& info = it_conn->second;
  if (info.state != ConnectionInfo::FINALISING) {
    SDL_WARN("Device_id: " << &device_uid << ", app_handle: " << &app_uid
                           << " connection not finalized");
    return;
  }
  connections_.erase(it_conn);
}

void TransportAdapterImpl::AddListener(TransportAdapterListener* listener) {
  SDL_TRACE("enter");
  listeners_.push_back(listener);
  SDL_TRACE("exit");
}

ApplicationList TransportAdapterImpl::GetApplicationList(
    const DeviceUID& device_id) const {
  SDL_TRACE("enter. device_id: " << &device_id);
  DeviceSptr device = FindDevice(device_id);
  if (device.valid()) {
    ApplicationList lst = device->GetApplicationList();
    SDL_TRACE("exit with ApplicationList. It's size = "
              << lst.size() << " Condition: device.valid()");
    return lst;
  }
  SDL_TRACE("exit with empty ApplicationList. Condition: NOT device.valid()");
  return ApplicationList();
}

void TransportAdapterImpl::ConnectionFinished(
    const DeviceUID& device_id, const ApplicationHandle& app_handle) {
  SDL_TRACE("enter. device_id: " << &device_id
                                 << ", app_handle: " << &app_handle);
  sync_primitives::AutoReadLock lock(connections_lock_);
  ConnectionMap::iterator it =
      connections_.find(std::make_pair(device_id, app_handle));
  if (it != connections_.end()) {
    ConnectionInfo& info = it->second;
    info.state = ConnectionInfo::FINALISING;
  }
}

void TransportAdapterImpl::ConnectionAborted(
    const DeviceUID& device_id,
    const ApplicationHandle& app_handle,
    const CommunicationError& error) {
  ConnectionFinished(device_id, app_handle);
  for (TransportAdapterListenerList::iterator it = listeners_.begin();
       it != listeners_.end();
       ++it) {
    (*it)->OnUnexpectedDisconnect(this, device_id, app_handle, error);
  }
}

bool TransportAdapterImpl::IsInitialised() const {
  SDL_TRACE("enter");
  if (!initialised_) {
    SDL_TRACE("exit with FALSE. Condition: !initialised_");
    return false;
  }
  if (device_scanner_ && !device_scanner_->IsInitialised()) {
    SDL_TRACE(
        "exit with FALSE. Condition: device_scanner_ && "
        "!device_scanner_->IsInitialised()");
    return false;
  }
  if (server_connection_factory_ &&
      !server_connection_factory_->IsInitialised()) {
    SDL_TRACE(
        "exit with FALSE. Condition: server_connection_factory_ && "
        "!server_connection_factory_->IsInitialised()");
    return false;
  }
  if (client_connection_listener_ &&
      !client_connection_listener_->IsInitialised()) {
    SDL_TRACE(
        "exit with FALSE. Condition: client_connection_listener_ && "
        "!client_connection_listener_->IsInitialised()");
    return false;
  }
  SDL_TRACE("exit with TRUE");
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

std::string TransportAdapterImpl::GetConnectionType() const {
  return devices_type[GetDeviceType()];
}

#ifdef TELEMETRY_MONITOR
void TransportAdapterImpl::SetTelemetryObserver(TMTelemetryObserver* observer) {
  metric_observer_ = observer;
}
#endif  // TELEMETRY_MONITOR

#ifdef TELEMETRY_MONITOR
TMTelemetryObserver* TransportAdapterImpl::GetTelemetryObserver() {
  return metric_observer_;
}
#endif  // TELEMETRY_MONITOR

void TransportAdapterImpl::Store() const {}

bool TransportAdapterImpl::Restore() {
  return true;
}

bool TransportAdapterImpl::ToBeAutoConnected(DeviceSptr device) const {
  return false;
}

bool TransportAdapterImpl::ToBeAutoDisconnected(DeviceSptr device) const {
  return true;
}

ConnectionSPtr TransportAdapterImpl::FindEstablishedConnection(
    const DeviceUID& device_id, const ApplicationHandle& app_handle) const {
  SDL_TRACE("enter. device_id: " << &device_id
                                 << ", app_handle: " << &app_handle);
  ConnectionSPtr connection;
  {
    sync_primitives::AutoReadLock lock(connections_lock_);
    ConnectionMap::const_iterator it =
        connections_.find(std::make_pair(device_id, app_handle));
    if (it != connections_.end()) {
      const ConnectionInfo& info = it->second;
      if (info.state == ConnectionInfo::ESTABLISHED) {
        connection = info.connection;
      }
    }
  }
  SDL_TRACE("exit with Connection: " << connection);
  return connection;
}

TransportAdapter::Error TransportAdapterImpl::ConnectDevice(DeviceSptr device) {
  SDL_TRACE("enter. device: " << device);
  DeviceUID device_id = device->unique_device_id();
  ApplicationList app_list = device->GetApplicationList();
  SDL_INFO("Device " << device->name() << " has " << app_list.size()
                     << " applications.");
  bool errors_occurred = false;
  for (ApplicationList::iterator it = app_list.begin(); it != app_list.end();
       ++it) {
    const ApplicationHandle app_handle = *it;
    SDL_DEBUG("Attempt to connect device " << device_id << ", channel "
                                           << app_handle);
    const Error error = Connect(device_id, app_handle);
    switch (error) {
      case OK:
        SDL_DEBUG("error = OK");
        break;
      case ALREADY_EXISTS:
        SDL_DEBUG("error = ALREADY_EXISTS");
        break;
      default:
        SDL_ERROR("Connect to device " << device_id << ", channel "
                                       << app_handle << " failed with error "
                                       << error);
        errors_occurred = true;
        SDL_DEBUG("switch (error), default case");
        break;
    }
  }
  if (errors_occurred) {
    SDL_TRACE("exit with error:FAIL");
    return FAIL;
  } else {
    SDL_TRACE("exit with error:OK");
    return OK;
  }
}

void TransportAdapterImpl::RemoveDevice(const DeviceUID& device_handle) {
  SDL_AUTO_TRACE();
  SDL_DEBUG("Device_handle: " << &device_handle);
  sync_primitives::AutoLock locker(devices_mutex_);
  DeviceMap::iterator i = devices_.find(device_handle);
  if (i != devices_.end()) {
    DeviceSptr device = i->second;
    if (!device->keep_on_disconnect()) {
      devices_.erase(i);
      for (TransportAdapterListenerList::iterator it = listeners_.begin();
           it != listeners_.end();
           ++it) {
        TransportAdapterListener* listener = *it;
        listener->OnDeviceListUpdated(this);
      }
    }
  }
}

}  // namespace transport_adapter
}  // namespace transport_manager
