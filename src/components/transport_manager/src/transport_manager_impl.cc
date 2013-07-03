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

#include <queue>
#include <pthread.h>
#include <algorithm>

#include "utils/macro.h"
#include "protocol_handler/raw_message.h"
#include "transport_manager/transport_manager_impl.h"
#include "transport_manager/transport_manager_listener.h"
#include "transport_manager/transport_manager_listener_impl.h"
#include "transport_manager/device_adapter_listener_impl.h"
#include "transport_manager/device_handle_generator_impl.h"

namespace transport_manager {

log4cxx::LoggerPtr TransportManagerImpl::logger_ = log4cxx::LoggerPtr(
    log4cxx::Logger::getLogger("TransportManager"));

TransportManager::~TransportManager() {
}
;

TransportManagerImpl::TransportManagerImpl()
    : message_queue_mutex_(),
      all_thread_active_(false),
      adapter_handler_(),
      messsage_queue_thread_(),
      event_queue_thread_(),
      device_listener_thread_wakeup_(),
      transport_manager_listener_(),
      device_adapter_listener_(),
      device_handle_generator_(0),
      is_initialized_(false) {

  pthread_mutex_init(&message_queue_mutex_, 0);
  pthread_mutex_init(&event_queue_mutex_, 0);
  pthread_cond_init(&device_listener_thread_wakeup_, NULL);
  device_adapter_listener_ = new DeviceAdapterListenerImpl(this);
//todo: uncoment when adapter is ready
//	DeviceAdapter *d = new BluetoothAdapter();
//	d->init(device_adapter_listener_, NULL);
//	addDeviceAdapter(d);
  //addDeviceAdapter(new TCPAdapter());

}

TransportManagerImpl::TransportManagerImpl(DeviceAdapter *device_adapter)
    : message_queue_mutex_(),
      all_thread_active_(false),
      adapter_handler_(),
      messsage_queue_thread_(),
      event_queue_thread_(),
      device_listener_thread_wakeup_(),
      transport_manager_listener_(),
      device_adapter_listener_(),
      device_handle_generator_(0),
      is_initialized_(false) {

  pthread_mutex_init(&message_queue_mutex_, 0);
  pthread_mutex_init(&event_queue_mutex_, 0);
  pthread_cond_init(&device_listener_thread_wakeup_, NULL);
  device_adapter_listener_ = new DeviceAdapterListenerImpl(this);
  device_handle_generator_ = new DeviceHandleGeneratorImpl();
  device_adapter->init(device_adapter_listener_, device_handle_generator_,
                       NULL);
  addDeviceAdapter(device_adapter);
}

//todo: more constructors

TransportManagerImpl::~TransportManagerImpl() {

  pthread_cond_signal(&device_listener_thread_wakeup_);
  all_thread_active_ = false;
  pthread_join(messsage_queue_thread_, 0);
  pthread_join(event_queue_thread_, 0);
  pthread_mutex_destroy(&message_queue_mutex_);
  pthread_mutex_destroy(&event_queue_mutex_);
  pthread_cond_destroy(&device_listener_thread_wakeup_);
  delete transport_manager_listener_;
  delete device_adapter_listener_;
  delete device_handle_generator_;
}

TransportManagerImpl* TransportManagerImpl::instance() {
  static TransportManagerImpl instance;
  if (false == instance.is_initialized_) {
    instance.init();
  }
  return &instance;
}

void TransportManagerImpl::connectDevice(const DeviceHandle &device_id,
                                         const ApplicationHandle &app_id,
                                         const SessionID &session_id) {
  if (false == this->is_initialized_) {
    //todo: log error
    return;
  }
  DeviceAdapter *da = adapter_handler_.getAdapterByDevice(device_id);
  if (NULL == da) {
    //error case
    return;
  }
  if (DeviceAdapter::Error::OK != da->connect(device_id, app_id, session_id)) {
    //error case
    return;
  }
}

void TransportManagerImpl::disconnectDevice(const SessionID &session_id) {
  if (false == this->is_initialized_) {
    //todo: log error
    return;
  }
  adapter_handler_.getAdapterBySession(session_id)->disconnect(session_id);
}

void TransportManagerImpl::registerEventListener(
    TransportManagerListener *listener) {
  this->set_transport_manager_listener(listener);
}

void TransportManagerImpl::registerAdapterListener(
    DeviceAdapterListener *listener) {
  this->set_device_adapter_listener(listener);
}

void TransportManagerImpl::sendMessageToDevice(
    const protocol_handler::RawMessage &message) {
  if (false == this->is_initialized_) {
    //todo: log error
    return;
  }
  this->postMessage(message);
}

void TransportManagerImpl::receiveEventFromDevice(
    const DeviceAdapterListener::DeviceAdapterEvent &event) {
  if (false == this->is_initialized_) {
    //todo: log error
    return;
  }
  this->postEvent(event);
}
void TransportManagerImpl::removeDevice(const DeviceHandle &device) {
  adapter_handler_.removeDevice(device);
}

void TransportManagerImpl::addDeviceAdapter(DeviceAdapter *device_adapter) {
  adapter_handler_.addAdapter(device_adapter);
}

void TransportManagerImpl::searchDevices(void) {
  if (false == this->is_initialized_) {
    //todo: log error
    return;
  }
  for (AdapterHandler::AdapterList::iterator da = adapter_handler_
      .device_adapters().begin();
      da != adapter_handler_.device_adapters().end(); ++da) {
    DeviceAdapter::Error err = (*da)->searchDevices();
    if (DeviceAdapter::Error::OK != err) {
      //todo: notify error
    }
  }
}

void TransportManagerImpl::init(void) {

  int error_code = pthread_create(&messsage_queue_thread_, 0,
                                  &messageQueueStartThread, this);

  if (0 == error_code) {
  } else {
    return;
  }

  error_code = pthread_create(&event_queue_thread_, 0,
                              &eventListenerStartThread, this);

  if (0 == error_code) {
  } else {
    return;
  }
  all_thread_active_ = true;
  is_initialized_ = true;
}

void TransportManagerImpl::postMessage(
    const protocol_handler::RawMessage &message) {
  //todo: check data copying
  protocol_handler::RawMessage msg(message.connection_key(),
                                   msg.protocol_version(), msg.serial_number(),
                                   msg.data(), msg.data_size());
  pthread_mutex_lock(&message_queue_mutex_);
  message_queue_.push_back(msg);
  pthread_mutex_unlock(&message_queue_mutex_);
}

void TransportManagerImpl::updateMessage(
    const protocol_handler::RawMessage &message) {
  pthread_mutex_lock(&message_queue_mutex_);
  //todo: define and implement
  pthread_mutex_unlock(&message_queue_mutex_);

}

void TransportManagerImpl::removeMessage(
    const protocol_handler::RawMessage &message) {
  pthread_mutex_lock(&message_queue_mutex_);
  for (MessageQueue::iterator it = message_queue_.begin();
      it != message_queue_.begin(); ++it) {
    if ((*it) == message) {
      //todo: add correct removing
    }
  }
  pthread_mutex_unlock(&message_queue_mutex_);
}

void TransportManagerImpl::removeEvent(
    const DeviceAdapterListenerImpl::DeviceAdapterEvent &event) {
  pthread_mutex_lock(&event_queue_mutex_);
  for (EventQueue::iterator it = event_queue_.begin();
      it != event_queue_.begin(); ++it) {
    if ((*it) == event) {
      //todo: add correct removing
    }
  }
  pthread_mutex_unlock(&event_queue_mutex_);
}

void TransportManagerImpl::postEvent(
    const DeviceAdapterListenerImpl::DeviceAdapterEvent &event) {
  DeviceAdapterListenerImpl::DeviceAdapterEvent evt(event.event_type(),
                                                    event.session_id(),
                                                    event.device_adapter(),
                                                    event.data(),
                                                    event.error());
  pthread_mutex_lock(&event_queue_mutex_);
  event_queue_.push_back(evt);
  pthread_mutex_unlock(&event_queue_mutex_);
}

void TransportManagerImpl::set_device_adapter_listener(
    DeviceAdapterListener *listener) {
  device_adapter_listener_ = listener;
}

void TransportManagerImpl::set_transport_manager_listener(
    TransportManagerListener *listener) {
  transport_manager_listener_ = listener;
}

void *TransportManagerImpl::eventListenerStartThread(void *data) {
  if (NULL != data) {
    static_cast<TransportManagerImpl *>(data)->eventListenerThread();
  }
  return 0;
}

void TransportManagerImpl::eventListenerThread(void) {

  while (true == all_thread_active_) {

    pthread_mutex_lock(&event_queue_mutex_);
    pthread_cond_wait(&device_listener_thread_wakeup_, &event_queue_mutex_);

    for (std::vector<DeviceAdapterListenerImpl::DeviceAdapterEvent>::iterator it =
        event_queue_.begin(); it != event_queue_.end(); ++it) {

      //todo: check that data is copied correctly here
      DeviceAdapter *da = (*it).device_adapter();
      SessionID sid = (*it).session_id();
      SearchDeviceError *srch_err;
      DeviceList dev_list;
      RawMessageSptr data;
      DataReceiveError *d_err;
      switch ((*it).event_type()) {
        case DeviceAdapterListenerImpl::EventTypeEnum::ON_SEARCH_DONE:
          dev_list = da->getDeviceList();
          for (DeviceList::iterator item = dev_list.begin();
              item != dev_list.end(); ++item) {
            adapter_handler_.addDevice(da, (*item));
            transport_manager_listener_->onSearchDeviceDone(
                (*item), da->getApplicationList((*item)));
          }
          break;
        case DeviceAdapterListenerImpl::EventTypeEnum::ON_SEARCH_FAIL:
          //error happened in real search process (external error)
          srch_err = static_cast<SearchDeviceError *>((*it).error());
          transport_manager_listener_->onSearchDeviceFailed(da, *srch_err);
          break;
        case DeviceAdapterListenerImpl::EventTypeEnum::ON_CONNECT_DONE:
          adapter_handler_.addSession((*it).device_adapter(),
                                      (*it).session_id());
          transport_manager_listener_->onConnectDone(da, sid);
          break;
        case DeviceAdapterListenerImpl::EventTypeEnum::ON_CONNECT_FAIL:
          break;
        case DeviceAdapterListenerImpl::EventTypeEnum::ON_DISCONNECT_DONE:
          adapter_handler_.removeSession((*it).device_adapter(),
                                         (*it).session_id());
          break;
        case DeviceAdapterListenerImpl::EventTypeEnum::ON_DISCONNECT_FAIL:
          break;
        case DeviceAdapterListenerImpl::EventTypeEnum::ON_SEND_DONE:
          data = ((*it).data());
          this->removeMessage(*data);
          break;
        case DeviceAdapterListenerImpl::EventTypeEnum::ON_SEND_FAIL:
          //todo: start timer here to wait before notify caller and remove unsent messages
          break;
        case DeviceAdapterListenerImpl::EventTypeEnum::ON_RECEIVED_DONE:
          data = (*it).data();
          transport_manager_listener_->onDataReceiveDone(da, sid, data);
          break;
        case DeviceAdapterListenerImpl::EventTypeEnum::ON_RECEIVED_FAIL:
          d_err = static_cast<DataReceiveError *>((*it).error());
          transport_manager_listener_->onDataReceiveFailed(da, sid, *d_err);
          break;
        case DeviceAdapterListenerImpl::EventTypeEnum::ON_COMMUNICATION_ERROR:
          break;
      }
      this->removeEvent((*it));
    }  //for
    pthread_mutex_unlock(&event_queue_mutex_);

  }  //end while true

  event_queue_.clear();
}

void *TransportManagerImpl::messageQueueStartThread(void *data) {
  if (NULL != data) {
    static_cast<TransportManagerImpl *>(data)->messageQueueThread();
  }
  return 0;
}

void TransportManagerImpl::messageQueueThread(void) {

  while (true == all_thread_active_) {
    //todo: add priority processing

    u_int serial_number = 0;
    protocol_handler::RawMessage *active_msg = NULL;
    pthread_mutex_lock(&message_queue_mutex_);
    for (std::vector<protocol_handler::RawMessage>::iterator msg =
        message_queue_.begin(); msg != message_queue_.end(); ++msg) {
      if ((*msg).serial_number() > serial_number) {
        active_msg = &(*msg);
        serial_number = (*msg).serial_number();
      }
    }
    if (NULL != active_msg) {
      RawMessageSptr msg_to_send(
          new protocol_handler::RawMessage(active_msg->connection_key(),
                                           active_msg->protocol_version(),
                                           active_msg->serial_number(),
                                           active_msg->data(),
                                           active_msg->data_size()));

      DeviceAdapter *device_adapter = adapter_handler_.getAdapterBySession(
          active_msg->connection_key());

      if (NULL == device_adapter) {
        //probably error no device adapters found
      } else {
        device_adapter->sendData(active_msg->connection_key(), msg_to_send);
      }
    }
    pthread_mutex_unlock(&message_queue_mutex_);
  }  //while(true)

  message_queue_.clear();

}

pthread_cond_t *TransportManagerImpl::getDeviceListenerThreadWakeup(void) {
  return &device_listener_thread_wakeup_;
}

DeviceAdapter *TransportManagerImpl::AdapterHandler::getAdapterBySession(
    SessionID session_id) {
  std::map<SessionID, DeviceAdapter *>::iterator da = session_to_adapter_map_
      .find(session_id);
  if (da != session_to_adapter_map_.begin()) {
    return (*da).second;
  }
  return NULL;
}

TransportManagerImpl::AdapterHandler::AdapterHandler() {

}

TransportManagerImpl::AdapterHandler::~AdapterHandler() {
  for (AdapterList::iterator it = device_adapters_.begin();
      it != device_adapters_.end(); ++it) {
    delete (*it);
  }
  device_adapters_.clear();
  session_to_adapter_map_.clear();
  device_to_adapter_multimap_.clear();
}

DeviceAdapter *TransportManagerImpl::AdapterHandler::getAdapterByDevice(
    DeviceHandle device_id) {
  std::map<DeviceHandle, DeviceAdapter *>::iterator da =
      device_to_adapter_multimap_.find(device_id);
  if (da != device_to_adapter_multimap_.end()) {
    return (*da).second;
  }
  return NULL;
}
void TransportManagerImpl::AdapterHandler::addAdapter(
    DeviceAdapter *device_adapter) {
  device_adapters_.push_back(device_adapter);
}

TransportManagerImpl::AdapterHandler::AdapterList TransportManagerImpl::AdapterHandler::device_adapters(
    void) {
  return device_adapters_;
}
void TransportManagerImpl::AdapterHandler::addSession(DeviceAdapter *da,
                                                      SessionID sid) {
  AdapterList::iterator item = std::find(device_adapters_.begin(),
                                         device_adapters_.end(), da);
  if (item == device_adapters_.end()) {
    //todo: error case: unknown adapter
    return;
  }
  session_to_adapter_map_.insert(std::make_pair(sid, da));
}

void TransportManagerImpl::AdapterHandler::removeSession(DeviceAdapter *da,
                                                         SessionID sid) {
  session_to_adapter_map_.erase(sid);
}

void TransportManagerImpl::AdapterHandler::addDevice(DeviceAdapter *da,
                                                     DeviceHandle did) {
  AdapterList::iterator item = std::find(device_adapters_.begin(),
                                         device_adapters_.end(), da);
  if (item == device_adapters_.end()) {
    //todo: error case: unknown adapter
    return;
  }
  device_to_adapter_multimap_.insert(std::make_pair(did, da));
}

void TransportManagerImpl::AdapterHandler::removeDevice(
    const DeviceHandle &device) {
  device_to_adapter_multimap_.erase(device);
}

void TransportManagerImpl::acceptConnect(const DeviceHandle &device_id,
                                         const ApplicationHandle &app_id,
                                         const SessionID &session_id) {
//todo: implement it
}

void TransportManagerImpl::declineConnect(const DeviceHandle &device_id,
                                          const ApplicationHandle &app_id) {
//todo: implement it

}

}  //namespace

