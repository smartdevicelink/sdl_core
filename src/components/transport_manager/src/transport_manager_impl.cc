/**
 * \file transport_manager_impl.cc
 * \brief TransportManagerImpl class source file.
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
#include <stdint.h>
#include <cstring>
#include <queue>
#include <set>
#include <algorithm>
#include <limits>
#include <functional>
#include <sstream>
#include "utils/macro.h"
#include "protocol_handler/raw_message.h"
#include "protocol_handler/protocol_packet.h"
#include "transport_manager/transport_manager_impl.h"
#include "transport_manager/transport_manager_listener.h"
#include "transport_manager/transport_manager_listener_empty.h"
#include "transport_manager/bluetooth/bluetooth_transport_adapter.h"
#include "transport_manager/tcp/tcp_transport_adapter.h"
#include "transport_manager/transport_adapter/transport_adapter.h"
#include "config_profile/profile.h"
#include "transport_manager/transport_adapter/transport_adapter_event.h"

using ::transport_manager::transport_adapter::TransportAdapter;

namespace transport_manager {

log4cxx::LoggerPtr TransportManagerImpl::logger_ =
    log4cxx::LoggerPtr(log4cxx::Logger::getLogger("TransportManager"));

TransportManagerImpl::Connection TransportManagerImpl::convert(TransportManagerImpl::ConnectionInternal& p) {
  TransportManagerImpl::Connection c;
  c.application = p.application;
  c.device = p.device;
  c.id = p.id;
  return c;
}

class TransportManagerImpl::IncomingDataHandler {
 public:
  IncomingDataHandler(TransportManagerImpl* tm_impl)
      : connections_data_(), tm_impl_(tm_impl) {}

  bool ProcessData(ConnectionUID connection_id, uint8_t* data,
                   std::size_t size) {
    LOG4CXX_TRACE(logger_, "Start of processing incoming data of size "
                               << size << " for connection " << connection_id);
    const uint32_t kBytesForSizeDetection = 8;
    ConnectionsData::iterator it = connections_data_.find(connection_id);
    if (connections_data_.end() == it) {
      LOG4CXX_ERROR(logger_, "ProcessData requested for unknown connection");
      return false;
    }
    std::vector<uint8_t>& connection_data = it->second;
    connection_data.insert(connection_data.end(), data, data + size);

    LOG4CXX_TRACE(logger_, "Total data size for connection "
                               << connection_id << " is "
                               << connection_data.size());
    while (connection_data.size() >= kBytesForSizeDetection) {
      const uint32_t packet_size =
          tm_impl_->GetPacketSize(kBytesForSizeDetection, &connection_data[0]);
      if (0 == packet_size) {
        LOG4CXX_ERROR(logger_, "Failed to get packet size");
        return false;
      }
      LOG4CXX_TRACE(logger_, "Packet size " << packet_size);
      if (connection_data.size() >= packet_size) {
        RawMessageSptr raw_message(new protocol_handler::RawMessage(
            connection_id, 0,  // It's not up to TM to know protocol version
            &connection_data[0], packet_size));
        tm_impl_->RaiseEvent(&TransportManagerListener::OnTMMessageReceived,
                             raw_message);
        connection_data.erase(connection_data.begin(),
                              connection_data.begin() + packet_size);
        LOG4CXX_TRACE(logger_,
                      "Packet created and passed, new data size for connection "
                          << connection_id << " is " << connection_data.size());
      } else {
        LOG4CXX_TRACE(logger_, "Packet data is not available yet");
        return true;
      }
    }
    return true;
  }

  void AddConnection(ConnectionUID connection_id) {
    connections_data_[connection_id];
  }

  void RemoveConnection(ConnectionUID connection_id) {
    connections_data_.erase(connection_id);
  }

 private:
  typedef std::map<ConnectionUID, std::vector<uint8_t> > ConnectionsData;
  ConnectionsData connections_data_;
  TransportManagerImpl* tm_impl_;
};

TransportManagerImpl::TransportManagerImpl()
    : message_queue_mutex_(),
      all_thread_active_(false),
      message_queue_thread_(),
      event_queue_thread_(),
      device_listener_thread_wakeup_(),
      is_initialized_(false),
      connection_id_counter_(0),
      incoming_data_handler_(new IncomingDataHandler(this)) {
  LOG4CXX_INFO(logger_, "==============================================");
#ifdef USE_RWLOCK
  pthread_rwlock_init(&message_queue_rwlock_, NULL);
  pthread_rwlock_init(&event_queue_rwlock_, NULL);
#endif
  pthread_mutex_init(&message_queue_mutex_, NULL);
  pthread_cond_init(&message_queue_cond_, NULL);
  pthread_mutex_init(&event_queue_mutex_, 0);
  pthread_cond_init(&device_listener_thread_wakeup_, NULL);
  LOG4CXX_INFO(logger_, "TransportManager object created");
}

TransportManagerImpl::~TransportManagerImpl() {
  LOG4CXX_INFO(logger_, "TransportManager object destroyed");

  for (std::vector<TransportAdapter*>::iterator it =
           transport_adapters_.begin();
       it != transport_adapters_.end(); ++it) {
    delete *it;
  }

  for (std::map<TransportAdapter*, TransportAdapterListenerImpl*>::iterator it =
           transport_adapter_listeners_.begin();
       it != transport_adapter_listeners_.end(); ++it) {
    delete it->second;
  }

  pthread_mutex_destroy(&message_queue_mutex_);
  pthread_cond_destroy(&message_queue_cond_);
  pthread_mutex_destroy(&event_queue_mutex_);
  pthread_cond_destroy(&device_listener_thread_wakeup_);
}

int TransportManagerImpl::ConnectDevice(const DeviceHandle& device_handle) {
  LOG4CXX_INFO(logger_, "Connect device called with arguments device_handle "
                            << device_handle);
  if (!this->is_initialized_) {
    LOG4CXX_ERROR(logger_, "TransportManager is not initialized.");
    return E_TM_IS_NOT_INITIALIZED;
  }

  DeviceUID device_id = converter_.HandleToUid(device_handle);
  LOG4CXX_INFO(logger_, "Convert handle to id " << device_id);

  transport_adapter::TransportAdapter* ta = device_to_adapter_map_[device_id];
  if (NULL == ta) {
    LOG4CXX_ERROR(logger_, "No device adapter found by id " << device_id);
    return E_INVALID_HANDLE;
  }
  transport_adapter::DeviceList dl = ta->GetDeviceList();
  transport_adapter::DeviceList::iterator it =
      std::find(dl.begin(), dl.end(), device_id);
  if (it == dl.end()) {
    LOG4CXX_INFO(logger_, "Device with handle " << device_handle << " and id "
                                                << device_id
                                                << " is not found");
    return E_INVALID_HANDLE;
  }

  ApplicationList app_list = ta->GetApplicationList(device_id);
  LOG4CXX_INFO(logger_, "app_list.size() = " << app_list.size());
  for (ApplicationList::iterator it = app_list.begin(); it != app_list.end();
       ++it) {
    const ApplicationHandle& app_handle = *it;
    LOG4CXX_INFO(logger_, "Attempt to connect device "
                              << device_id << ", channel " << app_handle);
    const TransportAdapter::Error ta_error = ta->Connect(device_id, app_handle);
    switch (ta_error) {
      case TransportAdapter::OK: {
        LOG4CXX_INFO(logger_, "OK");
        break;
      }
      case TransportAdapter::ALREADY_EXISTS: {
        LOG4CXX_INFO(logger_, "Already connected");
        break;
      }
      default: {
        LOG4CXX_ERROR(logger_, "Connect to device "
                                   << device_id << ", channel " << app_handle
                                   << " failed with error " << ta_error);
        return E_INTERNAL_ERROR;
      }
    }
  }
  LOG4CXX_INFO(logger_, "Connect device exit");
  return E_SUCCESS;
}

int TransportManagerImpl::DisconnectDevice(const DeviceHandle& device_handle) {
  LOG4CXX_INFO(logger_, "Disconnect device called");
  if (!this->is_initialized_) {
    LOG4CXX_ERROR(logger_, "TransportManager is not initialized.");
    return E_TM_IS_NOT_INITIALIZED;
  }
  DeviceUID device_id = converter_.HandleToUid(device_handle);
  LOG4CXX_INFO(logger_, "Convert handle to id" << device_id);

  transport_adapter::TransportAdapter* ta = device_to_adapter_map_[device_id];
  if (NULL == ta) {
    LOG4CXX_ERROR(logger_, "No device adapter found by id " << device_id);
    return E_INVALID_HANDLE;
  }
  ta->DisconnectDevice(device_id);
  LOG4CXX_INFO(logger_, "Disconnected");
  return E_SUCCESS;
}

int TransportManagerImpl::Disconnect(const ConnectionUID& cid) {
  if (!this->is_initialized_) {
    LOG4CXX_ERROR(logger_, "TransportManager is not initialized.");
    return E_TM_IS_NOT_INITIALIZED;
  }

  ConnectionInternal* connection = GetConnection(cid);
  if (connection == NULL) {
    LOG4CXX_ERROR(
        logger_,
        "TransportManagerImpl::Disconnect: Connection does not exist.");
    return E_INVALID_HANDLE;
  }

  pthread_mutex_lock(&event_queue_mutex_);
  int messages_count = 0;
  for (EventQueue::const_iterator it = event_queue_.begin();
    it != event_queue_.end();
    ++it) {
    if (it->application_id() == cid) {
      ++messages_count;
    }
  }
  pthread_mutex_unlock(&event_queue_mutex_);

  if (messages_count > 0) {
    connection->messages_count = messages_count;
    connection->shutDown = true;
    connection->timer->start(
      profile::Profile::instance()->transport_manager_disconnect_timeout()
    );
  } else {
    connection->transport_adapter->Disconnect(connection->device,
                                              connection->application);
  }
  return E_SUCCESS;
}

int TransportManagerImpl::DisconnectForce(const ConnectionUID& cid) {
  if (false == this->is_initialized_) {
    LOG4CXX_ERROR(logger_, "TransportManager is not initialized.");
    return E_TM_IS_NOT_INITIALIZED;
  }
  pthread_mutex_lock(&event_queue_mutex_);
  // Clear messages for this connection
  // Note that MessageQueue typedef is assumed to be std::list,
  // or there is a problem here. One more point versus typedefs-everywhere
  MessageQueue::iterator e = message_queue_.begin();
  while (e != message_queue_.end()) {
    if ((*e)->connection_key() == cid) {
      RaiseEvent(&TransportManagerListener::OnTMMessageSendFailed,
                 DataSendTimeoutError(), *e);
      e = message_queue_.erase(e);
    } else {
      ++e;
    }
  }
  pthread_mutex_unlock(&event_queue_mutex_);
  const ConnectionInternal* connection = GetConnection(cid);
  if (connection == NULL) {
    LOG4CXX_ERROR(
        logger_,
        "TransportManagerImpl::DisconnectForce: Connection does not exist.");
    return E_INVALID_HANDLE;
  }
  connection->transport_adapter->Disconnect(connection->device,
                                            connection->application);
  return E_SUCCESS;
}

int TransportManagerImpl::AddEventListener(TransportManagerListener* listener) {
  transport_manager_listener_.push_back(listener);
  return E_SUCCESS;
}

int TransportManagerImpl::Stop() {
  if (!all_thread_active_) return E_TM_IS_NOT_INITIALIZED;

  all_thread_active_ = false;

  pthread_mutex_lock(&event_queue_mutex_);
  pthread_cond_signal(&device_listener_thread_wakeup_);
  pthread_mutex_unlock(&event_queue_mutex_);

  pthread_mutex_lock(&message_queue_mutex_);
  pthread_cond_signal(&message_queue_cond_);
  pthread_mutex_unlock(&message_queue_mutex_);

  pthread_join(message_queue_thread_, 0);
  pthread_join(event_queue_thread_, 0);

  LOG4CXX_INFO(logger_, "TransportManager object stopped");

  return E_SUCCESS;
}

int TransportManagerImpl::SendMessageToDevice(const RawMessageSptr message) {
  LOG4CXX_INFO(logger_, "Send message to device called with arguments "
                            << message.get());
  if (false == this->is_initialized_) {
    LOG4CXX_ERROR(logger_, "TM is not initialized.");
    return E_TM_IS_NOT_INITIALIZED;
  }

  const ConnectionInternal* connection =
      GetConnection(message->connection_key());
  if (connection == NULL) {
    LOG4CXX_ERROR(logger_, "Connection with id " << message->connection_key()
                                                 << " does not exist.");
    return E_INVALID_HANDLE;
  }

  if (connection->shutDown) {
    LOG4CXX_ERROR(
        logger_,
        "TransportManagerImpl::Disconnect: Connection is to shut down.");
    return E_CONNECTION_IS_TO_SHUTDOWN;
  }

  this->PostMessage(message);
  LOG4CXX_INFO(logger_, "Message posted");
  return E_SUCCESS;
}

int TransportManagerImpl::ReceiveEventFromDevice(
    const TransportAdapterEvent& event) {
  if (false == this->is_initialized_) {
    LOG4CXX_ERROR(logger_, "TM is not initialized.");
    return E_TM_IS_NOT_INITIALIZED;
  }
  this->PostEvent(event);
  return E_SUCCESS;
}

int TransportManagerImpl::RemoveDevice(const DeviceHandle& device_handle) {
  DeviceUID device_id = converter_.HandleToUid(device_handle);
  if (false == this->is_initialized_) {
    LOG4CXX_ERROR(logger_, "TM is not initialized.");
    return E_TM_IS_NOT_INITIALIZED;
  }
  device_to_adapter_map_.erase(device_id);
  return E_SUCCESS;
}

int TransportManagerImpl::AddTransportAdapter(
    transport_adapter::TransportAdapter* transport_adapter) {
  LOG4CXX_INFO(logger_, "Add device adapter "
                            << transport_adapter << "["
                            << transport_adapter->GetDeviceType() << "]");

  if (transport_adapter_listeners_.find(transport_adapter) !=
          transport_adapter_listeners_.end()) {
      LOG4CXX_ERROR(logger_, "Adapter already exists.");
      return E_ADAPTER_EXISTS;
  }
  transport_adapter_listeners_[transport_adapter] =
      new TransportAdapterListenerImpl(this, transport_adapter);
  transport_adapter->AddListener(
      transport_adapter_listeners_[transport_adapter]);

  if (transport_adapter->IsInitialised() ||
      transport_adapter->Init() == TransportAdapter::OK) {
    transport_adapters_.push_back(transport_adapter);
  }

  return E_SUCCESS;
}

int TransportManagerImpl::SearchDevices(void) {
  if (!this->is_initialized_) {
    LOG4CXX_ERROR(logger_, "TM is not initialized");
    return E_TM_IS_NOT_INITIALIZED;
  }

  LOG4CXX_INFO(logger_, "Search device called");

  for (std::vector<TransportAdapter*>::iterator it =
           transport_adapters_.begin();
       it != transport_adapters_.end(); ++it) {
    LOG4CXX_INFO(logger_, "Iterating over transport adapters");
    TransportAdapter::Error scanResult = (*it)->SearchDevices();
    if (scanResult != transport_adapter::TransportAdapter::OK) {
      LOG4CXX_ERROR(logger_, "Transport Adapter search failed "
                                 << *it << "[" << (*it)->GetDeviceType()
                                 << "]");
      switch (scanResult) {
        case transport_adapter::TransportAdapter::NOT_SUPPORTED: {
          LOG4CXX_ERROR(logger_, "Search feature is not supported "
                                     << *it << "[" << (*it)->GetDeviceType()
                                     << "]");
          break;
        }
        case transport_adapter::TransportAdapter::BAD_STATE: {
          LOG4CXX_ERROR(logger_, "Transport Adapter has bad state "
                                     << *it << "[" << (*it)->GetDeviceType()
                                     << "]");
          break;
        }
      }
      // TODO(KKolodiy): I think return error from TA is bad
      return scanResult;
    }
  }
  LOG4CXX_INFO(logger_, "SearchDevices() function is complete");
  return E_SUCCESS;
}

int TransportManagerImpl::Init(void) {
  LOG4CXX_INFO(logger_, "Init is called");
  all_thread_active_ = true;

  int error_code =
      pthread_create(&message_queue_thread_, 0, &MessageQueueStartThread, this);

  if (0 != error_code) {
    LOG4CXX_ERROR(logger_,
                  "Message queue thread is not created exit with error code "
                      << error_code);
    return E_TM_IS_NOT_INITIALIZED;
  }

  error_code =
      pthread_create(&event_queue_thread_, 0, &EventListenerStartThread, this);

  if (0 != error_code) {
    LOG4CXX_ERROR(logger_,
                  "Event queue thread is not created exit with error code "
                      << error_code);
    return E_TM_IS_NOT_INITIALIZED;
  }

  is_initialized_ = true;
  LOG4CXX_INFO(logger_, "Init complete");
  return E_SUCCESS;
}

int TransportManagerImpl::Visibility(const bool& on_off) const {
  TransportAdapter::Error ret;

  LOG4CXX_INFO(logger_, "Visibility change requested to " << on_off);
  if (false == this->is_initialized_) {
    LOG4CXX_ERROR(logger_, "TM is not initialized");
    return E_TM_IS_NOT_INITIALIZED;
  }

  for (std::vector<TransportAdapter*>::const_iterator it =
           transport_adapters_.begin();
       it != transport_adapters_.end(); ++it) {
    if (on_off) {
      ret = (*it)->StartClientListening();
    } else {
      ret = (*it)->StopClientListening();
    }
    if (TransportAdapter::Error::NOT_SUPPORTED == ret) {
      LOG4CXX_INFO(logger_, "Visibility change is not supported for adapter "
                                << *it << "[" << (*it)->GetDeviceType() << "]");
    }
  }

  LOG4CXX_INFO(logger_, "Visibility change requested complete");
  return E_SUCCESS;
}

void TransportManagerImpl::UpdateDeviceList(TransportAdapter* ta) {
  std::set<DeviceInfo> old_devices;
  for (DeviceList::iterator it = device_list_.begin();
       it != device_list_.end();) {
    if (it->first == ta) {
      old_devices.insert(it->second);
      it = device_list_.erase(it);
    } else {
      ++it;
    }
  }

  std::set<DeviceInfo> new_devices;
  const transport_adapter::DeviceList dev_list = ta->GetDeviceList();
  for (transport_adapter::DeviceList::const_iterator it = dev_list.begin();
       it != dev_list.end(); ++it) {
    DeviceHandle device_handle = converter_.UidToHandle(*it);
    DeviceInfo info(device_handle, *it, ta->DeviceName(*it));
    device_list_.push_back(std::make_pair(ta, info));
    new_devices.insert(info);
  }

  std::set<DeviceInfo> added_devices;
  std::set_difference(new_devices.begin(), new_devices.end(),
    old_devices.begin(), old_devices.end(),
    std::inserter(added_devices, added_devices.begin()));
  for(std::set<DeviceInfo>::const_iterator it = added_devices.begin();
    it != added_devices.end();
    ++it) {
    RaiseEvent(&TransportManagerListener::OnDeviceAdded, *it);
  }

  std::set<DeviceInfo> removed_devices;
  std::set_difference(old_devices.begin(), old_devices.end(),
    new_devices.begin(), new_devices.end(),
    std::inserter(removed_devices, removed_devices.begin()));

  for(std::set<DeviceInfo>::const_iterator it = removed_devices.begin();
    it != removed_devices.end();
    ++it) {
    RaiseEvent(&TransportManagerListener::OnDeviceRemoved, *it);
  }
}

void TransportManagerImpl::PostMessage(const RawMessageSptr message) {
  LOG4CXX_INFO(logger_, "Post message called serial number " << message.get());

#ifdef USE_RWLOCK
  pthread_rwlock_wrlock(&message_queue_rwlock_);
#else
  pthread_mutex_lock(&message_queue_mutex_);
#endif
  message_queue_.push_back(message);
  pthread_cond_signal(&message_queue_cond_);
#ifdef USE_RWLOCK
  pthread_rwlock_unlock(&message_queue_rwlock_);
#else
  pthread_mutex_unlock(&message_queue_mutex_);
#endif
  LOG4CXX_INFO(logger_, "Post message complete");
}

void TransportManagerImpl::RemoveMessage(const RawMessageSptr message) {
  // TODO: Reconsider necessity of this method, remove if it's useless,
  //       make to work otherwise.
  //       2013-08-21 dchmerev@luxoft.com
  LOG4CXX_INFO(logger_, "Remove message called " << message.get());
#ifdef USE_RWLOCK
  pthread_rwlock_wrlock(&message_queue_rwlock_);
#else
  pthread_mutex_lock(&message_queue_mutex_);
#endif
  std::remove(message_queue_.begin(), message_queue_.end(), message);
#ifdef USE_RWLOCK
  pthread_rwlock_unlock(&message_queue_rwlock_);
#else
  pthread_mutex_unlock(&message_queue_mutex_);
#endif
  LOG4CXX_INFO(logger_, "Remove message from queue complete");
}

void TransportManagerImpl::PostEvent(const TransportAdapterEvent& event) {
#ifdef USE_RWLOCK
  pthread_rwlock_wrlock(&event_queue_rwlock_);
#else
  pthread_mutex_lock(&event_queue_mutex_);
#endif
  event_queue_.push_back(event);
  pthread_cond_signal(&device_listener_thread_wakeup_);
#ifdef USE_RWLOCK
  pthread_rwlock_unlock(&event_queue_rwlock_);
#else
  pthread_mutex_unlock(&event_queue_mutex_);
#endif
}

void* TransportManagerImpl::EventListenerStartThread(void* data) {
  if (NULL != data) {
    static_cast<TransportManagerImpl*>(data)->EventListenerThread();
  }
  return 0;
}

void TransportManagerImpl::AddConnection(const ConnectionInternal& c) {
  connections_.push_back(c);
  incoming_data_handler_->AddConnection(c.id);
}

void TransportManagerImpl::RemoveConnection(int id) {
  for (std::vector<ConnectionInternal>::iterator it = connections_.begin();
       it != connections_.end(); ++it) {
    if (it->id == id) {
      connections_.erase(it);
      break;
    }
  }
  incoming_data_handler_->RemoveConnection(id);
}

TransportManagerImpl::ConnectionInternal* TransportManagerImpl::GetConnection(
    const ConnectionUID& id) {
  for (std::vector<ConnectionInternal>::iterator  it = connections_.begin(); it != connections_.end(); ++it) {
    if (it->id == id) {
      return &*it;
    }
  }
  return NULL;
}

TransportManagerImpl::ConnectionInternal* TransportManagerImpl::GetConnection(
    const DeviceUID& device, const ApplicationHandle& application) {
  for (std::vector<ConnectionInternal>::iterator it = connections_.begin(); it != connections_.end(); ++it) {
    if (it->device == device && it->application == application) {
      return &*it;
    }
  }
  return NULL;
}

// TODO this function should be moved outside of TM to protocol handler or
// somewhere else
unsigned int TransportManagerImpl::GetPacketSize(unsigned int data_size,
                                                 unsigned char* first_bytes) {
  DCHECK(first_bytes);
  unsigned char offset = sizeof(uint32_t);
  if (data_size < 2 * offset) {
    LOG4CXX_ERROR(logger_, "Received bytes are not enough to parse fram size.");
    return 0;
  }

  unsigned char* received_bytes = first_bytes;
  DCHECK(received_bytes);

  unsigned char version = received_bytes[0] >> 4u;
  uint32_t frame_body_size = received_bytes[offset++] << 24u;
  frame_body_size |= received_bytes[offset++] << 16u;
  frame_body_size |= received_bytes[offset++] << 8u;
  frame_body_size |= received_bytes[offset++];

  unsigned int required_size = frame_body_size;
  switch (version) {
    case protocol_handler::PROTOCOL_VERSION_1:
      required_size += protocol_handler::PROTOCOL_HEADER_V1_SIZE;
      break;
    case protocol_handler::PROTOCOL_VERSION_2:
      required_size += protocol_handler::PROTOCOL_HEADER_V2_SIZE;
      break;
    default:
      LOG4CXX_ERROR(logger_, "Unknown protocol version.");
      return 0;
  }

  return required_size;
}

void TransportManagerImpl::OnDeviceListUpdated(TransportAdapter* ta) {
  const transport_adapter::DeviceList device_list = ta->GetDeviceList();
  LOG4CXX_INFO(logger_, "DEVICE_LIST_UPDATED " << device_list.size());
  for (transport_adapter::DeviceList::const_iterator it = device_list.begin();
       it != device_list.end(); ++it) {
    device_to_adapter_map_.insert(std::make_pair(*it, ta));
    DeviceHandle device_handle = converter_.UidToHandle(*it);
    DeviceInfo info(device_handle, *it, ta->DeviceName(*it));
    RaiseEvent(&TransportManagerListener::OnDeviceFound, info);
  }
  UpdateDeviceList(ta);
  std::vector<DeviceInfo> device_infos;
  for (DeviceList::const_iterator it = device_list_.begin();
       it != device_list_.end(); ++it) {
    device_infos.push_back(it->second);
  }
  RaiseEvent(&TransportManagerListener::OnDeviceListUpdated, device_infos);
}

void TransportManagerImpl::EventListenerThread(void) {
#ifndef USE_RWLOCK
  pthread_mutex_lock(&event_queue_mutex_);
#endif
  LOG4CXX_INFO(logger_, "Event listener thread started");
  while (true) {
    while (event_queue_.size() > 0) {
#ifdef USE_RWLOCK
      pthread_rwlock_rdlock(&event_queue_rwlock_);
#endif
      LOG4CXX_INFO(logger_, "Event listener queue pushed to process events");
      EventQueue::iterator current = event_queue_.begin();
      TransportAdapter* ta = current->transport_adapter();
      ApplicationHandle app_handle = current->application_id();
      DeviceUID device_id = current->device_uid();
      DeviceHandle device_handle;
      BaseError* error = current->event_error();
      RawMessageSptr data = current->data();

      int event_type = current->event_type();
      event_queue_.erase(current);
#ifdef USE_RWLOCK
      pthread_rwlock_unlock(&event_queue_rwlock_);
#else
      pthread_mutex_unlock(&event_queue_mutex_);
#endif
      transport_adapter::DeviceList dev_list;
      ConnectionInternal* connection = GetConnection(device_id, app_handle);
      std::vector<DeviceInfo>::iterator device_info_iterator;

      switch (event_type) {
        case TransportAdapterListenerImpl::EventTypeEnum::ON_SEARCH_DONE: {
          LOG4CXX_INFO(logger_, "Event ON_SEARCH_DONE");
          RaiseEvent(&TransportManagerListener::OnScanDevicesFinished);
          break;
        }
        case TransportAdapterListenerImpl::EventTypeEnum::ON_SEARCH_FAIL: {
          LOG4CXX_INFO(logger_, "Event ON_SEARCH_FAIL");
          // error happened in real search process (external error)
          RaiseEvent(&TransportManagerListener::OnScanDevicesFailed,
                     *static_cast<SearchDeviceError*>(error));
          break;
        }
        case TransportAdapterListenerImpl::EventTypeEnum::ON_DEVICE_LIST_UPDATED
            : {
          OnDeviceListUpdated(ta);
          break;
        }
        case TransportAdapterListenerImpl::EventTypeEnum::ON_CONNECT_DONE: {
          LOG4CXX_INFO(logger_, "Event ON_CONNECT_DONE");
          AddConnection(ConnectionInternal(this, ta, ++connection_id_counter_,
                                           device_id, app_handle));
          device_handle = converter_.UidToHandle(device_id);
          RaiseEvent(
              &TransportManagerListener::OnConnectionEstablished,
              DeviceInfo(device_handle, device_id, ta->DeviceName(device_id)),
              connection_id_counter_);
          break;
        }
        case TransportAdapterListenerImpl::EventTypeEnum::ON_CONNECT_FAIL: {
          LOG4CXX_INFO(logger_, "Event ON_CONNECT_FAIL");
          RaiseEvent(&TransportManagerListener::OnConnectionFailed,
                     DeviceInfo(converter_.UidToHandle(device_id), device_id,
                                ta->DeviceName(device_id)),
                     ConnectError());
          break;
        }
        case TransportAdapterListenerImpl::EventTypeEnum::ON_DISCONNECT_DONE: {
          LOG4CXX_INFO(logger_, "Event ON_DISCONNECT_DONE");
          if (connection == NULL) {
            LOG4CXX_ERROR(logger_, "Connection ('" << device_id << ", "
                                                   << app_handle
                                                   << ") not found");
            break;
          }
          RaiseEvent(&TransportManagerListener::OnConnectionClosed,
                     connection->id);
          RemoveConnection(connection->id);
          break;
        }
        case TransportAdapterListenerImpl::EventTypeEnum::ON_DISCONNECT_FAIL: {
          LOG4CXX_INFO(logger_, "Event ON_DISCONNECT_FAIL");
          device_handle = converter_.UidToHandle(device_id);
          RaiseEvent(&TransportManagerListener::OnDisconnectFailed,
                     device_handle, DisconnectDeviceError());
          break;
        }
        case TransportAdapterListenerImpl::EventTypeEnum::ON_SEND_DONE: {
          LOG4CXX_INFO(logger_, "Event ON_SEND_DONE");
          if (connection == NULL) {
            LOG4CXX_ERROR(logger_, "Connection ('" << device_id << ", "
                                                   << app_handle
                                                   << ") not found");
            break;
          }
          RaiseEvent(&TransportManagerListener::OnTMMessageSend, data);
          this->RemoveMessage(data);
          if (connection->shutDown && --connection->messages_count == 0) {
            connection->timer->stop();
            connection->transport_adapter->Disconnect(connection->device,
                                                      connection->application);
          }
          break;
        }
        case TransportAdapterListenerImpl::EventTypeEnum::ON_SEND_FAIL: {
          LOG4CXX_INFO(logger_, "Event ON_SEND_FAIL");
          if (connection == NULL) {
            LOG4CXX_ERROR(logger_, "Connection ('" << device_id << ", "
                                                   << app_handle
                                                   << ") not found");
            break;
          }

          // TODO(YK): start timer here to wait before notify caller
          // and remove unsent messages
          LOG4CXX_ERROR(logger_, "Transport adapter failed to send data");
          // TODO(YK): potential error case -> thread unsafe
          // update of message content
          data->set_waiting(true);
          if (data.valid()) {
            data->set_waiting(true);
          } else {
            LOG4CXX_ERROR(logger_, "Data is invalid");
          }
          break;
        }
        case TransportAdapterListenerImpl::EventTypeEnum::ON_RECEIVED_DONE: {
          LOG4CXX_INFO(logger_, "Event ON_RECEIVED_DONE");
          if (connection == NULL) {
            LOG4CXX_ERROR(logger_, "Connection ('" << device_id << ", "
                                                   << app_handle
                                                   << ") not found");
            break;
          }
          const bool ok = incoming_data_handler_->ProcessData(
              connection->id, data->data(), data->data_size());
          if (!ok) {
            LOG4CXX_ERROR(
                logger_,
                "Incoming data processing failed. Terminating connection.");
            DisconnectForce(connection->id);
          }
          break;
        }
        case TransportAdapterListenerImpl::EventTypeEnum::ON_RECEIVED_FAIL: {
          LOG4CXX_INFO(logger_, "Event ON_RECEIVED_FAIL");
          if (connection == NULL) {
            LOG4CXX_ERROR(logger_, "Connection ('" << device_id << ", "
                                                   << app_handle
                                                   << ") not found");
            break;
          }

          RaiseEvent(&TransportManagerListener::OnTMMessageReceiveFailed,
                     connection->id, *static_cast<DataReceiveError*>(error));
          break;
        }
        case TransportAdapterListenerImpl::EventTypeEnum::ON_COMMUNICATION_ERROR
            : {
          LOG4CXX_INFO(logger_, "Event ON_COMMUNICATION_ERROR");
          break;
        }
        case TransportAdapterListenerImpl::EventTypeEnum::
            ON_UNEXPECTED_DISCONNECT: {
          LOG4CXX_INFO(logger_, "Event ON_UNEXPECTED_DISCONNECT");
          if (connection) {
            RaiseEvent(&TransportManagerListener::OnUnexpectedDisconnect,
                       connection->id,
                       *static_cast<CommunicationError*>(error));
          } else {
            LOG4CXX_ERROR(logger_, "Connection ('" << device_id << ", "
                                                   << app_handle
                                                   << ") not found");
          }
          break;
        }
      }  // switch
      delete error;
#ifndef USE_RWLOCK
      pthread_mutex_lock(&event_queue_mutex_);
#endif
    }  // while (event_queue_.size() > 0)

    if (all_thread_active_)
      pthread_cond_wait(&device_listener_thread_wakeup_, &event_queue_mutex_);
    else
      break;
  }  // while (true)

  pthread_mutex_unlock(&event_queue_mutex_);

  LOG4CXX_INFO(logger_, "Event listener thread finished");
}
void* TransportManagerImpl::MessageQueueStartThread(void* data) {
  if (NULL != data) {
    static_cast<TransportManagerImpl*>(data)->MessageQueueThread();
  }
  return 0;
}

void TransportManagerImpl::MessageQueueThread(void) {
  LOG4CXX_INFO(logger_, "Message queue thread started");

#ifndef USE_RWLOCK
  pthread_mutex_lock(&message_queue_mutex_);
#endif

  while (all_thread_active_) {
    // TODO(YK): add priority processing

    while (message_queue_.size() > 0) {
#ifdef USE_RWLOCK
      pthread_rwlock_rdlock(&message_queue_rwlock_);
#endif
      MessageQueue::iterator it = message_queue_.begin();
      while (it != message_queue_.end() && it->valid() && (*it)->IsWaiting()) {
        ++it;
      }
      if (it == message_queue_.end()) {
#ifdef USE_RWLOCK
        pthread_rwlock_unlock(&message_queue_rwlock_);
#endif
        break;
      }
      RawMessageSptr active_msg = *it;
#ifdef USE_RWLOCK
      pthread_rwlock_unlock(&message_queue_rwlock_);
#else
      pthread_mutex_unlock(&message_queue_mutex_);
#endif
      if (active_msg.valid() && !active_msg->IsWaiting()) {
        ConnectionInternal* connection =
            GetConnection(active_msg->connection_key());
        if (connection == NULL) {
          std::stringstream ss;
          ss << "Connection " << active_msg->connection_key() << " not found";
          LOG4CXX_ERROR(logger_, ss.str());
          RaiseEvent(&TransportManagerListener::OnTMMessageSendFailed,
                     DataSendError(ss.str()), active_msg);
          message_queue_.remove(active_msg);
          continue;
        }
        TransportAdapter* transport_adapter = connection->transport_adapter;
        LOG4CXX_INFO(logger_, "Got adapter "
                                  << transport_adapter << "["
                                  << transport_adapter->GetDeviceType() << "]"
                                  << " by session id "
                                  << active_msg->connection_key());

        if (NULL == transport_adapter) {
          std::string error_text =
              "Transport adapter is not found - message removed";
          LOG4CXX_ERROR(logger_, error_text);
          RaiseEvent(&TransportManagerListener::OnTMMessageSendFailed,
                     DataSendError(error_text), active_msg);
          message_queue_.remove(active_msg);
        } else {
          if (TransportAdapter::OK ==
              transport_adapter->SendData(
                  connection->device, connection->application, active_msg)) {
            LOG4CXX_INFO(logger_, "Data sent to adapter");
            active_msg->set_waiting(true);
          } else {
            LOG4CXX_ERROR(logger_, "Data sent error");
	    RaiseEvent(&TransportManagerListener::OnTMMessageSendFailed,
	             DataSendError("Send failed - message removed"),
		     active_msg);
            message_queue_.remove(active_msg);
          }
        }
      }
#ifndef USE_RWLOCK
      pthread_mutex_lock(&message_queue_mutex_);
#endif
    }
    pthread_cond_wait(&message_queue_cond_, &message_queue_mutex_);
  }  //  while(true)

  message_queue_.clear();

  pthread_mutex_unlock(&message_queue_mutex_);
  LOG4CXX_INFO(logger_, "Message queue thread finished");
}

}  // namespace transport_manager


