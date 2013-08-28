/**
 * \file transport_manager_impl.cpp
 * \brief TransportManager class source file.
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
#include <cstring>
#include <queue>
#include <algorithm>
#include <limits>
#include <functional>

#include "utils/macro.h"
#include "protocol_handler/raw_message.h"
#include "transport_manager/transport_manager_impl.h"
#include "transport_manager/transport_manager_listener.h"
#include "transport_manager/transport_manager_listener_impl.h"
#include "transport_manager/device_adapter/device_adapter_listener_impl.h"
#include "transport_manager/timer.h"
#include "transport_manager/bluetooth/bluetooth_adapter.h"
#include "transport_manager/tcp/tcp_adapter.h"
#include "transport_manager/device_adapter/device_adapter.h"

using ::transport_manager::device_adapter::DeviceAdapter;
using ::transport_manager::device_adapter::DeviceAdapterSptr;

namespace transport_manager {

log4cxx::LoggerPtr TransportManagerImpl::logger_ = log4cxx::LoggerPtr(
      log4cxx::Logger::getLogger("TransportManager"));

TransportManagerImpl::TransportManagerImpl(const TransportManagerAttr& config)
  : message_queue_mutex_(),
    all_thread_active_(false),
    messsage_queue_thread_(),
    event_queue_thread_(),
    device_listener_thread_wakeup_(),
    is_initialized_(false),
    connection_id_counter_(0),
    config_(config),
    da_scanned(0),
    protocol_handler_(nullptr),
    search_in_progress_(false) {

  LOG4CXX_INFO(logger_, "==============================================")
  pthread_mutex_init(&message_queue_mutex_, nullptr);
  pthread_cond_init(&message_queue_cond, nullptr);
  pthread_mutex_init(&event_queue_mutex_, 0);
  pthread_cond_init(&device_listener_thread_wakeup_, NULL);
  LOG4CXX_INFO(logger_, "TransportManager object created")
}

TransportManagerImpl::~TransportManagerImpl() {
  all_thread_active_ = false;

  pthread_mutex_lock(&event_queue_mutex_);
  pthread_cond_signal(&device_listener_thread_wakeup_);
  pthread_mutex_unlock(&event_queue_mutex_);

  pthread_mutex_lock(&message_queue_mutex_);
  pthread_cond_signal(&message_queue_cond);
  pthread_mutex_unlock(&message_queue_mutex_);

  pthread_join(messsage_queue_thread_, 0);
  pthread_join(event_queue_thread_, 0);

  pthread_mutex_destroy(&message_queue_mutex_);
  pthread_cond_destroy(&message_queue_cond);
  pthread_mutex_destroy(&event_queue_mutex_);
  pthread_cond_destroy(&device_listener_thread_wakeup_);

  for (std::map<DeviceAdapterSptr, DeviceAdapterListenerImpl*>::iterator it =
         device_adapter_listeners_.begin(); it != device_adapter_listeners_.end();
       ++it) {
    delete it->second;
  }
  LOG4CXX_INFO(logger_, "TransportManager object destroyed")
}

std::vector<TransportManagerImpl::Connection> TransportManagerImpl::getConnectionList() {
  std::vector<TransportManagerImpl::Connection> rc(connections_.size());
  std::transform(connections_.begin(), connections_.end(), rc.begin(),
  [&](ConnectionInternal & p) {
    Connection c;
    c.application = p.application;
    c.device = p.device;
    c.id = p.id;
    return c;
  });
  return rc;
}

int TransportManagerImpl::connectDevice(const DeviceHandle& device_handle) {
  LOG4CXX_INFO(
    logger_,
    "Connect device called with arguments device_handle " << device_handle)
  if (!this->is_initialized_) {
    LOG4CXX_ERROR(logger_, "TransportManager is not initialized.")
    return E_TM_IS_NOT_INITIALIZED;
  }

  DeviceUID device_id = converter_.HandleToUid(device_handle);
  LOG4CXX_INFO(logger_, "Convert handle to id " << device_id)

  device_adapter::DeviceAdapterSptr da = device_to_adapter_map_[device_id];
  if (!da.valid()) {
    LOG4CXX_ERROR(logger_, "No device adapter found by id " << device_id)
    return E_INVALID_HANDLE;
  }
  device_adapter::DeviceList dl = da->getDeviceList();
  device_adapter::DeviceList::iterator it = std::find(dl.begin(), dl.end(),
      device_id);
  if (it == dl.end()) {
    LOG4CXX_INFO(
      logger_,
      "Device with handle " << device_handle << " and id " << device_id << " is not found");
    return E_INVALID_HANDLE;
  }

  ApplicationList app_list = da->getApplicationList(device_id);
  LOG4CXX_INFO(logger_, "app_list.size() = " << app_list.size())
  for (ApplicationList::iterator it = app_list.begin(); it != app_list.end();
       ++it) {
    if (NULL != getConnection(device_id, *it)) {
      LOG4CXX_ERROR(logger_, "Connect application failed E_CONNECTION_EXISTS")
      return E_CONNECTION_EXISTS;
    } else {
      LOG4CXX_ERROR(logger_, "attempt to connect device")
      if (device_adapter::DeviceAdapter::OK != da->connect(device_id, *it)) {
        LOG4CXX_ERROR(logger_, "Connect application failed E_INTERNAL_ERROR")
        return E_INTERNAL_ERROR;
      }
    }
  }
  LOG4CXX_INFO(logger_, "Connect device exit")
  return E_SUCCESS;
}

int TransportManagerImpl::disconnectDevice(const DeviceHandle& device_handle) {
  LOG4CXX_INFO(logger_, "Disconnect device called")
  if (!this->is_initialized_) {
    LOG4CXX_ERROR(logger_, "TransportManager is not initialized.")
    return E_TM_IS_NOT_INITIALIZED;
  }
  DeviceUID device_id = converter_.HandleToUid(device_handle);
  LOG4CXX_INFO(logger_, "Convert handle to id" << device_id)

  device_adapter::DeviceAdapterSptr da = device_to_adapter_map_[device_id];
  if (!da.valid()) {
    LOG4CXX_ERROR(logger_, "No device adapter found by id " << device_id)
    return E_INVALID_HANDLE;
  }
  da->disconnectDevice(device_id);
  LOG4CXX_INFO(logger_, "Disconnected")
  return E_SUCCESS;
}

void TransportManagerImpl::disconnectFailedRoutine(void* p) {
  void** param = static_cast<void**>(p);
  TransportManagerImpl* tm = static_cast<TransportManagerImpl*>(param[0]);
  ConnectionInternal* c = static_cast<ConnectionInternal*>(param[1]);
  LOG4CXX_INFO(logger_, "Disconnection failed")
  tm->raiseEvent(&TransportManagerListener::OnDisconnectFailed,
                 tm->converter_.UidToHandle(c->device),
                 DisconnectDeviceError());
  c->shutDown = false;
  c->timer.stop();
  delete[] param;
}

int TransportManagerImpl::disconnect(const ConnectionUID& cid) {
  if (!this->is_initialized_) {
    LOG4CXX_ERROR(logger_, "TransportManager is not initialized.")
    return E_TM_IS_NOT_INITIALIZED;
  }

  ConnectionInternal* connection = getConnection(cid);
  if (connection == NULL) {
    LOG4CXX_ERROR(
      logger_, "TransportManagerImpl::disconnect: Connection does not exist.")
    return E_INVALID_HANDLE;
  }

  pthread_mutex_lock(&event_queue_mutex_);
  int messages_count = 0;
  for (auto e : event_queue_) {
    if (e.application_id() == cid) {
      ++messages_count;
    }
  }
  pthread_mutex_unlock(&event_queue_mutex_);

  if (messages_count > 0) {
    connection->messages_count = messages_count;
    connection->shutDown = true;
    void** param = new void*[2];
    param[0] = this;
    param[1] = &connection;
    Timer timer(config_.disconnectTimeout, &disconnectFailedRoutine, &param, true);
    connection->timer = timer;
    timer.start();
  } else {
    connection->device_adapter->disconnect(connection->device,
                                           connection->application);
  }
  return E_SUCCESS;
}

int TransportManagerImpl::disconnectForce(const ConnectionUID& cid) {
  if (false == this->is_initialized_) {
    LOG4CXX_ERROR(logger_, "TransportManager is not initialized.")
    return E_TM_IS_NOT_INITIALIZED;
  }
  pthread_mutex_lock(&event_queue_mutex_);
  // Clear messages for this connection
  // Note that MessageQueue typedef is assumed to be std::list,
  // or there is a problem here. One more point versus typedefs-everywhere
  auto e = message_queue_.begin();
  while (e != message_queue_.end()) {
    if ((*e)->connection_key() == cid) {
      raiseEvent(&TransportManagerListener::OnTMMessageSendFailed,
                 DataSendTimeoutError(), *e);
      e = message_queue_.erase(e);
    } else {
      ++e;
    }
  }
  pthread_mutex_unlock(&event_queue_mutex_);
  const ConnectionInternal* connection = getConnection(cid);
  if (connection == NULL) {
    LOG4CXX_ERROR(
      logger_,
      "TransportManagerImpl::disconnectForce: Connection does not exist.")
    return E_INVALID_HANDLE;
  }
  connection->device_adapter->disconnect(connection->device,
                                         connection->application);
  return E_SUCCESS;
}

int TransportManagerImpl::addEventListener(TransportManagerListener* listener) {
  transport_manager_listener_.push_back(listener);
  return E_SUCCESS;
}

int TransportManagerImpl::removeEventListener(
  TransportManagerListener* listener) {
  transport_manager_listener_.remove(listener);
  return E_SUCCESS;
}

int TransportManagerImpl::sendMessageToDevice(const RawMessageSptr message) {
  LOG4CXX_INFO(logger_,
               "Send message to device called with arguments " << message.get())
  if (false == this->is_initialized_) {
    LOG4CXX_ERROR(logger_, "TM is not initialized.")
    return E_TM_IS_NOT_INITIALIZED;
  }

  const ConnectionInternal* connection = getConnection(
      message->connection_key());
  if (connection == NULL) {
    LOG4CXX_ERROR(
      logger_,
      "Connection with id " << message->connection_key() << " does not exist.")
    return E_INVALID_HANDLE;
  }

  if (connection->shutDown) {
    LOG4CXX_ERROR(
      logger_,
      "TransportManagerImpl::disconnect: Connection is to shut down.")
    return E_CONNECTION_IS_TO_SHUTDOWN;
  }

  this->postMessage(message);
  LOG4CXX_INFO(logger_, "Message posted")
  return E_SUCCESS;
}

int TransportManagerImpl::receiveEventFromDevice(
  const DeviceAdapterEvent& event) {
  if (false == this->is_initialized_) {
    LOG4CXX_ERROR(logger_, "TM is not initialized.")
    return E_TM_IS_NOT_INITIALIZED;
  }
  this->postEvent(event);
  return E_SUCCESS;
}

int TransportManagerImpl::removeDevice(const DeviceHandle& device_handle) {
  DeviceUID device_id = converter_.HandleToUid(device_handle);
  if (false == this->is_initialized_) {
    LOG4CXX_ERROR(logger_, "TM is not initialized.")
    return E_TM_IS_NOT_INITIALIZED;
  }
  device_to_adapter_map_.erase(device_id);
  return E_SUCCESS;
}

int TransportManagerImpl::addDeviceAdapter(
  device_adapter::DeviceAdapterSptr device_adapter) {
  LOG4CXX_INFO(
    logger_,
    "Add device adapter " << device_adapter.get() << "[" << device_adapter->getDeviceType() << "]");

  device_adapter_listeners_[device_adapter] = new DeviceAdapterListenerImpl(
    this);
  device_adapter->addListener(device_adapter_listeners_[device_adapter]);

  if (device_adapter->isInitialised()
      || device_adapter->init() == DeviceAdapter::OK) {
    device_adapters_.push_back(device_adapter);
  }

  return E_SUCCESS;
}

int TransportManagerImpl::removeDeviceAdapter(
  device_adapter::DeviceAdapterSptr device_adapter) {

  std::vector<device_adapter::DeviceAdapterSptr>::iterator it;
  it = std::find(device_adapters_.begin(), device_adapters_.end(),
                 device_adapter);
  if (it != device_adapters_.end()) {
    std::map<DeviceAdapterSptr, DeviceAdapterListenerImpl*>::iterator it_listener;
    it_listener = device_adapter_listeners_.find(*it);
    if (it_listener != device_adapter_listeners_.end()) {
      (*it)->removeListener(it_listener->second);
      delete it_listener->second;
      device_adapter_listeners_.erase(it_listener);
    }
    device_adapters_.erase(it);
  }
  return E_SUCCESS;
}

int TransportManagerImpl::SearchDevices(void) {
  if (!this->is_initialized_) {
    LOG4CXX_ERROR(logger_, "TM is not initialized")
    return E_TM_IS_NOT_INITIALIZED;
  }

  if (search_in_progress_) {
    return E_SEARCH_IN_PROGRESS;
  }
  da_scanned = 0;
  search_in_progress_ = true;
  device_list_.clear();
  LOG4CXX_INFO(logger_, "Search device called")

  for (std::vector<DeviceAdapterSptr>::iterator it = device_adapters_.begin();
       it != device_adapters_.end(); ++it) {
    LOG4CXX_INFO(logger_, "Iterating over transport adapters");
    int scanResult = (*it)->SearchDevices();
    if (scanResult != device_adapter::DeviceAdapter::OK) {
      ++da_scanned;
      LOG4CXX_ERROR(
        logger_,
        "Transport Adapter search failed " << (*it).get() << "[" << (*it)->getDeviceType() << "]")
      switch (scanResult) {
        case device_adapter::DeviceAdapter::NOT_SUPPORTED:
          LOG4CXX_ERROR(
            logger_,
            "Search feature is not supported " << it->get() << "[" << (*it)->getDeviceType() << "]")
          break;
        case device_adapter::DeviceAdapter::BAD_STATE:
          LOG4CXX_ERROR(
            logger_,
            "Transport Adapter has bad state " << it->get() << "[" << (*it)->getDeviceType() << "]")
          break;
      }
      return scanResult;
    }
  }
  LOG4CXX_INFO(logger_, "SearchDevices() function is complete")
  return E_SUCCESS;
}

int TransportManagerImpl::init(void) {
  LOG4CXX_INFO(logger_, "Init is called")
  all_thread_active_ = true;

  pthread_mutex_lock(&message_queue_mutex_);
  int error_code = pthread_create(&messsage_queue_thread_, 0,
                                  &messageQueueStartThread, this);
  // Wait while thread starts loop
  pthread_mutex_lock(&message_queue_mutex_);
  pthread_mutex_unlock(&message_queue_mutex_);

  if (0 != error_code) {
    LOG4CXX_ERROR(
      logger_,
      "Message queue thread is not created exit with error code " << error_code)
    return E_TM_IS_NOT_INITIALIZED;
  }

  pthread_mutex_lock(&event_queue_mutex_);
  error_code = pthread_create(&event_queue_thread_, 0,
                              &eventListenerStartThread, this);
  // Wait while thread starts loop
  pthread_mutex_lock(&event_queue_mutex_);
  pthread_mutex_unlock(&event_queue_mutex_);

  if (0 != error_code) {
    LOG4CXX_ERROR(
      logger_,
      "Event queue thread is not created exit with error code " << error_code)
    return E_TM_IS_NOT_INITIALIZED;
  }

  is_initialized_ = true;
  LOG4CXX_INFO(logger_, "Init complete")
  return E_SUCCESS;
}

int TransportManagerImpl::Visibility(const bool& on_off) const {
  bool ret;

  LOG4CXX_INFO(logger_, "Visibility change requested to " << on_off)
  if (false == this->is_initialized_) {
    LOG4CXX_ERROR(logger_, "TM is not initialized")
    return E_TM_IS_NOT_INITIALIZED;
  }

  for (std::vector<DeviceAdapterSptr>::const_iterator it =
         device_adapters_.begin();
       it != device_adapters_.end(); ++it) {
    if (on_off) {
      ret = (*it)->startClientListening();
    } else {
      ret = (*it)->stopClientListening();
    }
    if (DeviceAdapter::Error::NOT_SUPPORTED == ret) {
      LOG4CXX_INFO(
        logger_,
        "Visibility change is not supported for adapter " << (*it).get() << "[" << (*it)->getDeviceType() << "]");
    }
  }

  LOG4CXX_INFO(logger_, "Visibility change requested complete")
  return E_SUCCESS;
}

const std::vector<DeviceInfo>& TransportManagerImpl::getDeviceList() const {
  return device_list_;
}

void TransportManagerImpl::postMessage(const RawMessageSptr message) {
  LOG4CXX_INFO(logger_, "Post message called serial number " << message.get())

  pthread_mutex_lock(&message_queue_mutex_);
  message_queue_.push_back(message);
  pthread_cond_signal(&message_queue_cond);
  pthread_mutex_unlock(&message_queue_mutex_);
  LOG4CXX_INFO(logger_, "Post message complete")
}

void TransportManagerImpl::removeMessage(const RawMessageSptr message) {
  // TODO: Reconsider necessity of this method, remove if it's useless,
  //       make to work otherwise.
  //       2013-08-21 dchmerev@luxoft.com
  LOG4CXX_INFO(logger_, "Remove message called " << message.get())
  pthread_mutex_lock(&message_queue_mutex_);
  std::remove(message_queue_.begin(), message_queue_.end(), message);
  pthread_mutex_unlock(&message_queue_mutex_);
  LOG4CXX_INFO(logger_, "Remove message from queue complete")
}

void TransportManagerImpl::removeEvent(const DeviceAdapterEvent& event) {
  LOG4CXX_INFO(logger_, "Remove event from queue called")
  pthread_mutex_lock(&event_queue_mutex_);
  std::remove(event_queue_.begin(), event_queue_.end(), event);
  pthread_cond_signal(&device_listener_thread_wakeup_);
  pthread_mutex_unlock(&event_queue_mutex_);
  LOG4CXX_INFO(logger_, "Remove event from queue complete")
}

void TransportManagerImpl::postEvent(const DeviceAdapterEvent& event) {
  pthread_mutex_lock(&event_queue_mutex_);
  event_queue_.push_back(event);
  pthread_cond_signal(&device_listener_thread_wakeup_);
  pthread_mutex_unlock(&event_queue_mutex_);
}

void* TransportManagerImpl::eventListenerStartThread(void* data) {
  if (NULL != data) {
    static_cast<TransportManagerImpl*>(data)->eventListenerThread();
  }
  return 0;
}

void TransportManagerImpl::addConnection(const ConnectionInternal& c) {
  connections_.push_back(c);
}

void TransportManagerImpl::removeConnection(int id) {
  for (std::vector<ConnectionInternal>::iterator it = connections_.begin();
       it != connections_.end(); ++it) {
    if (it->id == id) {
      connections_.erase(it);
      break;
    }
  }
}

TransportManagerImpl::ConnectionInternal* TransportManagerImpl::getConnection(
  const ConnectionUID& id) {
  for (auto it = connections_.begin(); it != connections_.end(); ++it) {
    if (it->id == id) {
      return &*it;
    }
  }
  return NULL;
}

TransportManagerImpl::ConnectionInternal* TransportManagerImpl::getConnection(
  const DeviceUID& device, const ApplicationHandle& application) {
  for (auto it = connections_.begin(); it != connections_.end(); ++it) {
    if (it->device == device && it->application == application) {
      return &*it;
    }
  }
  return NULL;
}

//YK: temp solution until B1.0 release - begin
void TransportManagerImpl::addDataToContainer(
  ConnectionUID id,
  std::map<ConnectionUID, std::pair<unsigned int, unsigned char*>>& container,
  unsigned char* data, unsigned int data_size) {
  unsigned int buff_size = container[id].first;
  unsigned char* buff = container[id].second;
  unsigned char* new_buff = new unsigned char[buff_size + data_size];
  memcpy(new_buff, buff, buff_size);
  memcpy(new_buff + buff_size, data, data_size);
  delete[] buff;
  container[id] = std::make_pair(buff_size + data_size, new_buff);

}

bool TransportManagerImpl::getFrameSize(unsigned char* data,
                                        unsigned int data_size,
                                        unsigned int& frame_size) {
  unsigned int magic_number = 2 * sizeof(uint32_t);
  if (data_size < magic_number) {
    return false;
  }
  frame_size = protocol_handler_->GetPacketSize(data_size, data);
  return true;
}
bool TransportManagerImpl::getFrame(
  std::map<ConnectionUID, std::pair<unsigned int, unsigned char*>>& container,
  ConnectionUID id, unsigned int frame_size, unsigned char** frame) {

  unsigned int buff_size = container[id].first;
  if (frame_size <= buff_size) {
    unsigned char* buff = container[id].second;
    *frame = new unsigned char[frame_size];
    memcpy(*frame, buff, frame_size);
    unsigned int new_buff_size = buff_size - frame_size;
    if (0 != new_buff_size) {
      unsigned char* new_buff = new unsigned char[new_buff_size];
      memcpy(new_buff, buff + frame_size, new_buff_size);
      container[id] = std::make_pair(new_buff_size, new_buff);
    } else {
      container.erase(id);
    }
    delete[] buff;
    return true;
  }
  return false;
}
//YK: temp solution until B1.0 release - end

void TransportManagerImpl::eventListenerThread(void) {
  //YK: temp solution until B1.0 release - begin
  bool frame_ready = true;
  bool size_ready = false;
  unsigned int frame_size = 0;
  unsigned char* frame = nullptr;
  bool is_new;
  std::map<ConnectionUID, std::pair<unsigned int, unsigned char*>> data_container;
  //YK: temp solution until B1.0 release - end

  LOG4CXX_INFO(logger_, "Event listener thread started")
  while (all_thread_active_) {
    pthread_cond_wait(&device_listener_thread_wakeup_, &event_queue_mutex_);
    while (event_queue_.size() > 0) {
      LOG4CXX_INFO(logger_, "Event listener queue pushed to process events")
      EventQueue::iterator current = event_queue_.begin();
      DeviceAdapterSptr da = current->device_adapter();
      ApplicationHandle app_handle = current->application_id();
      DeviceUID device_id = current->device_uid();
      DeviceHandle device_handle;
      BaseError* error = current->event_error();
      RawMessageSptr data = current->data();

      int event_type = current->event_type();
      event_queue_.erase(current);
      pthread_mutex_unlock(&event_queue_mutex_);

      device_adapter::DeviceList dev_list;
      ConnectionInternal* connection = getConnection(device_id, app_handle);
      std::vector<DeviceInfo>::iterator device_info_iterator;

      switch (event_type) {
        case DeviceAdapterListenerImpl::EventTypeEnum::ON_SEARCH_DONE:
          LOG4CXX_INFO(logger_, "Event ON_SEARCH_DONE")
          LOG4CXX_INFO(
            logger_,
            "Get device list for adapter " << da.get() << "[" << da->getDeviceType() << "]")
          dev_list = da->getDeviceList();
          da_scanned += 1;
          for (device_adapter::DeviceList::iterator item = dev_list.begin();
               item != dev_list.end(); ++item) {
            LOG4CXX_INFO(logger_, "Iterating over device list " << (*item))
            device_to_adapter_map_.insert(std::make_pair(*item, da));
            DeviceHandle device_handle = converter_.UidToHandle(*item);
            DeviceInfo info(device_handle, *item, da->DeviceName(*item));
            device_list_.push_back(info);
            raiseEvent(&TransportManagerListener::OnDeviceFound, info);

            LOG4CXX_INFO(logger_, "Callback called")
          }
          if (da_scanned == device_adapters_.size()) {
            raiseEvent(&TransportManagerListener::OnDeviceListUpdated, device_list_);
            if (0 == device_list_.size()) {
              LOG4CXX_INFO(logger_, "No device found event raised")
              raiseEvent(&TransportManagerListener::OnNoDeviceFound);
            }
            raiseEvent(&TransportManagerListener::OnScanDevicesFinished);
            search_in_progress_ = false;
          }
          break;
        case DeviceAdapterListenerImpl::EventTypeEnum::ON_SEARCH_FAIL: {
          LOG4CXX_INFO(logger_, "Event ON_SEARCH_FAIL");
          // error happened in real search process (external error)
          da_scanned += 1;
          raiseEvent(&TransportManagerListener::OnScanDevicesFailed,
                     *static_cast<SearchDeviceError*>(error));
          if (da_scanned == device_adapters_.size()) {
            raiseEvent(&TransportManagerListener::OnScanDevicesFinished);
            search_in_progress_ = false;
          }
        }
        break;
        case DeviceAdapterListenerImpl::EventTypeEnum::ON_CONNECT_DONE:
          LOG4CXX_INFO(logger_, "Event ON_CONNECT_DONE")
          connections_.push_back(
            ConnectionInternal(da, ++connection_id_counter_, device_id,
                               app_handle));
          device_handle = converter_.UidToHandle(device_id, is_new);
          if (is_new) {
            raiseEvent(
              &TransportManagerListener::OnDeviceFound,
              DeviceInfo(device_handle, device_id,
                         da->DeviceName(device_id)));
          }

          device_list_.push_back(DeviceInfo(device_handle, device_id,
                                            da->DeviceName(device_id)));
          raiseEvent(&TransportManagerListener::OnDeviceListUpdated, device_list_);
          raiseEvent(
            &TransportManagerListener::OnConnectionEstablished,
            DeviceInfo(device_handle, device_id, da->DeviceName(device_id)),
            connection_id_counter_);
          break;
        case DeviceAdapterListenerImpl::EventTypeEnum::ON_CONNECT_FAIL:
          LOG4CXX_INFO(logger_, "Event ON_CONNECT_FAIL")
          raiseEvent(&TransportManagerListener::OnConnectionFailed,
                     DeviceInfo(converter_.UidToHandle(device_id), device_id, da->DeviceName(device_id)),
                     ConnectError());
          break;
        case DeviceAdapterListenerImpl::EventTypeEnum::ON_DISCONNECT_DONE: {
          LOG4CXX_INFO(logger_, "Event ON_DISCONNECT_DONE");
          if (connection == NULL) {
            LOG4CXX_ERROR(
              logger_,
              "Connection ('" << device_id << ", " << app_handle << ") not found")
            break;
          }
          raiseEvent(&TransportManagerListener::OnConnectionClosed,
                     connection->id);
          removeConnection(connection->id);
        }
        break;
        case DeviceAdapterListenerImpl::EventTypeEnum::ON_DISCONNECT_FAIL: {
          LOG4CXX_INFO(logger_, "Event ON_DISCONNECT_FAIL")
          DeviceHandle device_handle = converter_.UidToHandle(device_id);
          raiseEvent(&TransportManagerListener::OnDisconnectFailed,
                     device_handle, DisconnectDeviceError());
        }
        break;
        case DeviceAdapterListenerImpl::EventTypeEnum::ON_SEND_DONE: {
          LOG4CXX_INFO(logger_, "Event ON_SEND_DONE");
          if (connection == NULL) {
            LOG4CXX_ERROR(
              logger_,
              "Connection ('" << device_id << ", " << app_handle << ") not found")
            break;
          }
          raiseEvent(&TransportManagerListener::OnTMMessageSend);
          this->removeMessage(data);
          if (connection->shutDown && --connection->messages_count == 0) {
            connection->timer.stop();
            connection->device_adapter->disconnect(connection->device,
                                                   connection->application);
          }
        }
        break;
        case DeviceAdapterListenerImpl::EventTypeEnum::ON_SEND_FAIL: {
          LOG4CXX_INFO(logger_, "Event ON_SEND_FAIL")
          if (connection == NULL) {
            LOG4CXX_ERROR(
              logger_,
              "Connection ('" << device_id << ", " << app_handle << ") not found")
            break;
          }

          // TODO(YK): start timer here to wait before notify caller
          // and remove unsent messages
          LOG4CXX_ERROR(logger_, "Device adapter failed to send data")
          // TODO(YK): potential error case -> thread unsafe
          // update of message content
          data->set_waiting(true);
          if (data.valid()) {
            data->set_waiting(true);
          } else {
            LOG4CXX_ERROR(logger_, "Data is invalid")
          }
        }
        break;
        case DeviceAdapterListenerImpl::EventTypeEnum::ON_RECEIVED_DONE:
          LOG4CXX_INFO(logger_, "Event ON_RECEIVED_DONE")
          if (connection == NULL) {
            LOG4CXX_ERROR(
              logger_,
              "Connection ('" << device_id << ", " << app_handle << ") not found");
            break;
          }
          data->set_connection_key(connection->id);
          //YK: temp solution until B1.0 release - begin
          if (!size_ready) {
            //get size only when last complete frame successfully sent to upper level
            this->addDataToContainer(connection->id, data_container,
                                     data->data(), data->data_size());
            if (!(size_ready = this->getFrameSize(data->data(),
                                                  data->data_size(), frame_size))) {
              //save data for future use because there is not enough data in current mesage to get frame size
              break;
            }
          } else {
            //if current frame is not complete - accumulate data from each new message
            this->addDataToContainer(connection->id, data_container,
                                     data->data(), data->data_size());
          }
          if (0 == frame_size) {
            LOG4CXX_ERROR(logger_,
                          "Unexpected zero size frame. Terminating connection.")
            this->disconnectForce(connection->id);
            size_ready = false;
            frame_ready = false;
          } else {
            //get all completed frames from buffer until incomplete frame reached
            frame_ready = this->getFrame(data_container, connection->id,
                                         frame_size, &frame);
          }
          while (frame_ready) {
            RawMessageSptr tmp_msg(
              new protocol_handler::RawMessage(data->connection_key(),
                                               data->protocol_version(),
                                               frame, frame_size));
            raiseEvent(&TransportManagerListener::OnTMMessageReceived, tmp_msg);
            delete[] frame;
            size_ready = this->getFrameSize(
                           data_container[connection->id].second,
                           data_container[connection->id].first, frame_size);
            if (0 == frame_size) {
              LOG4CXX_ERROR(
                logger_,
                "Unexpected zero size frame. Terminating connection.")
              this->disconnectForce(connection->id);
              size_ready = false;
            }
            if (size_ready) {
              frame_ready = this->getFrame(data_container, connection->id,
                                           frame_size, &frame);
            } else {
              frame_ready = false;
            }
          }
          //YK: temp solution until B1.0 release - end
          break;
        case DeviceAdapterListenerImpl::EventTypeEnum::ON_RECEIVED_FAIL: {
          LOG4CXX_INFO(logger_, "Event ON_RECEIVED_FAIL");
          if (connection == NULL) {
            LOG4CXX_ERROR(
              logger_,
              "Connection ('" << device_id << ", " << app_handle << ") not found")
            break;
          }

          raiseEvent(&TransportManagerListener::OnTMMessageReceiveFailed,
                     connection->id, *static_cast<DataReceiveError*>(error));
        }
        break;
        case DeviceAdapterListenerImpl::EventTypeEnum::ON_COMMUNICATION_ERROR:
          LOG4CXX_INFO(logger_, "Event ON_COMMUNICATION_ERROR")
          break;
        case DeviceAdapterListenerImpl::EventTypeEnum::ON_UNEXPECTED_DISCONNECT:
          LOG4CXX_INFO(logger_, "Event ON_UNEXPECTED_DISCONNECT")
          // Remove device from list
          for (auto it = device_list_.begin();
               it != device_list_.end();
               ++it) {
            if (it->device_handle() == device_handle) {
              device_list_.erase(it);
              break;
            }
          }
          raiseEvent(&TransportManagerListener::OnDeviceListUpdated, device_list_);
          break;
      }  // switch
      delete error;
      pthread_mutex_lock(&event_queue_mutex_);
    }  // while (event_queue_.size() > 0)
  }  // while (all_thread_active_)

  LOG4CXX_INFO(logger_, "Event listener thread finished")
}
void* TransportManagerImpl::messageQueueStartThread(void* data) {
  if (NULL != data) {
    static_cast<TransportManagerImpl*>(data)->messageQueueThread();
  }
  return 0;
}

void TransportManagerImpl::messageQueueThread(void) {
  LOG4CXX_INFO(logger_, "Message queue thread started")
  while (all_thread_active_) {
    // TODO(YK): add priority processing

    pthread_cond_wait(&message_queue_cond, &message_queue_mutex_);
    while (message_queue_.size() > 0) {
      MessageQueue::iterator it = message_queue_.begin();
      while (it != message_queue_.end() && it->valid() && (*it)->IsWaiting()) {
        ++it;
      }
      if (it == message_queue_.end()) {
        break;
      }
      RawMessageSptr active_msg = *it;
      pthread_mutex_unlock(&message_queue_mutex_);

      if (active_msg.valid() && !active_msg->IsWaiting()) {
        ConnectionInternal* connection = getConnection(
                                           active_msg->connection_key());
        if (connection == NULL) {
          std::stringstream ss;
          ss << "Connection " << active_msg->connection_key() << " not found";
          LOG4CXX_ERROR(logger_, ss.str());
          raiseEvent(&TransportManagerListener::OnTMMessageSendFailed,
                     DataSendError(ss.str()), active_msg);
          message_queue_.remove(active_msg);
          continue;
        }
        DeviceAdapterSptr device_adapter = connection->device_adapter;
        LOG4CXX_INFO(
          logger_,
          "Got adapter " << device_adapter.get() << "[" << device_adapter->getDeviceType() << "]" << " by session id " << active_msg->connection_key())

        if (!device_adapter.valid()) {
          std::string error_text = "Device adapter is not found - message removed";
          LOG4CXX_ERROR(logger_, error_text);
          raiseEvent(&TransportManagerListener::OnTMMessageSendFailed,
                     DataSendError(error_text), active_msg);
          message_queue_.remove(active_msg);
        } else {
          if (DeviceAdapter::OK
              == device_adapter->sendData(connection->device,
                                          connection->application,
                                          active_msg)) {
            LOG4CXX_INFO(logger_, "Data sent to adapter")
            active_msg->set_waiting(true);
          } else {
            LOG4CXX_ERROR(logger_, "Data sent error")
          }
        }
      }
      pthread_mutex_lock(&message_queue_mutex_);
    }
  }  //  while(true)

  message_queue_.clear();
  LOG4CXX_INFO(logger_, "Message queue thread finished")
}

void TransportManagerImpl::set_protocol_handler(
  protocol_handler::ProtocolHandler* ph) {
  //YK: temp solution until B1.0 release
  protocol_handler_ = ph;
}

}  // namespace transport_manager

