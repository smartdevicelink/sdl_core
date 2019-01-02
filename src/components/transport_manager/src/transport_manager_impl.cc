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

#include "transport_manager/transport_manager_impl.h"

#include <stdint.h>
#include <cstring>
#include <queue>
#include <set>
#include <algorithm>
#include <limits>
#include <functional>
#include <sstream>
#include <iostream>

#include "utils/macro.h"
#include "utils/logger.h"

#include "utils/timer_task_impl.h"
#include "transport_manager/common.h"
#include "transport_manager/transport_manager_listener.h"
#include "transport_manager/transport_manager_listener_empty.h"
#include "transport_manager/transport_adapter/transport_adapter.h"
#include "transport_manager/cloud/cloud_websocket_transport_adapter.h"
#include "transport_manager/transport_adapter/transport_adapter_event.h"
#include "config_profile/profile.h"

using ::transport_manager::transport_adapter::TransportAdapter;

namespace {
struct ConnectionFinder {
  const uint32_t id_;
  explicit ConnectionFinder(const uint32_t id) : id_(id) {}
  bool operator()(const transport_manager::TransportManagerImpl::Connection&
                      connection) const {
    return id_ == connection.id;
  }
};
}  // namespace

namespace transport_manager {

CREATE_LOGGERPTR_GLOBAL(logger_, "TransportManager")

TransportManagerImpl::Connection TransportManagerImpl::convert(
    const TransportManagerImpl::ConnectionInternal& p) {
  LOG4CXX_TRACE(logger_, "enter. ConnectionInternal: " << &p);
  TransportManagerImpl::Connection c;
  c.application = p.application;
  c.device = p.device;
  c.id = p.id;
  LOG4CXX_TRACE(
      logger_,
      "exit with TransportManagerImpl::Connection. It's ConnectionUID = "
          << c.id);
  return c;
}

TransportManagerImpl::TransportManagerImpl(
    const TransportManagerSettings& settings)
    : is_initialized_(false)
#ifdef TELEMETRY_MONITOR
    , metric_observer_(NULL)
#endif  // TELEMETRY_MONITOR
    , connection_id_counter_(0)
    , message_queue_("TM MessageQueue", this)
    , event_queue_("TM EventQueue", this)
    , settings_(settings)
    , device_switch_timer_(
          "Device reconection timer",
          new timer::TimerTaskImpl<TransportManagerImpl>(
              this, &TransportManagerImpl::ReconnectionTimeout)) {
  LOG4CXX_TRACE(logger_, "TransportManager has created");
}

TransportManagerImpl::~TransportManagerImpl() {
  LOG4CXX_DEBUG(logger_, "TransportManager object destroying");
  message_queue_.Shutdown();
  event_queue_.Shutdown();

  for (std::vector<TransportAdapter*>::iterator it =
           transport_adapters_.begin();
       it != transport_adapters_.end();
       ++it) {
    delete *it;
  }

  for (std::map<TransportAdapter*, TransportAdapterListenerImpl*>::iterator it =
           transport_adapter_listeners_.begin();
       it != transport_adapter_listeners_.end();
       ++it) {
    delete it->second;
  }

  LOG4CXX_INFO(logger_, "TransportManager object destroyed");
}

void TransportManagerImpl::ReconnectionTimeout() {
  LOG4CXX_AUTO_TRACE(logger_);
  RaiseEvent(&TransportManagerListener::OnDeviceSwitchingFinish,
             device_to_reconnect_);
}

void TransportManagerImpl::AddCloudDevice(
    const transport_manager::transport_adapter::CloudAppProperties
        cloud_properties) {
  // todo put conversion into own function

  transport_adapter::DeviceType type = transport_adapter::DeviceType::UNKNOWN;
  if ((cloud_properties.cloud_transport_type == "WS") ||
      (cloud_properties.cloud_transport_type == "WSS")) {
    type = transport_adapter::DeviceType::CLOUD_WEBSOCKET;
  } else {
    return;
  }

  std::vector<TransportAdapter*>::iterator ta = transport_adapters_.begin();
  for (; ta != transport_adapters_.end(); ++ta) {
    if ((*ta)->GetDeviceType() == type) {
      (*ta)->CreateDevice(cloud_properties.endpoint);
      transport_adapter::CloudWebsocketTransportAdapter* cta =
          static_cast<transport_adapter::CloudWebsocketTransportAdapter*>(*ta);
      cta->SetAppCloudTransportConfig(cloud_properties.endpoint,
                                      cloud_properties);
    }
  }

  return;
}

void TransportManagerImpl::RemoveCloudDevice(const DeviceHandle device_handle) {
  DisconnectDevice(device_handle);
}

int TransportManagerImpl::ConnectDevice(const DeviceHandle device_handle) {
  LOG4CXX_TRACE(logger_, "enter. DeviceHandle: " << &device_handle);
  if (!this->is_initialized_) {
    LOG4CXX_ERROR(logger_, "TransportManager is not initialized.");
    LOG4CXX_TRACE(
        logger_,
        "exit with E_TM_IS_NOT_INITIALIZED. Condition: !this->is_initialized_");
    return E_TM_IS_NOT_INITIALIZED;
  }

  DeviceUID device_id = converter_.HandleToUid(device_handle);
  LOG4CXX_DEBUG(logger_, "Convert handle to id:" << device_id);

  sync_primitives::AutoReadLock lock(device_to_adapter_map_lock_);
  DeviceToAdapterMap::iterator it = device_to_adapter_map_.find(device_id);
  if (it == device_to_adapter_map_.end()) {
    LOG4CXX_ERROR(logger_, "No device adapter found by id " << device_id);
    LOG4CXX_TRACE(logger_, "exit with E_INVALID_HANDLE. Condition: NULL == ta");
    return E_INVALID_HANDLE;
  }
  transport_adapter::TransportAdapter* ta = it->second;

  TransportAdapter::Error ta_error = ta->ConnectDevice(device_id);
  int err = (TransportAdapter::OK == ta_error) ? E_SUCCESS : E_INTERNAL_ERROR;
  LOG4CXX_TRACE(logger_, "exit with error: " << err);
  return err;
}

ConnectionStatus TransportManagerImpl::GetConnectionStatus(
    const DeviceHandle& device_handle) const {
  DeviceUID device_id = converter_.HandleToUid(device_handle);

  sync_primitives::AutoReadLock lock(device_to_adapter_map_lock_);
  DeviceToAdapterMap::const_iterator it =
      device_to_adapter_map_.find(device_id);
  if (it == device_to_adapter_map_.end()) {
    LOG4CXX_ERROR(logger_, "No device adapter found by id " << device_handle);
    LOG4CXX_TRACE(logger_, "exit with E_INVALID_HANDLE. Condition: NULL == ta");
    return ConnectionStatus::INVALID;
  }
  transport_adapter::TransportAdapter* ta = it->second;
  return ta->GetConnectionStatus(device_id);
}

int TransportManagerImpl::DisconnectDevice(const DeviceHandle device_handle) {
  LOG4CXX_TRACE(logger_, "enter. DeviceHandle: " << &device_handle);
  if (!this->is_initialized_) {
    LOG4CXX_ERROR(logger_, "TransportManager is not initialized.");
    LOG4CXX_TRACE(
        logger_,
        "exit with E_TM_IS_NOT_INITIALIZED. Condition: !this->is_initialized_");
    return E_TM_IS_NOT_INITIALIZED;
  }
  DeviceUID device_id = converter_.HandleToUid(device_handle);
  LOG4CXX_DEBUG(logger_, "Convert handle to id:" << device_id);

  sync_primitives::AutoReadLock lock(device_to_adapter_map_lock_);
  DeviceToAdapterMap::iterator it = device_to_adapter_map_.find(device_id);
  if (it == device_to_adapter_map_.end()) {
    LOG4CXX_WARN(logger_, "No device adapter found by id " << device_id);
    LOG4CXX_TRACE(logger_, "exit with E_INVALID_HANDLE. Condition: NULL == ta");
    return E_INVALID_HANDLE;
  }
  transport_adapter::TransportAdapter* ta = it->second;
  ta->DisconnectDevice(device_id);
  LOG4CXX_TRACE(logger_, "exit with E_SUCCESS");
  return E_SUCCESS;
}

int TransportManagerImpl::Disconnect(const ConnectionUID cid) {
  LOG4CXX_TRACE(logger_, "enter. ConnectionUID: " << &cid);
  if (!this->is_initialized_) {
    LOG4CXX_ERROR(logger_, "TransportManager is not initialized.");
    LOG4CXX_TRACE(
        logger_,
        "exit with E_TM_IS_NOT_INITIALIZED. Condition: !this->is_initialized_");
    return E_TM_IS_NOT_INITIALIZED;
  }

  sync_primitives::AutoReadLock lock(connections_lock_);
  ConnectionInternal* connection = GetConnection(cid);
  if (NULL == connection) {
    LOG4CXX_ERROR(
        logger_,
        "TransportManagerImpl::Disconnect: Connection does not exist.");
    LOG4CXX_TRACE(logger_,
                  "exit with E_INVALID_HANDLE. Condition: NULL == connection");
    return E_INVALID_HANDLE;
  }

  connection->transport_adapter->Disconnect(connection->device,
                                            connection->application);
  // TODO(dchmerev@luxoft.com): Return disconnect timeout
  /*
  int messages_count = 0;
  for (EventQueue::const_iterator it = event_queue_.begin();
       it != event_queue_.end();
       ++it) {
    if (it->application_id == static_cast<ApplicationHandle>(cid)) {
      ++messages_count;
    }
  }

  if (messages_count > 0) {
    connection->messages_count = messages_count;
    connection->shutDown = true;

    const uint32_t disconnect_timeout =
      get_settings().transport_manager_disconnect_timeout();
    if (disconnect_timeout > 0) {
      connection->timer->start(disconnect_timeout);
    }
  } else {
    connection->transport_adapter->Disconnect(connection->device,
        connection->application);
  }
  */
  LOG4CXX_TRACE(logger_, "exit with E_SUCCESS");
  return E_SUCCESS;
}

int TransportManagerImpl::DisconnectForce(const ConnectionUID cid) {
  LOG4CXX_TRACE(logger_, "enter ConnectionUID: " << &cid);
  if (false == this->is_initialized_) {
    LOG4CXX_ERROR(logger_, "TransportManager is not initialized.");
    LOG4CXX_TRACE(logger_,
                  "exit with E_TM_IS_NOT_INITIALIZED. Condition: false == "
                  "this->is_initialized_");
    return E_TM_IS_NOT_INITIALIZED;
  }
  sync_primitives::AutoReadLock lock(connections_lock_);
  const ConnectionInternal* connection = GetConnection(cid);
  if (NULL == connection) {
    LOG4CXX_ERROR(
        logger_,
        "TransportManagerImpl::DisconnectForce: Connection does not exist.");
    LOG4CXX_TRACE(logger_,
                  "exit with E_INVALID_HANDLE. Condition: NULL == connection");
    return E_INVALID_HANDLE;
  }
  connection->transport_adapter->Disconnect(connection->device,
                                            connection->application);
  LOG4CXX_TRACE(logger_, "exit with E_SUCCESS");
  return E_SUCCESS;
}

int TransportManagerImpl::AddEventListener(TransportManagerListener* listener) {
  LOG4CXX_TRACE(logger_, "enter. TransportManagerListener: " << listener);
  transport_manager_listener_.push_back(listener);
  LOG4CXX_TRACE(logger_, "exit with E_SUCCESS");
  return E_SUCCESS;
}

void TransportManagerImpl::DisconnectAllDevices() {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoReadLock lock(device_list_lock_);
  for (DeviceInfoList::iterator i = device_list_.begin();
       i != device_list_.end();
       ++i) {
    DeviceInfo& device = i->second;
    DisconnectDevice(device.device_handle());
  }
}

void TransportManagerImpl::TerminateAllAdapters() {
  LOG4CXX_AUTO_TRACE(logger_);
  for (std::vector<TransportAdapter*>::iterator i = transport_adapters_.begin();
       i != transport_adapters_.end();
       ++i) {
    (*i)->Terminate();
  }
}

int TransportManagerImpl::InitAllAdapters() {
  LOG4CXX_AUTO_TRACE(logger_);
  for (std::vector<TransportAdapter*>::iterator i = transport_adapters_.begin();
       i != transport_adapters_.end();
       ++i) {
    if ((*i)->Init() != TransportAdapter::OK) {
      return E_ADAPTERS_FAIL;
    }
  }
  return E_SUCCESS;
}

int TransportManagerImpl::Stop() {
  LOG4CXX_AUTO_TRACE(logger_);
  if (!is_initialized_) {
    LOG4CXX_WARN(logger_, "TransportManager is not initialized_");
    return E_TM_IS_NOT_INITIALIZED;
  }

  message_queue_.Shutdown();
  event_queue_.Shutdown();

  DisconnectAllDevices();
  TerminateAllAdapters();

  is_initialized_ = false;
  return E_SUCCESS;
}

int TransportManagerImpl::SendMessageToDevice(
    const ::protocol_handler::RawMessagePtr message) {
  LOG4CXX_TRACE(logger_, "enter. RawMessageSptr: " << message);
  LOG4CXX_INFO(logger_,
               "Send message to device called with arguments "
                   << message.get());
  if (false == this->is_initialized_) {
    LOG4CXX_ERROR(logger_, "TM is not initialized.");
    LOG4CXX_TRACE(logger_,
                  "exit with E_TM_IS_NOT_INITIALIZED. Condition: false == "
                  "this->is_initialized_");
    return E_TM_IS_NOT_INITIALIZED;
  }

  {
    sync_primitives::AutoReadLock lock(connections_lock_);
    const ConnectionInternal* connection =
        GetConnection(message->connection_key());
    if (NULL == connection) {
      LOG4CXX_ERROR(logger_,
                    "Connection with id " << message->connection_key()
                                          << " does not exist.");
      LOG4CXX_TRACE(
          logger_, "exit with E_INVALID_HANDLE. Condition: NULL == connection");
      return E_INVALID_HANDLE;
    }

    if (connection->shutdown_) {
      LOG4CXX_ERROR(
          logger_,
          "TransportManagerImpl::Disconnect: Connection is to shut down.");
      LOG4CXX_TRACE(logger_,
                    "exit with E_CONNECTION_IS_TO_SHUTDOWN. Condition: "
                    "connection->shutDown");
      return E_CONNECTION_IS_TO_SHUTDOWN;
    }
  }
#ifdef TELEMETRY_MONITOR
  if (metric_observer_) {
    metric_observer_->StartRawMsg(message.get());
  }
#endif  // TELEMETRY_MONITOR
  this->PostMessage(message);
  LOG4CXX_TRACE(logger_, "exit with E_SUCCESS");
  return E_SUCCESS;
}

void TransportManagerImpl::RunAppOnDevice(const DeviceHandle device_handle,
                                          const std::string& bundle_id) {
  if (!this->is_initialized_) {
    LOG4CXX_ERROR(logger_, "TransportManager is not initialized.");
    return;
  }
  DeviceUID device_id = converter_.HandleToUid(device_handle);
  LOG4CXX_DEBUG(logger_, "Convert handle to id:" << device_id);

  sync_primitives::AutoReadLock lock(device_to_adapter_map_lock_);
  DeviceToAdapterMap::iterator it = device_to_adapter_map_.find(device_id);
  if (it == device_to_adapter_map_.end()) {
    LOG4CXX_ERROR(logger_, "No device adapter found by id " << device_id);
    return;
  }
  transport_adapter::TransportAdapter* ta = it->second;

  if (!ta) {
    LOG4CXX_ERROR(logger_,
                  "Transport adapter for device: " << device_id << " is NULL");
    return;
  }

  ta->RunAppOnDevice(device_id, bundle_id);

  return;
}

int TransportManagerImpl::ReceiveEventFromDevice(
    const TransportAdapterEvent& event) {
  LOG4CXX_TRACE(logger_, "enter. TransportAdapterEvent: " << &event);
  if (!is_initialized_) {
    LOG4CXX_ERROR(logger_, "TM is not initialized.");
    LOG4CXX_TRACE(logger_,
                  "exit with E_TM_IS_NOT_INITIALIZED. Condition: false == "
                  "this->is_initialized_");
    return E_TM_IS_NOT_INITIALIZED;
  }
  this->PostEvent(event);
  LOG4CXX_TRACE(logger_, "exit with E_SUCCESS");
  return E_SUCCESS;
}

int TransportManagerImpl::RemoveDevice(const DeviceHandle device_handle) {
  LOG4CXX_TRACE(logger_, "enter. DeviceHandle: " << &device_handle);
  DeviceUID device_id = converter_.HandleToUid(device_handle);
  if (false == this->is_initialized_) {
    LOG4CXX_ERROR(logger_, "TM is not initialized.");
    LOG4CXX_TRACE(logger_,
                  "exit with E_TM_IS_NOT_INITIALIZED. Condition: false == "
                  "this->is_initialized_");
    return E_TM_IS_NOT_INITIALIZED;
  }
  sync_primitives::AutoWriteLock lock(device_to_adapter_map_lock_);
  device_to_adapter_map_.erase(device_id);
  LOG4CXX_TRACE(logger_, "exit with E_SUCCESS");
  return E_SUCCESS;
}

int TransportManagerImpl::AddTransportAdapter(
    transport_adapter::TransportAdapter* transport_adapter) {
  LOG4CXX_TRACE(logger_, "enter. TransportAdapter: " << transport_adapter);

  if (transport_adapter_listeners_.find(transport_adapter) !=
      transport_adapter_listeners_.end()) {
    LOG4CXX_ERROR(logger_, "Adapter already exists.");
    LOG4CXX_TRACE(logger_,
                  "exit with E_ADAPTER_EXISTS. Condition: "
                  "transport_adapter_listeners_.find(transport_adapter) != "
                  "transport_adapter_listeners_.end()");
    return E_ADAPTER_EXISTS;
  }

  auto listener = new TransportAdapterListenerImpl(this, transport_adapter);

  transport_adapter->AddListener(listener);

  if (transport_adapter->IsInitialised() ||
      transport_adapter->Init() == TransportAdapter::OK) {
    transport_adapter_listeners_[transport_adapter] = listener;
    transport_adapters_.push_back(transport_adapter);
  } else {
    delete listener;
    delete transport_adapter;
  }
  LOG4CXX_TRACE(logger_, "exit with E_SUCCESS");
  return E_SUCCESS;
}

int TransportManagerImpl::SearchDevices() {
  LOG4CXX_TRACE(logger_, "enter");
  if (!this->is_initialized_) {
    LOG4CXX_ERROR(logger_, "TM is not initialized");
    LOG4CXX_TRACE(
        logger_,
        "exit with E_TM_IS_NOT_INITIALIZED. Condition: !this->is_initialized_");
    return E_TM_IS_NOT_INITIALIZED;
  }

  LOG4CXX_INFO(logger_, "Search device called");

  bool success_occurred = false;

  for (std::vector<TransportAdapter*>::iterator it =
           transport_adapters_.begin();
       it != transport_adapters_.end();
       ++it) {
    LOG4CXX_DEBUG(logger_, "Iterating over transport adapters");
    TransportAdapter::Error scanResult = (*it)->SearchDevices();
    if (transport_adapter::TransportAdapter::OK == scanResult) {
      success_occurred = true;
    } else {
      LOG4CXX_ERROR(logger_,
                    "Transport Adapter search failed "
                        << *it << "[" << (*it)->GetDeviceType() << "]");
      switch (scanResult) {
        case transport_adapter::TransportAdapter::NOT_SUPPORTED: {
          LOG4CXX_ERROR(logger_,
                        "Search feature is not supported "
                            << *it << "[" << (*it)->GetDeviceType() << "]");
          LOG4CXX_DEBUG(logger_,
                        "scanResult = TransportAdapter::NOT_SUPPORTED");
          break;
        }
        case transport_adapter::TransportAdapter::BAD_STATE: {
          LOG4CXX_ERROR(logger_,
                        "Transport Adapter has bad state "
                            << *it << "[" << (*it)->GetDeviceType() << "]");
          LOG4CXX_DEBUG(logger_, "scanResult = TransportAdapter::BAD_STATE");
          break;
        }
        default: {
          LOG4CXX_ERROR(logger_, "Invalid scan result");
          LOG4CXX_DEBUG(logger_, "scanResult = default switch case");
          return E_ADAPTERS_FAIL;
        }
      }
    }
  }
  int transport_adapter_search =
      (success_occurred || transport_adapters_.empty()) ? E_SUCCESS
                                                        : E_ADAPTERS_FAIL;
  if (transport_adapter_search == E_SUCCESS) {
    LOG4CXX_TRACE(logger_,
                  "exit with E_SUCCESS. Condition: success_occured || "
                  "transport_adapters_.empty()");
  } else {
    LOG4CXX_TRACE(logger_,
                  "exit with E_ADAPTERS_FAIL. Condition: success_occured || "
                  "transport_adapters_.empty()");
  }
  return transport_adapter_search;
}

int TransportManagerImpl::Init(resumption::LastState& last_state) {
  // Last state requred to initialize Transport adapters
  UNUSED(last_state);
  LOG4CXX_TRACE(logger_, "enter");
  is_initialized_ = true;
  LOG4CXX_TRACE(logger_, "exit with E_SUCCESS");
  return E_SUCCESS;
}

int TransportManagerImpl::Reinit() {
  LOG4CXX_AUTO_TRACE(logger_);
  DisconnectAllDevices();
  TerminateAllAdapters();
  device_to_adapter_map_.clear();
  connection_id_counter_ = 0;
  int ret = InitAllAdapters();
  return ret;
}

int TransportManagerImpl::Visibility(const bool& on_off) const {
  LOG4CXX_TRACE(logger_, "enter. On_off: " << &on_off);
  TransportAdapter::Error ret;

  LOG4CXX_DEBUG(logger_, "Visibility change requested to " << on_off);
  if (!is_initialized_) {
    LOG4CXX_ERROR(logger_, "TM is not initialized");
    LOG4CXX_TRACE(logger_,
                  "exit with E_TM_IS_NOT_INITIALIZED. Condition: false == "
                  "is_initialized_");
    return E_TM_IS_NOT_INITIALIZED;
  }

  for (std::vector<TransportAdapter*>::const_iterator it =
           transport_adapters_.begin();
       it != transport_adapters_.end();
       ++it) {
    if (on_off) {
      ret = (*it)->StartClientListening();
    } else {
      ret = (*it)->StopClientListening();
    }
    if (TransportAdapter::Error::NOT_SUPPORTED == ret) {
      LOG4CXX_DEBUG(logger_,
                    "Visibility change is not supported for adapter "
                        << *it << "[" << (*it)->GetDeviceType() << "]");
    }
  }
  LOG4CXX_TRACE(logger_, "exit with E_SUCCESS");
  return E_SUCCESS;
}

void TransportManagerImpl::UpdateDeviceList(TransportAdapter* ta) {
  LOG4CXX_TRACE(logger_, "enter. TransportAdapter: " << ta);
  std::set<DeviceInfo> old_devices;
  std::set<DeviceInfo> new_devices;
  {
    sync_primitives::AutoWriteLock lock(device_list_lock_);
    for (DeviceInfoList::iterator it = device_list_.begin();
         it != device_list_.end();) {
      if (it->first == ta) {
        old_devices.insert(it->second);
        it = device_list_.erase(it);
      } else {
        ++it;
      }
    }

    const DeviceList dev_list = ta->GetDeviceList();
    for (DeviceList::const_iterator it = dev_list.begin(); it != dev_list.end();
         ++it) {
      DeviceHandle device_handle =
          converter_.UidToHandle(*it, ta->GetConnectionType());
      DeviceInfo info(
          device_handle, *it, ta->DeviceName(*it), ta->GetConnectionType());
      device_list_.push_back(std::make_pair(ta, info));
      new_devices.insert(info);
    }
  }

  std::set<DeviceInfo> added_devices;
  std::set_difference(new_devices.begin(),
                      new_devices.end(),
                      old_devices.begin(),
                      old_devices.end(),
                      std::inserter(added_devices, added_devices.begin()));
  for (std::set<DeviceInfo>::const_iterator it = added_devices.begin();
       it != added_devices.end();
       ++it) {
    RaiseEvent(&TransportManagerListener::OnDeviceAdded, *it);
  }

  std::set<DeviceInfo> removed_devices;
  std::set_difference(old_devices.begin(),
                      old_devices.end(),
                      new_devices.begin(),
                      new_devices.end(),
                      std::inserter(removed_devices, removed_devices.begin()));

  for (std::set<DeviceInfo>::const_iterator it = removed_devices.begin();
       it != removed_devices.end();
       ++it) {
    RaiseEvent(&TransportManagerListener::OnDeviceRemoved, *it);
  }
  LOG4CXX_TRACE(logger_, "exit");
}

void TransportManagerImpl::PostMessage(
    const ::protocol_handler::RawMessagePtr message) {
  LOG4CXX_TRACE(logger_, "enter. RawMessageSptr: " << message);
  message_queue_.PostMessage(message);
  LOG4CXX_TRACE(logger_, "exit");
}

void TransportManagerImpl::PostEvent(const TransportAdapterEvent& event) {
  LOG4CXX_AUTO_TRACE(logger_);
  LOG4CXX_DEBUG(logger_, "TransportAdapterEvent: " << &event);
  event_queue_.PostMessage(event);
}

const TransportManagerSettings& TransportManagerImpl::get_settings() const {
  return settings_;
}

void TransportManagerImpl::AddConnection(const ConnectionInternal& c) {
  LOG4CXX_AUTO_TRACE(logger_);
  LOG4CXX_DEBUG(logger_, "ConnectionInternal: " << &c);
  sync_primitives::AutoWriteLock lock(connections_lock_);
  connections_.push_back(c);
}

void TransportManagerImpl::RemoveConnection(
    const uint32_t id, transport_adapter::TransportAdapter* transport_adapter) {
  LOG4CXX_AUTO_TRACE(logger_);
  LOG4CXX_DEBUG(logger_, "Id: " << id);
  sync_primitives::AutoWriteLock lock(connections_lock_);
  LOG4CXX_DEBUG(logger_, "Removing connection with id: " << id);
  const std::vector<ConnectionInternal>::iterator it = std::find_if(
      connections_.begin(), connections_.end(), ConnectionFinder(id));
  if (connections_.end() != it) {
    if (transport_adapter) {
      transport_adapter->RemoveFinalizedConnection(it->device, it->application);
    }
    connections_.erase(it);
  }
}

void TransportManagerImpl::DeactivateDeviceConnections(
    const DeviceUID& device_uid) {
  LOG4CXX_AUTO_TRACE(logger_);

  sync_primitives::AutoWriteLock lock(connections_lock_);
  LOG4CXX_DEBUG(logger_,
                "Deactivating connections for device with UID: " << device_uid);

  size_t counter = 0;
  for (std::vector<ConnectionInternal>::iterator it = connections_.begin();
       it != connections_.end();
       ++it) {
    if (it->device == device_uid) {
      it->active_ = false;
      ++counter;
    }
  }
  LOG4CXX_DEBUG(logger_,
                "Deactivated "
                    << counter
                    << " connections for device with UID: " << device_uid);
}

TransportManagerImpl::ConnectionInternal* TransportManagerImpl::GetConnection(
    const ConnectionUID id) {
  LOG4CXX_AUTO_TRACE(logger_);
  LOG4CXX_DEBUG(logger_, "ConnectionUID: " << id);
  for (std::vector<ConnectionInternal>::iterator it = connections_.begin();
       it != connections_.end();
       ++it) {
    if (it->id == id) {
      LOG4CXX_DEBUG(logger_, "ConnectionInternal. It's address: " << &*it);
      return &*it;
    }
  }
  return NULL;
}

TransportManagerImpl::ConnectionInternal* TransportManagerImpl::GetConnection(
    const DeviceUID& device, const ApplicationHandle& application) {
  LOG4CXX_AUTO_TRACE(logger_);
  LOG4CXX_DEBUG(
      logger_, "DeviceUID: " << device << "ApplicationHandle: " << application);
  for (std::vector<ConnectionInternal>::iterator it = connections_.begin();
       it != connections_.end();
       ++it) {
    if (it->device == device && it->application == application) {
      LOG4CXX_DEBUG(logger_, "ConnectionInternal. It's address: " << &*it);
      return &*it;
    }
  }
  return NULL;
}

TransportManagerImpl::ConnectionInternal*
TransportManagerImpl::GetActiveConnection(
    const DeviceUID& device, const ApplicationHandle& application) {
  LOG4CXX_AUTO_TRACE(logger_);
  LOG4CXX_DEBUG(logger_,
                "DeviceUID: " << device
                              << " ApplicationHandle: " << application);
  for (std::vector<ConnectionInternal>::iterator it = connections_.begin();
       it != connections_.end();
       ++it) {
    if (it->device == device && it->application == application && it->active_) {
      LOG4CXX_DEBUG(logger_, "ConnectionInternal. It's address: " << &*it);
      return &*it;
    }
  }
  return NULL;
}

namespace {

struct IOSBTAdapterFinder {
  bool operator()(const std::vector<TransportAdapter*>::value_type& i) const {
    return i->GetDeviceType() == transport_adapter::DeviceType::IOS_BT;
  }
};

struct SwitchableFinder {
  explicit SwitchableFinder(SwitchableDevices::const_iterator what)
      : what_(what) {}
  bool operator()(const SwitchableDevices::value_type& i) const {
    return what_->second == i.second;
  }

 private:
  SwitchableDevices::const_iterator what_;
};

}  // namespace

void TransportManagerImpl::TryDeviceSwitch(
    transport_adapter::TransportAdapter* adapter) {
  LOG4CXX_AUTO_TRACE(logger_);
  if (adapter->GetDeviceType() != transport_adapter::DeviceType::IOS_USB) {
    LOG4CXX_ERROR(logger_, "Switching requested not from iAP-USB transport.");
    return;
  }

  const auto ios_bt_adapter = std::find_if(transport_adapters_.begin(),
                                           transport_adapters_.end(),
                                           IOSBTAdapterFinder());

  if (transport_adapters_.end() == ios_bt_adapter) {
    LOG4CXX_WARN(
        logger_,
        "There is no iAP2 Bluetooth adapter found. Switching is not possible.");
    return;
  }

  const SwitchableDevices usb_switchable_devices =
      adapter->GetSwitchableDevices();
  const auto bt_switchable_devices = (*ios_bt_adapter)->GetSwitchableDevices();
  auto bt = bt_switchable_devices.end();
  auto usb = usb_switchable_devices.begin();
  for (; usb != usb_switchable_devices.end(); ++usb) {
    SwitchableFinder finder(usb);
    bt = std::find_if(
        bt_switchable_devices.begin(), bt_switchable_devices.end(), finder);

    if (bt != bt_switchable_devices.end()) {
      break;
    }
  }

  if (bt_switchable_devices.end() == bt) {
    LOG4CXX_WARN(logger_,
                 "No suitable for switching iAP2 Bluetooth device found.");
    return;
  }

  LOG4CXX_DEBUG(logger_,
                "Found UUID suitable for transport switching: " << bt->second);
  LOG4CXX_DEBUG(
      logger_, "Device to switch from: " << bt->first << " to: " << usb->first);

  sync_primitives::AutoWriteLock lock(device_to_adapter_map_lock_);

  const auto bt_device_uid = bt->first;
  const auto device_to_switch = device_to_adapter_map_.find(bt_device_uid);
  if (device_to_adapter_map_.end() == device_to_switch) {
    LOG4CXX_ERROR(logger_,
                  "There is no known device found with UID "
                      << bt_device_uid
                      << " . Transport switching is not possible.");
    DCHECK_OR_RETURN_VOID(false);
    return;
  }

  const auto usb_uid = usb->first;
  const auto bt_uid = device_to_switch->first;
  const auto bt_adapter = device_to_switch->second;

  LOG4CXX_DEBUG(logger_,
                "Known device with UID "
                    << bt_uid << " is appropriate for transport switching.");

  RaiseEvent(
      &TransportManagerListener::OnDeviceSwitchingStart, bt_uid, usb_uid);

  bt_adapter->StopDevice(bt_uid);
  adapter->DeviceSwitched(usb_uid);

  DeactivateDeviceConnections(bt_uid);

  device_to_reconnect_ = bt_uid;

  const uint32_t timeout = get_settings().app_transport_change_timer() +
                           get_settings().app_transport_change_timer_addition();
  device_switch_timer_.Start(timeout, timer::kSingleShot);

  LOG4CXX_DEBUG(logger_,
                "Device switch for device id " << bt_uid << " is done.");
  return;
}

bool TransportManagerImpl::UpdateDeviceMapping(
    transport_adapter::TransportAdapter* ta) {
  const DeviceList adapter_device_list = ta->GetDeviceList();
  LOG4CXX_DEBUG(logger_, "DEVICE_LIST_UPDATED " << adapter_device_list.size());

  sync_primitives::AutoWriteLock lock(device_to_adapter_map_lock_);

  LOG4CXX_DEBUG(logger_,
                "Before cleanup and update. Device map size is "
                    << device_to_adapter_map_.size());

  for (auto item = device_to_adapter_map_.begin();
       device_to_adapter_map_.end() != item;) {
    const auto adapter = item->second;
    if (adapter != ta) {
      ++item;
      continue;
    }

    const auto device_uid = item->first;
    if (adapter_device_list.end() != std::find(adapter_device_list.begin(),
                                               adapter_device_list.end(),
                                               device_uid)) {
      ++item;
      continue;
    }

    device_to_adapter_map_.erase(item);
    item = device_to_adapter_map_.begin();
  }

  LOG4CXX_DEBUG(logger_,
                "After cleanup. Device map size is "
                    << device_to_adapter_map_.size());

  for (DeviceList::const_iterator it = adapter_device_list.begin();
       it != adapter_device_list.end();
       ++it) {
    const auto device_uid = *it;
    const auto result =
        device_to_adapter_map_.insert(std::make_pair(device_uid, ta));
    if (!result.second) {
      LOG4CXX_WARN(logger_,
                   "Device UID "
                       << device_uid
                       << " is known already. Processing skipped."
                          "Connection type is: " << ta->GetConnectionType());
      continue;
    }
    DeviceHandle device_handle =
        converter_.UidToHandle(device_uid, ta->GetConnectionType());
    DeviceInfo info(device_handle,
                    device_uid,
                    ta->DeviceName(device_uid),
                    ta->GetConnectionType());
    RaiseEvent(&TransportManagerListener::OnDeviceFound, info);
  }

  LOG4CXX_DEBUG(logger_,
                "After update. Device map size is "
                    << device_to_adapter_map_.size());

  return true;
}

void TransportManagerImpl::OnDeviceListUpdated(TransportAdapter* ta) {
  LOG4CXX_TRACE(logger_, "enter. TransportAdapter: " << ta);
  if (!UpdateDeviceMapping(ta)) {
    LOG4CXX_ERROR(logger_, "Device list update failed.");
    return;
  }
  UpdateDeviceList(ta);
  std::vector<DeviceInfo> device_infos;
  device_list_lock_.AcquireForReading();
  for (DeviceInfoList::const_iterator it = device_list_.begin();
       it != device_list_.end();
       ++it) {
    device_infos.push_back(it->second);
  }
  device_list_lock_.Release();
  RaiseEvent(&TransportManagerListener::OnDeviceListUpdated, device_infos);
  LOG4CXX_TRACE(logger_, "exit");
}

void TransportManagerImpl::Handle(TransportAdapterEvent event) {
  LOG4CXX_TRACE(logger_, "enter");
  switch (event.event_type) {
    case EventTypeEnum::ON_SEARCH_DONE: {
      RaiseEvent(&TransportManagerListener::OnScanDevicesFinished);
      LOG4CXX_DEBUG(logger_, "event_type = ON_SEARCH_DONE");
      break;
    }
    case EventTypeEnum::ON_SEARCH_FAIL: {
      // error happened in real search process (external error)
      RaiseEvent(&TransportManagerListener::OnScanDevicesFailed,
                 *static_cast<SearchDeviceError*>(event.event_error.get()));
      LOG4CXX_DEBUG(logger_, "event_type = ON_SEARCH_FAIL");
      break;
    }
    case EventTypeEnum::ON_DEVICE_LIST_UPDATED: {
      OnDeviceListUpdated(event.transport_adapter);
      LOG4CXX_DEBUG(logger_, "event_type = ON_DEVICE_LIST_UPDATED");
      break;
    }
    case EventTypeEnum::ON_TRANSPORT_SWITCH_REQUESTED: {
      TryDeviceSwitch(event.transport_adapter);
      LOG4CXX_DEBUG(logger_, "event_type = ON_TRANSPORT_SWITCH_REQUESTED");
      break;
    }
    case EventTypeEnum::ON_FIND_NEW_APPLICATIONS_REQUEST: {
      RaiseEvent(&TransportManagerListener::OnFindNewApplicationsRequest);
      LOG4CXX_DEBUG(logger_, "event_type = ON_FIND_NEW_APPLICATIONS_REQUEST");
      break;
    }
    case EventTypeEnum::ON_CONNECTION_STATUS_UPDATED: {
      RaiseEvent(&TransportManagerListener::OnConnectionStatusUpdated);
      LOG4CXX_DEBUG(logger_, "event_type = ON_CONNECTION_STATUS_UPDATED");
      break;
    }
    case EventTypeEnum::ON_CONNECT_PENDING: {
      const DeviceHandle device_handle = converter_.UidToHandle(
          event.device_uid, event.transport_adapter->GetConnectionType());
      AddConnection(ConnectionInternal(this,
                                       event.transport_adapter,
                                       ++connection_id_counter_,
                                       event.device_uid,
                                       event.application_id,
                                       device_handle));
      RaiseEvent(
          &TransportManagerListener::OnConnectionPending,
          DeviceInfo(device_handle,
                     event.device_uid,
                     event.transport_adapter->DeviceName(event.device_uid),
                     event.transport_adapter->GetConnectionType()),
          connection_id_counter_);
      LOG4CXX_DEBUG(logger_, "event_type = ON_CONNECT_PENDING");
      break;
    }
    case EventTypeEnum::ON_CONNECT_DONE: {
      const DeviceHandle device_handle = converter_.UidToHandle(
          event.device_uid, event.transport_adapter->GetConnectionType());

      int connection_id = 0;
      std::vector<ConnectionInternal>::iterator it = connections_.begin();
      std::vector<ConnectionInternal>::iterator end = connections_.end();
      for (; it != end; ++it) {
        if (it->transport_adapter != event.transport_adapter) {
          continue;
        } else if (it->Connection::device != event.device_uid) {
          continue;
        } else if (it->Connection::application != event.application_id) {
          continue;
        } else if (it->device_handle_ != device_handle) {
          continue;
        } else {
          LOG4CXX_DEBUG(logger_, "Connection Object Already Exists");
          connection_id = it->Connection::id;
          break;
        }
      }

      if (it == end) {
        AddConnection(ConnectionInternal(this,
                                         event.transport_adapter,
                                         ++connection_id_counter_,
                                         event.device_uid,
                                         event.application_id,
                                         device_handle));
        connection_id = connection_id_counter_;
      }

      RaiseEvent(
          &TransportManagerListener::OnConnectionEstablished,
          DeviceInfo(device_handle,
                     event.device_uid,
                     event.transport_adapter->DeviceName(event.device_uid),
                     event.transport_adapter->GetConnectionType()),
          connection_id);
      LOG4CXX_DEBUG(logger_, "event_type = ON_CONNECT_DONE");
      break;
    }
    case EventTypeEnum::ON_CONNECT_FAIL: {
      RaiseEvent(
          &TransportManagerListener::OnConnectionFailed,
          DeviceInfo(converter_.UidToHandle(
                         event.device_uid,
                         event.transport_adapter->GetConnectionType()),
                     event.device_uid,
                     event.transport_adapter->DeviceName(event.device_uid),
                     event.transport_adapter->GetConnectionType()),
          ConnectError());
      LOG4CXX_DEBUG(logger_, "event_type = ON_CONNECT_FAIL");
      break;
    }
    case EventTypeEnum::ON_DISCONNECT_DONE: {
      connections_lock_.AcquireForReading();
      ConnectionInternal* connection =
          GetConnection(event.device_uid, event.application_id);
      if (NULL == connection) {
        LOG4CXX_ERROR(logger_, "Connection not found");
        LOG4CXX_DEBUG(logger_,
                      "event_type = ON_DISCONNECT_DONE && NULL == connection");
        connections_lock_.Release();
        break;
      }
      const ConnectionUID id = connection->id;
      connections_lock_.Release();

      RaiseEvent(&TransportManagerListener::OnConnectionClosed, id);
      RemoveConnection(id, connection->transport_adapter);
      LOG4CXX_DEBUG(logger_, "event_type = ON_DISCONNECT_DONE");
      break;
    }
    case EventTypeEnum::ON_DISCONNECT_FAIL: {
      const DeviceHandle device_handle = converter_.UidToHandle(
          event.device_uid, event.transport_adapter->GetConnectionType());
      RaiseEvent(&TransportManagerListener::OnDisconnectFailed,
                 device_handle,
                 DisconnectDeviceError());
      LOG4CXX_DEBUG(logger_, "event_type = ON_DISCONNECT_FAIL");
      break;
    }
    case EventTypeEnum::ON_SEND_DONE: {
#ifdef TELEMETRY_MONITOR
      if (metric_observer_) {
        metric_observer_->StopRawMsg(event.event_data.get());
      }
#endif  // TELEMETRY_MONITOR
      sync_primitives::AutoReadLock lock(connections_lock_);
      ConnectionInternal* connection =
          GetConnection(event.device_uid, event.application_id);
      if (connection == NULL) {
        LOG4CXX_ERROR(logger_,
                      "Connection ('" << event.device_uid << ", "
                                      << event.application_id << ") not found");
        LOG4CXX_DEBUG(
            logger_,
            "event_type = ON_SEND_DONE. Condition: NULL == connection");
        break;
      }
      RaiseEvent(&TransportManagerListener::OnTMMessageSend, event.event_data);
      if (connection->shutdown_ && --connection->messages_count == 0) {
        connection->timer->Stop();
        connection->transport_adapter->Disconnect(connection->device,
                                                  connection->application);
      }
      LOG4CXX_DEBUG(logger_, "event_type = ON_SEND_DONE");
      break;
    }
    case EventTypeEnum::ON_SEND_FAIL: {
#ifdef TELEMETRY_MONITOR
      if (metric_observer_) {
        metric_observer_->StopRawMsg(event.event_data.get());
      }
#endif  // TELEMETRY_MONITOR
      {
        sync_primitives::AutoReadLock lock(connections_lock_);
        ConnectionInternal* connection =
            GetConnection(event.device_uid, event.application_id);
        if (connection == NULL) {
          LOG4CXX_ERROR(logger_,
                        "Connection ('" << event.device_uid << ", "
                                        << event.application_id
                                        << ") not found");
          LOG4CXX_DEBUG(
              logger_,
              "event_type = ON_SEND_FAIL. Condition: NULL == connection");
          break;
        }
      }

      // TODO(YK): start timer here to wait before notify caller
      // and remove unsent messages
      LOG4CXX_ERROR(logger_, "Transport adapter failed to send data");
      // TODO(YK): potential error case -> thread unsafe
      // update of message content
      if (event.event_data.use_count() != 0) {
        event.event_data->set_waiting(true);
      } else {
        LOG4CXX_DEBUG(logger_, "Data is invalid");
      }
      LOG4CXX_DEBUG(logger_, "eevent_type = ON_SEND_FAIL");
      break;
    }
    case EventTypeEnum::ON_RECEIVED_DONE: {
      {
        sync_primitives::AutoReadLock lock(connections_lock_);
        ConnectionInternal* connection =
            GetActiveConnection(event.device_uid, event.application_id);
        if (connection == NULL) {
          LOG4CXX_ERROR(logger_,
                        "Connection ('" << event.device_uid << ", "
                                        << event.application_id
                                        << ") not found");
          LOG4CXX_DEBUG(
              logger_,
              "event_type = ON_RECEIVED_DONE. Condition: NULL == connection");
          break;
        }
        event.event_data->set_connection_key(connection->id);
      }
#ifdef TELEMETRY_MONITOR
      if (metric_observer_) {
        metric_observer_->StopRawMsg(event.event_data.get());
      }
#endif  // TELEMETRY_MONITOR
      RaiseEvent(&TransportManagerListener::OnTMMessageReceived,
                 event.event_data);
      LOG4CXX_DEBUG(logger_, "event_type = ON_RECEIVED_DONE");
      break;
    }
    case EventTypeEnum::ON_RECEIVED_FAIL: {
      LOG4CXX_DEBUG(logger_, "Event ON_RECEIVED_FAIL");
      connections_lock_.AcquireForReading();
      ConnectionInternal* connection =
          GetActiveConnection(event.device_uid, event.application_id);
      if (connection == NULL) {
        LOG4CXX_ERROR(logger_,
                      "Connection ('" << event.device_uid << ", "
                                      << event.application_id << ") not found");
        connections_lock_.Release();
        break;
      }
      connections_lock_.Release();

      RaiseEvent(&TransportManagerListener::OnTMMessageReceiveFailed,
                 *static_cast<DataReceiveError*>(event.event_error.get()));
      LOG4CXX_DEBUG(logger_, "event_type = ON_RECEIVED_FAIL");
      break;
    }
    case EventTypeEnum::ON_COMMUNICATION_ERROR: {
      LOG4CXX_DEBUG(logger_, "event_type = ON_COMMUNICATION_ERROR");
      break;
    }
    case EventTypeEnum::ON_UNEXPECTED_DISCONNECT: {
      connections_lock_.AcquireForReading();
      ConnectionInternal* connection =
          GetConnection(event.device_uid, event.application_id);
      if (connection) {
        const ConnectionUID id = connection->id;
        connections_lock_.Release();
        RaiseEvent(&TransportManagerListener::OnUnexpectedDisconnect,
                   id,
                   *static_cast<CommunicationError*>(event.event_error.get()));
        RemoveConnection(id, connection->transport_adapter);
      } else {
        connections_lock_.Release();
        LOG4CXX_ERROR(logger_,
                      "Connection ('" << event.device_uid << ", "
                                      << event.application_id << ") not found");
      }
      LOG4CXX_DEBUG(logger_, "eevent_type = ON_UNEXPECTED_DISCONNECT");
      break;
    }
    case EventTypeEnum::ON_TRANSPORT_CONFIG_UPDATED: {
      LOG4CXX_DEBUG(logger_, "event_type = ON_TRANSPORT_CONFIG_UPDATED");
      transport_adapter::TransportConfig config =
          event.transport_adapter->GetTransportConfiguration();
      RaiseEvent(&TransportManagerListener::OnTransportConfigUpdated, config);
      break;
    }
  }  // switch
  LOG4CXX_TRACE(logger_, "exit");
}

#ifdef TELEMETRY_MONITOR
void TransportManagerImpl::SetTelemetryObserver(TMTelemetryObserver* observer) {
  metric_observer_ = observer;
}
#endif  // TELEMETRY_MONITOR

void TransportManagerImpl::Handle(::protocol_handler::RawMessagePtr msg) {
  LOG4CXX_TRACE(logger_, "enter");
  sync_primitives::AutoReadLock lock(connections_lock_);
  ConnectionInternal* connection = GetConnection(msg->connection_key());
  if (connection == NULL) {
    LOG4CXX_WARN(logger_,
                 "Connection " << msg->connection_key() << " not found");
    RaiseEvent(&TransportManagerListener::OnTMMessageSendFailed,
               DataSendTimeoutError(),
               msg);
    return;
  }

  TransportAdapter* transport_adapter = connection->transport_adapter;
  LOG4CXX_DEBUG(logger_,
                "Got adapter " << transport_adapter << "["
                               << transport_adapter->GetDeviceType() << "]"
                               << " by session id " << msg->connection_key());

  if (NULL == transport_adapter) {
    std::string error_text = "Transport adapter is not found";
    LOG4CXX_ERROR(logger_, error_text);
    RaiseEvent(&TransportManagerListener::OnTMMessageSendFailed,
               DataSendError(error_text),
               msg);
  } else {
    if (TransportAdapter::OK ==
        transport_adapter->SendData(
            connection->device, connection->application, msg)) {
      LOG4CXX_TRACE(logger_, "Data sent to adapter");
    } else {
      LOG4CXX_ERROR(logger_, "Data sent error");
      RaiseEvent(&TransportManagerListener::OnTMMessageSendFailed,
                 DataSendError("Send failed"),
                 msg);
    }
  }
  LOG4CXX_TRACE(logger_, "exit");
}

TransportManagerImpl::ConnectionInternal::ConnectionInternal(
    TransportManagerImpl* transport_manager,
    TransportAdapter* transport_adapter,
    const ConnectionUID id,
    const DeviceUID& dev_id,
    const ApplicationHandle& app_id,
    const DeviceHandle device_handle)
    : transport_manager(transport_manager)
    , transport_adapter(transport_adapter)
    , timer(std::make_shared<timer::Timer,
                             const char*,
                             ::timer::TimerTaskImpl<ConnectionInternal>*>(
          "TM DiscRoutine",
          new ::timer::TimerTaskImpl<ConnectionInternal>(
              this, &ConnectionInternal::DisconnectFailedRoutine)))
    , shutdown_(false)
    , device_handle_(device_handle)
    , messages_count(0)
    , active_(true) {
  Connection::id = id;
  Connection::device = dev_id;
  Connection::application = app_id;
}

void TransportManagerImpl::ConnectionInternal::DisconnectFailedRoutine() {
  LOG4CXX_TRACE(logger_, "enter");
  transport_manager->RaiseEvent(&TransportManagerListener::OnDisconnectFailed,
                                device_handle_,
                                DisconnectDeviceError());
  shutdown_ = false;
  timer->Stop();
  LOG4CXX_TRACE(logger_, "exit");
}

DeviceHandle TransportManagerImpl::Handle2GUIDConverter::UidToHandle(
    const DeviceUID& dev_uid, const std::string& connection_type) {
  DeviceHandle handle = hash_function_(dev_uid + connection_type);

  {
    sync_primitives::AutoReadLock lock(conversion_table_lock_);

    auto it = std::find_if(conversion_table_.begin(),
                           conversion_table_.end(),
                           HandleFinder(handle));

    if (it != conversion_table_.end()) {
      LOG4CXX_DEBUG(logger_,
                    "Handle for UID is found: " << std::get<0>(*it) << "/"
                                                << std::get<1>(*it) << "/"
                                                << std::get<2>(*it));
      return std::get<2>(*it);
    }
  }

  sync_primitives::AutoWriteLock lock(conversion_table_lock_);

  auto t = std::make_tuple(dev_uid, connection_type, handle);
  conversion_table_.push_back(
      std::make_tuple(dev_uid, connection_type, handle));
  LOG4CXX_DEBUG(logger_,
                "Handle for UID is added: " << std::get<0>(t) << "/"
                                            << std::get<1>(t) << "/"
                                            << std::get<2>(t));
  return handle;
}

DeviceUID TransportManagerImpl::Handle2GUIDConverter::HandleToUid(
    const DeviceHandle handle) {
  sync_primitives::AutoReadLock lock(conversion_table_lock_);

  auto it = std::find_if(
      conversion_table_.begin(), conversion_table_.end(), HandleFinder(handle));

  if (it != conversion_table_.end()) {
    LOG4CXX_DEBUG(logger_,
                  "Handle is found: " << std::get<0>(*it) << "/"
                                      << std::get<1>(*it) << "/"
                                      << std::get<2>(*it));
    return std::get<0>(*it);
  }

  LOG4CXX_DEBUG(logger_, "Handle is not found: " << handle);
  return DeviceUID("uknown_uid");
}

}  // namespace transport_manager
