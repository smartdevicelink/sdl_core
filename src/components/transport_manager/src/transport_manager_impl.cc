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
#include <limits>

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
      is_initialized_(false) {

  LOG4CXX_INFO(logger_, "==============================================");
  pthread_mutex_init(&message_queue_mutex_, 0);
  pthread_mutex_init(&event_queue_mutex_, 0);
  pthread_cond_init(&device_listener_thread_wakeup_, NULL);
//todo: uncoment when adapter is ready
//	DeviceAdapter *d = new BluetoothAdapter();
//	d->init(device_adapter_listener_, NULL);
//	addDeviceAdapter(d);
  //addDeviceAdapter(new TCPAdapter());
  LOG4CXX_INFO(logger_, "TM object created.");
}

TransportManagerImpl::TransportManagerImpl(DeviceAdapter *device_adapter)
    : message_queue_mutex_(),
      all_thread_active_(false),
      adapter_handler_(),
      messsage_queue_thread_(),
      event_queue_thread_(),
      device_listener_thread_wakeup_(),
      transport_manager_listener_(),
      is_initialized_(false) {

  LOG4CXX_INFO(logger_, "==============================================");
  pthread_mutex_init(&message_queue_mutex_, 0);
  pthread_mutex_init(&event_queue_mutex_, 0);
  pthread_cond_init(&device_listener_thread_wakeup_, NULL);
  device_adapter->addListener(new DeviceAdapterListenerImpl(this));
  device_adapter->init(new DeviceHandleGeneratorImpl(),
                       NULL);
  addDeviceAdapter(device_adapter);
  LOG4CXX_INFO(logger_, "TM object with device adapter created.");
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
  //delete transport_manager_listener_;
  //delete device_adapter_listener_;
  LOG4CXX_INFO(logger_, "TM object destroyed.");
}

TransportManagerImpl* TransportManagerImpl::instance() {
  static TransportManagerImpl instance;
  if (false == instance.is_initialized_) {
    instance.init();
  }
  LOG4CXX_INFO(logger_, "TM instance returned " << &instance);
  return &instance;
}

void TransportManagerImpl::connectDevice(const DeviceHandle &device_id,
                                         const ApplicationHandle &app_id,
                                         const SessionID &session_id) {
  LOG4CXX_INFO(
      logger_,
      "Connect device called with arguments device_id " << device_id << " app_id " << app_id << " session_id " << session_id);

  if (false == this->is_initialized_) {
    //todo: log error
    LOG4CXX_ERROR(logger_, "TM is not initialized.");
    return;
  }
  DeviceAdapter *da = adapter_handler_.getAdapterByDevice(device_id);
  if (NULL == da) {
    //error case
    LOG4CXX_ERROR(logger_, "No device adapter found by id " << device_id);
    return;
  }
  if (DeviceAdapter::Error::OK != da->connect(device_id, app_id, session_id)) {
    LOG4CXX_ERROR(logger_, "Connect failed");
    //error case
    return;
  }
  LOG4CXX_INFO(logger_, "Connect complete");
}

void TransportManagerImpl::disconnectDevice(const SessionID &session_id) {
  LOG4CXX_INFO(logger_, "Disconnect device called");
  if (false == this->is_initialized_) {
    LOG4CXX_ERROR(logger_, "TM is not initialized.");
    //todo: log error
    return;
  }
  adapter_handler_.getAdapterBySession(session_id)->disconnect(session_id);
  LOG4CXX_INFO(logger_, "Disconnected");
}

void TransportManagerImpl::addEventListener(
    TransportManagerListener *listener) {
  transport_manager_listener_.push_back(listener);
}

void TransportManagerImpl::sendMessageToDevice(const RawMessageSptr message) {
  LOG4CXX_INFO(
      logger_,
      "Send message to device called with arguments serial number " << message->serial_number());
  if (false == this->is_initialized_) {
    LOG4CXX_ERROR(logger_, "TM is not initialized.");
    //todo: log error
    return;
  }
  this->postMessage(message);
  LOG4CXX_INFO(logger_, "Message posted");
}

void TransportManagerImpl::receiveEventFromDevice(
    const DeviceAdapterListener::DeviceAdapterEvent &event) {
  if (false == this->is_initialized_) {
    LOG4CXX_ERROR(logger_, "TM is not initialized.");
    //todo: log error
    return;
  }
  this->postEvent(event);
}
void TransportManagerImpl::removeDevice(const DeviceHandle &device) {
  adapter_handler_.removeDevice(device);
}

void TransportManagerImpl::addDeviceAdapter(DeviceAdapter *device_adapter) {
  LOG4CXX_INFO(logger_, "Add device adapter " << device_adapter);
  adapter_handler_.addAdapter(device_adapter);
}
void TransportManagerImpl::addAdapterListener(DeviceAdapter *adapter, DeviceAdapterListener *listener){
  LOG4CXX_INFO(logger_, "Add device adapter listener is called for adapter " << adapter << " listener " << listener);
  AdapterHandler::AdapterList al = const_cast<AdapterHandler::AdapterList &>(adapter_handler_.device_adapters());
  AdapterHandler::AdapterList::iterator it = std::find(al.begin(), al.end(), adapter);
  if(NULL == (*it)){
    LOG4CXX_ERROR(logger_, "Device adapter is not known");
    return;
  }
  (*it)->addListener(listener);
  LOG4CXX_INFO(logger_, "Add device adapter listener call complete");

}

void TransportManagerImpl::searchDevices(void) {
  LOG4CXX_INFO(logger_, "Search device called");
  if (false == this->is_initialized_) {
    LOG4CXX_ERROR(logger_, "TM is not initialized");
    //todo: log error
    return;
  }
  for (AdapterHandler::AdapterList::const_iterator it = adapter_handler_
      .device_adapters().begin();
      it != adapter_handler_.device_adapters().end(); ++it) {
    LOG4CXX_INFO(logger_, "Iterating over device adapters");
    if (DeviceAdapter::Error::OK != (*it)->searchDevices()) {
      LOG4CXX_ERROR(logger_, "Device adapter search failed");
      //todo: notify error
    }
  }
  LOG4CXX_INFO(logger_, "Search complete");
}

void TransportManagerImpl::init(void) {
  LOG4CXX_INFO(logger_, "Init is called");
  int error_code = pthread_create(&messsage_queue_thread_, 0,
                                  &messageQueueStartThread, this);

  all_thread_active_ = true;
  if (0 == error_code) {
  } else {
    LOG4CXX_ERROR(
        logger_,
        "Message queue thread is not created exit with error code " << error_code);
    return;
  }

  error_code = pthread_create(&event_queue_thread_, 0,
                              &eventListenerStartThread, this);

  if (0 == error_code) {
  } else {
    LOG4CXX_ERROR(
        logger_,
        "Event queue thread is not created exit with error code " << error_code);
    return;
  }
  is_initialized_ = true;
  LOG4CXX_INFO(logger_, "Init complete");
}

void TransportManagerImpl::postMessage(const RawMessageSptr message) {
  //todo: check data copying
  LOG4CXX_INFO(logger_,
               "Post message called serial number " << message->serial_number());
  pthread_mutex_lock(&message_queue_mutex_);
  //todo: check that same message is not posted again
  message_queue_.push_back(message);
  pthread_mutex_unlock(&message_queue_mutex_);
  LOG4CXX_INFO(logger_, "Post message complete");
}

/* not clear when this function shall be used
 void TransportManagerImpl::updateMessage(const RawMessageSptr old_message, const RawMessageSptr new_message) {
 pthread_mutex_lock(&message_queue_mutex_);
 pthread_mutex_unlock(&message_queue_mutex_);

 }
 */

void TransportManagerImpl::removeMessage(const RawMessageSptr message) {
  LOG4CXX_INFO(logger_, "Remove message called " << message->serial_number());
  pthread_mutex_lock(&message_queue_mutex_);
  std::remove(message_queue_.begin(), message_queue_.end(), message);
  pthread_mutex_unlock(&message_queue_mutex_);
  LOG4CXX_INFO(logger_, "Remove message from queue complete");
}

void TransportManagerImpl::removeEvent(
    const DeviceAdapterListenerImpl::DeviceAdapterEvent &event) {
  LOG4CXX_INFO(logger_, "Remove event from queue called");
  pthread_mutex_lock(&event_queue_mutex_);
  std::remove(event_queue_.begin(), event_queue_.end(), event);
  pthread_mutex_unlock(&event_queue_mutex_);
  LOG4CXX_INFO(logger_, "Remove event from queue complete");
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

void *TransportManagerImpl::eventListenerStartThread(void *data) {
  if (NULL != data) {
    static_cast<TransportManagerImpl *>(data)->eventListenerThread();
  }
  return 0;
}

void TransportManagerImpl::eventListenerThread(void) {

  LOG4CXX_INFO(logger_, "Event listener thread started");
  while (true == all_thread_active_) {

    pthread_mutex_lock(&event_queue_mutex_);
    pthread_cond_wait(&device_listener_thread_wakeup_, &event_queue_mutex_);

    for (EventQueue::iterator it = event_queue_.begin();
        it != event_queue_.end(); ++it) {

      //todo: check that data is copied correctly here
      DeviceAdapter *da = (*it).device_adapter();
      SessionID sid = (*it).session_id();
      SearchDeviceError *srch_err;
      DeviceList dev_list;
      RawMessageSptr data;
      DataReceiveError *d_err;
      LOG4CXX_INFO(
          logger_,
          "Iterating over event queue items session id" << sid << "type " << (*it).event_type());
      switch ((*it).event_type()) {
        case DeviceAdapterListenerImpl::EventTypeEnum::ON_SEARCH_DONE:
          LOG4CXX_INFO(logger_, "Get device list for adapter " << da)
          ;
          dev_list = da->getDeviceList();
          for (DeviceList::iterator item = dev_list.begin();
              item != dev_list.end(); ++item) {
            LOG4CXX_INFO(logger_, "Iterating over device list " << (*item));
            adapter_handler_.addDevice(da, (*item));
            for (TransportManagerListenerList::iterator dal_it =
                transport_manager_listener_.begin();
                dal_it != transport_manager_listener_.end(); ++dal_it) {
              (*dal_it)->onSearchDeviceDone((*item),
                                            da->getApplicationList((*item)));
            }
            LOG4CXX_INFO(logger_, "Callback called");
          }
          break;
        case DeviceAdapterListenerImpl::EventTypeEnum::ON_SEARCH_FAIL:
          //error happened in real search process (external error)
          srch_err = static_cast<SearchDeviceError *>((*it).error());
          for (TransportManagerListenerList::iterator dal_it =
              transport_manager_listener_.begin();
              dal_it != transport_manager_listener_.end(); ++dal_it) {
            (*dal_it)->onSearchDeviceFailed(da, *srch_err);
          }
          break;
        case DeviceAdapterListenerImpl::EventTypeEnum::ON_CONNECT_DONE:
          adapter_handler_.addSession((*it).device_adapter(),
                                      (*it).session_id());
          for (TransportManagerListenerList::iterator dal_it =
              transport_manager_listener_.begin();
              dal_it != transport_manager_listener_.end(); ++dal_it) {
            (*dal_it)->onConnectDone(da, sid);
          }
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
          this->removeMessage(data);
          break;
        case DeviceAdapterListenerImpl::EventTypeEnum::ON_SEND_FAIL:
          //todo: start timer here to wait before notify caller and remove unsent messages
          LOG4CXX_ERROR(logger_, "Device adapter failed to send data")
          ;
          //todo: potential error case -> thread unsafe update of message content
          data->set_waiting(true);  //mark message for re-send
          break;
        case DeviceAdapterListenerImpl::EventTypeEnum::ON_RECEIVED_DONE:
          data = (*it).data();
          for (TransportManagerListenerList::iterator dal_it =
              transport_manager_listener_.begin();
              dal_it != transport_manager_listener_.end(); ++dal_it) {
            (*dal_it)->onDataReceiveDone(da, sid, data);
          }
          break;
        case DeviceAdapterListenerImpl::EventTypeEnum::ON_RECEIVED_FAIL:
          d_err = static_cast<DataReceiveError *>((*it).error());
          for (TransportManagerListenerList::iterator dal_it =
              transport_manager_listener_.begin();
              dal_it != transport_manager_listener_.end(); ++dal_it) {
            (*dal_it)->onDataReceiveFailed(da, sid, *d_err);
          }
          break;
        case DeviceAdapterListenerImpl::EventTypeEnum::ON_COMMUNICATION_ERROR:
          break;
      }
      //todo:
      event_queue_.remove((*it));
    }  //for
    pthread_mutex_unlock(&event_queue_mutex_);

  }  //end while true

  event_queue_.clear();
  LOG4CXX_INFO(logger_, "Event listener thread finished");
}

void *TransportManagerImpl::messageQueueStartThread(void *data) {
  if (NULL != data) {
    static_cast<TransportManagerImpl *>(data)->messageQueueThread();
  }
  return 0;
}

void TransportManagerImpl::messageQueueThread(void) {

  LOG4CXX_INFO(logger_, "Message queue thread started");
  while (true == all_thread_active_) {
    //todo: add priority processing

    u_int serial_number = std::numeric_limits<unsigned int>::max();
    RawMessageSptr active_msg;
    pthread_mutex_lock(&message_queue_mutex_);
    for (MessageQueue::iterator it = message_queue_.begin();
        it != message_queue_.end(); ++it) {
      LOG4CXX_INFO(logger_,
                   "Iterating over message queue " << (*it)->serial_number());
      if ((*it)->serial_number() < serial_number) {
        LOG4CXX_INFO(logger_, "Found message with min serial number");
        active_msg = (*it);
        serial_number = (*it)->serial_number();
      }
    }
    if (active_msg.valid() && false == active_msg->isWaiting()) {
      DeviceAdapter *device_adapter = adapter_handler_.getAdapterBySession(
          active_msg->connection_key());
      LOG4CXX_INFO(
          logger_,
          "Got adapter " << device_adapter << " by session id " << active_msg->connection_key());

      if (NULL == device_adapter) {
        //probably error no device adapters found
        //todo: define shall we remove message or send it back to upper level
        message_queue_.remove(active_msg);
        LOG4CXX_ERROR(logger_, "Device adapter is not found remove message");
      } else {
        if (DeviceAdapter::Error::OK
            == device_adapter->sendData(active_msg->connection_key(),
                                        active_msg)) {
          LOG4CXX_INFO(logger_, "Data sent to adapter");
          active_msg->set_waiting(true);
        } else {
          LOG4CXX_ERROR(logger_, "Data sent error");
        }
      }
    }
    pthread_mutex_unlock(&message_queue_mutex_);
  }  //while(true)

  message_queue_.clear();
  LOG4CXX_INFO(logger_, "Message queue thread finished");
}

pthread_cond_t *TransportManagerImpl::getDeviceListenerThreadWakeup(void) {
  return &device_listener_thread_wakeup_;
}

DeviceAdapter *TransportManagerImpl::AdapterHandler::getAdapterBySession(
    SessionID session_id) {
  LOG4CXX_INFO(logger_, "Add adapter by session called " << session_id);
  std::map<SessionID, DeviceAdapter *>::iterator da = session_to_adapter_map_
      .find(session_id);
  if (da != session_to_adapter_map_.begin()) {
    LOG4CXX_INFO(logger_, "Device adapter found");
    return (*da).second;
  }
  LOG4CXX_INFO(logger_, "Device adapter was not found");
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
  LOG4CXX_INFO(logger_, "Adapter handler destroyed");
}

DeviceAdapter *TransportManagerImpl::AdapterHandler::getAdapterByDevice(
    DeviceHandle device_id) {
  LOG4CXX_INFO(logger_, "Get adapter by device called");
  std::map<DeviceHandle, DeviceAdapter *>::iterator da =
      device_to_adapter_multimap_.find(device_id);
  if (da != device_to_adapter_multimap_.end()) {
    LOG4CXX_INFO(logger_, "Device adapter found");
    return (*da).second;
  }
  LOG4CXX_INFO(logger_, "Device adapter was not found");
  return NULL;
}
void TransportManagerImpl::AdapterHandler::addAdapter(
    DeviceAdapter *device_adapter) {
  LOG4CXX_INFO(logger_, "Add device adapter called " << device_adapter);
  device_adapters_.push_back(device_adapter);
}

const TransportManagerImpl::AdapterHandler::AdapterList &TransportManagerImpl::AdapterHandler::device_adapters(
    void) {
  return device_adapters_;
}
void TransportManagerImpl::AdapterHandler::addSession(DeviceAdapter *da,
                                                      SessionID sid) {
  LOG4CXX_INFO(logger_,
               "Add session called with sid " << sid << " adapter " << da);
  AdapterList::iterator item = std::find(device_adapters_.begin(),
                                         device_adapters_.end(), da);
  if (item == device_adapters_.end()) {
    LOG4CXX_ERROR(logger_, "Device adapter is not known");
    //todo: error case: unknown adapter
    return;
  }
  session_to_adapter_map_.insert(std::make_pair(sid, da));
  LOG4CXX_INFO(logger_, "Add session complete");
}

void TransportManagerImpl::AdapterHandler::removeSession(DeviceAdapter *da,
                                                         SessionID sid) {
  LOG4CXX_INFO(logger_,
               "Remove session called with sid " << sid << " adapter " << da);
  session_to_adapter_map_.erase(sid);
}

void TransportManagerImpl::AdapterHandler::addDevice(DeviceAdapter *da,
                                                     DeviceHandle did) {
  LOG4CXX_INFO(logger_, "Add device with did " << did << " adapter " << da);
  AdapterList::iterator item = std::find(device_adapters_.begin(),
                                         device_adapters_.end(), da);
  if (item == device_adapters_.end()) {
    LOG4CXX_ERROR(logger_, "Device adapter is not known");
    //todo: error case: unknown adapter
    return;
  }
  device_to_adapter_multimap_.insert(std::make_pair(did, da));
  LOG4CXX_INFO(logger_, "Add device complete");
}

void TransportManagerImpl::AdapterHandler::removeDevice(
    const DeviceHandle &device) {
  LOG4CXX_INFO(logger_, "Remove device is called" << device);
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

