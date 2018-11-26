/*
 * Copyright (c) 2017, Ford Motor Company
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
#include "utils/helpers.h"
#include "utils/logger.h"
#include "utils/timer_task_impl.h"

#include "transport_manager/transport_adapter/transport_adapter_impl.h"
#include "transport_manager/transport_adapter/transport_adapter_listener.h"
#include "transport_manager/transport_adapter/device_scanner.h"
#include "transport_manager/transport_adapter/server_connection_factory.h"
#include "transport_manager/transport_adapter/client_connection_listener.h"

namespace transport_manager {
namespace transport_adapter {

const char* tc_enabled = "enabled";
const char* tc_tcp_port = "tcp_port";
const char* tc_tcp_ip_address = "tcp_ip_address";

CREATE_LOGGERPTR_GLOBAL(logger_, "TransportManager")
namespace {
DeviceTypes devicesType = {
    std::make_pair(DeviceType::AOA, std::string("USB_AOA")),
    std::make_pair(DeviceType::BLUETOOTH, std::string("BLUETOOTH")),
    std::make_pair(DeviceType::IOS_BT, std::string("BLUETOOTH_IOS")),
    std::make_pair(DeviceType::IOS_USB, std::string("USB_IOS")),
    std::make_pair(DeviceType::TCP, std::string("WIFI")),
    std::make_pair(DeviceType::IOS_USB_HOST_MODE,
                   std::string("USB_IOS_HOST_MODE")),
    std::make_pair(DeviceType::IOS_USB_DEVICE_MODE,
                   std::string("USB_IOS_DEVICE_MODE")),
    std::make_pair(DeviceType::IOS_CARPLAY_WIRELESS,
                   std::string("CARPLAY_WIRELESS_IOS")),
    std::make_pair(DeviceType::CLOUD_WEBSOCKET,
                   std::string("CLOUD_WEBSOCKET"))};
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
  listeners_.clear();
  Terminate();

  if (device_scanner_) {
    LOG4CXX_DEBUG(logger_, "Deleting device_scanner_ " << device_scanner_);
    delete device_scanner_;
    LOG4CXX_DEBUG(logger_, "device_scanner_ deleted.");
  }
  if (server_connection_factory_) {
    LOG4CXX_DEBUG(logger_,
                  "Deleting server_connection_factory "
                      << server_connection_factory_);
    delete server_connection_factory_;
    LOG4CXX_DEBUG(logger_, "server_connection_factory deleted.");
  }
  if (client_connection_listener_) {
    LOG4CXX_DEBUG(logger_,
                  "Deleting client_connection_listener_ "
                      << client_connection_listener_);
    delete client_connection_listener_;
    LOG4CXX_DEBUG(logger_, "client_connection_listener_ deleted.");
  }
}

void TransportAdapterImpl::Terminate() {
  if (device_scanner_) {
    device_scanner_->Terminate();
    LOG4CXX_DEBUG(logger_,
                  "device_scanner_ " << device_scanner_ << " terminated.");
  }
  if (server_connection_factory_) {
    server_connection_factory_->Terminate();
    LOG4CXX_DEBUG(logger_,
                  "server_connection_factory " << server_connection_factory_
                                               << " terminated.");
  }
  if (client_connection_listener_) {
    client_connection_listener_->Terminate();
    LOG4CXX_DEBUG(logger_,
                  "client_connection_listener_ " << client_connection_listener_
                                                 << " terminated.");
  }

  ConnectionMap connections;
  connections_lock_.AcquireForWriting();
  std::swap(connections, connections_);
  connections_lock_.Release();
  connections.clear();

  LOG4CXX_DEBUG(logger_, "Connections deleted");

  DeviceMap devices;
  devices_mutex_.Acquire();
  std::swap(devices, devices_);
  devices_mutex_.Release();
  devices.clear();

  LOG4CXX_DEBUG(logger_, "Devices deleted");
}

TransportAdapter::Error TransportAdapterImpl::Init() {
  LOG4CXX_TRACE(logger_, "enter");

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

  if (get_settings().use_last_state() && initialised_) {
    if (!Restore()) {
      LOG4CXX_WARN(logger_, "could not restore transport adapter state");
    }
  }
  LOG4CXX_TRACE(logger_, "exit with error: " << error);
  return error;
}

TransportAdapter::Error TransportAdapterImpl::SearchDevices() {
  LOG4CXX_TRACE(logger_, "enter");
  if (device_scanner_ == NULL) {
    LOG4CXX_TRACE(logger_, "exit with NOT_SUPPORTED");
    return NOT_SUPPORTED;
  } else if (!device_scanner_->IsInitialised()) {
    LOG4CXX_TRACE(logger_, "exit with BAD_STATE");
    return BAD_STATE;
  }
  TransportAdapter::Error er = device_scanner_->Scan();
  LOG4CXX_TRACE(logger_, "exit with error: " << er);
  return er;
}

TransportAdapter::Error TransportAdapterImpl::Connect(
    const DeviceUID& device_id, const ApplicationHandle& app_handle) {
  LOG4CXX_TRACE(logger_,
                "enter. DeviceUID " << device_id << " ApplicationHandle "
                                    << app_handle);
  if (server_connection_factory_ == 0) {
    LOG4CXX_TRACE(logger_, "exit with NOT_SUPPORTED");
    return NOT_SUPPORTED;
  }
  if (!server_connection_factory_->IsInitialised()) {
    LOG4CXX_TRACE(logger_, "exit with BAD_STATE");
    return BAD_STATE;
  }
  if (!initialised_) {
    LOG4CXX_TRACE(logger_, "exit with BAD_STATE");
    return BAD_STATE;
  }

  connections_lock_.AcquireForWriting();

  std::pair<DeviceUID, ApplicationHandle> connection_key;
  const bool already_exists =
      connections_.end() != connections_.find(connection_key);
  ConnectionInfo& info = connections_[connection_key];
  if (!already_exists) {
    info.app_handle = app_handle;
    info.device_id = device_id;
    info.state = ConnectionInfo::NEW;
  }
  const bool pending_app = ConnectionInfo::PENDING == info.state;
  connections_lock_.Release();

  if (already_exists && !pending_app) {
    LOG4CXX_TRACE(logger_, "exit with ALREADY_EXISTS");
    return ALREADY_EXISTS;
  }

  const TransportAdapter::Error err =
      server_connection_factory_->CreateConnection(device_id, app_handle);
  if (TransportAdapter::OK != err) {
    connections_lock_.AcquireForWriting();
    connections_.erase(std::make_pair(device_id, app_handle));
    connections_lock_.Release();
  }
  LOG4CXX_TRACE(logger_, "exit with error: " << err);
  return err;
}

TransportAdapter::Error TransportAdapterImpl::ConnectDevice(
    const DeviceUID& device_handle) {
  LOG4CXX_TRACE(logger_, "enter with device_handle: " << &device_handle);
  DeviceSptr device = FindDevice(device_handle);
  if (device) {
    TransportAdapter::Error err = ConnectDevice(device);
    if (FAIL == err && GetDeviceType() == DeviceType::CLOUD_WEBSOCKET) {
      LOG4CXX_TRACE(logger_,
                    "Error occurred while connecting cloud app: " << err);
      if (device->connection_status() == ConnectionStatus::PENDING) {
        device->set_connection_status(ConnectionStatus::RETRY);
        device->reset_retry_count();
      }

      if (device->retry_count() >
          get_settings().cloud_app_max_retry_attempts()) {
        device->set_connection_status(ConnectionStatus::PENDING);
        device->reset_retry_count();
        return err;
      }

      device->next_retry();
      TimerSPtr retry_timer(std::make_shared<timer::Timer>(
          "RetryConnectionTimer",
          new timer::TimerTaskImpl<TransportAdapterImpl>(
              this, &TransportAdapterImpl::RetryConnection)));
      sync_primitives::AutoLock locker(retry_timer_pool_lock_);
      retry_timer_pool_.push(std::make_pair(retry_timer, device_handle));
      retry_timer->Start(get_settings().cloud_app_retry_timeout(),
                         timer::kSingleShot);
    } else if (OK == err) {
      device->set_connection_status(ConnectionStatus::CONNECTED);
    }
    LOG4CXX_TRACE(logger_, "exit with error: " << err);
    return err;
  } else {
    LOG4CXX_TRACE(logger_, "exit with BAD_PARAM");
    return BAD_PARAM;
  }
}

void TransportAdapterImpl::RetryConnection() {
  const DeviceUID device_id = GetNextRetryDevice();
  if (device_id.empty()) {
    LOG4CXX_ERROR(logger_,
                  "Unable to find timer, ignoring RetryConnection request");
    return;
  }
  ConnectDevice(device_id);
}

DeviceUID TransportAdapterImpl::GetNextRetryDevice() {
  sync_primitives::AutoLock locker(retry_timer_pool_lock_);
  while (!retry_timer_pool_.empty()) {
    auto timer_entry = retry_timer_pool_.front();
    if (timer_entry.first->is_running()) {
      return timer_entry.second;
    }
    retry_timer_pool_.pop();
  }
  return std::string();
}

ConnectionStatus TransportAdapterImpl::GetConnectionStatus(
    const DeviceUID& device_handle) const {
  DeviceSptr device = FindDevice(device_handle);
  return device.use_count() == 0 ? ConnectionStatus::INVALID
                                 : device->connection_status();
}

TransportAdapter::Error TransportAdapterImpl::Disconnect(
    const DeviceUID& device_id, const ApplicationHandle& app_handle) {
  LOG4CXX_TRACE(logger_,
                "enter. device_id: " << &device_id
                                     << ", device_id: " << &device_id);
  if (!initialised_) {
    LOG4CXX_TRACE(logger_, "exit with BAD_STATE");
    return BAD_STATE;
  }
  ConnectionSPtr connection = FindEstablishedConnection(device_id, app_handle);
  if (connection) {
    TransportAdapter::Error err = connection->Disconnect();
    LOG4CXX_TRACE(logger_, "exit with error: " << err);
    return err;
  } else {
    LOG4CXX_TRACE(logger_, "exit with BAD_PARAM");
    return BAD_PARAM;
  }
}

TransportAdapter::Error TransportAdapterImpl::DisconnectDevice(
    const DeviceUID& device_id) {
  LOG4CXX_TRACE(logger_, "enter. device_id: " << &device_id);
  if (!initialised_) {
    LOG4CXX_TRACE(logger_, "exit with BAD_STATE");
    return BAD_STATE;
  }

  Error error = OK;
  DeviceSptr device = FindDevice(device_id);
  device->set_connection_status(ConnectionStatus::CLOSING);

  std::vector<ConnectionInfo> to_disconnect;
  connections_lock_.AcquireForReading();
  for (ConnectionMap::const_iterator i = connections_.begin();
       i != connections_.end();
       ++i) {
    ConnectionInfo info = i->second;
    if (info.device_id == device_id &&
        info.state != ConnectionInfo::FINALISING) {
      to_disconnect.push_back(info);
    }
  }
  connections_lock_.Release();

  for (std::vector<ConnectionInfo>::const_iterator j = to_disconnect.begin();
       j != to_disconnect.end();
       ++j) {
    ConnectionInfo info = *j;
    if (OK != info.connection->Disconnect()) {
      error = FAIL;
      LOG4CXX_ERROR(logger_, "Error on disconnect " << error);
    }
  }

  return error;
}

TransportAdapter::Error TransportAdapterImpl::SendData(
    const DeviceUID& device_id,
    const ApplicationHandle& app_handle,
    const ::protocol_handler::RawMessagePtr data) {
  LOG4CXX_TRACE(logger_,
                "enter. device_id: " << &device_id << ", app_handle: "
                                     << &app_handle << ", data: " << data);
  if (!initialised_) {
    LOG4CXX_TRACE(logger_, "exit with BAD_STATE");
    return BAD_STATE;
  }

  ConnectionSPtr connection = FindEstablishedConnection(device_id, app_handle);
  if (connection) {
    TransportAdapter::Error err = connection->SendData(data);
    LOG4CXX_TRACE(logger_, "exit with error: " << err);
    return err;
  } else {
    LOG4CXX_TRACE(logger_, "exit with BAD_PARAM");
    return BAD_PARAM;
  }
}

TransportAdapter::Error TransportAdapterImpl::StartClientListening() {
  LOG4CXX_TRACE(logger_, "enter");
  if (client_connection_listener_ == 0) {
    LOG4CXX_TRACE(logger_, "exit with NOT_SUPPORTED");
    return NOT_SUPPORTED;
  }
  if (!client_connection_listener_->IsInitialised()) {
    LOG4CXX_TRACE(logger_, "exit with BAD_STATE");
    return BAD_STATE;
  }
  TransportAdapter::Error err = client_connection_listener_->StartListening();
  LOG4CXX_TRACE(logger_, "exit with error: " << err);
  return err;
}

TransportAdapter::Error TransportAdapterImpl::StopClientListening() {
  LOG4CXX_TRACE(logger_, "enter");
  if (client_connection_listener_ == 0) {
    LOG4CXX_TRACE(logger_, "exit with NOT_SUPPORTED");
    return NOT_SUPPORTED;
  }
  if (!client_connection_listener_->IsInitialised()) {
    LOG4CXX_TRACE(logger_, "exit with BAD_STATE");
    return BAD_STATE;
  }
  TransportAdapter::Error err = client_connection_listener_->StopListening();
  sync_primitives::AutoLock locker(devices_mutex_);
  for (DeviceMap::iterator it = devices_.begin(); it != devices_.end(); ++it) {
    it->second->Stop();
  }
  LOG4CXX_TRACE(logger_, "exit with error: " << err);
  return err;
}

DeviceList TransportAdapterImpl::GetDeviceList() const {
  LOG4CXX_AUTO_TRACE(logger_);
  DeviceList devices;
  sync_primitives::AutoLock locker(devices_mutex_);
  for (DeviceMap::const_iterator it = devices_.begin(); it != devices_.end();
       ++it) {
    devices.push_back(it->first);
  }
  LOG4CXX_TRACE(logger_,
                "exit with DeviceList. It's' size = " << devices.size());
  return devices;
}

DeviceSptr TransportAdapterImpl::AddDevice(DeviceSptr device) {
  LOG4CXX_TRACE(logger_, "enter. device: " << device);
  DeviceSptr existing_device;
  bool same_device_found = false;
  devices_mutex_.Acquire();
  for (DeviceMap::const_iterator i = devices_.begin(); i != devices_.end();
       ++i) {
    existing_device = i->second;
    if (device->IsSameAs(existing_device.get())) {
      same_device_found = true;
      LOG4CXX_DEBUG(logger_, "device " << device << "already exists");
      break;
    }
  }
  if (!same_device_found) {
    devices_[device->unique_device_id()] = device;
  }
  devices_mutex_.Release();
  if (same_device_found) {
    LOG4CXX_TRACE(logger_, "exit with TRUE. Condition: same_device_found");
    return existing_device;
  } else {
    device->set_connection_status(ConnectionStatus::PENDING);
    for (TransportAdapterListenerList::iterator it = listeners_.begin();
         it != listeners_.end();
         ++it) {
      (*it)->OnDeviceListUpdated(this);
    }
    if (ToBeAutoConnected(device)) {
      ConnectDevice(device);
    }
    LOG4CXX_TRACE(logger_, "exit with DeviceSptr " << device);
    return device;
  }
}

void TransportAdapterImpl::SearchDeviceDone(const DeviceVector& devices) {
  LOG4CXX_TRACE(logger_, "enter. devices: " << &devices);
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
        LOG4CXX_DEBUG(logger_, "device found. DeviceSptr" << it->second);
        break;
      }
    }
    devices_mutex_.Release();

    if (!device_found) {
      LOG4CXX_INFO(logger_,
                   "Adding new device " << device->unique_device_id() << " (\""
                                        << device->name() << "\")");
    }

    device->set_keep_on_disconnect(true);
    new_devices[device->unique_device_id()] = device;
  }

  connections_lock_.AcquireForReading();
  std::set<DeviceUID> connected_devices;
  for (ConnectionMap::const_iterator it = connections_.begin();
       it != connections_.end();
       ++it) {
    const ConnectionInfo& info = it->second;
    if (info.state != ConnectionInfo::FINALISING) {
      connected_devices.insert(info.device_id);
    }
  }
  connections_lock_.Release();

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
  LOG4CXX_TRACE(logger_, "exit");
}

void TransportAdapterImpl::ApplicationListUpdated(
    const DeviceUID& device_handle) {
  // default implementation does nothing
  // and is reimplemented in MME transport adapter only
}

void TransportAdapterImpl::FindNewApplicationsRequest() {
  LOG4CXX_TRACE(logger_, "enter");
  for (TransportAdapterListenerList::iterator i = listeners_.begin();
       i != listeners_.end();
       ++i) {
    TransportAdapterListener* listener = *i;
    listener->OnFindNewApplicationsRequest(this);
  }
  LOG4CXX_TRACE(logger_, "exit");
}

void TransportAdapterImpl::SearchDeviceFailed(const SearchDeviceError& error) {
  LOG4CXX_TRACE(logger_, "enter");
  for (TransportAdapterListenerList::iterator it = listeners_.begin();
       it != listeners_.end();
       ++it) {
    (*it)->OnSearchDeviceFailed(this, error);
  }
  LOG4CXX_TRACE(logger_, "exit");
}

bool TransportAdapterImpl::IsSearchDevicesSupported() const {
  LOG4CXX_AUTO_TRACE(logger_);
  return device_scanner_ != 0;
}

bool TransportAdapterImpl::IsServerOriginatedConnectSupported() const {
  LOG4CXX_AUTO_TRACE(logger_);
  return server_connection_factory_ != 0;
}

bool TransportAdapterImpl::IsClientOriginatedConnectSupported() const {
  LOG4CXX_AUTO_TRACE(logger_);
  return client_connection_listener_ != 0;
}

void TransportAdapterImpl::ConnectionCreated(
    ConnectionSPtr connection,
    const DeviceUID& device_id,
    const ApplicationHandle& app_handle) {
  LOG4CXX_TRACE(logger_,
                "enter connection:" << connection
                                    << ", device_id: " << &device_id
                                    << ", app_handle: " << &app_handle);
  connections_lock_.AcquireForReading();
  ConnectionInfo& info = connections_[std::make_pair(device_id, app_handle)];
  info.app_handle = app_handle;
  info.device_id = device_id;
  info.connection = connection;
  info.state = ConnectionInfo::NEW;
  connections_lock_.Release();
}

void TransportAdapterImpl::DeviceDisconnected(
    const DeviceUID& device_handle, const DisconnectDeviceError& error) {
  const DeviceUID device_uid = device_handle;
  LOG4CXX_TRACE(logger_,
                "enter. device_handle: " << &device_uid
                                         << ", error: " << &error);
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

  connections_lock_.AcquireForWriting();
  for (ApplicationList::const_iterator i = app_list.begin();
       i != app_list.end();
       ++i) {
    ApplicationHandle app_handle = *i;
    connections_.erase(std::make_pair(device_uid, app_handle));
  }
  connections_lock_.Release();

  RemoveDevice(device_uid);
  LOG4CXX_TRACE(logger_, "exit");
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
      LOG4CXX_DEBUG(logger_,
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
  LOG4CXX_TRACE(logger_,
                "enter. device_id: " << &device_uid
                                     << ", app_handle: " << &app_uid);
  DeviceSptr device = FindDevice(device_handle);
  if (!device) {
    LOG4CXX_WARN(logger_, "Device: uid " << &device_uid << " not found");
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
  connections_lock_.AcquireForWriting();
  connections_.erase(std::make_pair(device_uid, app_uid));
  connections_lock_.Release();

  if (device_disconnected) {
    RemoveDevice(device_uid);
  }

  Store();
  LOG4CXX_TRACE(logger_, "exit");
}

void TransportAdapterImpl::DataReceiveDone(
    const DeviceUID& device_id,
    const ApplicationHandle& app_handle,
    ::protocol_handler::RawMessagePtr message) {
  LOG4CXX_TRACE(logger_,
                "enter. device_id: " << &device_id
                                     << ", app_handle: " << &app_handle
                                     << ", message: " << message);

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
  LOG4CXX_TRACE(logger_, "exit");
}

void TransportAdapterImpl::DataReceiveFailed(
    const DeviceUID& device_id,
    const ApplicationHandle& app_handle,
    const DataReceiveError& error) {
  LOG4CXX_TRACE(logger_, "enter");
  for (TransportAdapterListenerList::iterator it = listeners_.begin();
       it != listeners_.end();
       ++it) {
    (*it)->OnDataReceiveFailed(this, device_id, app_handle, error);
  }
  LOG4CXX_TRACE(logger_, "exit");
}

void TransportAdapterImpl::DataSendDone(
    const DeviceUID& device_id,
    const ApplicationHandle& app_handle,
    ::protocol_handler::RawMessagePtr message) {
  LOG4CXX_TRACE(logger_, "enter");
  for (TransportAdapterListenerList::iterator it = listeners_.begin();
       it != listeners_.end();
       ++it) {
    (*it)->OnDataSendDone(this, device_id, app_handle, message);
  }
  LOG4CXX_TRACE(logger_, "exit");
}

void TransportAdapterImpl::DataSendFailed(
    const DeviceUID& device_id,
    const ApplicationHandle& app_handle,
    ::protocol_handler::RawMessagePtr message,
    const DataSendError& error) {
  LOG4CXX_TRACE(logger_, "enter");
  for (TransportAdapterListenerList::iterator it = listeners_.begin();
       it != listeners_.end();
       ++it) {
    (*it)->OnDataSendFailed(this, device_id, app_handle, message, error);
  }
  LOG4CXX_TRACE(logger_, "exit");
}

void TransportAdapterImpl::TransportConfigUpdated(
    const TransportConfig& new_config) {
  LOG4CXX_AUTO_TRACE(logger_);
  for (TransportAdapterListenerList::iterator it = listeners_.begin();
       it != listeners_.end();
       ++it) {
    (*it)->OnTransportConfigUpdated(this);
  }
}

void TransportAdapterImpl::DoTransportSwitch() const {
  LOG4CXX_AUTO_TRACE(logger_);
  std::for_each(
      listeners_.begin(),
      listeners_.end(),
      std::bind2nd(
          std::mem_fun(&TransportAdapterListener::OnTransportSwitchRequested),
          this));
}

void TransportAdapterImpl::DeviceSwitched(const DeviceUID& device_handle) {
  LOG4CXX_DEBUG(logger_,
                "Switching is not implemented for that adapter type "
                    << GetConnectionType().c_str());
  UNUSED(device_handle);
}

DeviceSptr TransportAdapterImpl::FindDevice(const DeviceUID& device_id) const {
  LOG4CXX_TRACE(logger_, "enter. device_id: " << &device_id);
  DeviceSptr ret;
  sync_primitives::AutoLock locker(devices_mutex_);
  LOG4CXX_DEBUG(logger_, "devices_.size() = " << devices_.size());
  DeviceMap::const_iterator it = devices_.find(device_id);
  if (it != devices_.end()) {
    ret = it->second;
  } else {
    LOG4CXX_WARN(logger_, "Device " << device_id << " not found.");
  }
  LOG4CXX_TRACE(logger_, "exit with DeviceSptr: " << ret);
  return ret;
}

void TransportAdapterImpl::ConnectPending(const DeviceUID& device_id,
                                          const ApplicationHandle& app_handle) {
  connections_lock_.AcquireForReading();
  ConnectionMap::iterator it_conn =
      connections_.find(std::make_pair(device_id, app_handle));
  if (it_conn != connections_.end()) {
    ConnectionInfo& info = it_conn->second;
    info.state = ConnectionInfo::PENDING;
  }
  connections_lock_.Release();

  for (TransportAdapterListenerList::iterator it = listeners_.begin();
       it != listeners_.end();
       ++it) {
    (*it)->OnConnectPending(this, device_id, app_handle);
  }
}

void TransportAdapterImpl::ConnectDone(const DeviceUID& device_id,
                                       const ApplicationHandle& app_handle) {
  LOG4CXX_TRACE(logger_,
                "enter. device_id: " << &device_id
                                     << ", app_handle: " << &app_handle);
  connections_lock_.AcquireForReading();
  ConnectionMap::iterator it_conn =
      connections_.find(std::make_pair(device_id, app_handle));
  if (it_conn != connections_.end()) {
    ConnectionInfo& info = it_conn->second;
    info.state = ConnectionInfo::ESTABLISHED;
  }
  connections_lock_.Release();

  for (TransportAdapterListenerList::iterator it = listeners_.begin();
       it != listeners_.end();
       ++it) {
    (*it)->OnConnectDone(this, device_id, app_handle);
  }

  Store();
  LOG4CXX_TRACE(logger_, "exit");
}

void TransportAdapterImpl::ConnectFailed(const DeviceUID& device_handle,
                                         const ApplicationHandle& app_handle,
                                         const ConnectError& error) {
  const DeviceUID device_uid = device_handle;
  const ApplicationHandle app_uid = app_handle;
  LOG4CXX_TRACE(logger_,
                "enter. device_id: " << &device_uid << ", app_handle: "
                                     << &app_uid << ", error: " << &error);
  connections_lock_.AcquireForWriting();
  connections_.erase(std::make_pair(device_uid, app_uid));
  connections_lock_.Release();
  for (TransportAdapterListenerList::iterator it = listeners_.begin();
       it != listeners_.end();
       ++it) {
    (*it)->OnConnectFailed(this, device_uid, app_uid, error);
  }
  LOG4CXX_TRACE(logger_, "exit");
}

void TransportAdapterImpl::RemoveFinalizedConnection(
    const DeviceUID& device_handle, const ApplicationHandle& app_handle) {
  const DeviceUID device_uid = device_handle;
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoWriteLock lock(connections_lock_);
  ConnectionMap::iterator it_conn =
      connections_.find(std::make_pair(device_uid, app_handle));
  if (it_conn == connections_.end()) {
    LOG4CXX_WARN(logger_,
                 "Device_id: " << &device_uid << ", app_handle: " << &app_handle
                               << " connection not found");
    return;
  }
  const ConnectionInfo& info = it_conn->second;
  if (info.state != ConnectionInfo::FINALISING) {
    LOG4CXX_WARN(logger_,
                 "Device_id: " << &device_uid << ", app_handle: " << &app_handle
                               << " connection not finalized");
    return;
  }
  connections_.erase(it_conn);
}

void TransportAdapterImpl::AddListener(TransportAdapterListener* listener) {
  LOG4CXX_TRACE(logger_, "enter");
  listeners_.push_back(listener);
  LOG4CXX_TRACE(logger_, "exit");
}

ApplicationList TransportAdapterImpl::GetApplicationList(
    const DeviceUID& device_id) const {
  LOG4CXX_TRACE(logger_, "enter. device_id: " << &device_id);
  DeviceSptr device = FindDevice(device_id);
  if (device.use_count() != 0) {
    ApplicationList lst = device->GetApplicationList();
    LOG4CXX_TRACE(logger_,
                  "exit with ApplicationList. It's size = "
                      << lst.size() << " Condition: device.use_count() != 0");
    return lst;
  }
  LOG4CXX_TRACE(logger_,
                "exit with empty ApplicationList. Condition: NOT "
                "device.use_count() != 0");
  return ApplicationList();
}

void TransportAdapterImpl::ConnectionFinished(
    const DeviceUID& device_id, const ApplicationHandle& app_handle) {
  LOG4CXX_TRACE(logger_,
                "enter. device_id: " << &device_id
                                     << ", app_handle: " << &app_handle);
  connections_lock_.AcquireForReading();
  ConnectionMap::iterator it =
      connections_.find(std::make_pair(device_id, app_handle));
  if (it != connections_.end()) {
    ConnectionInfo& info = it->second;
    info.state = ConnectionInfo::FINALISING;
  }
  connections_lock_.Release();
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
  LOG4CXX_TRACE(logger_, "enter");
  if (!initialised_) {
    LOG4CXX_TRACE(logger_, "exit with FALSE. Condition: !initialised_");
    return false;
  }
  if (device_scanner_ && !device_scanner_->IsInitialised()) {
    LOG4CXX_TRACE(logger_,
                  "exit with FALSE. Condition: device_scanner_ && "
                  "!device_scanner_->IsInitialised()");
    return false;
  }
  if (server_connection_factory_ &&
      !server_connection_factory_->IsInitialised()) {
    LOG4CXX_TRACE(logger_,
                  "exit with FALSE. Condition: server_connection_factory_ && "
                  "!server_connection_factory_->IsInitialised()");
    return false;
  }
  if (client_connection_listener_ &&
      !client_connection_listener_->IsInitialised()) {
    LOG4CXX_TRACE(logger_,
                  "exit with FALSE. Condition: client_connection_listener_ && "
                  "!client_connection_listener_->IsInitialised()");
    return false;
  }
  LOG4CXX_TRACE(logger_, "exit with TRUE");
  return true;
}

std::string TransportAdapterImpl::DeviceName(const DeviceUID& device_id) const {
  DeviceSptr device = FindDevice(device_id);
  if (device.use_count() != 0) {
    return device->name();
  } else {
    return "";
  }
}

void TransportAdapterImpl::StopDevice(const DeviceUID& device_id) const {
  LOG4CXX_AUTO_TRACE(logger_);
  DeviceSptr device = FindDevice(device_id);
  if (device) {
    device->Stop();
  }
}

std::string TransportAdapterImpl::GetConnectionType() const {
  return devicesType[GetDeviceType()];
}

SwitchableDevices TransportAdapterImpl::GetSwitchableDevices() const {
  LOG4CXX_AUTO_TRACE(logger_);
  SwitchableDevices devices;
  sync_primitives::AutoLock locker(devices_mutex_);
  for (DeviceMap::const_iterator it = devices_.begin(); it != devices_.end();
       ++it) {
    const auto device_uid = it->first;
    const auto device = it->second;
    const auto transport_switch_id = device->transport_switch_id();
    if (transport_switch_id.empty()) {
      LOG4CXX_DEBUG(logger_,
                    "Device is not suitable for switching: " << device_uid);
      continue;
    }
    LOG4CXX_DEBUG(logger_, "Device is suitable for switching: " << device_uid);
    devices.insert(std::make_pair(device_uid, transport_switch_id));
  }
  LOG4CXX_INFO(logger_,
               "Found number of switchable devices: " << devices.size());
  return devices;
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
  LOG4CXX_TRACE(logger_,
                "enter. device_id: " << &device_id
                                     << ", app_handle: " << &app_handle);
  ConnectionSPtr connection;
  connections_lock_.AcquireForReading();
  ConnectionMap::const_iterator it =
      connections_.find(std::make_pair(device_id, app_handle));
  if (it != connections_.end()) {
    const ConnectionInfo& info = it->second;
    if (info.state == ConnectionInfo::ESTABLISHED) {
      connection = info.connection;
    }
  }
  connections_lock_.Release();
  LOG4CXX_TRACE(logger_, "exit with Connection: " << connection);
  return connection;
}

TransportAdapter::Error TransportAdapterImpl::ConnectDevice(DeviceSptr device) {
  LOG4CXX_TRACE(logger_, "enter. device: " << device);
  DeviceUID device_id = device->unique_device_id();
  ApplicationList app_list = device->GetApplicationList();
  LOG4CXX_INFO(logger_,
               "Device " << device->name() << " has " << app_list.size()
                         << " applications.");
  bool errors_occurred = false;
  for (ApplicationList::iterator it = app_list.begin(); it != app_list.end();
       ++it) {
    const ApplicationHandle app_handle = *it;
    LOG4CXX_DEBUG(logger_,
                  "Attempt to connect device " << device_id << ", channel "
                                               << app_handle);
    const Error error = Connect(device_id, app_handle);
    switch (error) {
      case OK:
        LOG4CXX_DEBUG(logger_, "error = OK");
        break;
      case ALREADY_EXISTS:
        LOG4CXX_DEBUG(logger_, "error = ALREADY_EXISTS");
        break;
      default:
        LOG4CXX_ERROR(logger_,
                      "Connect to device " << device_id << ", channel "
                                           << app_handle
                                           << " failed with error " << error);
        errors_occurred = true;
        LOG4CXX_DEBUG(logger_, "switch (error), default case");
        break;
    }
  }
  if (errors_occurred) {
    LOG4CXX_TRACE(logger_, "exit with error:FAIL");
    return FAIL;
  } else {
    LOG4CXX_TRACE(logger_, "exit with error:OK");
    return OK;
  }
}

void TransportAdapterImpl::RunAppOnDevice(const DeviceUID& device_uid,
                                          const std::string& bundle_id) {
  LOG4CXX_AUTO_TRACE(logger_);

  DeviceSptr device = FindDevice(device_uid);
  if (!device) {
    LOG4CXX_WARN(logger_,
                 "Device with id: " << device_uid << " Not found"
                                    << "withing list of connected deviced");
    return;
  }

  device->LaunchApp(bundle_id);
}

void TransportAdapterImpl::RemoveDevice(const DeviceUID& device_handle) {
  LOG4CXX_AUTO_TRACE(logger_);
  LOG4CXX_DEBUG(logger_, "Device_handle: " << &device_handle);
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
